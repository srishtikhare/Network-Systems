#include <sys/socket.h>       /*  socket definitions        */
#include <sys/types.h>        /*  socket types              */
#include <sys/wait.h>         /*  for waitpid()             */
#include <arpa/inet.h>        /*  inet (3) funtions         */
#include <unistd.h>           /*  misc. UNIX functions      */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wait.h>
#include <stdbool.h>

#define LISTENQ          (1024)
#define MAXBUFSIZE 1024
#define TIMEOUT 10

int SERVER_PORT;
int flag = -1;
bool keep_alive= false;
struct timeval timeout;
int return_status;

char* content_typ[9] = {0};
char* content_ext[9] = {0};

char buff[MAXBUFSIZE];
char temp_buffer[MAXBUFSIZE];
char directory[50];
char directory_index_file[10];
char header_message[50];
char content_length[50];
char content_type[50];
char fail_message[1024];
char total_path[50];

struct Request_Info{
    char request_method[MAXBUFSIZE];
    char request_uri[MAXBUFSIZE];
    char request_version[MAXBUFSIZE];
};

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

        /* Retrieve port number from ws.conf file */
        if(checker(input,"Listen") == 1)
        {
        	input = strtok(NULL, " ");
        	SERVER_PORT = atoi(input);
        }
        /* Retrieve root path from ws.conf file */
        if(checker(input,"DocumentRoot") == 1)
        {
        	input = strtok(NULL, "\"");
        	strncpy(directory,input,strlen(input));
            strncpy(total_path,directory,strlen(directory));
        }
        /* Retrieve directory index from ws.conf file */
        if(checker(input,"DirectoryIndex") == 1)
        {
        	input = strtok(NULL, "\n");
        	strncpy(directory_index_file,input,strlen(input));
        }
        /* Retrieve file extension types from ws.conf file */
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
}

/* Definition of 'BadRequestError' function */
void BadRequestError(int new_socket, struct Request_Info * reqinf, int opt_flag)
{
    char httpText[MAXBUFSIZE];
    char contentMessage[MAXBUFSIZE];

    if(flag == 0)
    {
        printf("\nIn BadRequestError function\n");
        bzero(httpText,sizeof(httpText));
        bzero(contentMessage,sizeof(contentMessage));

        printf("\nSending BadRequestError:\n");
        printf("**************\n");

        /* Constructing the HTTP response */
        sprintf(httpText, "%s %s %s",reqinf->request_version,"400","Bad Request\n\n");
        /* Sending response back to the client */
        write(new_socket, httpText, strlen(httpText));
        printf("httpText: %s\n",httpText);
    
        sprintf(contentMessage, "<html><body><H1>400 Bad Request Reason: Invalid Method: %s </H1></body></html>",reqinf->request_method);
        write(new_socket, contentMessage, strlen(contentMessage));
        printf("contentMessage: %s\n",contentMessage);

        printf("**************\n\n");
    }
    else if(flag == 1)
    {
        printf("\nIn BadRequestError function\n");
        bzero(httpText,sizeof(httpText));
        bzero(contentMessage,sizeof(contentMessage));

        printf("\nSending BadRequestError:\n");
        printf("**************\n");

        /* Constructing the HTTP response */
        sprintf(httpText, "%s %s %s",reqinf->request_version,"400","Bad Request\n\n");
        /* Sending response back to the client */
        write(new_socket, httpText, strlen(httpText));
        printf("httpText: %s\n",httpText);
    
        sprintf(contentMessage, "<html><body><H1>400 Bad Request Reason: Invalid HTTP-Version: %s </H1></body></html>",reqinf->request_version);
        write(new_socket, contentMessage, strlen(contentMessage));
        printf("contentMessage: %s\n",contentMessage);
        printf("**************\n\n");
    }
}

/* Definition of 'send_notFoundError' function */
void send_notFoundError(int new_socket, struct Request_Info * reqinf)
{
    printf("\nIn send_notFoundError function\n");
    char httpText[MAXBUFSIZE];
    char contentMessage[MAXBUFSIZE];
    bzero(httpText,sizeof(httpText));
    bzero(contentMessage,sizeof(contentMessage));

    printf("\nSending not found error:\n");
    printf("**************\n");

    /* Constructing the HTTP response */
    sprintf(httpText, "%s %s %s",reqinf->request_version,"404","Not Found\n\n");
    /* Sending response back to the client */
    write(new_socket, httpText, strlen(httpText));
    printf("httpText: %s\n",httpText);
    
    sprintf(contentMessage, "<html><body><H1>404 Not Found Reason URL does not exist : %s </H1></body></html>",reqinf->request_uri);
    write(new_socket, contentMessage, strlen(contentMessage));
    printf("contentMessage: %s\n",contentMessage);
    printf("**************\n\n");
}

