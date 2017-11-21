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
#include <errno.h>
#include <time.h>   /* Timer header file to use timeval struct */

#define MAXBUFSIZE 1000 /* Define maximum buffer size */

char encrypt_decrypt_key = 11; /* Define key for encryption/decryption */

/* Define packet for reliable transmission */
struct reliable_packet
{
	int seq_id;
  	char data[MAXBUFSIZE];
	int packet_length;
};

struct timeval tv;
/* Function to check if command equals a string */
int checker(char input[],char check[])
{
    int i,result=1;
    for(i=0;input[i]!='\0' && check[i]!='\0';i++){
        if(input[i]!=check[i]){
            result=0;
            break;
        }
    }
    return result;
}

/* GET function on client side */
void get_client(int socket_id, char *file_name, struct sockaddr_in remote_address, int address_length)
{
    unsigned long size_buffer;
    int count_local = 0;
    int getbytes;
    char msg[30];

    getbytes = recvfrom(socket_id, &msg, sizeof(msg), 0, (struct sockaddr *)&remote_address, &address_length);

    if(checker(msg,"File does not exist") == 1)
    {
        printf("File does not exist on server's side\n");
    }
    else
    {
        getbytes = recvfrom(socket_id, &size_buffer, sizeof(size_buffer), 0, (struct sockaddr *)&remote_address, &address_length);

        FILE *fp;
        fp = fopen(file_name, "w+");
        ssize_t encodedfilesize = ntohl(size_buffer);
        printf("File size to be received from server = %ld\n",encodedfilesize);
        int size_of_file = 0;
        /* Keep track of file size received being less than original file size*/
        while(size_of_file < encodedfilesize)
        {
            struct reliable_packet *rb = malloc(sizeof(struct reliable_packet));
            int ack;
            getbytes = recvfrom(socket_id, rb, sizeof(struct reliable_packet), 0, (struct sockaddr *)&remote_address, &address_length);
            printf("Bytes received in one go = %d\n",getbytes);
            /* Decrypt the packet when its received */
            for(int index=0;index<rb->packet_length;index++)
            {
                rb->data[index] ^= encrypt_decrypt_key;
            }
            printf("Decrypted packet\n");
            ack = rb->seq_id;
            ssize_t ACK = htonl(ack);

            if(rb->seq_id == count_local)
            {
                /*If packet received, send ACK */
                printf("ACK sent = %d\n", ack);
                getbytes = sendto(socket_id, &ACK, sizeof(ACK), 0, (struct sockaddr *)&remote_address, sizeof(remote_address));
                printf("Writing to file\n");
                fwrite(rb->data,sizeof(char),rb->packet_length,fp);
                size_of_file += rb->packet_length;
                count_local += 1;
            }
            else
            {/* If packet not received, send ACK for last received packet */
                printf("Packet lost. Sending ACK of last received packet = %d.\n",ack);
                getbytes = sendto(socket_id, &ACK, sizeof(ACK), 0, (struct sockaddr *)&remote_address, sizeof(remote_address));
            }
            free(rb);
        }
        printf("File size received from server = %d\n",size_of_file);
        fclose(fp);
    }
}

