#include <sys/socket.h>       /*  socket definitions        */
#include <sys/types.h>        /*  socket types              */
#include <sys/wait.h>         /*  for waitpid()             */
#include <arpa/inet.h>        /*  inet (3) funtions         */
#include <unistd.h>           /*  misc. UNIX functions      */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wait.h>

#define LISTENQ          (1024)
#define MAXBUFSIZE 1024

int SERVER_PORT;

char* content_typ[9];
char* content_ext[9];

char buffer[MAXBUFSIZE];
char directory[50];
char directory_index_file[10];
char content_length[50];
char content_type[50];

//enum Request_Method { GET, HEAD, UNSUPPORTED };

void Error_Quit(char const * msg) {
    fprintf(stderr, "WEBSERV: %s\n", msg);
    exit(EXIT_FAILURE);
}

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

void load_config(void)
{
	FILE* file = fopen("ws.conf", "r"); /* should check the result */
    char *line = NULL;
    size_t len = 0;
    int index = 0;

    while(-1 != getline(&line, &len, file))
	{
		char* input = strtok(line, " ");

        if(checker(input,"Listen") == 1)
        {
        	input = strtok(NULL, " ");
        	SERVER_PORT = atoi(input);
        }
        if(checker(input,"DocumentRoot") == 1)
        {
        	input = strtok(NULL, " ");
        	strncpy(directory,input,strlen(input));
        }
        if(checker(input,"DirectoryIndex") == 1)
        {
        	input = strtok(NULL, "\n");
        	strncpy(directory_index_file,input,strlen(input));
        }
        if((checker(input,".html")==1) && (index==0))
        {
            content_typ[index] = strdup(line);
            input = strtok(NULL, "\n"); 
            content_ext[index] = strdup(input);
            index++;
        }
        if((checker(input,".htm")==1) && (index==1))
        {
            content_typ[index] = strdup(line);
            input = strtok(NULL, "\n"); 
            content_ext[index] = strdup(input);
            index++;
        }
        if((checker(input,".txt")==1) && (index==2))
        {
            content_typ[index] = strdup(line);
            input = strtok(NULL, "\n"); 
            content_ext[index] = strdup(input);
            index++;
        }
        if((checker(input,".png")==1) && (index==3))
        {
            content_typ[index] = strdup(line);
            input = strtok(NULL, "\n"); 
            content_ext[index] = strdup(input);
            index++;
        }
        if((checker(input,".gif")==1) && (index==4))
        {
            content_typ[index] = strdup(line);
            input = strtok(NULL, "\n"); 
            content_ext[index] = strdup(input);
            index++;
        }
        if((checker(input,".jpg")==1) && (index==5))
        {
            content_typ[index] = strdup(line);
            input = strtok(NULL, "\n"); 
            content_ext[index] = strdup(input);
            index++;
        }
        if((checker(input,".css")==1) && (index==6))
        {
            content_typ[index] = strdup(line);
            input = strtok(NULL, "\n"); 
            content_ext[index] = strdup(input);
            index++;
        }
        if((checker(input,".js")==1) && (index==7))
        {
            content_typ[index] = strdup(line);
            input = strtok(NULL, "\n"); 
            content_ext[index] = strdup(input);
            index++;
        }
        if((checker(input,".ico")==1) && (index==8))
        {
            content_typ[index] = strdup(line);
            input = strtok(NULL, "\n"); 
            content_ext[index] = strdup(input);
            index++;
        }
    }
    fclose(file);
    printf("%s %s\n",content_typ[0],content_ext[0]);
    printf("%s %s\n",content_typ[1],content_ext[1]);
    printf("%s %s\n",content_typ[2],content_ext[2]);
    printf("%s %s\n",content_typ[3],content_ext[3]);
    printf("%s %s\n",content_typ[4],content_ext[4]);
    printf("%s %s\n",content_typ[5],content_ext[5]);
    printf("%s %s\n",content_typ[6],content_ext[6]);
    printf("%s %s\n",content_typ[7],content_ext[7]);
    printf("%s %s\n",content_typ[8],content_ext[8]);
}