/* Definition of 'send_ExtNotFoundError' function */
void send_ExtNotFoundError(int new_socket, struct Request_Info * reqinf, char* fname)
{
    printf("\nIn send_ExtNotFoundError function\n");
    char httpText[MAXBUFSIZE];
    char contentMessage[MAXBUFSIZE];
    bzero(httpText,sizeof(httpText));
    bzero(contentMessage,sizeof(contentMessage));

    printf("\nSending send_ExtNotFoundError:\n");
    printf("**************\n");

    /* Constructing the HTTP response */
    sprintf(httpText, "%s %s %s",reqinf->request_version,"501","Not Implemented\n\n");
    /* Sending response back to the client */
    write(new_socket, httpText, strlen(httpText));
    printf("httpText: %s\n",httpText);
    
    sprintf(contentMessage, "<html><body><H1>501 Not Implemented 501 : %s </H1></body></html>",fname);
    write(new_socket, contentMessage, strlen(contentMessage));
    printf("contentMessage: %s\n",contentMessage);
    printf("**************\n\n");
}

/* Definition of 'Cannot_Allocate_Memory_Error' function */
void Cannot_Allocate_Memory_Error(int new_socket, struct Request_Info * reqinf)
{
    printf("\nIn Cannot_Allocate_Memory_Error function\n");
    char httpText[MAXBUFSIZE];
    char contentMessage[MAXBUFSIZE];
    bzero(httpText,sizeof(httpText));
    bzero(contentMessage,sizeof(contentMessage));

    printf("\nSending Cannot_Allocate_Memory_Error:\n");
    printf("**************\n");

    /* Constructing the HTTP response */
    sprintf(httpText, "%s %s %s",reqinf->request_version,"500","Internal Server Error:cannot allocate memory\n\n");
    /* Sending response back to the client */
    write(new_socket, httpText, strlen(httpText));
    printf("httpText: %s\n",httpText);
    printf("**************\n\n");
}

