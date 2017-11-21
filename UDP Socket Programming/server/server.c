#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <dirent.h>
#include <time.h>

#define MAXBUFSIZE 1000
char encrypt_decrypt_key = 11;

struct reliable_packet
{
	int seq_id;
  	char data[MAXBUFSIZE];
	int packet_length;
};	//structure to pass for reliable communication

struct timeval tv;

int checker(char input[],char check[])
{
    /* compare string with value */
    int i,result=1;
    for(i=0;input[i]!='\0' && check[i]!='\0';i++){
        if(input[i]!=check[i]){
            result=0;
            break;
        }
    }
    /* Return the compare result */
    return result;
}

/* GET function on server side */
void get_server(int socket_id, char *file_name, struct sockaddr_in remote_address, unsigned int remote_len)
{
    tv.tv_sec = 0;  /* Initialize timer values */
    tv.tv_usec = 500000;

    FILE *fp;
    unsigned long fsize = 0;
    ssize_t encodedfilesize;
    fp = fopen(file_name, "r"); /* Open the file to send */
    int size_of_file = 0;
    int getbytes;
    char error_message[30];

    if (fp == NULL)
    {   /* If file does not exist, notify the client */
        printf("File does not exist. Notified client\n");
        strcpy(error_message,"File does not exist");
        getbytes = sendto(socket_id, &error_message, sizeof(error_message), 0, (struct sockaddr *)&remote_address, remote_len);
    }
    else
    {
        int count = 0;
        ssize_t ACK;
        int ack;
        /* If file exists, begin routine to send to the client */
        bzero(error_message,sizeof(error_message));
        strcpy(error_message,"File exists");
        getbytes = sendto(socket_id, &error_message, sizeof(error_message), 0, (struct sockaddr *)&remote_address, remote_len);
        /* Find out the length of the file to send to client */
        fseek(fp, 0, SEEK_END);
        fsize = ftell(fp);
        fseek(fp,0,SEEK_SET);
        encodedfilesize = htonl(fsize);
        /* Send file size to client */
        getbytes = sendto(socket_id, &encodedfilesize, sizeof(encodedfilesize), 0, (struct sockaddr *)&remote_address, remote_len);
        printf("File size to be sent to client = %ld\n",fsize);
        /* Set the timeout value on receive function */
        if(setsockopt(socket_id, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv))<0)
            exit(0);
        /* While the file size read is still less than its original size, keep transmitting */
        while(size_of_file < fsize)
        {
            struct reliable_packet *rb = malloc(sizeof(struct reliable_packet));
            rb->seq_id = count; /* Assign sequence ID to the packet */
            /* Read the file bytes into a buffer */
            int number = fread(rb->data,sizeof(char),MAXBUFSIZE,fp);
            rb->packet_length = number;
            printf("Number of data bytes to send = %d\n", number);
            /* Encrypt the packet before sending */
            for(int index=0;index<number;index++)
            {
                rb->data[index] ^= encrypt_decrypt_key;
            }
            /* Send packet to client */
            getbytes = sendto(socket_id, rb, sizeof(struct reliable_packet), 0, (struct sockaddr *)&remote_address, remote_len);
            printf("Bytes sent in one go = %d\n",getbytes);
            /* If ACK not received from client, retransmit the same packet */
            if(recvfrom(socket_id, &ACK, sizeof(ACK), 0, (struct sockaddr *)&remote_address, &remote_len)<0)
            {
                printf("ACK not received. Retransmitting packet number %d\n",rb->seq_id);
                fseek(fp,size_of_file,SEEK_SET);
            }
            else
            {   /* Increment count and send next packet */
                ack = ntohl(ACK);
                if(ack == count)
                {
                    printf("ACK received = %d. Sending next packet\n",ack);
                    size_of_file += rb->packet_length;
                    count += 1;
                }
                else
                    fseek(fp,size_of_file,SEEK_SET);
            }
            free(rb);
        }
        printf("File size sent to client = %d\n",size_of_file);
        fclose(fp);
    }
}