int ServiceReq(int connection)
{
	//struct Request_Info reqinfo;
	char servreq_buffer[MAXBUFSIZE],req_temp[MAXBUFSIZE];

	int return_status = recv(connection, servreq_buffer, MAXBUFSIZE, 0);
    strncpy(req_temp,servreq_buffer,strlen(servreq_buffer));
    printf("%s\n",req_temp);
    char* file_to_send = strtok(req_temp, " ");
    file_to_send = strtok(NULL, " ");
    printf("file to send = %s\n",file_to_send);

	if(return_status < 0)
		printf("Error\n");
	else if(return_status == 0)
		printf("Connection disconnected\n");
	else
	{
		printf("%s\n\n",servreq_buffer);
		char* req_meth = strtok(servreq_buffer, " ");
		char ok[4] = " Ok\n";
		//printf("%s",ok);
		char* bad_request = " Bad Request\n";
		//printf("%s\n",req_meth);

		if(checker(req_meth,"GET")==1)
		{
			printf("HERE 1\n");
		 	char header_status[4];
		 	bzero(header_status,sizeof(header_status));
		 	strcpy(header_status," 200");

		 	char* filename = directory_index_file;
		 	// char* file_to_send = strtok(NULL, " ");
    //         printf("file to send = %s\n",file_to_send);
		 	char* protocol_ver = strtok(NULL, "\n");
		 	char header_message[20];
		 	bzero(header_message,sizeof(header_message));
		 	strncpy(header_message,protocol_ver,8);
		 	strcat(header_message,header_status);
		 	strcat(header_message,ok);
            printf("header sent = %s\n",header_message);
		 	printf("HERE 2\n");

		 	FILE *fp;
		 	unsigned long fsize = 0;
    		int size_of_file = 0;
            //int s=0;

		 	fp = fopen(filename, "r");

        	if (fp == NULL)
    		{
        		printf("File does not exist\n");
    		}
			else
			{
                printf("File exists\n");
				fseek(fp, 0, SEEK_END);
        		fsize = ftell(fp);
        		fseek(fp,0,SEEK_SET);
        		printf("File size to be sent = %ld\n",fsize);

        		sprintf(content_length,"Content-length: %ld\n",fsize);
                //sprintf(content_type,"Content-length: %ld",fsize);

                write(connection, header_message, strlen(header_message));
                write(connection, content_length, strlen(content_length));
                write(connection, "Content-Type: text/html\n\n", 25);
                //write(connection, "<html><body><H1>Hello world</H1></body></html>",46);

            	while(size_of_file < fsize)
		    	{
		    		bzero(buffer,sizeof(buffer));
		        	int bytes_read = fread(buffer,1,MAXBUFSIZE,fp);
		        	write(connection, buffer,bytes_read);
                    size_of_file += bytes_read;
		    	}
                printf("Sent data\n");
        		fclose(fp);
			}
        }
	}

	// write(connection, "HTTP/1.1 200 OK\n", 16);
 //    write(connection, "Content-length: 46\n", 19);
 //    write(connection, "Content-Type: text/html\n\n", 25);
 //    write(connection, "<html><body><H1>Hello world</H1></body></html>",46);

    close(connection);

 	return 0;
}

int main(int argc, char *argv[]) {

    int    listener, connection;
    pid_t  pid;
    struct sockaddr_in servaddr;

    load_config();

    /*  Create socket  */

    if ( (listener = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
		Error_Quit("Couldn't create listening socket.");
	else
		printf("Created socket\n");


    /* Populate socket address structure */

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(SERVER_PORT);


    /* Assign socket address to socket */ 
    if ( bind(listener, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 )
		Error_Quit("Couldn't bind listening socket.");
	else
		printf("Binded socket\n");

    /* Make socket a listening socket */
    if ( listen(listener, LISTENQ) < 0 )
		Error_Quit("Call to listen failed.");
	else
		printf("Created listening socket\n");

	while(1)
	{
		printf("Got request\n");
		if ( (connection = accept(listener, NULL, NULL)) < 0 )
	    Error_Quit("Error calling accept()");

		/* Create child process */
		if ( (pid = fork()) == 0 )
		{
			/* Close the parent process */
			if ( close(listener) < 0 )
			Error_Quit("Error closing listening socket in child.");

			ServiceReq(connection);
			//exit(0);
			close(connection);
			exit(0);
		}
		else
			close(connection);

		waitpid(-1, NULL, WNOHANG);
	}
	/* Close the parent process */
	if ( close(listener) < 0 )
	Error_Quit("Error closing listening socket in child.");
}