/* PUT function on client side */
void put_client(int socket_id, char *file_name, struct sockaddr_in remote_address, int address_length)
{
    tv.tv_sec = 0;
    tv.tv_usec = 500000;
    FILE *fp;
    unsigned long fsize = 0;
    ssize_t encodedfilesize;
    int putbytes;
    char msg[30];

    fp = fopen(file_name, "r");

    int size_of_file = 0;

    if (fp == NULL)
    {
        printf("File does not exist\n");
        strcpy(msg,"File does not exist");
        putbytes = sendto(socket_id, &msg, sizeof(msg), 0, (struct sockaddr *)&remote_address, address_length);
    }
    else
    {
        int count = 0;
        ssize_t ACK;
        int ack;
        /* If file exists, begin routine to send to the server */
        bzero(msg,sizeof(msg));
        strcpy(msg,"File exists");
        putbytes = sendto(socket_id, &msg, sizeof(msg), 0, (struct sockaddr *)&remote_address, address_length);
        /* Find out the length of the file to send to server */
        fseek(fp, 0, SEEK_END);
        fsize = ftell(fp);
        fseek(fp,0,SEEK_SET);
        encodedfilesize = htonl(fsize);
        /* Send file size to server */
        putbytes = sendto(socket_id, &encodedfilesize, sizeof(encodedfilesize), 0, (struct sockaddr *)&remote_address, address_length);
        printf("File size to be sent to server = %ld\n",fsize);
         /* Set the timeout value on receive function */
        if(setsockopt(socket_id, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv))<0)
            exit(0);
        /* While the file size read is still less than its original size, keep transmitting */
        while(size_of_file < fsize)
        {
            struct reliable_packet *rb = malloc(sizeof(struct reliable_packet));
            rb->seq_id = count; /* Assign sequence ID to the packet */
            int number = fread(rb->data,sizeof(char),MAXBUFSIZE,fp);
            rb->packet_length = number;
            printf("Number of data bytes to send = %d\n", number);
            for(int index=0;index<number;index++)
            {
                rb->data[index] ^= encrypt_decrypt_key;
            }
            printf("Encrypted packet\n");
            putbytes = sendto(socket_id, rb, sizeof(struct reliable_packet), 0, (struct sockaddr *)&remote_address, address_length);
            printf("Bytes sent in one go = %d\n",putbytes);
            /* If ACK not received from server, retransmit the same packet */
            if(recvfrom(socket_id, &ACK, sizeof(ACK), 0, (struct sockaddr *)&remote_address, &address_length)<0)
            {
                printf("ACK not received. Retransmitting packet number %d\n", rb->seq_id);
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
        printf("File size sent to server = %d\n",size_of_file);
        fclose(fp);
    }
}

/* LS function on client side */
void list_client(int socket_id, struct sockaddr_in remote_address, int address_length)
{
    printf("\nList files\n");
    int listbytes;
    char list_buffer[MAXBUFSIZE];
    char *msg = "List files in your directory";
    listbytes = sendto(socket_id, msg, sizeof(msg), 0, (struct sockaddr *)&remote_address, address_length);

    listbytes = recvfrom(socket_id, &list_buffer, sizeof(list_buffer), 0, (struct sockaddr *)&remote_address, &address_length);
    printf("\nFiles in server's directory are as follows:\n\n");
    printf("%s\n",list_buffer);
}

/* DELETE function on client side */
void delete_client(int socket_id, char *file_name, struct sockaddr_in remote_address, int address_length)
{
    printf("\nDelete file on server side\n");
    int deletebytes;
    char delete_buffer[MAXBUFSIZE];
    strcpy(delete_buffer,"Delete file ");
    /* Send message to server to perform delete file operation */
    deletebytes = sendto(socket_id, delete_buffer, sizeof(delete_buffer), 0, (struct sockaddr *)&remote_address, address_length);
    /* Send server the name of the file to delete */
    deletebytes = sendto(socket_id, file_name, sizeof(file_name), 0, (struct sockaddr *)&remote_address, address_length);
    /* Zero out the buffer */
    bzero(delete_buffer,sizeof(delete_buffer));
    /* Receive status message form the server */
    deletebytes = recvfrom(socket_id, &delete_buffer, sizeof(delete_buffer), 0, (struct sockaddr *)&remote_address, &address_length);
    if(checker(delete_buffer,"File does not exist") == 1) /* If error, print of console */
    {
        printf("\nFile does not exist on server's side\n");
    }
    else /* Print on console what server said*/
    {
        bzero(delete_buffer,sizeof(delete_buffer));
        deletebytes = recvfrom(socket_id, &delete_buffer, sizeof(delete_buffer), 0, (struct sockaddr *)&remote_address, &address_length);
        printf("\nServer says: %s\n",delete_buffer);
    } 
}

/* EXIT function on client side */
void exit_client(int socket_id, struct sockaddr_in remote_address, int address_length)
{
    char exit_buffer[MAXBUFSIZE];
    int exitbytes;

    /* Receive status message from the server */
    exitbytes = recvfrom(socket_id, &exit_buffer, sizeof(exit_buffer), 0, (struct sockaddr *)&remote_address, &address_length);
    if (checker(exit_buffer,"exit") == 1) /* If server has exited gracefully */
    {
        printf("\nClient exiting gracefully\n");    
        close(socket_id);   /* Exit client as well */
    }
}

int main (int argc, char * argv[])
{
    int nbytes;                             // number of bytes send by sendto()
    int sock;                               //this will be our socket

    struct sockaddr_in remote;              //"Internet socket address structure"

    if (argc < 3)
    {
        printf("USAGE:  <server_ip> <server_port>\n");
	    exit(1);
    }

    char input_command[MAXBUFSIZE];
    char input_com[MAXBUFSIZE];
    int index=0;

    /******************
    Here we populate a sockaddr_in struct with
    information regarding where we'd like to send our packet 
    i.e the Server.
    ******************/
    bzero(&remote,sizeof(remote));               //zero the struct
    remote.sin_family = AF_INET;                 //address family
    remote.sin_port = htons(atoi(argv[2]));      //sets port to network byte order
    remote.sin_addr.s_addr = inet_addr(argv[1]); //sets remote IP address

    //Causes the system to create a generic socket of type UDP (datagram)
    if ((sock = socket(AF_INET,SOCK_DGRAM,0)) < 0)
    {
        printf("Unable to create socket\n");
    }

    while(1)
    {
        tv.tv_sec = 0;
        tv.tv_usec = 0;
        if(setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv))<0)
                exit(0);
        printf("\n*****************************************************");
        printf("\nGET file from server location ---> get [filename]");
        printf("\nPUT file at server location -----> put [filename]");
        printf("\nLIST files at server location ---> ls");
        printf("\nDELETE file at server location --> delete [filename]");
        printf("\nEXIT server ---------------------> exit");
        printf("\n*****************************************************\n");

        printf("\nEnter a command: ");
        scanf(" %[^\n]s", input_command);
        printf("You entered: %s\n", input_command);
        strcpy(input_com,input_command);

        /* Parse the input for command and filename */
        char filename[10], command[10];
        char* word = strtok(input_command, " ");
        strcpy(command,word);
        word = strtok(NULL, " ");
        if(word != NULL)
        {
            strcpy(filename,word);        
        }

        word = strtok(NULL, " ");
        if(word != NULL)
        {
            bzero(command,sizeof(command));
            strcpy(command,"other");        
        }
        else
            /******************
            sendto() sends immediately.  
            it will report an error if the message fails to leave the computer
            however, with UDP, there is no error if the message is lost in the network once it leaves the computer.
            ******************/
            nbytes = sendto(sock, input_com, sizeof(input_com), 0, (struct sockaddr *)&remote, sizeof(remote));

        struct sockaddr_in from_addr;
        int addr_length = sizeof(struct sockaddr);
        /* Server sends file to the client */
        if(checker(command,"get") == 1)	//Client receives
        {
            get_client(sock,filename,remote,addr_length);
        }
        /* Server receivs file from the client */
        else if(checker(command,"put") == 1)	//Client sends
        {
            put_client(sock,filename,remote,addr_length);
	   }
       /* Client asks Server to list files in its directory and sends to client*/
	   else if(checker(command,"ls") == 1)
	   {
            list_client(sock,remote,addr_length);
	   }
       /* Client asks Server to delete a file in its directory */
	   else if(checker(command,"delete") == 1)
	   {
            delete_client(sock,filename,remote,addr_length);
	   }
       /* Client asks Server to exit gracefully */
	   else if(checker(command,"exit") == 1)
	   {
            exit_client(sock,remote,addr_length);
	   }
        /* Client responds to wrong command */
       else if(checker(command,"other") == 1)
       {
            printf("Wrong command entered. Enter again.\n");
       }
    }
}