/* PUT function on server side */
void put_server(int socket_id, char *file_name, struct sockaddr_in remote_address, unsigned int remote_len)
{
    unsigned long size_buffer;
    int count_local = 0;
    int putbytes;
    char msg[30];
    /* Receive message from client to know if file exits */
    putbytes = recvfrom(socket_id, &msg, sizeof(msg), 0, (struct sockaddr *)&remote_address, &remote_len);

    if(checker(msg,"File does not exist") == 1)
    {
        printf("File does not exist on client's side\n");
    }
    else
    {
        /* Receive file size from client to receive */
        putbytes = recvfrom(socket_id, &size_buffer, sizeof(size_buffer), 0, (struct sockaddr *)&remote_address, &remote_len);

        FILE *fp;
        fp = fopen(file_name, "w+");
        ssize_t encodedfilesize = ntohl(size_buffer);
        printf("File size to be received from client = %ld\n",encodedfilesize);
        int size_of_file = 0;
        /* Keep track of file size received being less than original file size*/
        while(size_of_file < encodedfilesize)
        {
            struct reliable_packet *rb = malloc(sizeof(struct reliable_packet));
            int ack;
            putbytes = recvfrom(socket_id, rb, sizeof(struct reliable_packet), 0, (struct sockaddr *)&remote_address, &remote_len);
            printf("Bytes received in one go = %d\n",putbytes);
	    printf("DATA BEFORE DECRYPT\n");
	    printf("%s\n",rb->data);
            /* Decrypt the packet when its received */
            for(int index=0;index<rb->packet_length;index++)
            {
                rb->data[index] ^= encrypt_decrypt_key;
            }
	    printf("DATA AFTER DECRYPT\n");
	    printf("%s\n",rb->data);
            printf("Decrypted packet\n");
            ack = rb->seq_id;
            ssize_t ACK = htonl(ack);
            if(rb->seq_id == count_local)
            {
                /*If packet received, send ACK */
                fwrite(rb->data,sizeof(char),rb->packet_length,fp);
                printf("ACK sent = %d\n", ack);
                putbytes = sendto(socket_id, &ACK, sizeof(ACK), 0, (struct sockaddr *)&remote_address, remote_len);
                printf("Writing to file\n");
                size_of_file += rb->packet_length;
                count_local += 1;
            }
            else
            {/* If packet not received, send ACK for last received packet */
                printf("Packet lost. Sending ACK of last received packet = %d.\n",ack);
                putbytes = sendto(socket_id, &ACK, sizeof(ACK), 0, (struct sockaddr *)&remote_address, remote_len);
            }
            free(rb);
        }
        printf("File size received from client = %d\n",size_of_file);
        fclose(fp);
    }
}

/* LIST function on server side */
void list_server(int socket_id, struct sockaddr_in remote_address, unsigned int remote_len)
{
    char list_buffer[MAXBUFSIZE];
    char msg[MAXBUFSIZE];
    int listbytes;
    bzero(msg,sizeof(msg));
    listbytes = recvfrom(socket_id, msg, sizeof(msg), 0, (struct sockaddr *)&remote_address, &remote_len);
    printf("\nClient says:\n");
    printf("%s\n",msg);

    struct dirent *de;  // Pointer for directory entry
 
    // opendir() returns a pointer of DIR type. 
    DIR *dr = opendir(".");
 
    if (dr == NULL)  // opendir returns NULL if couldn't open directory
    {
        printf("Could not open current directory" );
    }

    const char* extension = "\n";
    bzero(list_buffer,sizeof(list_buffer));
    while ((de = readdir(dr)) != NULL)
    {
        char* name = de->d_name;
        strcat(list_buffer, name); /* add the extension */
        strcat(list_buffer, extension); /* add the extension */
    }
    printf("%s\n",list_buffer);
    /* Send the list of files to client */
    listbytes = sendto(socket_id, list_buffer, sizeof(list_buffer), 0, (struct sockaddr *)&remote_address, remote_len);
    printf("\nSent the list of files to client\n");
    closedir(dr);
}