int ServiceReq(int connection)
{
    struct Request_Info reqinfo;
    char servreq_buffer[MAXBUFSIZE];
    bzero(servreq_buffer,MAXBUFSIZE);

    while ( (return_status = recv(connection, servreq_buffer, MAXBUFSIZE, 0)) > 0 ) 
    {
        bzero(temp_buffer,MAXBUFSIZE);
        strncpy(temp_buffer,servreq_buffer,strlen(servreq_buffer));

        // Check if the connection alive is there or not
        char *keepa = strstr(servreq_buffer,"Connection: keep-alive");
   
        if(keepa)
        {
            // if keep alive is there then set the socket timeout to alive_timeout
            timeout.tv_sec = TIMEOUT;
            setsockopt(connection, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout,sizeof(struct timeval));
        }
        else
        {
            timeout.tv_sec = 0;
            setsockopt(connection, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout,sizeof(struct timeval));
        }
    }

    /* Print received request content */
    printf("\nReceived request content:\n");
    printf("**************\n");
    printf("%s",servreq_buffer);
    printf("**************\n");
    char* requestLine = strtok(servreq_buffer, "\n");
    sscanf(requestLine, "%s %s %s", reqinfo.request_method, reqinfo.request_uri, reqinfo.request_version);
    printf("request method = %s\n",reqinfo.request_method);
    printf("request uri = %s\n",reqinfo.request_uri);
    printf("request version = %s\n",reqinfo.request_version);

    /* Check if request version is correct */
    if((checker(reqinfo.request_version,"HTTP/1.0")==1) | (checker(reqinfo.request_version,"HTTP/1.1")==1))
    {   /* Check if request method is correct */
        if(checker(reqinfo.request_method,"GET")==1)
        {
            FILE *fp;
            unsigned long fsize = 0;
            int size_of_file = 0;

            if(checker(reqinfo.request_uri,"/ ")==1)
            {
                char index_html[50] = "/index.html";
                strcat(total_path,index_html);
                printf("total path = %s\n",total_path);

                /* Open index.html file */
                fp = fopen(total_path, "r");

                if (fp == NULL)
                {
                    printf("index.html File does not exist\n");

                    send_notFoundError(connection,&reqinfo);    //Error
                }
                else
                {
                    /* Open index.html file */
                    printf("File exists\n");
                    fseek(fp, 0, SEEK_END);
                    fsize = ftell(fp);
                    fseek(fp,0,SEEK_SET);
                    printf("File size to be sent = %ld\n",fsize);

                    /* Clear buffers */
                    bzero(header_message,strlen(header_message));
                    bzero(content_length,strlen(content_length));
                    bzero(content_type,strlen(content_type));

                    /* Create headers */
                    sprintf(header_message,"%s 200 Document Follows\n",reqinfo.request_version);
                    sprintf(content_length,"Content-length: %ld\n",fsize);
                    sprintf(content_type,"Content-Type: text/html\n\n");

                    /* Send header, content length and content type */
                    write(connection, header_message, strlen(header_message));
                    write(connection, content_length, strlen(content_length));
                    write(connection, content_type, strlen(content_type));

                    /* Send file data to webserver */
                    char* buffer = (char*)malloc(fsize);
                    if(buffer == NULL)
                        Cannot_Allocate_Memory_Error(connection,&reqinfo);  // Call Error 500, if required
                    else
                    {    /* Send data to server */
                        fread(buffer,1,fsize,fp);
                        write(connection, buffer,fsize);
                        printf("Sent data\n");
                    }
                    free(buffer);   //Free buffer pointer
                    fclose(fp);
                }
                /* Send required files to server */
                char welcome_image[50] = "/images/welcome.png";
                bzero(total_path,strlen(total_path));
                strcat(total_path,welcome_image);
                fp = fopen(total_path, "r");

                /* Check if file exists */
                if (fp == NULL)
                {
                    printf("welcome.png File does not exist\n");
                    send_notFoundError(connection,&reqinfo);    //Call error, if required
                }
                else
                {
                    printf("File exists\n");
                    fseek(fp, 0, SEEK_END);
                    fsize = ftell(fp);
                    fseek(fp,0,SEEK_SET);
                    printf("File size to be sent = %ld\n",fsize);

                    /* Clear buffers */
                    bzero(header_message, strlen(header_message));
                    bzero(content_length, strlen(content_length));
                    bzero(content_type, strlen(content_type));

                    sprintf(header_message,"%s 200 Document Follows\n",reqinfo.request_version);
                    sprintf(content_length,"Content-length: %ld\n",fsize);
                    sprintf(content_type,"Content-Type: image/png\n\n");

                    /* Send header, content length and content type to webserver */
                    write(connection, header_message, strlen(header_message));
                    write(connection, content_length, strlen(content_length));
                    write(connection, content_type, strlen(content_type));

                    char* buffer = (char*)malloc(fsize);
                    if(buffer == NULL)
                        Cannot_Allocate_Memory_Error(connection,&reqinfo);
                    else
                    {   /* Send data to server */
                        fread(buffer,1,fsize,fp);
                        write(connection, buffer,fsize);
                        printf("Sent data\n");
                    }
                    free(buffer);
                    fclose(fp);
                }
                memset(requestLine,0,sizeof(requestLine));
                memset(reqinfo.request_uri,0,sizeof(reqinfo.request_uri));
                bzero(total_path,strlen(total_path));
                strncpy(total_path,directory,strlen(directory));
            }
            else
            {
                char url_path_temp[50],total_path_temp[50];
                strncpy(total_path_temp,total_path,strlen(total_path));
                strcpy(url_path_temp,reqinfo.request_uri);
                strcat(total_path_temp,reqinfo.request_uri);
                printf("total path temp = %s\n",total_path_temp);

                fp = fopen(total_path_temp, "r");

                if(fp == NULL)
                {
                    printf("File does not exist\n");
                    send_notFoundError(connection,&reqinfo);
                    return -1;
                }

                char url_path[50];
                strcpy(url_path,reqinfo.request_uri);
                strcat(total_path,reqinfo.request_uri);
                printf("URL Path = %s\n",url_path);
                char* filename = strtok(url_path,"/");
                filename = strtok(NULL,"/");
                char fname[50];
                strncpy(fname,filename,strlen(filename));
                printf("filename = %s\n",filename);
                char* extension = strtok(filename,".");
                extension = strtok(NULL,".");
                printf("extension = %s",extension);
                char extension_to_search[50] = ".";
                strcat(extension_to_search,extension);
                printf("extension to search = %s\n",extension_to_search);

                int ind=-1;

                for(int i=0;i<9;i++)
                {
                    if(strncmp(content_typ[i],extension_to_search,strlen(extension_to_search))==0)
                        ind = i;
                }

                if(ind >= 0)
                {
                    printf("extension found\n");

                    fp = fopen(total_path, "r");

                    if (fp == NULL)
                    {
                        printf("%s File does not exist\n",filename);
                        send_notFoundError(connection,&reqinfo);
                    }
                    else
                    {
                        printf("File exists\n");
                        fseek(fp, 0, SEEK_END);
                        fsize = ftell(fp);
                        fseek(fp,0,SEEK_SET);

                        sprintf(header_message,"%s 200 Document Follows\n",reqinfo.request_version);
                        sprintf(content_length,"Content-length: %ld\n",fsize);
                        sprintf(content_type,"Content-Type: %s\n\n",content_ext[ind]);

                        /* Send header, content length and content type to webserver */
                        write(connection, header_message, strlen(header_message));
                        write(connection, content_length, strlen(content_length));
                        write(connection, content_type, strlen(content_type));

                        char* buffer = (char*)malloc(fsize);
                        if(buffer == NULL)
                            Cannot_Allocate_Memory_Error(connection,&reqinfo);
                        else
                        {
                            fread(buffer,1,fsize,fp);
                            write(connection, buffer,fsize);
                            printf("Sent data\n");
                        }
                        free(buffer);
                        fclose(fp);
                    }
                }
                else
                {
                    printf("extension not found\n");
                    send_ExtNotFoundError(connection,&reqinfo,fname);
                }
            }
        }
        else if(checker(reqinfo.request_method,"POST")==1)
        {
            printf("temp buffer = %s\n",temp_buffer);
            char* temp = strtok(temp_buffer,"\n");
            temp = strtok(NULL,"\n");
            temp = strtok(NULL,"\n");
            temp = strtok(NULL,"\n");
            temp = strtok(NULL,"\n");
            printf("Data to append = %s\n",temp);

            FILE *fp;
            unsigned long fsize = 0;
            int size_of_file = 0;
            printf("reqinfo.request_uri = %s\n",reqinfo.request_uri);
            if((checker(reqinfo.request_uri,"/ ")==1)|(checker(reqinfo.request_uri,"/index.html")==1))
            {
                char index_html[50] = "/index.html";
                strcat(total_path,index_html);
                printf("total path = %s\n",total_path);

                fp = fopen(total_path, "r");

                if (fp == NULL)
                {
                    printf("index.html File does not exist\n");

                    send_notFoundError(connection,&reqinfo);
                }
                else
                {
                    printf("File exists\n");
                    fseek(fp, 0, SEEK_END);
                    fsize = ftell(fp);
                    fseek(fp,0,SEEK_SET);
                    printf("File size to be sent = %ld\n",fsize);

                    /* Clear buffers */
                    bzero(header_message,strlen(header_message));
                    bzero(content_length,strlen(content_length));
                    bzero(content_type,strlen(content_type));

                    sprintf(header_message,"%s 200 Document Follows\n",reqinfo.request_version);
                    sprintf(content_length,"Content-length: %ld\n",fsize);
                    sprintf(content_type,"Content-Type: text/html\n\n");

                    /* Send header, content length and content type to webserver */
                    write(connection, header_message, strlen(header_message));
                    write(connection, content_length, strlen(content_length));
                    write(connection, content_type, strlen(content_type));

                    bzero(buff,sizeof(buff));
                    sprintf(buff,"\n\n%s\n\n\n\n",temp);
                    write(connection, buff, strlen(buff));

                    char* buffer = (char*)malloc(fsize);
                    if(buffer == NULL)
                        Cannot_Allocate_Memory_Error(connection,&reqinfo);  //Call error 500, if required
                    else
                    {
                        fread(buffer,1,fsize,fp);
                        write(connection, buffer,fsize);
                        printf("Sent data\n");
                    }
                    free(buffer);
                    fclose(fp);
                }
                memset(requestLine,0,sizeof(requestLine));
                memset(reqinfo.request_uri,0,sizeof(reqinfo.request_uri));
                bzero(total_path,strlen(total_path));
                strncpy(total_path,directory,strlen(directory));
            }
        }
        else
        {
            flag = 0;   //set flag to call needed error
            BadRequestError(connection,&reqinfo,flag);
        }
    }
    else
    {
        flag = 1;   //set flag to call needed error
        BadRequestError(connection,&reqinfo,flag);
    }

    return 0;
}

int main(int argc, char *argv[]) {

    int    listener, connection;
    pid_t  pid;
    struct sockaddr_in servaddr;
    int result=0;

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

    int temporary = 1;
    setsockopt(connection, IPPROTO_TCP, SO_REUSEADDR, (const void *)&temporary, sizeof(temporary));

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

            result = ServiceReq(connection);
            if(result == -1)
                continue;
			
			exit(0);
            close(connection);
		}
		else
			close(connection);
	}
    
	/* Close the parent process */
	if ( close(listener) < 0 )
	Error_Quit("Error closing listening socket in child.");
}