/* DELETE function on server side */
void delete_server(int socket_id, char *file_name, struct sockaddr_in remote_address, unsigned int remote_len)
{
    char delete_buffer[MAXBUFSIZE];
    int deletebytes;

    deletebytes = recvfrom(socket_id, delete_buffer, sizeof(delete_buffer), 0, (struct sockaddr *)&remote_address, &remote_len);
    printf("\nClient says:\n");
    printf("%s\n",delete_buffer);

    deletebytes = recvfrom(socket_id, file_name, sizeof(file_name), 0, (struct sockaddr *)&remote_address, &remote_len);

    FILE *fp;
    fp = fopen(file_name, "r");
    /* If file doesn't exist, send error message */
    if (fp == NULL)
    {
        printf("File does not exist. Notified client.\n");
        bzero(delete_buffer,sizeof(delete_buffer));
        strcpy(delete_buffer,"File does not exist");
        deletebytes = sendto(socket_id, &delete_buffer, sizeof(delete_buffer), 0, (struct sockaddr *)&remote_address, remote_len);
    }
    else
    {
        /* If file exists, send message saying the same and proceed */
        bzero(delete_buffer,sizeof(delete_buffer));
        strcpy(delete_buffer,"File exists. Now deleting.\n");
        deletebytes = sendto(socket_id, &delete_buffer, sizeof(delete_buffer), 0, (struct sockaddr *)&remote_address, remote_len);
        
        int status = remove(file_name); 
        if( status == 0 )
        {
            /* If able to open the file, delete it and send confirmation message to client */
            printf("%s file deleted successfully.\n",file_name);
            bzero(delete_buffer,sizeof(delete_buffer));
            strcpy(delete_buffer,"File deleted successfully");
            deletebytes = sendto(socket_id, delete_buffer, sizeof(delete_buffer), 0, (struct sockaddr *)&remote_address, remote_len);
        }
        else
        {
            /* If failed to open the file, send error message to client */
            printf("Unable to delete the file\n");
            printf("Error\n");
            bzero(delete_buffer,sizeof(delete_buffer));
            strcpy(delete_buffer,"Error. Unable to delete the file");
            deletebytes = sendto(socket_id, delete_buffer, sizeof(delete_buffer), 0, (struct sockaddr *)&remote_address, remote_len);
        }
    }
}

/* EXIT function on server side */
void exit_server(int socket_id, struct sockaddr_in remote_address, unsigned int remote_len)
{
    printf("Server exiting gracefully\n");
    char msg[] = "exit";
    int exitbytes;
    /* Notify the client to exit as well */
    exitbytes = sendto(socket_id, msg, sizeof(msg), 0, (struct sockaddr *)&remote_address, remote_len);
    /* Exit server */
    close(socket_id);
}

int main (int argc, char * argv[])
{
	int sock;                           //This will be our socket
	struct sockaddr_in sin, remote;     //"Internet socket address structure"
	unsigned int remote_length;         //length of the sockaddr_in structure
	int nbytes;                        //number of bytes we receive in our message
	char buffer[MAXBUFSIZE];

	char client_input[30];

	if (argc != 2)
	{
		printf ("USAGE:  <port>\n");
		exit(1);
	}

	/******************
	This code populates the sockaddr_in struct with
	the information about our socket
	******************/
	bzero(&sin,sizeof(sin));                    //zero the struct
	sin.sin_family = AF_INET;                   //address family
	sin.sin_port = htons(atoi(argv[1]));        //htons() sets the port # to network byte order
	sin.sin_addr.s_addr = INADDR_ANY;           //supplies the IP address of the local machine

	//Causes the system to create a generic socket of type UDP (datagram)
	if ((sock = socket(AF_INET,SOCK_DGRAM,0)) < 0)
	{
		printf("unable to create socket");
	}

	/******************
	Once we've created a socket, we must bind that socket to the 
	local address and port we've supplied in the sockaddr_in struct
	******************/
	if (bind(sock, (struct sockaddr *)&sin, sizeof(sin)) < 0)
	{
		printf("unable to bind socket\n");
	}

	remote_length = sizeof(remote);

    while(1)
      {
        tv.tv_sec = 0;
        tv.tv_usec = 0;
        if(setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv))<0)
                exit(0);
        printf("\nListening to client\n");
	
        //waits for an incoming message
	    nbytes = recvfrom(sock, client_input, sizeof(client_input), 0, (struct sockaddr *)&remote, &remote_length);

	    printf("The client says %s\n", client_input);

        /* Parse the input for command and filename */
	    char filename[10], command[10];
        char* word = strtok(client_input, " ");
        strcpy(command,word);
        printf("%s\n", command);
        word = strtok(NULL, " ");

        if(word != NULL)
        {
    	   strcpy(filename,word);
    	   printf("%s\n", filename);
        }    
        /* Server sends file to the client */
        if(checker(command,"get") == 1)
        {
            get_server(sock,filename,remote,remote_length);
        }
        /* Server receivs file from the client */
        else if(checker(command,"put") == 1)
        {
            put_server(sock,filename,remote,remote_length);
        }
        /* Server lists files in its directory and sends to client*/
        else if(checker(command,"ls") == 1)
	   {
            list_server(sock,remote,remote_length);
	   }
       /* Server deleted a file in its directory */
	   else if(checker(command,"delete") == 1)
	   {
            delete_server(sock,filename,remote,remote_length);
        }
        /* Server exits gracefully */
        else if(checker(command,"exit") == 1)
	   {
            exit_server(sock,remote,remote_length);
	   }
	}
}
