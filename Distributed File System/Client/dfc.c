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
#include <stdio.h>
#include <openssl/md5.h>
#include <unistd.h>
#include <math.h>

#define LISTENQ          (1024)
#define MAXBUFSIZE 1024
#define TIMEOUT 10

char ip_address[10];
char port1[5];
char port2[5];
char port3[5];
char port4[5];
char config_filename[10];
char username[10];
char password[10];
int dfs1_part1,dfs1_part2,dfs2_part1,dfs2_part2,\
	dfs3_part1,dfs3_part2,dfs4_part1,dfs4_part2;
char input_command[MAXBUFSIZE];
char *req_method;
int msg_int = 5;
int is_file = -1;
int send_file = -1;
char *msg = "Invalid Username/Password. Please try again.";
char M[3];
char what_i_have[MAXBUFSIZE];
char get_temp[MAXBUFSIZE];
int read_1 = 0;
int read_2 = 0;
int read_3 = 0;
int read_4 = 0;;
char *buf_1;
char *buf_2;
char *buf_3;
char *buf_4;
char PASSWORD[MAXBUFSIZE];

char server_msg[10];
char client_msg[3];

char message_receive[45];
char dfs1_list[MAXBUFSIZE];
char dfs2_list[MAXBUFSIZE];
char dfs3_list[MAXBUFSIZE];
char dfs4_list[MAXBUFSIZE];
char final_list[MAXBUFSIZE];
char temp[MAXBUFSIZE];
char fn[MAXBUFSIZE],f[MAXBUFSIZE];

int bytes_rec_dfs1=0,bytes_rec_dfs2=0,bytes_rec_dfs3=0,bytes_rec_dfs4=0;

int flag_1=0, flag_2=0, flag_3=0, flag_4=0;

struct username_password
{
    char user[MAXBUFSIZE];
    char passwd[MAXBUFSIZE];
};

struct reliable_packet
{
	int packet_length;
	char request_method[MAXBUFSIZE];
	char fname[MAXBUFSIZE];
	char fold[MAXBUFSIZE];
	char sub_fold[MAXBUFSIZE];
	int part_number;
};

struct get_packet
{
    int number;
    int size;
};

int dfs1[4][4]=
		{
			{1,2},
			{4,1},
			{3,4},
			{2,3}
		};

int dfs2[4][4]=
		{
			{2,3},
			{1,2},
			{4,1},
			{3,4}
		};

int dfs3[4][4]=
		{
			{3,4},
			{2,3},
			{1,2},
			{4,1}
		};

int dfs4[4][4]=
		{
			{4,1},
			{3,4},
			{2,3},
			{1,2}
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

void strip(char *s) {
    char *p2 = s;
    while(*s != '\0') {
        if(*s != '\t' && *s != '\n') {
            *p2++ = *s++;
        } else {
            ++s;
        }
    }
    *p2 = '\0';
}

int CalcFileMD5(char *filename, char *md5_sum)
{
    unsigned char c[MD5_DIGEST_LENGTH];
    int i,x;
    FILE *inFile = fopen (filename, "rb");
    MD5_CTX mdContext;
    int bytes;
    unsigned char data[1024];
    //char *character;

    if (inFile == NULL) {
        printf ("%s can't be opened.\n", filename);
        return -1;
    }

    MD5_Init (&mdContext);
    while ((bytes = fread (data, 1, 1024, inFile)) != 0)
        MD5_Update (&mdContext, data, bytes);
    MD5_Final (c,&mdContext);
    for(i = 0; i < (MD5_DIGEST_LENGTH); i++) printf("%02x", c[i]);
    //printf ("hash character = %c\n", c[MD5_DIGEST_LENGTH]);
    printf("\n");

    //x = (int)c[MD5_DIGEST_LENGTH-1] - 48;

    if((c[MD5_DIGEST_LENGTH-1] == 'a') | (c[MD5_DIGEST_LENGTH-1] =='b') | (c[MD5_DIGEST_LENGTH-1] =='c') | \
    	(c[MD5_DIGEST_LENGTH-1] =='d') | (c[MD5_DIGEST_LENGTH-1] =='e') | (c[MD5_DIGEST_LENGTH-1] =='f'))
    	x = (int)c[MD5_DIGEST_LENGTH-1] - 48;
    else
     	x = (int)c[MD5_DIGEST_LENGTH-1];

    fclose (inFile);
    return x;
}

void location(int x)
{
	if(x == 0)
	{
		dfs1_part1 = dfs1[0][0];
		dfs1_part2 = dfs1[0][1];
		dfs2_part1 = dfs2[0][0];
		dfs2_part2 = dfs2[0][1]; 
		dfs3_part1 = dfs3[0][0];
		dfs3_part2 = dfs3[0][1]; 
		dfs4_part1 = dfs4[0][0];
		dfs4_part2 = dfs4[0][1];  
	}
	else if(x == 1)
	{
		dfs1_part1 = dfs1[1][0];
		dfs1_part2 = dfs1[1][1];
		dfs2_part1 = dfs2[1][0];
		dfs2_part2 = dfs2[1][1]; 
		dfs3_part1 = dfs3[1][0];
		dfs3_part2 = dfs3[1][1]; 
		dfs4_part1 = dfs4[1][0];
		dfs4_part2 = dfs4[1][1];  
	}
	else if(x == 2)
	{
		dfs1_part1 = dfs1[2][0];
		dfs1_part2 = dfs1[2][1];
		dfs2_part1 = dfs2[2][0];
		dfs2_part2 = dfs2[2][1]; 
		dfs3_part1 = dfs3[2][0];
		dfs3_part2 = dfs3[2][1]; 
		dfs4_part1 = dfs4[2][0];
		dfs4_part2 = dfs4[2][1];  
	}
	if(x == 3)
	{
		dfs1_part1 = dfs1[3][0];
		dfs1_part2 = dfs1[3][1];
		dfs2_part1 = dfs2[3][0];
		dfs2_part2 = dfs2[3][1]; 
		dfs3_part1 = dfs3[3][0];
		dfs3_part2 = dfs3[3][1]; 
		dfs4_part1 = dfs4[3][0];
		dfs4_part2 = dfs4[3][1];  
	}
}

void load_config(void)
{
	FILE* file = fopen(config_filename, "r"); /* should check the result */
    char *line = NULL;
    size_t len = 0;
    int index = 0;

    bzero(ip_address,strlen(ip_address));
	bzero(port1,strlen(port1));
	bzero(port2,strlen(port2));
	bzero(port3,strlen(port3));
	bzero(port4,strlen(port4));
	bzero(username,strlen(username));
	bzero(password,strlen(password));

    while(-1 != getline(&line, &len, file))
	{
		char* input = strtok(line, " ");

        /* Retrieve ip address and port numbers from dfc.conf file */
        if(checker(input,"Server") == 1)
        {
        	input = strtok(NULL, " ");
        	
        	if(checker(input,"DFS1") == 1)
        	{
        		input = strtok(NULL, ":");
        		strncpy(ip_address,input,strlen(input));
        		input = strtok(NULL, "\n");
        		strncpy(port1,input,strlen(input));
        	}
        	if(checker(input,"DFS2") == 1)
        	{
        		input = strtok(NULL, ":");
        		input = strtok(NULL, "\n");
        		strncpy(port2,input,strlen(input));
        	}
        	if(checker(input,"DFS3") == 1)
        	{
        		input = strtok(NULL, ":");
        		input = strtok(NULL, "\n");
        		strncpy(port3,input,strlen(input));
        	}
        	if(checker(input,"DFS4") == 1)
        	{
        		input = strtok(NULL, ":");
        		input = strtok(NULL, "\n");
        		strncpy(port4,input,strlen(input));
        	}
        }

        /* Retrieve username from dfc.conf file */
        if(checker(input,"Username") == 1)
        {
        	input = strtok(NULL, " ");
        	input = strtok(NULL, " ");
        	strip(input);
        	strncpy(username,input,strlen(input));
        }

        /* Retrieve password from dfc.conf file */
        if(checker(input,"Password") == 1)
        {
        	input = strtok(NULL, " ");
        	input = strtok(NULL, " ");
        	strip(input);
        	strncpy(password,input,strlen(input));
        }
    }
    fclose(file);
}

int main(int argc, char *argv[])
{
	int clientSocket;
 	char buffer[1024];
 	struct sockaddr_in serverAddr;
 	socklen_t addr_size;

 	char *md5_sum;
 	int out;

 	strcpy(config_filename,argv[1]);
 	load_config();

 	while(1)
 	{
 		printf("\nEnter a command: ");

    	scanf(" %[^\n]s", input_command);

		req_method = strtok(input_command," ");
		printf("req_method = %s\n",req_method);

		if(checker(req_method,"PUT") == 1)
		{
			char message[MAXBUFSIZE];
			char *filename = strtok(NULL," ");
			printf("FILENAME = %s\n", filename);
			char *fo_put = strtok(NULL,"\n");
			printf("FO = %s\n",fo_put);

			out = CalcFileMD5(filename, md5_sum);
			//printf("out = %d\n",out);
			//printf("md5_sum = %s\n",md5_sum);
			int rem = out % 4;
			printf("Remainder is %d\n",rem);
			location(rem);

			FILE *fp;
            unsigned long fsize = 0, last_part=0;

            fp = fopen(filename, "rb");

            if(fp == NULL)
            	printf("File does not exist at client side\n");
            else
            {
            	fseek(fp, 0, SEEK_END);
        		fsize = ftell(fp);
        		fseek(fp,0,SEEK_SET);
        		printf("Size of %s = %ld\n",filename,fsize);
        		unsigned long size_to_send = fsize / 4;
        		printf("Size to send = %ld\n",size_to_send);
        		last_part = fsize - (3*size_to_send);

        		char *buffer_p1 = (char*)malloc(size_to_send);
				char *buffer_p2 = (char*)malloc(size_to_send);
				char *buffer_p3 = (char*)malloc(size_to_send);
				char *buffer_p4 = (char*)malloc(last_part);

        		int read1 = fread(buffer_p1,1,size_to_send,fp);
        		int read2 = fread(buffer_p2,1,size_to_send,fp);
        		int read3 = fread(buffer_p3,1,size_to_send,fp);
        		int read4 = fread(buffer_p4,1,last_part,fp);

        		printf("read1 = %d\n", read1);
        		printf("read2 = %d\n", read2);
        		printf("read3 = %d\n", read3);
        		printf("read4 = %d\n", read4);

        		printf("***************************************************************** DFS 1 - Part 1 *****************************************************************\n");
        		if(dfs1_part1 == 1)
				{
					printf("\nCLIENT REACHES dfs1_part1 == 1\n");

					struct username_password *up = malloc(sizeof(struct username_password));
					memset(up,0,sizeof(struct username_password));

					printf("USERNAME = %s\n",username);
 					strcpy(up->user,username);
 					printf("up->user = %s\n",up->user);
 					printf("PASSWORD = %s\n",password);
 					strncpy(up->passwd,password,strlen(password));
 					printf("up->passwd = %s\n",up->passwd);

 					bzero(PASSWORD,MAXBUFSIZE);
 					strncpy(PASSWORD,up->passwd,strlen(up->passwd));

					struct reliable_packet *rb = malloc(sizeof(struct reliable_packet));
					memset(rb,0,sizeof(struct reliable_packet));

					memcpy(rb->request_method, req_method, strlen(req_method));
					printf("rb->request_method = %s\n",rb->request_method);
					
					rb->packet_length = read1;
					printf("rb->packet_length = %d\n",rb->packet_length);

					char *data = (char *)malloc(read1);
					memcpy(data,buffer_p1,read1);

					strncpy(rb->fname,filename,strlen(filename));

					strncpy(rb->fold,"/DFS1/",strlen("/DFS1/"));
					if(fo_put == NULL)
						strncpy(rb->sub_fold,"NULL",strlen("NULL"));
					else
						strncpy(rb->sub_fold,fo_put,strlen(fo_put));
					rb->part_number = dfs1_part1;

					/*---- Create the socket. The three arguments are: ----*/
 					/* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
 					clientSocket = socket(PF_INET, SOCK_STREAM, 0);
 					/*---- Configure settings of the server address struct ----*/
 					/* Address family = Internet */
 					serverAddr.sin_family = AF_INET;
 					/* Set port number, using htons function to use proper byte order */
 					serverAddr.sin_port = htons(atoi(port1));
 					/* Set IP address to localhost */
 					serverAddr.sin_addr.s_addr = inet_addr(ip_address);
 					/* Set all bits of the padding field to 0 */ 
 					memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
 					/*---- Connect the socket to the server using the address struct ----*/
 					addr_size = sizeof serverAddr;
 					connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

 					printf("Packet length to send = %d\n", rb->packet_length);
    				printf("Packet request_method to send = %s\n", rb->request_method);

    				int bytes = send(clientSocket, up, sizeof(struct username_password), MSG_NOSIGNAL);
 					printf("Bytes sent = %d\n",bytes);
 					printf("Username and Password sent\n");

 					bytes = recv(clientSocket, &msg_int, sizeof(msg_int), 0);

 					if(msg_int == 1)
 					{
 						bytes = send(clientSocket, rb, sizeof(struct reliable_packet), MSG_NOSIGNAL);
 						printf("Bytes sent = %d\n",bytes);
 						printf("Packet sent\n");

 						int count = 0;
 						for(int i=0;i<read1;i++)
 						{
 							data[i] = data[i] ^ PASSWORD[count];
 							count++;
 							if(count == strlen(PASSWORD))
 								count = 0;
 						}
 						printf("ENCRYPTED\n");

 						bytes = send(clientSocket, data, read1, MSG_NOSIGNAL);
 						printf("Bytes sent = %d\n",bytes);
 						printf("Data sent\n");
 					}
 					else if(msg_int == -1)
 					{
 						bytes = recv(clientSocket, &message_receive, sizeof(message_receive), 0);
 						printf("MESSAGE RECEIVED = %s\n", message_receive);
 						bzero(message_receive,strlen(message_receive));
 					}

 					free(up);
 					free(rb);
 					free(data);
				}
				else if(dfs1_part1 == 2)
				{
					printf("\nCLIENT REACHES dfs1_part1 == 2\n");

					struct username_password *up = malloc(sizeof(struct username_password));
					memset(up,0,sizeof(struct username_password));

					printf("USERNAME = %s\n",username);
 					strcpy(up->user,username);
 					printf("up->user = %s\n",up->user);
 					printf("PASSWORD = %s\n",password);
 					strncpy(up->passwd,password,strlen(password));
 					printf("up->passwd = %s\n",up->passwd);

					struct reliable_packet *rb = malloc(sizeof(struct reliable_packet));
					memset(rb,0,sizeof(struct reliable_packet));

					memcpy(rb->request_method, req_method, strlen(req_method));
					printf("rb->request_method = %s\n",rb->request_method);
					
					rb->packet_length = read2;
					printf("rb->packet_length = %d\n",rb->packet_length);

					char *data = (char *)malloc(read2);
					memcpy(data,buffer_p2,read2);

					strncpy(rb->fname,filename,strlen(filename));

					strncpy(rb->fold,"/DFS1/",strlen("/DFS1/"));
					if(fo_put == NULL)
						strncpy(rb->sub_fold,"NULL",strlen("NULL"));
					else
						strncpy(rb->sub_fold,fo_put,strlen(fo_put));
					rb->part_number = dfs1_part1;

					/*---- Create the socket. The three arguments are: ----*/
 					/* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
 					clientSocket = socket(PF_INET, SOCK_STREAM, 0);
 					/*---- Configure settings of the server address struct ----*/
 					/* Address family = Internet */
 					serverAddr.sin_family = AF_INET;
 					/* Set port number, using htons function to use proper byte order */
 					serverAddr.sin_port = htons(atoi(port1));
 					/* Set IP address to localhost */
 					serverAddr.sin_addr.s_addr = inet_addr(ip_address);
 					/* Set all bits of the padding field to 0 */ 
 					memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
 					/*---- Connect the socket to the server using the address struct ----*/
 					addr_size = sizeof serverAddr;
 					connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

 					printf("Packet length to send = %d\n", rb->packet_length);
    				printf("Packet request_method to send = %s\n", rb->request_method);

    				int bytes = send(clientSocket, up, sizeof(struct username_password), MSG_NOSIGNAL);
 					printf("Bytes sent = %d\n",bytes);
 					printf("Username and Password sent\n");

 					bytes = recv(clientSocket, &msg_int, sizeof(msg_int), 0);

 					if(msg_int == 1)
 					{
 						bytes = send(clientSocket, rb, sizeof(struct reliable_packet), MSG_NOSIGNAL);
 						printf("Bytes sent = %d\n",bytes);
 						printf("Packet sent\n");
 						int count = 0;
 						for(int i=0;i<read2;i++)
 						{
 							data[i] = data[i] ^ PASSWORD[count];
 							count++;
 							if(count == strlen(PASSWORD))
 								count = 0;
 						}
 						printf("ENCRYPTED\n");
 						bytes = send(clientSocket, data, read2, MSG_NOSIGNAL);
 						printf("Bytes sent = %d\n",bytes);
 						printf("Data sent\n");
 					}
 					else if(msg_int == -1)
 					{
 						bytes = recv(clientSocket, &message_receive, sizeof(message_receive), 0);
 						printf("MESSAGE RECEIVED = %s\n", message_receive);
 						bzero(message_receive,strlen(message_receive));
 						printf("USERNAME AND PASSWORD NOT VERIFIED\n");
 					}

 					free(up);
 					free(rb);
 					free(data);
				}
				else if(dfs1_part1 == 3)
				{
					printf("CLIENT REACHES dfs1_part1 == 3\n");

					struct username_password *up = malloc(sizeof(struct username_password));
					memset(up,0,sizeof(struct username_password));

					printf("USERNAME = %s\n",username);
 					strcpy(up->user,username);
 					printf("up->user = %s\n",up->user);
 					printf("PASSWORD = %s\n",password);
 					strncpy(up->passwd,password,strlen(password));
 					printf("up->passwd = %s\n",up->passwd);

					struct reliable_packet *rb = malloc(sizeof(struct reliable_packet));
					memset(rb,0,sizeof(struct reliable_packet));

					memcpy(rb->request_method, req_method, strlen(req_method));
					printf("rb->request_method = %s\n",rb->request_method);
					
					rb->packet_length = read3;
					printf("rb->packet_length = %d\n",rb->packet_length);

					char *data = (char *)malloc(read3);
					memcpy(data,buffer_p3,read3);

					strncpy(rb->fname,filename,strlen(filename));

					strncpy(rb->fold,"/DFS1/",strlen("/DFS1/"));
					if(fo_put == NULL)
						strncpy(rb->sub_fold,"NULL",strlen("NULL"));
					else
						strncpy(rb->sub_fold,fo_put,strlen(fo_put));
					rb->part_number = dfs1_part1;

					/*---- Create the socket. The three arguments are: ----*/
 					/* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
 					clientSocket = socket(PF_INET, SOCK_STREAM, 0);
 					/*---- Configure settings of the server address struct ----*/
 					/* Address family = Internet */
 					serverAddr.sin_family = AF_INET;
 					/* Set port number, using htons function to use proper byte order */
 					serverAddr.sin_port = htons(atoi(port1));
 					/* Set IP address to localhost */
 					serverAddr.sin_addr.s_addr = inet_addr(ip_address);
 					/* Set all bits of the padding field to 0 */ 
 					memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
 					/*---- Connect the socket to the server using the address struct ----*/
 					addr_size = sizeof serverAddr;
 					connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

 					printf("Packet length to send = %d\n", rb->packet_length);
    				printf("Packet request_method to send = %s\n", rb->request_method);

    				int bytes = send(clientSocket, up, sizeof(struct username_password), MSG_NOSIGNAL);
 					printf("Bytes sent = %d\n",bytes);
 					printf("Username and Password sent\n");

 					bytes = recv(clientSocket, &msg_int, sizeof(msg_int), 0);

 					if(msg_int == 1)
 					{
 						bytes = send(clientSocket, rb, sizeof(struct reliable_packet), MSG_NOSIGNAL);
 						printf("Bytes sent = %d\n",bytes);
 						printf("Packet sent\n");
 						int count = 0;
 						for(int i=0;i<read3;i++)
 						{
 							data[i] = data[i] ^ PASSWORD[count];
 							count++;
 							if(count == strlen(PASSWORD))
 								count = 0;
 						}
 						printf("ENCRYPTED\n");
 						bytes = send(clientSocket, data, read3, MSG_NOSIGNAL);
 						printf("Bytes sent = %d\n",bytes);
 						printf("Data sent\n");
 					}
 					else if(msg_int == -1)
 					{
 						bytes = recv(clientSocket, &message_receive, sizeof(message_receive), 0);
 						printf("MESSAGE RECEIVED = %s\n", message_receive);
 						bzero(message_receive,strlen(message_receive));
 						printf("USERNAME AND PASSWORD NOT VERIFIED\n");
 					}

 					free(up);
 					free(rb);
 					free(data);
				}
				else if(dfs1_part1 == 4)
				{
					printf("\nCLIENT REACHES dfs1_part1 == 4\n");

					struct username_password *up = malloc(sizeof(struct username_password));
					memset(up,0,sizeof(struct username_password));

					printf("USERNAME = %s\n",username);
 					strcpy(up->user,username);
 					printf("up->user = %s\n",up->user);
 					printf("PASSWORD = %s\n",password);
 					strncpy(up->passwd,password,strlen(password));
 					printf("up->passwd = %s\n",up->passwd);

					struct reliable_packet *rb = malloc(sizeof(struct reliable_packet));
					memset(rb,0,sizeof(struct reliable_packet));

					memcpy(rb->request_method, req_method, strlen(req_method));
					printf("rb->request_method = %s\n",rb->request_method);
					
					rb->packet_length = read4;
					printf("rb->packet_length = %d\n",rb->packet_length);

					char *data = (char *)malloc(read4);
					memcpy(data,buffer_p4,read4);

					strncpy(rb->fname,filename,strlen(filename));

					strncpy(rb->fold,"/DFS1/",strlen("/DFS1/"));
					if(fo_put == NULL)
						strncpy(rb->sub_fold,"NULL",strlen("NULL"));
					else
						strncpy(rb->sub_fold,fo_put,strlen(fo_put));
					rb->part_number = dfs1_part1;

					/*---- Create the socket. The three arguments are: ----*/
 					/* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
 					clientSocket = socket(PF_INET, SOCK_STREAM, 0);
 					/*---- Configure settings of the server address struct ----*/
 					/* Address family = Internet */
 					serverAddr.sin_family = AF_INET;
 					/* Set port number, using htons function to use proper byte order */
 					serverAddr.sin_port = htons(atoi(port1));
 					/* Set IP address to localhost */
 					serverAddr.sin_addr.s_addr = inet_addr(ip_address);
 					/* Set all bits of the padding field to 0 */ 
 					memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
 					/*---- Connect the socket to the server using the address struct ----*/
 					addr_size = sizeof serverAddr;
 					connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

 					printf("Packet length to send = %d\n", rb->packet_length);
    				printf("Packet request_method to send = %s\n", rb->request_method);

    				int bytes = send(clientSocket, up, sizeof(struct username_password), MSG_NOSIGNAL);
 					//printf("Bytes sent = %d\n",bytes);
 					printf("Username and Password sent\n");

 					bytes = recv(clientSocket, &msg_int, sizeof(msg_int), 0);

 					if(msg_int == 1)
 					{
 						bytes = send(clientSocket, rb, sizeof(struct reliable_packet), MSG_NOSIGNAL);
 						//printf("Bytes sent = %d\n",bytes);
 						printf("Packet sent\n");
 						int count = 0;
 						for(int i=0;i<read4;i++)
 						{
 							data[i] = data[i] ^ PASSWORD[count];
 							count++;
 							if(count == strlen(PASSWORD))
 								count = 0;
 						}
 						printf("ENCRYPTED\n");
 						bytes = send(clientSocket, data, read4, MSG_NOSIGNAL);
 						printf("Bytes sent = %d\n",bytes);
 						printf("Data sent\n");
 					}
 					else if(msg_int == -1)
 					{
 						bytes = recv(clientSocket, &message_receive, sizeof(message_receive), 0);
 						printf("MESSAGE RECEIVED = %s\n", message_receive);
 						bzero(message_receive,strlen(message_receive));
 						printf("USERNAME AND PASSWORD NOT VERIFIED\n");
 					}

 					free(up);
 					free(rb);
 					free(data);
				}
				printf("***************************************************************** DFS 1 - Part 2 *****************************************************************\n");
				if(dfs1_part2 == 1)
				{
					printf("\nCLIENT REACHES dfs1_part2 == 1\n");

					struct username_password *up = malloc(sizeof(struct username_password));
					memset(up,0,sizeof(struct username_password));

					printf("USERNAME = %s\n",username);
 					strcpy(up->user,username);
 					printf("up->user = %s\n",up->user);
 					printf("PASSWORD = %s\n",password);
 					strncpy(up->passwd,password,strlen(password));
 					printf("up->passwd = %s\n",up->passwd);

					struct reliable_packet *rb = malloc(sizeof(struct reliable_packet));
					memset(rb,0,sizeof(struct reliable_packet));

					memcpy(rb->request_method, req_method, strlen(req_method));
					printf("rb->request_method = %s\n",rb->request_method);
					
					rb->packet_length = read1;
					printf("rb->packet_length = %d\n",rb->packet_length);

					char *data = (char *)malloc(read1);
					memcpy(data,buffer_p1,read1);

					strncpy(rb->fname,filename,strlen(filename));

					strncpy(rb->fold,"/DFS1/",strlen("/DFS1/"));
					if(fo_put == NULL)
						strncpy(rb->sub_fold,"NULL",strlen("NULL"));
					else
						strncpy(rb->sub_fold,fo_put,strlen(fo_put));
					rb->part_number = dfs1_part2;

					/*---- Create the socket. The three arguments are: ----*/
 					/* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
 					clientSocket = socket(PF_INET, SOCK_STREAM, 0);
 					/*---- Configure settings of the server address struct ----*/
 					/* Address family = Internet */
 					serverAddr.sin_family = AF_INET;
 					/* Set port number, using htons function to use proper byte order */
 					serverAddr.sin_port = htons(atoi(port1));
 					/* Set IP address to localhost */
 					serverAddr.sin_addr.s_addr = inet_addr(ip_address);
 					/* Set all bits of the padding field to 0 */ 
 					memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
 					/*---- Connect the socket to the server using the address struct ----*/
 					addr_size = sizeof serverAddr;
 					connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

 					printf("Packet length to send = %d\n", rb->packet_length);
    				printf("Packet request_method to send = %s\n", rb->request_method);

    				int bytes = send(clientSocket, up, sizeof(struct username_password), MSG_NOSIGNAL);
 					printf("Bytes sent = %d\n",bytes);
 					printf("Username and Password sent\n");

 					bytes = recv(clientSocket, &msg_int, sizeof(msg_int), 0);

 					if(msg_int == 1)
 					{
 						bytes = send(clientSocket, rb, sizeof(struct reliable_packet), MSG_NOSIGNAL);
 						printf("Bytes sent = %d\n",bytes);
 						printf("Packet sent\n");
 						int count = 0;
 						for(int i=0;i<read1;i++)
 						{
 							data[i] = data[i] ^ PASSWORD[count];
 							count++;
 							if(count == strlen(PASSWORD))
 								count = 0;
 						}
 						printf("ENCRYPTED\n");
 						bytes = send(clientSocket, data, read1, MSG_NOSIGNAL);
 						printf("Bytes sent = %d\n",bytes);
 						printf("Data sent\n");
 					}
 					else if(msg_int == -1)
 					{
 						bytes = recv(clientSocket, &message_receive, sizeof(message_receive), 0);
 						printf("MESSAGE RECEIVED = %s\n", message_receive);
 						bzero(message_receive,strlen(message_receive));
 						printf("USERNAME AND PASSWORD NOT VERIFIED\n");
 					}

 					free(up);
 					free(rb);
 					free(data);	
				}
				else if(dfs1_part2 == 2)
				{	
					printf("\nCLIENT REACHES dfs1_part2 == 2\n");

					struct username_password *up = malloc(sizeof(struct username_password));
					memset(up,0,sizeof(struct username_password));

					printf("USERNAME = %s\n",username);
 					strcpy(up->user,username);
 					printf("up->user = %s\n",up->user);
 					printf("PASSWORD = %s\n",password);
 					strncpy(up->passwd,password,strlen(password));
 					printf("up->passwd = %s\n",up->passwd);

					struct reliable_packet *rb = malloc(sizeof(struct reliable_packet));
					memset(rb,0,sizeof(struct reliable_packet));

					memcpy(rb->request_method, req_method, strlen(req_method));
					printf("rb->request_method = %s\n",rb->request_method);
					
					rb->packet_length = read2;
					printf("rb->packet_length = %d\n",rb->packet_length);

					char *data = (char *)malloc(read2);
					memcpy(data,buffer_p2,read2);

					strncpy(rb->fname,filename,strlen(filename));

					strncpy(rb->fold,"/DFS1/",strlen("/DFS1/"));
					if(fo_put == NULL)
						strncpy(rb->sub_fold,"NULL",strlen("NULL"));
					else
						strncpy(rb->sub_fold,fo_put,strlen(fo_put));
					rb->part_number = dfs1_part2;

					/*---- Create the socket. The three arguments are: ----*/
 					/* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
 					clientSocket = socket(PF_INET, SOCK_STREAM, 0);
 					/*---- Configure settings of the server address struct ----*/
 					/* Address family = Internet */
 					serverAddr.sin_family = AF_INET;
 					/* Set port number, using htons function to use proper byte order */
 					serverAddr.sin_port = htons(atoi(port1));
 					/* Set IP address to localhost */
 					serverAddr.sin_addr.s_addr = inet_addr(ip_address);
 					/* Set all bits of the padding field to 0 */ 
 					memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
 					/*---- Connect the socket to the server using the address struct ----*/
 					addr_size = sizeof serverAddr;
 					connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

 					printf("Packet length to send = %d\n", rb->packet_length);
    				printf("Packet request_method to send = %s\n", rb->request_method);

    				int bytes = send(clientSocket, up, sizeof(struct username_password), MSG_NOSIGNAL);
 					printf("Bytes sent = %d\n",bytes);
 					printf("Username and Password sent\n");

 					bytes = recv(clientSocket, &msg_int, sizeof(msg_int), 0);

 					if(msg_int == 1)
 					{
 						bytes = send(clientSocket, rb, sizeof(struct reliable_packet), MSG_NOSIGNAL);
 						printf("Bytes sent = %d\n",bytes);
 						printf("Packet sent\n");
 						int count = 0;
 						for(int i=0;i<read2;i++)
 						{
 							data[i] = data[i] ^ PASSWORD[count];
 							count++;
 							if(count == strlen(PASSWORD))
 								count = 0;
 						}
 						printf("ENCRYPTED\n");
 						bytes = send(clientSocket, data, read2, MSG_NOSIGNAL);
 						printf("Bytes sent = %d\n",bytes);
 						printf("Data sent\n");
 					}
 					else if(msg_int == -1)
 					{
 						bytes = recv(clientSocket, &message_receive, sizeof(message_receive), 0);
 						printf("MESSAGE RECEIVED = %s\n", message_receive);
 						bzero(message_receive,strlen(message_receive));
 						printf("USERNAME AND PASSWORD NOT VERIFIED\n");
 					}

 					free(up);
 					free(rb);
 					free(data);
				}
				else if(dfs1_part2 == 3)
				{
					printf("\nCLIENT REACHES dfs1_part2 == 3\n");

					struct username_password *up = malloc(sizeof(struct username_password));
					memset(up,0,sizeof(struct username_password));

					printf("USERNAME = %s\n",username);
 					strcpy(up->user,username);
 					printf("up->user = %s\n",up->user);
 					printf("PASSWORD = %s\n",password);
 					strncpy(up->passwd,password,strlen(password));
 					printf("up->passwd = %s\n",up->passwd);

					struct reliable_packet *rb = malloc(sizeof(struct reliable_packet));
					memset(rb,0,sizeof(struct reliable_packet));

					memcpy(rb->request_method, req_method, strlen(req_method));
					printf("rb->request_method = %s\n",rb->request_method);
					
					rb->packet_length = read3;
					printf("rb->packet_length = %d\n",rb->packet_length);

					char *data = (char *)malloc(read3);
					memcpy(data,buffer_p3,read3);

					strncpy(rb->fname,filename,strlen(filename));

					strncpy(rb->fold,"/DFS1/",strlen("/DFS1/"));
					if(fo_put == NULL)
						strncpy(rb->sub_fold,"NULL",strlen("NULL"));
					else
						strncpy(rb->sub_fold,fo_put,strlen(fo_put));
					rb->part_number = dfs1_part2;

					/*---- Create the socket. The three arguments are: ----*/
 					/* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
 					clientSocket = socket(PF_INET, SOCK_STREAM, 0);
 					/*---- Configure settings of the server address struct ----*/
 					/* Address family = Internet */
 					serverAddr.sin_family = AF_INET;
 					/* Set port number, using htons function to use proper byte order */
 					serverAddr.sin_port = htons(atoi(port1));
 					/* Set IP address to localhost */
 					serverAddr.sin_addr.s_addr = inet_addr(ip_address);
 					/* Set all bits of the padding field to 0 */ 
 					memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
 					/*---- Connect the socket to the server using the address struct ----*/
 					addr_size = sizeof serverAddr;
 					connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

 					printf("Packet length to send = %d\n", rb->packet_length);
    				printf("Packet request_method to send = %s\n", rb->request_method);

    				int bytes = send(clientSocket, up, sizeof(struct username_password), MSG_NOSIGNAL);
 					printf("Bytes sent = %d\n",bytes);
 					printf("Username and Password sent\n");

 					bytes = recv(clientSocket, &msg_int, sizeof(msg_int), 0);

 					if(msg_int == 1)
 					{
 						bytes = send(clientSocket, rb, sizeof(struct reliable_packet), MSG_NOSIGNAL);
 						printf("Bytes sent = %d\n",bytes);
 						printf("Packet sent\n");
 						int count = 0;
 						for(int i=0;i<read3;i++)
 						{
 							data[i] = data[i] ^ PASSWORD[count];
 							count++;
 							if(count == strlen(PASSWORD))
 								count = 0;
 						}
 						printf("ENCRYPTED\n");
 						bytes = send(clientSocket, data, read3, MSG_NOSIGNAL);
 						printf("Bytes sent = %d\n",bytes);
 						printf("Data sent\n");
 					}
 					else if(msg_int == -1)
 					{
 						bytes = recv(clientSocket, &message_receive, sizeof(message_receive), 0);
 						printf("MESSAGE RECEIVED = %s\n", message_receive);
 						bzero(message_receive,strlen(message_receive));
 						printf("USERNAME AND PASSWORD NOT VERIFIED\n");
 					}

 					free(up);
 					free(rb);
 					free(data);
				}
				else if(dfs1_part2 == 4)
				{
					printf("\nCLIENT REACHES dfs1_part2 == 4\n");

					struct username_password *up = malloc(sizeof(struct username_password));
					memset(up,0,sizeof(struct username_password));

					printf("USERNAME = %s\n",username);
 					strcpy(up->user,username);
 					printf("up->user = %s\n",up->user);
 					printf("PASSWORD = %s\n",password);
 					strncpy(up->passwd,password,strlen(password));
 					printf("up->passwd = %s\n",up->passwd);

					struct reliable_packet *rb = malloc(sizeof(struct reliable_packet));
					memset(rb,0,sizeof(struct reliable_packet));

					memcpy(rb->request_method, req_method, strlen(req_method));
					printf("rb->request_method = %s\n",rb->request_method);
					
					rb->packet_length = read4;
					printf("rb->packet_length = %d\n",rb->packet_length);

					char *data = (char *)malloc(read4);
					memcpy(data,buffer_p4,read4);

					strncpy(rb->fname,filename,strlen(filename));

					strncpy(rb->fold,"/DFS1/",strlen("/DFS1/"));
					if(fo_put == NULL)
						strncpy(rb->sub_fold,"NULL",strlen("NULL"));
					else
						strncpy(rb->sub_fold,fo_put,strlen(fo_put));
					rb->part_number = dfs1_part2;

					/*---- Create the socket. The three arguments are: ----*/
 					/* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
 					clientSocket = socket(PF_INET, SOCK_STREAM, 0);
 					/*---- Configure settings of the server address struct ----*/
 					/* Address family = Internet */
 					serverAddr.sin_family = AF_INET;
 					/* Set port number, using htons function to use proper byte order */
 					serverAddr.sin_port = htons(atoi(port1));
 					/* Set IP address to localhost */
 					serverAddr.sin_addr.s_addr = inet_addr(ip_address);
 					/* Set all bits of the padding field to 0 */ 
 					memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
 					/*---- Connect the socket to the server using the address struct ----*/
 					addr_size = sizeof serverAddr;
 					connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

 					printf("Packet length to send = %d\n", rb->packet_length);
    				printf("Packet request_method to send = %s\n", rb->request_method);

    				int bytes = send(clientSocket, up, sizeof(struct username_password), MSG_NOSIGNAL);
 					//printf("Bytes sent = %d\n",bytes);
 					printf("Username and Password sent\n");

 					bytes = recv(clientSocket, &msg_int, sizeof(msg_int), 0);

 					if(msg_int == 1)
 					{
 						bytes = send(clientSocket, rb, sizeof(struct reliable_packet), MSG_NOSIGNAL);
 						//printf("Bytes sent = %d\n",bytes);
 						printf("Packet sent\n");
 						int count = 0;
 						for(int i=0;i<read4;i++)
 						{
 							data[i] = data[i] ^ PASSWORD[count];
 							count++;
 							if(count == strlen(PASSWORD))
 								count = 0;
 						}
 						printf("ENCRYPTED\n");
 						bytes = send(clientSocket, data, read4, MSG_NOSIGNAL);
 						printf("Bytes sent = %d\n",bytes);
 						printf("Data sent\n");
 					}
 					else if(msg_int == -1)
 					{
 						bytes = recv(clientSocket, &message_receive, sizeof(message_receive), 0);
 						printf("MESSAGE RECEIVED = %s\n", message_receive);
 						bzero(message_receive,strlen(message_receive));
 						printf("USERNAME AND PASSWORD NOT VERIFIED\n");
 					}

 					free(up);
 					free(rb);
 					free(data);
				}
				printf("***************************************************************** DFS 2 - Part 1 *****************************************************************\n");
				if(dfs2_part1 == 1)
				{
					printf("\nCLIENT REACHES dfs2_part1 == 1\n");

					struct username_password *up = malloc(sizeof(struct username_password));
					memset(up,0,sizeof(struct username_password));

					printf("USERNAME = %s\n",username);
 					strcpy(up->user,username);
 					printf("up->user = %s\n",up->user);
 					printf("PASSWORD = %s\n",password);
 					strncpy(up->passwd,password,strlen(password));
 					printf("up->passwd = %s\n",up->passwd);

					struct reliable_packet *rb = malloc(sizeof(struct reliable_packet));
					memset(rb,0,sizeof(struct reliable_packet));

					memcpy(rb->request_method, req_method, strlen(req_method));
					printf("rb->request_method = %s\n",rb->request_method);
					
					rb->packet_length = read1;
					printf("rb->packet_length = %d\n",rb->packet_length);

					char *data = (char *)malloc(read1);
					memcpy(data,buffer_p1,read1);

					strncpy(rb->fname,filename,strlen(filename));

					strncpy(rb->fold,"/DFS2/",strlen("/DFS2/"));
					if(fo_put == NULL)
						strncpy(rb->sub_fold,"NULL",strlen("NULL"));
					else
						strncpy(rb->sub_fold,fo_put,strlen(fo_put));
					rb->part_number = dfs2_part1;

					/*---- Create the socket. The three arguments are: ----*/
 					/* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
 					clientSocket = socket(PF_INET, SOCK_STREAM, 0);
 					/*---- Configure settings of the server address struct ----*/
 					/* Address family = Internet */
 					serverAddr.sin_family = AF_INET;
 					/* Set port number, using htons function to use proper byte order */
 					serverAddr.sin_port = htons(atoi(port2));
 					/* Set IP address to localhost */
 					serverAddr.sin_addr.s_addr = inet_addr(ip_address);
 					/* Set all bits of the padding field to 0 */ 
 					memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
 					/*---- Connect the socket to the server using the address struct ----*/
 					addr_size = sizeof serverAddr;
 					connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

 					printf("Packet length to send = %d\n", rb->packet_length);
    				printf("Packet request_method to send = %s\n", rb->request_method);

    				int bytes = send(clientSocket, up, sizeof(struct username_password), MSG_NOSIGNAL);
 					//printf("Bytes sent = %d\n",bytes);
 					printf("Username and Password sent\n");

 					bytes = recv(clientSocket, &msg_int, sizeof(msg_int), 0);

 					if(msg_int == 1)
 					{
 						bytes = send(clientSocket, rb, sizeof(struct reliable_packet), MSG_NOSIGNAL);
 						//printf("Bytes sent = %d\n",bytes);
 						printf("Packet sent\n");
 						int count = 0;
 						for(int i=0;i<read1;i++)
 						{
 							data[i] = data[i] ^ PASSWORD[count];
 							count++;
 							if(count == strlen(PASSWORD))
 								count = 0;
 						}
 						printf("ENCRYPTED\n");
 						bytes = send(clientSocket, data, read1, MSG_NOSIGNAL);
 						printf("Bytes sent = %d\n",bytes);
 						printf("Data sent\n");
 					}
 					else if(msg_int == -1)
 					{
 						bytes = recv(clientSocket, &message_receive, sizeof(message_receive), 0);
 						printf("MESSAGE RECEIVED = %s\n", message_receive);
 						bzero(message_receive,strlen(message_receive));
 						printf("USERNAME AND PASSWORD NOT VERIFIED\n");
 					}

 					free(up);
 					free(rb);
 					free(data);
				}
				else if(dfs2_part1 == 2)
				{
					printf("\nCLIENT REACHES dfs2_part1 == 2\n");

					struct username_password *up = malloc(sizeof(struct username_password));
					memset(up,0,sizeof(struct username_password));

					printf("USERNAME = %s\n",username);
 					strcpy(up->user,username);
 					printf("up->user = %s\n",up->user);
 					printf("PASSWORD = %s\n",password);
 					strncpy(up->passwd,password,strlen(password));
 					printf("up->passwd = %s\n",up->passwd);

					struct reliable_packet *rb = malloc(sizeof(struct reliable_packet));
					memset(rb,0,sizeof(struct reliable_packet));

					memcpy(rb->request_method, req_method, strlen(req_method));
					printf("rb->request_method = %s\n",rb->request_method);
					
					rb->packet_length = read2;
					printf("rb->packet_length = %d\n",rb->packet_length);

					char *data = (char *)malloc(read2);
					memcpy(data,buffer_p2,read2);

					strncpy(rb->fname,filename,strlen(filename));

					strncpy(rb->fold,"/DFS2/",strlen("/DFS2/"));
					if(fo_put == NULL)
						strncpy(rb->sub_fold,"NULL",strlen("NULL"));
					else
						strncpy(rb->sub_fold,fo_put,strlen(fo_put));
					rb->part_number = dfs2_part1;

					/*---- Create the socket. The three arguments are: ----*/
 					/* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
 					clientSocket = socket(PF_INET, SOCK_STREAM, 0);
 					/*---- Configure settings of the server address struct ----*/
 					/* Address family = Internet */
 					serverAddr.sin_family = AF_INET;
 					/* Set port number, using htons function to use proper byte order */
 					serverAddr.sin_port = htons(atoi(port2));
 					/* Set IP address to localhost */
 					serverAddr.sin_addr.s_addr = inet_addr(ip_address);
 					/* Set all bits of the padding field to 0 */ 
 					memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
 					/*---- Connect the socket to the server using the address struct ----*/
 					addr_size = sizeof serverAddr;
 					connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

 					printf("Packet length to send = %d\n", rb->packet_length);
    				printf("Packet request_method to send = %s\n", rb->request_method);

    				int bytes = send(clientSocket, up, sizeof(struct username_password), MSG_NOSIGNAL);
 					//printf("Bytes sent = %d\n",bytes);
 					printf("Username and Password sent\n");

 					bytes = recv(clientSocket, &msg_int, sizeof(msg_int), 0);

 					if(msg_int == 1)
 					{
 						bytes = send(clientSocket, rb, sizeof(struct reliable_packet), MSG_NOSIGNAL);
 						//printf("Bytes sent = %d\n",bytes);
 						printf("Packet sent\n");
 						int count = 0;
 						for(int i=0;i<read2;i++)
 						{
 							data[i] = data[i] ^ PASSWORD[count];
 							count++;
 							if(count == strlen(PASSWORD))
 								count = 0;
 						}
 						printf("ENCRYPTED\n");
 						bytes = send(clientSocket, data, read2, MSG_NOSIGNAL);
 						printf("Bytes sent = %d\n",bytes);
 						printf("Data sent\n");
 					}
 					else if(msg_int == -1)
 					{
 						bytes = recv(clientSocket, &message_receive, sizeof(message_receive), 0);
 						printf("MESSAGE RECEIVED = %s\n", message_receive);
 						bzero(message_receive,strlen(message_receive));
 						printf("USERNAME AND PASSWORD NOT VERIFIED\n");
 					}

 					free(up);
 					free(rb);
 					free(data);

				}
				else if(dfs2_part1 == 3)
				{
					printf("\nCLIENT REACHES dfs2_part1 == 3\n");

					struct username_password *up = malloc(sizeof(struct username_password));
					memset(up,0,sizeof(struct username_password));

					printf("USERNAME = %s\n",username);
 					strcpy(up->user,username);
 					printf("up->user = %s\n",up->user);
 					printf("PASSWORD = %s\n",password);
 					strncpy(up->passwd,password,strlen(password));
 					printf("up->passwd = %s\n",up->passwd);

					struct reliable_packet *rb = malloc(sizeof(struct reliable_packet));
					memset(rb,0,sizeof(struct reliable_packet));

					memcpy(rb->request_method, req_method, strlen(req_method));
					printf("rb->request_method = %s\n",rb->request_method);
					
					rb->packet_length = read3;
					printf("rb->packet_length = %d\n",rb->packet_length);

					char *data = (char *)malloc(read3);
					memcpy(data,buffer_p3,read3);

					strncpy(rb->fname,filename,strlen(filename));

					strncpy(rb->fold,"/DFS2/",strlen("/DFS2/"));
					if(fo_put == NULL)
						strncpy(rb->sub_fold,"NULL",strlen("NULL"));
					else
						strncpy(rb->sub_fold,fo_put,strlen(fo_put));
					rb->part_number = dfs2_part1;

					/*---- Create the socket. The three arguments are: ----*/
 					/* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
 					clientSocket = socket(PF_INET, SOCK_STREAM, 0);
 					/*---- Configure settings of the server address struct ----*/
 					/* Address family = Internet */
 					serverAddr.sin_family = AF_INET;
 					/* Set port number, using htons function to use proper byte order */
 					serverAddr.sin_port = htons(atoi(port2));
 					/* Set IP address to localhost */
 					serverAddr.sin_addr.s_addr = inet_addr(ip_address);
 					/* Set all bits of the padding field to 0 */ 
 					memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
 					/*---- Connect the socket to the server using the address struct ----*/
 					addr_size = sizeof serverAddr;
 					connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

 					printf("Packet length to send = %d\n", rb->packet_length);
    				printf("Packet request_method to send = %s\n", rb->request_method);

    				int bytes = send(clientSocket, up, sizeof(struct username_password), MSG_NOSIGNAL);
 					//printf("Bytes sent = %d\n",bytes);
 					printf("Username and Password sent\n");

 					bytes = recv(clientSocket, &msg_int, sizeof(msg_int), 0);

 					if(msg_int == 1)
 					{
 						bytes = send(clientSocket, rb, sizeof(struct reliable_packet), MSG_NOSIGNAL);
 						//printf("Bytes sent = %d\n",bytes);
 						printf("Packet sent\n");
 						int count = 0;
 						for(int i=0;i<read3;i++)
 						{
 							data[i] = data[i] ^ PASSWORD[count];
 							count++;
 							if(count == strlen(PASSWORD))
 								count = 0;
 						}
 						printf("ENCRYPTED\n");
 						bytes = send(clientSocket, data, read3, MSG_NOSIGNAL);
 						printf("Bytes sent = %d\n",bytes);
 						printf("Data sent\n");
 					}
 					else if(msg_int == -1)
 					{
 						bytes = recv(clientSocket, &message_receive, sizeof(message_receive), 0);
 						printf("MESSAGE RECEIVED = %s\n", message_receive);
 						bzero(message_receive,strlen(message_receive));
 						printf("USERNAME AND PASSWORD NOT VERIFIED\n");
 					}

 					free(up);
 					free(rb);
 					free(data);
				}
				else if(dfs2_part1 == 4)
				{
					printf("\nCLIENT REACHES dfs2_part1 == 4\n");

					struct username_password *up = malloc(sizeof(struct username_password));
					memset(up,0,sizeof(struct username_password));

					printf("USERNAME = %s\n",username);
 					strcpy(up->user,username);
 					printf("up->user = %s\n",up->user);
 					printf("PASSWORD = %s\n",password);
 					strncpy(up->passwd,password,strlen(password));
 					printf("up->passwd = %s\n",up->passwd);

					struct reliable_packet *rb = malloc(sizeof(struct reliable_packet));
					memset(rb,0,sizeof(struct reliable_packet));

					memcpy(rb->request_method, req_method, strlen(req_method));
					printf("rb->request_method = %s\n",rb->request_method);
					
					rb->packet_length = read4;
					printf("rb->packet_length = %d\n",rb->packet_length);

					char *data = (char *)malloc(read1);
					memcpy(data,buffer_p4,read4);

					strncpy(rb->fname,filename,strlen(filename));

					strncpy(rb->fold,"/DFS2/",strlen("/DFS2/"));
					if(fo_put == NULL)
						strncpy(rb->sub_fold,"NULL",strlen("NULL"));
					else
						strncpy(rb->sub_fold,fo_put,strlen(fo_put));
					rb->part_number = dfs2_part1;

					/*---- Create the socket. The three arguments are: ----*/
 					/* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
 					clientSocket = socket(PF_INET, SOCK_STREAM, 0);
 					/*---- Configure settings of the server address struct ----*/
 					/* Address family = Internet */
 					serverAddr.sin_family = AF_INET;
 					/* Set port number, using htons function to use proper byte order */
 					serverAddr.sin_port = htons(atoi(port2));
 					/* Set IP address to localhost */
 					serverAddr.sin_addr.s_addr = inet_addr(ip_address);
 					/* Set all bits of the padding field to 0 */ 
 					memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
 					/*---- Connect the socket to the server using the address struct ----*/
 					addr_size = sizeof serverAddr;
 					connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

 					printf("Packet length to send = %d\n", rb->packet_length);
    				printf("Packet request_method to send = %s\n", rb->request_method);

    				int bytes = send(clientSocket, up, sizeof(struct username_password), MSG_NOSIGNAL);
 					//printf("Bytes sent = %d\n",bytes);
 					printf("Username and Password sent\n");

 					bytes = recv(clientSocket, &msg_int, sizeof(msg_int), 0);

 					if(msg_int == 1)
 					{
 						bytes = send(clientSocket, rb, sizeof(struct reliable_packet), MSG_NOSIGNAL);
 						//printf("Bytes sent = %d\n",bytes);
 						printf("Packet sent\n");
 						int count = 0;
 						for(int i=0;i<read4;i++)
 						{
 							data[i] = data[i] ^ PASSWORD[count];
 							count++;
 							if(count == strlen(PASSWORD))
 								count = 0;
 						}
 						printf("ENCRYPTED\n");
 						bytes = send(clientSocket, data, read4, MSG_NOSIGNAL);
 						printf("Bytes sent = %d\n",bytes);
 						printf("Data sent\n");
 					}
 					else if(msg_int == -1)
 					{
 						bytes = recv(clientSocket, &message_receive, sizeof(message_receive), 0);
 						printf("MESSAGE RECEIVED = %s\n", message_receive);
 						bzero(message_receive,strlen(message_receive));
 						printf("USERNAME AND PASSWORD NOT VERIFIED\n");
 					}

 					free(up);
 					free(rb);
 					free(data);
				}
				printf("***************************************************************** DFS 2 - Part 2 *****************************************************************\n");
				if(dfs2_part2 == 1)
				{
					printf("\nCLIENT REACHES dfs2_part2 == 1\n");

					struct username_password *up = malloc(sizeof(struct username_password));
					memset(up,0,sizeof(struct username_password));

					printf("USERNAME = %s\n",username);
 					strcpy(up->user,username);
 					printf("up->user = %s\n",up->user);
 					printf("PASSWORD = %s\n",password);
 					strncpy(up->passwd,password,strlen(password));
 					printf("up->passwd = %s\n",up->passwd);

					struct reliable_packet *rb = malloc(sizeof(struct reliable_packet));
					memset(rb,0,sizeof(struct reliable_packet));

					memcpy(rb->request_method, req_method, strlen(req_method));
					printf("rb->request_method = %s\n",rb->request_method);
					
					rb->packet_length = read1;
					printf("rb->packet_length = %d\n",rb->packet_length);

					char *data = (char *)malloc(read1);
					memcpy(data,buffer_p1,read1);

					strncpy(rb->fname,filename,strlen(filename));

					strncpy(rb->fold,"/DFS2/",strlen("/DFS2/"));
					if(fo_put == NULL)
						strncpy(rb->sub_fold,"NULL",strlen("NULL"));
					else
						strncpy(rb->sub_fold,fo_put,strlen(fo_put));
					rb->part_number = dfs2_part2;

					/*---- Create the socket. The three arguments are: ----*/
 					/* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
 					clientSocket = socket(PF_INET, SOCK_STREAM, 0);
 					/*---- Configure settings of the server address struct ----*/
 					/* Address family = Internet */
 					serverAddr.sin_family = AF_INET;
 					/* Set port number, using htons function to use proper byte order */
 					serverAddr.sin_port = htons(atoi(port2));
 					/* Set IP address to localhost */
 					serverAddr.sin_addr.s_addr = inet_addr(ip_address);
 					/* Set all bits of the padding field to 0 */ 
 					memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
 					/*---- Connect the socket to the server using the address struct ----*/
 					addr_size = sizeof serverAddr;
 					connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

 					printf("Packet length to send = %d\n", rb->packet_length);
    				printf("Packet request_method to send = %s\n", rb->request_method);

    				int bytes = send(clientSocket, up, sizeof(struct username_password), MSG_NOSIGNAL);
 					//printf("Bytes sent = %d\n",bytes);
 					printf("Username and Password sent\n");

 					bytes = recv(clientSocket, &msg_int, sizeof(msg_int), 0);

 					if(msg_int == 1)
 					{
 						bytes = send(clientSocket, rb, sizeof(struct reliable_packet), MSG_NOSIGNAL);
 						//printf("Bytes sent = %d\n",bytes);
 						printf("Packet sent\n");
 						int count = 0;
 						for(int i=0;i<read1;i++)
 						{
 							data[i] = data[i] ^ PASSWORD[count];
 							count++;
 							if(count == strlen(PASSWORD))
 								count = 0;
 						}
 						printf("ENCRYPTED\n");
 						bytes = send(clientSocket, data, read1, MSG_NOSIGNAL);
 						printf("Bytes sent = %d\n",bytes);
 						printf("Data sent\n");
 					}
 					else if(msg_int == -1)
 					{
 						bytes = recv(clientSocket, &message_receive, sizeof(message_receive), 0);
 						printf("MESSAGE RECEIVED = %s\n", message_receive);
 						bzero(message_receive,strlen(message_receive));
 					}

 					free(up);
 					free(rb);
 					free(data);
				}
				else if(dfs2_part2 == 2)
				{
					printf("\nCLIENT REACHES dfs2_part2 == 2\n");

					struct username_password *up = malloc(sizeof(struct username_password));
					memset(up,0,sizeof(struct username_password));

					printf("USERNAME = %s\n",username);
 					strcpy(up->user,username);
 					printf("up->user = %s\n",up->user);
 					printf("PASSWORD = %s\n",password);
 					strncpy(up->passwd,password,strlen(password));
 					printf("up->passwd = %s\n",up->passwd);

					struct reliable_packet *rb = malloc(sizeof(struct reliable_packet));
					memset(rb,0,sizeof(struct reliable_packet));

					memcpy(rb->request_method, req_method, strlen(req_method));
					printf("rb->request_method = %s\n",rb->request_method);
					
					rb->packet_length = read2;
					printf("rb->packet_length = %d\n",rb->packet_length);

					char *data = (char *)malloc(read2);
					memcpy(data,buffer_p2,read2);

					strncpy(rb->fname,filename,strlen(filename));

					strncpy(rb->fold,"/DFS2/",strlen("/DFS2/"));
					if(fo_put == NULL)
						strncpy(rb->sub_fold,"NULL",strlen("NULL"));
					else
						strncpy(rb->sub_fold,fo_put,strlen(fo_put));
					rb->part_number = dfs2_part2;

					/*---- Create the socket. The three arguments are: ----*/
 					/* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
 					clientSocket = socket(PF_INET, SOCK_STREAM, 0);
 					/*---- Configure settings of the server address struct ----*/
 					/* Address family = Internet */
 					serverAddr.sin_family = AF_INET;
 					/* Set port number, using htons function to use proper byte order */
 					serverAddr.sin_port = htons(atoi(port2));
 					/* Set IP address to localhost */
 					serverAddr.sin_addr.s_addr = inet_addr(ip_address);
 					/* Set all bits of the padding field to 0 */ 
 					memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
 					/*---- Connect the socket to the server using the address struct ----*/
 					addr_size = sizeof serverAddr;
 					connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

 					printf("Packet length to send = %d\n", rb->packet_length);
    				printf("Packet request_method to send = %s\n", rb->request_method);

    				int bytes = send(clientSocket, up, sizeof(struct username_password), MSG_NOSIGNAL);
 					//printf("Bytes sent = %d\n",bytes);
 					printf("Username and Password sent\n");

 					bytes = recv(clientSocket, &msg_int, sizeof(msg_int), 0);

 					if(msg_int == 1)
 					{
 						bytes = send(clientSocket, rb, sizeof(struct reliable_packet), MSG_NOSIGNAL);
 						//printf("Bytes sent = %d\n",bytes);
 						printf("Packet sent\n");
 						int count = 0;
 						for(int i=0;i<read2;i++)
 						{
 							data[i] = data[i] ^ PASSWORD[count];
 							count++;
 							if(count == strlen(PASSWORD))
 								count = 0;
 						}
 						printf("ENCRYPTED\n");
 						bytes = send(clientSocket, data, read2, MSG_NOSIGNAL);
 						printf("Bytes sent = %d\n",bytes);
 						printf("Data sent\n");
 					}
 					else if(msg_int == -1)
 					{
 						bytes = recv(clientSocket, &message_receive, sizeof(message_receive), 0);
 						printf("MESSAGE RECEIVED = %s\n", message_receive);
 						bzero(message_receive,strlen(message_receive));
 						printf("USERNAME AND PASSWORD NOT VERIFIED\n");
 					}

 					free(up);
 					free(rb);
 					free(data);

				}
				else if(dfs2_part2 == 3)
				{
					printf("\nCLIENT REACHES dfs2_part2 == 3\n");

					struct username_password *up = malloc(sizeof(struct username_password));
					memset(up,0,sizeof(struct username_password));

					printf("USERNAME = %s\n",username);
 					strcpy(up->user,username);
 					printf("up->user = %s\n",up->user);
 					printf("PASSWORD = %s\n",password);
 					strncpy(up->passwd,password,strlen(password));
 					printf("up->passwd = %s\n",up->passwd);

					struct reliable_packet *rb = malloc(sizeof(struct reliable_packet));
					memset(rb,0,sizeof(struct reliable_packet));

					memcpy(rb->request_method, req_method, strlen(req_method));
					printf("rb->request_method = %s\n",rb->request_method);
					
					rb->packet_length = read3;
					printf("rb->packet_length = %d\n",rb->packet_length);

					char *data = (char *)malloc(read3);
					memcpy(data,buffer_p3,read3);

					strncpy(rb->fname,filename,strlen(filename));

					strncpy(rb->fold,"/DFS2/",strlen("/DFS2/"));
					if(fo_put == NULL)
						strncpy(rb->sub_fold,"NULL",strlen("NULL"));
					else
						strncpy(rb->sub_fold,fo_put,strlen(fo_put));
					rb->part_number = dfs2_part2;

					/*---- Create the socket. The three arguments are: ----*/
 					/* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
 					clientSocket = socket(PF_INET, SOCK_STREAM, 0);
 					/*---- Configure settings of the server address struct ----*/
 					/* Address family = Internet */
 					serverAddr.sin_family = AF_INET;
 					/* Set port number, using htons function to use proper byte order */
 					serverAddr.sin_port = htons(atoi(port2));
 					/* Set IP address to localhost */
 					serverAddr.sin_addr.s_addr = inet_addr(ip_address);
 					/* Set all bits of the padding field to 0 */ 
 					memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
 					/*---- Connect the socket to the server using the address struct ----*/
 					addr_size = sizeof serverAddr;
 					connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

 					printf("Packet length to send = %d\n", rb->packet_length);
    				printf("Packet request_method to send = %s\n", rb->request_method);

    				int bytes = send(clientSocket, up, sizeof(struct username_password), MSG_NOSIGNAL);
 					//printf("Bytes sent = %d\n",bytes);
 					printf("Username and Password sent\n");

 					bytes = recv(clientSocket, &msg_int, sizeof(msg_int), 0);

 					if(msg_int == 1)
 					{
 						bytes = send(clientSocket, rb, sizeof(struct reliable_packet), MSG_NOSIGNAL);
 						//printf("Bytes sent = %d\n",bytes);
 						printf("Packet sent\n");
 						int count = 0;
 						for(int i=0;i<read3;i++)
 						{
 							data[i] = data[i] ^ PASSWORD[count];
 							count++;
 							if(count == strlen(PASSWORD))
 								count = 0;
 						}
 						printf("ENCRYPTED\n");
 						bytes = send(clientSocket, data, read3, MSG_NOSIGNAL);
 						printf("Bytes sent = %d\n",bytes);
 						printf("Data sent\n");
 					}
 					else if(msg_int == -1)
 					{
 						bytes = recv(clientSocket, &message_receive, sizeof(message_receive), 0);
 						printf("MESSAGE RECEIVED = %s\n", message_receive);
 						bzero(message_receive,strlen(message_receive));
 						printf("USERNAME AND PASSWORD NOT VERIFIED\n");
 					}

 					free(up);
 					free(rb);
 					free(data);
				}
				else if(dfs2_part2 == 4)
				{
					printf("\nCLIENT REACHES dfs2_part2 == 4\n");

					struct username_password *up = malloc(sizeof(struct username_password));
					memset(up,0,sizeof(struct username_password));

					printf("USERNAME = %s\n",username);
 					strcpy(up->user,username);
 					printf("up->user = %s\n",up->user);
 					printf("PASSWORD = %s\n",password);
 					strncpy(up->passwd,password,strlen(password));
 					printf("up->passwd = %s\n",up->passwd);

					struct reliable_packet *rb = malloc(sizeof(struct reliable_packet));
					memset(rb,0,sizeof(struct reliable_packet));

					memcpy(rb->request_method, req_method, strlen(req_method));
					printf("rb->request_method = %s\n",rb->request_method);
					
					rb->packet_length = read4;
					printf("rb->packet_length = %d\n",rb->packet_length);

					char *data = (char *)malloc(read1);
					memcpy(data,buffer_p4,read4);

					strncpy(rb->fname,filename,strlen(filename));

					strncpy(rb->fold,"/DFS2/",strlen("/DFS2/"));
					if(fo_put == NULL)
						strncpy(rb->sub_fold,"NULL",strlen("NULL"));
					else
						strncpy(rb->sub_fold,fo_put,strlen(fo_put));
					rb->part_number = dfs2_part2;

					/*---- Create the socket. The three arguments are: ----*/
 					/* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
 					clientSocket = socket(PF_INET, SOCK_STREAM, 0);
 					/*---- Configure settings of the server address struct ----*/
 					/* Address family = Internet */
 					serverAddr.sin_family = AF_INET;
 					/* Set port number, using htons function to use proper byte order */
 					serverAddr.sin_port = htons(atoi(port2));
 					/* Set IP address to localhost */
 					serverAddr.sin_addr.s_addr = inet_addr(ip_address);
 					/* Set all bits of the padding field to 0 */ 
 					memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
 					/*---- Connect the socket to the server using the address struct ----*/
 					addr_size = sizeof serverAddr;
 					connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

 					printf("Packet length to send = %d\n", rb->packet_length);
    				printf("Packet request_method to send = %s\n", rb->request_method);

    				int bytes = send(clientSocket, up, sizeof(struct username_password), MSG_NOSIGNAL);
 					//printf("Bytes sent = %d\n",bytes);
 					printf("Username and Password sent\n");

 					bytes = recv(clientSocket, &msg_int, sizeof(msg_int), 0);

 					if(msg_int == 1)
 					{
 						bytes = send(clientSocket, rb, sizeof(struct reliable_packet), MSG_NOSIGNAL);
 						//printf("Bytes sent = %d\n",bytes);
 						printf("Packet sent\n");
 						int count = 0;
 						for(int i=0;i<read4;i++)
 						{
 							data[i] = data[i] ^ PASSWORD[count];
 							count++;
 							if(count == strlen(PASSWORD))
 								count = 0;
 						}
 						printf("ENCRYPTED\n");
 						bytes = send(clientSocket, data, read4, MSG_NOSIGNAL);
 						printf("Bytes sent = %d\n",bytes);
 						printf("Data sent\n");
 					}
 					else if(msg_int == -1)
 					{
 						bytes = recv(clientSocket, &message_receive, sizeof(message_receive), 0);
 						printf("MESSAGE RECEIVED = %s\n", message_receive);
 						bzero(message_receive,strlen(message_receive));
 						printf("USERNAME AND PASSWORD NOT VERIFIED\n");
 					}

 					free(up);
 					free(rb);
 					free(data);
				}
				//printf("CLIENT REACHES B\n");
				printf("***************************************************************** DFS 3 - Part 1 *****************************************************************\n");
				if(dfs3_part1 == 1)
				{
					printf("\nCLIENT REACHES dfs3_part1 == 1\n");

					struct username_password *up = malloc(sizeof(struct username_password));
					memset(up,0,sizeof(struct username_password));

					printf("USERNAME = %s\n",username);
 					strcpy(up->user,username);
 					printf("up->user = %s\n",up->user);
 					printf("PASSWORD = %s\n",password);
 					strncpy(up->passwd,password,strlen(password));
 					printf("up->passwd = %s\n",up->passwd);

					struct reliable_packet *rb = malloc(sizeof(struct reliable_packet));
					memset(rb,0,sizeof(struct reliable_packet));

					memcpy(rb->request_method, req_method, strlen(req_method));
					printf("rb->request_method = %s\n",rb->request_method);
					
					rb->packet_length = read1;
					printf("rb->packet_length = %d\n",rb->packet_length);

					char *data = (char *)malloc(read1);
					memcpy(data,buffer_p1,read1);

					strncpy(rb->fname,filename,strlen(filename));

					strncpy(rb->fold,"/DFS3/",strlen("/DFS3/"));
					if(fo_put == NULL)
						strncpy(rb->sub_fold,"NULL",strlen("NULL"));
					else
						strncpy(rb->sub_fold,fo_put,strlen(fo_put));
					rb->part_number = dfs3_part1;

					/*---- Create the socket. The three arguments are: ----*/
 					/* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
 					clientSocket = socket(PF_INET, SOCK_STREAM, 0);
 					/*---- Configure settings of the server address struct ----*/
 					/* Address family = Internet */
 					serverAddr.sin_family = AF_INET;
 					/* Set port number, using htons function to use proper byte order */
 					serverAddr.sin_port = htons(atoi(port3));
 					/* Set IP address to localhost */
 					serverAddr.sin_addr.s_addr = inet_addr(ip_address);
 					/* Set all bits of the padding field to 0 */ 
 					memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
 					/*---- Connect the socket to the server using the address struct ----*/
 					addr_size = sizeof serverAddr;
 					connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

 					printf("Packet length to send = %d\n", rb->packet_length);
    				printf("Packet request_method to send = %s\n", rb->request_method);

    				int bytes = send(clientSocket, up, sizeof(struct username_password), MSG_NOSIGNAL);
 					//printf("Bytes sent = %d\n",bytes);
 					printf("Username and Password sent\n");

 					bytes = recv(clientSocket, &msg_int, sizeof(msg_int), 0);

 					if(msg_int == 1)
 					{
 						bytes = send(clientSocket, rb, sizeof(struct reliable_packet), MSG_NOSIGNAL);
 						//printf("Bytes sent = %d\n",bytes);
 						printf("Packet sent\n");
 						int count = 0;
 						for(int i=0;i<read1;i++)
 						{
 							data[i] = data[i] ^ PASSWORD[count];
 							count++;
 							if(count == strlen(PASSWORD))
 								count = 0;
 						}
 						printf("ENCRYPTED\n");
 						bytes = send(clientSocket, data, read1, MSG_NOSIGNAL);
 						printf("Bytes sent = %d\n",bytes);
 						printf("Data sent\n");
 					}
 					else if(msg_int == -1)
 					{
 						bytes = recv(clientSocket, &message_receive, sizeof(message_receive), 0);
 						printf("MESSAGE RECEIVED = %s\n", message_receive);
 						bzero(message_receive,strlen(message_receive));
 						printf("USERNAME AND PASSWORD NOT VERIFIED\n");
 					}

 					free(up);
 					free(rb);
 					free(data);
				}
				else if(dfs3_part1 == 2)
				{
					printf("\nCLIENT REACHES dfs3_part1 == 2\n");

					struct username_password *up = malloc(sizeof(struct username_password));
					memset(up,0,sizeof(struct username_password));

					printf("USERNAME = %s\n",username);
 					strcpy(up->user,username);
 					printf("up->user = %s\n",up->user);
 					printf("PASSWORD = %s\n",password);
 					strncpy(up->passwd,password,strlen(password));
 					printf("up->passwd = %s\n",up->passwd);

					struct reliable_packet *rb = malloc(sizeof(struct reliable_packet));
					memset(rb,0,sizeof(struct reliable_packet));

					memcpy(rb->request_method, req_method, strlen(req_method));
					printf("rb->request_method = %s\n",rb->request_method);
					
					rb->packet_length = read2;
					printf("rb->packet_length = %d\n",rb->packet_length);

					char *data = (char *)malloc(read2);
					memcpy(data,buffer_p2,read2);

					strncpy(rb->fname,filename,strlen(filename));

					strncpy(rb->fold,"/DFS3/",strlen("/DFS3/"));
					if(fo_put == NULL)
						strncpy(rb->sub_fold,"NULL",strlen("NULL"));
					else
						strncpy(rb->sub_fold,fo_put,strlen(fo_put));
					rb->part_number = dfs3_part1;

					/*---- Create the socket. The three arguments are: ----*/
 					/* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
 					clientSocket = socket(PF_INET, SOCK_STREAM, 0);
 					/*---- Configure settings of the server address struct ----*/
 					/* Address family = Internet */
 					serverAddr.sin_family = AF_INET;
 					/* Set port number, using htons function to use proper byte order */
 					serverAddr.sin_port = htons(atoi(port3));
 					/* Set IP address to localhost */
 					serverAddr.sin_addr.s_addr = inet_addr(ip_address);
 					/* Set all bits of the padding field to 0 */ 
 					memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
 					/*---- Connect the socket to the server using the address struct ----*/
 					addr_size = sizeof serverAddr;
 					connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

 					printf("Packet length to send = %d\n", rb->packet_length);
    				printf("Packet request_method to send = %s\n", rb->request_method);

    				int bytes = send(clientSocket, up, sizeof(struct username_password), MSG_NOSIGNAL);
 					//printf("Bytes sent = %d\n",bytes);
 					printf("Username and Password sent\n");

 					bytes = recv(clientSocket, &msg_int, sizeof(msg_int), 0);

 					if(msg_int == 1)
 					{
 						bytes = send(clientSocket, rb, sizeof(struct reliable_packet), MSG_NOSIGNAL);
 						//printf("Bytes sent = %d\n",bytes);
 						printf("Packet sent\n");
 						int count = 0;
 						for(int i=0;i<read2;i++)
 						{
 							data[i] = data[i] ^ PASSWORD[count];
 							count++;
 							if(count == strlen(PASSWORD))
 								count = 0;
 						}
 						printf("ENCRYPTED\n");
 						bytes = send(clientSocket, data, read2, MSG_NOSIGNAL);
 						printf("Bytes sent = %d\n",bytes);
 						printf("Data sent\n");
 					}
 					else if(msg_int == -1)
 					{
 						bytes = recv(clientSocket, &message_receive, sizeof(message_receive), 0);
 						printf("MESSAGE RECEIVED = %s\n", message_receive);
 						bzero(message_receive,strlen(message_receive));
 						printf("USERNAME AND PASSWORD NOT VERIFIED\n");
 					}

 					free(up);
 					free(rb);
 					free(data);

				}
				else if(dfs3_part1 == 3)
				{
					printf("\nCLIENT REACHES dfs3_part1 == 3\n");

					struct username_password *up = malloc(sizeof(struct username_password));
					memset(up,0,sizeof(struct username_password));

					printf("USERNAME = %s\n",username);
 					strcpy(up->user,username);
 					printf("up->user = %s\n",up->user);
 					printf("PASSWORD = %s\n",password);
 					strncpy(up->passwd,password,strlen(password));
 					printf("up->passwd = %s\n",up->passwd);

					struct reliable_packet *rb = malloc(sizeof(struct reliable_packet));
					memset(rb,0,sizeof(struct reliable_packet));

					memcpy(rb->request_method, req_method, strlen(req_method));
					printf("rb->request_method = %s\n",rb->request_method);
					
					rb->packet_length = read3;
					printf("rb->packet_length = %d\n",rb->packet_length);

					char *data = (char *)malloc(read3);
					memcpy(data,buffer_p3,read3);

					strncpy(rb->fname,filename,strlen(filename));

					strncpy(rb->fold,"/DFS3/",strlen("/DFS3/"));
					if(fo_put == NULL)
						strncpy(rb->sub_fold,"NULL",strlen("NULL"));
					else
						strncpy(rb->sub_fold,fo_put,strlen(fo_put));
					rb->part_number = dfs3_part1;

					/*---- Create the socket. The three arguments are: ----*/
 					/* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
 					clientSocket = socket(PF_INET, SOCK_STREAM, 0);
 					/*---- Configure settings of the server address struct ----*/
 					/* Address family = Internet */
 					serverAddr.sin_family = AF_INET;
 					/* Set port number, using htons function to use proper byte order */
 					serverAddr.sin_port = htons(atoi(port3));
 					/* Set IP address to localhost */
 					serverAddr.sin_addr.s_addr = inet_addr(ip_address);
 					/* Set all bits of the padding field to 0 */ 
 					memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
 					/*---- Connect the socket to the server using the address struct ----*/
 					addr_size = sizeof serverAddr;
 					connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

 					printf("Packet length to send = %d\n", rb->packet_length);
    				printf("Packet request_method to send = %s\n", rb->request_method);

    				int bytes = send(clientSocket, up, sizeof(struct username_password), MSG_NOSIGNAL);
 					//printf("Bytes sent = %d\n",bytes);
 					printf("Username and Password sent\n");

 					bytes = recv(clientSocket, &msg_int, sizeof(msg_int), 0);

 					if(msg_int == 1)
 					{
 						bytes = send(clientSocket, rb, sizeof(struct reliable_packet), MSG_NOSIGNAL);
 						//printf("Bytes sent = %d\n",bytes);
 						printf("Packet sent\n");
 						int count = 0;
 						for(int i=0;i<read3;i++)
 						{
 							data[i] = data[i] ^ PASSWORD[count];
 							count++;
 							if(count == strlen(PASSWORD))
 								count = 0;
 						}
 						printf("ENCRYPTED\n");
 						bytes = send(clientSocket, data, read3, MSG_NOSIGNAL);
 						printf("Bytes sent = %d\n",bytes);
 						printf("Data sent\n");
 					}
 					else if(msg_int == -1)
 					{
 						bytes = recv(clientSocket, &message_receive, sizeof(message_receive), 0);
 						printf("MESSAGE RECEIVED = %s\n", message_receive);
 						bzero(message_receive,strlen(message_receive));
 						printf("USERNAME AND PASSWORD NOT VERIFIED\n");
 					}

 					free(up);
 					free(rb);
 					free(data);
				}
				else if(dfs3_part1 == 4)
				{
					printf("\nCLIENT REACHES dfs3_part1 == 4\n");

					struct username_password *up = malloc(sizeof(struct username_password));
					memset(up,0,sizeof(struct username_password));

					printf("USERNAME = %s\n",username);
 					strcpy(up->user,username);
 					printf("up->user = %s\n",up->user);
 					printf("PASSWORD = %s\n",password);
 					strncpy(up->passwd,password,strlen(password));
 					printf("up->passwd = %s\n",up->passwd);

					struct reliable_packet *rb = malloc(sizeof(struct reliable_packet));
					memset(rb,0,sizeof(struct reliable_packet));

					memcpy(rb->request_method, req_method, strlen(req_method));
					printf("rb->request_method = %s\n",rb->request_method);
					
					rb->packet_length = read4;
					printf("rb->packet_length = %d\n",rb->packet_length);

					char *data = (char *)malloc(read1);
					memcpy(data,buffer_p4,read4);

					strncpy(rb->fname,filename,strlen(filename));

					strncpy(rb->fold,"/DFS3/",strlen("/DFS3/"));
					if(fo_put == NULL)
						strncpy(rb->sub_fold,"NULL",strlen("NULL"));
					else
						strncpy(rb->sub_fold,fo_put,strlen(fo_put));
					rb->part_number = dfs3_part1;

					/*---- Create the socket. The three arguments are: ----*/
 					/* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
 					clientSocket = socket(PF_INET, SOCK_STREAM, 0);
 					/*---- Configure settings of the server address struct ----*/
 					/* Address family = Internet */
 					serverAddr.sin_family = AF_INET;
 					/* Set port number, using htons function to use proper byte order */
 					serverAddr.sin_port = htons(atoi(port3));
 					/* Set IP address to localhost */
 					serverAddr.sin_addr.s_addr = inet_addr(ip_address);
 					/* Set all bits of the padding field to 0 */ 
 					memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
 					/*---- Connect the socket to the server using the address struct ----*/
 					addr_size = sizeof serverAddr;
 					connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

 					printf("Packet length to send = %d\n", rb->packet_length);
    				printf("Packet request_method to send = %s\n", rb->request_method);

    				int bytes = send(clientSocket, up, sizeof(struct username_password), MSG_NOSIGNAL);
 					//printf("Bytes sent = %d\n",bytes);
 					printf("Username and Password sent\n");

 					bytes = recv(clientSocket, &msg_int, sizeof(msg_int), 0);

 					if(msg_int == 1)
 					{
 						bytes = send(clientSocket, rb, sizeof(struct reliable_packet), MSG_NOSIGNAL);
 						//printf("Bytes sent = %d\n",bytes);
 						printf("Packet sent\n");
 						int count = 0;
 						for(int i=0;i<read4;i++)
 						{
 							data[i] = data[i] ^ PASSWORD[count];
 							count++;
 							if(count == strlen(PASSWORD))
 								count = 0;
 						}
 						printf("ENCRYPTED\n");
 						bytes = send(clientSocket, data, read4, MSG_NOSIGNAL);
 						printf("Bytes sent = %d\n",bytes);
 						printf("Data sent\n");
 					}
 					else if(msg_int == -1)
 					{
 						bytes = recv(clientSocket, &message_receive, sizeof(message_receive), 0);
 						printf("MESSAGE RECEIVED = %s\n", message_receive);
 						bzero(message_receive,strlen(message_receive));
 						printf("USERNAME AND PASSWORD NOT VERIFIED\n");
 					}

 					free(up);
 					free(rb);
 					free(data);
				}
				printf("***************************************************************** DFS 3 - Part 2 *****************************************************************\n");
				if(dfs3_part2 == 1)
				{
					printf("\nCLIENT REACHES dfs3_part2 == 1\n");

					struct username_password *up = malloc(sizeof(struct username_password));
					memset(up,0,sizeof(struct username_password));

					printf("USERNAME = %s\n",username);
 					strcpy(up->user,username);
 					printf("up->user = %s\n",up->user);
 					printf("PASSWORD = %s\n",password);
 					strncpy(up->passwd,password,strlen(password));
 					printf("up->passwd = %s\n",up->passwd);

					struct reliable_packet *rb = malloc(sizeof(struct reliable_packet));
					memset(rb,0,sizeof(struct reliable_packet));

					memcpy(rb->request_method, req_method, strlen(req_method));
					printf("rb->request_method = %s\n",rb->request_method);
					
					rb->packet_length = read1;
					printf("rb->packet_length = %d\n",rb->packet_length);

					char *data = (char *)malloc(read1);
					memcpy(data,buffer_p1,read1);

					strncpy(rb->fname,filename,strlen(filename));

					strncpy(rb->fold,"/DFS3/",strlen("/DFS3/"));
					if(fo_put == NULL)
						strncpy(rb->sub_fold,"NULL",strlen("NULL"));
					else
						strncpy(rb->sub_fold,fo_put,strlen(fo_put));
					rb->part_number = dfs3_part2;

					/*---- Create the socket. The three arguments are: ----*/
 					/* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
 					clientSocket = socket(PF_INET, SOCK_STREAM, 0);
 					/*---- Configure settings of the server address struct ----*/
 					/* Address family = Internet */
 					serverAddr.sin_family = AF_INET;
 					/* Set port number, using htons function to use proper byte order */
 					serverAddr.sin_port = htons(atoi(port3));
 					/* Set IP address to localhost */
 					serverAddr.sin_addr.s_addr = inet_addr(ip_address);
 					/* Set all bits of the padding field to 0 */ 
 					memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
 					/*---- Connect the socket to the server using the address struct ----*/
 					addr_size = sizeof serverAddr;
 					connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

 					printf("Packet length to send = %d\n", rb->packet_length);
    				printf("Packet request_method to send = %s\n", rb->request_method);

    				int bytes = send(clientSocket, up, sizeof(struct username_password), MSG_NOSIGNAL);
 					//printf("Bytes sent = %d\n",bytes);
 					printf("Username and Password sent\n");

 					bytes = recv(clientSocket, &msg_int, sizeof(msg_int), 0);

 					if(msg_int == 1)
 					{
 						bytes = send(clientSocket, rb, sizeof(struct reliable_packet), MSG_NOSIGNAL);
 						//printf("Bytes sent = %d\n",bytes);
 						printf("Packet sent\n");
 						int count = 0;
 						for(int i=0;i<read1;i++)
 						{
 							data[i] = data[i] ^ PASSWORD[count];
 							count++;
 							if(count == strlen(PASSWORD))
 								count = 0;
 						}
 						printf("ENCRYPTED\n");
 						bytes = send(clientSocket, data, read1, MSG_NOSIGNAL);
 						printf("Bytes sent = %d\n",bytes);
 						printf("Data sent\n");
 					}
 					else if(msg_int == -1)
 					{
 						bytes = recv(clientSocket, &message_receive, sizeof(message_receive), 0);
 						printf("MESSAGE RECEIVED = %s\n", message_receive);
 						bzero(message_receive,strlen(message_receive));
 						printf("USERNAME AND PASSWORD NOT VERIFIED\n");
 					}

 					free(up);
 					free(rb);
 					free(data);
				}
				else if(dfs3_part2 == 2)
				{
					printf("\nCLIENT REACHES dfs3_part2 == 2\n");

					struct username_password *up = malloc(sizeof(struct username_password));
					memset(up,0,sizeof(struct username_password));

					printf("USERNAME = %s\n",username);
 					strcpy(up->user,username);
 					printf("up->user = %s\n",up->user);
 					printf("PASSWORD = %s\n",password);
 					strncpy(up->passwd,password,strlen(password));
 					printf("up->passwd = %s\n",up->passwd);

					struct reliable_packet *rb = malloc(sizeof(struct reliable_packet));
					memset(rb,0,sizeof(struct reliable_packet));

					memcpy(rb->request_method, req_method, strlen(req_method));
					printf("rb->request_method = %s\n",rb->request_method);
					
					rb->packet_length = read2;
					printf("rb->packet_length = %d\n",rb->packet_length);

					char *data = (char *)malloc(read2);
					memcpy(data,buffer_p2,read2);

					strncpy(rb->fname,filename,strlen(filename));

					strncpy(rb->fold,"/DFS3/",strlen("/DFS3/"));
					if(fo_put == NULL)
						strncpy(rb->sub_fold,"NULL",strlen("NULL"));
					else
						strncpy(rb->sub_fold,fo_put,strlen(fo_put));
					rb->part_number = dfs3_part2;

					/*---- Create the socket. The three arguments are: ----*/
 					/* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
 					clientSocket = socket(PF_INET, SOCK_STREAM, 0);
 					/*---- Configure settings of the server address struct ----*/
 					/* Address family = Internet */
 					serverAddr.sin_family = AF_INET;
 					/* Set port number, using htons function to use proper byte order */
 					serverAddr.sin_port = htons(atoi(port3));
 					/* Set IP address to localhost */
 					serverAddr.sin_addr.s_addr = inet_addr(ip_address);
 					/* Set all bits of the padding field to 0 */ 
 					memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
 					/*---- Connect the socket to the server using the address struct ----*/
 					addr_size = sizeof serverAddr;
 					connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

 					printf("Packet length to send = %d\n", rb->packet_length);
    				printf("Packet request_method to send = %s\n", rb->request_method);

    				int bytes = send(clientSocket, up, sizeof(struct username_password), MSG_NOSIGNAL);
 					//printf("Bytes sent = %d\n",bytes);
 					printf("Username and Password sent\n");

 					bytes = recv(clientSocket, &msg_int, sizeof(msg_int), 0);

 					if(msg_int == 1)
 					{
 						bytes = send(clientSocket, rb, sizeof(struct reliable_packet), MSG_NOSIGNAL);
 						//printf("Bytes sent = %d\n",bytes);
 						printf("Packet sent\n");
 						int count = 0;
 						for(int i=0;i<read2;i++)
 						{
 							data[i] = data[i] ^ PASSWORD[count];
 							count++;
 							if(count == strlen(PASSWORD))
 								count = 0;
 						}
 						printf("ENCRYPTED\n");
 						bytes = send(clientSocket, data, read2, MSG_NOSIGNAL);
 						printf("Bytes sent = %d\n",bytes);
 						printf("Data sent\n");
 					}
 					else if(msg_int == -1)
 					{
 						bytes = recv(clientSocket, &message_receive, sizeof(message_receive), 0);
 						printf("MESSAGE RECEIVED = %s\n", message_receive);
 						bzero(message_receive,strlen(message_receive));
 						printf("USERNAME AND PASSWORD NOT VERIFIED\n");
 					}

 					free(up);
 					free(rb);
 					free(data);

				}
				else if(dfs3_part2 == 3)
				{
					printf("\nCLIENT REACHES dfs3_part2 == 3\n");

					struct username_password *up = malloc(sizeof(struct username_password));
					memset(up,0,sizeof(struct username_password));

					printf("USERNAME = %s\n",username);
 					strcpy(up->user,username);
 					printf("up->user = %s\n",up->user);
 					printf("PASSWORD = %s\n",password);
 					strncpy(up->passwd,password,strlen(password));
 					printf("up->passwd = %s\n",up->passwd);

					struct reliable_packet *rb = malloc(sizeof(struct reliable_packet));
					memset(rb,0,sizeof(struct reliable_packet));

					memcpy(rb->request_method, req_method, strlen(req_method));
					printf("rb->request_method = %s\n",rb->request_method);
					
					rb->packet_length = read3;
					printf("rb->packet_length = %d\n",rb->packet_length);

					char *data = (char *)malloc(read3);
					memcpy(data,buffer_p3,read3);

					strncpy(rb->fname,filename,strlen(filename));

					strncpy(rb->fold,"/DFS3/",strlen("/DFS3/"));
					if(fo_put == NULL)
						strncpy(rb->sub_fold,"NULL",strlen("NULL"));
					else
						strncpy(rb->sub_fold,fo_put,strlen(fo_put));
					rb->part_number = dfs3_part2;

					/*---- Create the socket. The three arguments are: ----*/
 					/* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
 					clientSocket = socket(PF_INET, SOCK_STREAM, 0);
 					/*---- Configure settings of the server address struct ----*/
 					/* Address family = Internet */
 					serverAddr.sin_family = AF_INET;
 					/* Set port number, using htons function to use proper byte order */
 					serverAddr.sin_port = htons(atoi(port3));
 					/* Set IP address to localhost */
 					serverAddr.sin_addr.s_addr = inet_addr(ip_address);
 					/* Set all bits of the padding field to 0 */ 
 					memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
 					/*---- Connect the socket to the server using the address struct ----*/
 					addr_size = sizeof serverAddr;
 					connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

 					printf("Packet length to send = %d\n", rb->packet_length);
    				printf("Packet request_method to send = %s\n", rb->request_method);

    				int bytes = send(clientSocket, up, sizeof(struct username_password), MSG_NOSIGNAL);
 					//printf("Bytes sent = %d\n",bytes);
 					printf("Username and Password sent\n");

 					bytes = recv(clientSocket, &msg_int, sizeof(msg_int), 0);

 					if(msg_int == 1)
 					{
 						bytes = send(clientSocket, rb, sizeof(struct reliable_packet), MSG_NOSIGNAL);
 						//printf("Bytes sent = %d\n",bytes);
 						printf("Packet sent\n");
 						int count = 0;
 						for(int i=0;i<read3;i++)
 						{
 							data[i] = data[i] ^ PASSWORD[count];
 							count++;
 							if(count == strlen(PASSWORD))
 								count = 0;
 						}
 						printf("ENCRYPTED\n");
 						bytes = send(clientSocket, data, read3, MSG_NOSIGNAL);
 						printf("Bytes sent = %d\n",bytes);
 						printf("Data sent\n");
 					}
 					else if(msg_int == -1)
 					{
 						bytes = recv(clientSocket, &message_receive, sizeof(message_receive), 0);
 						printf("MESSAGE RECEIVED = %s\n", message_receive);
 						bzero(message_receive,strlen(message_receive));
 						printf("USERNAME AND PASSWORD NOT VERIFIED\n");
 					}

 					free(up);
 					free(rb);
 					free(data);
				}
				else if(dfs3_part2 == 4)
				{
					printf("\nCLIENT REACHES dfs3_part2 == 4\n");

					struct username_password *up = malloc(sizeof(struct username_password));
					memset(up,0,sizeof(struct username_password));

					printf("USERNAME = %s\n",username);
 					strcpy(up->user,username);
 					printf("up->user = %s\n",up->user);
 					printf("PASSWORD = %s\n",password);
 					strncpy(up->passwd,password,strlen(password));
 					printf("up->passwd = %s\n",up->passwd);

					struct reliable_packet *rb = malloc(sizeof(struct reliable_packet));
					memset(rb,0,sizeof(struct reliable_packet));

					memcpy(rb->request_method, req_method, strlen(req_method));
					printf("rb->request_method = %s\n",rb->request_method);
					
					rb->packet_length = read4;
					printf("rb->packet_length = %d\n",rb->packet_length);

					char *data = (char *)malloc(read1);
					memcpy(data,buffer_p4,read4);

					strncpy(rb->fname,filename,strlen(filename));

					strncpy(rb->fold,"/DFS3/",strlen("/DFS3/"));
					if(fo_put == NULL)
						strncpy(rb->sub_fold,"NULL",strlen("NULL"));
					else
						strncpy(rb->sub_fold,fo_put,strlen(fo_put));
					rb->part_number = dfs3_part2;

					/*---- Create the socket. The three arguments are: ----*/
 					/* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
 					clientSocket = socket(PF_INET, SOCK_STREAM, 0);
 					/*---- Configure settings of the server address struct ----*/
 					/* Address family = Internet */
 					serverAddr.sin_family = AF_INET;
 					/* Set port number, using htons function to use proper byte order */
 					serverAddr.sin_port = htons(atoi(port3));
 					/* Set IP address to localhost */
 					serverAddr.sin_addr.s_addr = inet_addr(ip_address);
 					/* Set all bits of the padding field to 0 */ 
 					memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
 					/*---- Connect the socket to the server using the address struct ----*/
 					addr_size = sizeof serverAddr;
 					connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

 					printf("Packet length to send = %d\n", rb->packet_length);
    				printf("Packet request_method to send = %s\n", rb->request_method);

    				int bytes = send(clientSocket, up, sizeof(struct username_password), MSG_NOSIGNAL);
 					//printf("Bytes sent = %d\n",bytes);
 					printf("Username and Password sent\n");

 					bytes = recv(clientSocket, &msg_int, sizeof(msg_int), 0);

 					if(msg_int == 1)
 					{
 						bytes = send(clientSocket, rb, sizeof(struct reliable_packet), MSG_NOSIGNAL);
 						//printf("Bytes sent = %d\n",bytes);
 						printf("Packet sent\n");
 						int count = 0;
 						for(int i=0;i<read4;i++)
 						{
 							data[i] = data[i] ^ PASSWORD[count];
 							count++;
 							if(count == strlen(PASSWORD))
 								count = 0;
 						}
 						printf("ENCRYPTED\n");
 						bytes = send(clientSocket, data, read4, MSG_NOSIGNAL);
 						printf("Bytes sent = %d\n",bytes);
 						printf("Data sent\n");
 					}
 					else if(msg_int == -1)
 					{
 						bytes = recv(clientSocket, &message_receive, sizeof(message_receive), 0);
 						printf("MESSAGE RECEIVED = %s\n", message_receive);
 						bzero(message_receive,strlen(message_receive));
 						printf("USERNAME AND PASSWORD NOT VERIFIED\n");
 					}

 					free(up);
 					free(rb);
 					free(data);
				}
				printf("***************************************************************** DFS 4 - Part 1 *****************************************************************\n");
				if(dfs4_part1 == 1)
				{
					printf("\nCLIENT REACHES dfs4_part1 == 1\n");

					struct username_password *up = malloc(sizeof(struct username_password));
					memset(up,0,sizeof(struct username_password));

					printf("USERNAME = %s\n",username);
 					strcpy(up->user,username);
 					printf("up->user = %s\n",up->user);
 					printf("PASSWORD = %s\n",password);
 					strncpy(up->passwd,password,strlen(password));
 					printf("up->passwd = %s\n",up->passwd);

					struct reliable_packet *rb = malloc(sizeof(struct reliable_packet));
					memset(rb,0,sizeof(struct reliable_packet));

					memcpy(rb->request_method, req_method, strlen(req_method));
					printf("rb->request_method = %s\n",rb->request_method);
					
					rb->packet_length = read1;
					printf("rb->packet_length = %d\n",rb->packet_length);

					char *data = (char *)malloc(read1);
					memcpy(data,buffer_p1,read1);

					strncpy(rb->fname,filename,strlen(filename));

					strncpy(rb->fold,"/DFS4/",strlen("/DFS4/"));
					if(fo_put == NULL)
						strncpy(rb->sub_fold,"NULL",strlen("NULL"));
					else
						strncpy(rb->sub_fold,fo_put,strlen(fo_put));
					rb->part_number = dfs4_part1;

					/*---- Create the socket. The three arguments are: ----*/
 					/* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
 					clientSocket = socket(PF_INET, SOCK_STREAM, 0);
 					/*---- Configure settings of the server address struct ----*/
 					/* Address family = Internet */
 					serverAddr.sin_family = AF_INET;
 					/* Set port number, using htons function to use proper byte order */
 					serverAddr.sin_port = htons(atoi(port4));
 					/* Set IP address to localhost */
 					serverAddr.sin_addr.s_addr = inet_addr(ip_address);
 					/* Set all bits of the padding field to 0 */ 
 					memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
 					/*---- Connect the socket to the server using the address struct ----*/
 					addr_size = sizeof serverAddr;
 					connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

 					printf("Packet length to send = %d\n", rb->packet_length);
    				printf("Packet request_method to send = %s\n", rb->request_method);

    				int bytes = send(clientSocket, up, sizeof(struct username_password), MSG_NOSIGNAL);
 					//printf("Bytes sent = %d\n",bytes);
 					printf("Username and Password sent\n");

 					bytes = recv(clientSocket, &msg_int, sizeof(msg_int), 0);

 					if(msg_int == 1)
 					{
 						bytes = send(clientSocket, rb, sizeof(struct reliable_packet), MSG_NOSIGNAL);
 						//printf("Bytes sent = %d\n",bytes);
 						printf("Packet sent\n");
 						int count = 0;
 						for(int i=0;i<read1;i++)
 						{
 							data[i] = data[i] ^ PASSWORD[count];
 							count++;
 							if(count == strlen(PASSWORD))
 								count = 0;
 						}
 						printf("ENCRYPTED\n");
 						bytes = send(clientSocket, data, read1, MSG_NOSIGNAL);
 						printf("Bytes sent = %d\n",bytes);
 						printf("Data sent\n");
 					}
 					else if(msg_int == -1)
 					{
 						bytes = recv(clientSocket, &message_receive, sizeof(message_receive), 0);
 						printf("MESSAGE RECEIVED = %s\n", message_receive);
 						bzero(message_receive,strlen(message_receive));
 						printf("USERNAME AND PASSWORD NOT VERIFIED\n");
 					}

 					free(up);
 					free(rb);
 					free(data);
				}
				else if(dfs4_part1 == 2)
				{
					printf("\nCLIENT REACHES dfs4_part1 == 2\n");

					struct username_password *up = malloc(sizeof(struct username_password));
					memset(up,0,sizeof(struct username_password));

					printf("USERNAME = %s\n",username);
 					strcpy(up->user,username);
 					printf("up->user = %s\n",up->user);
 					printf("PASSWORD = %s\n",password);
 					strncpy(up->passwd,password,strlen(password));
 					printf("up->passwd = %s\n",up->passwd);

					struct reliable_packet *rb = malloc(sizeof(struct reliable_packet));
					memset(rb,0,sizeof(struct reliable_packet));

					memcpy(rb->request_method, req_method, strlen(req_method));
					printf("rb->request_method = %s\n",rb->request_method);
					
					rb->packet_length = read2;
					printf("rb->packet_length = %d\n",rb->packet_length);

					char *data = (char *)malloc(read2);
					memcpy(data,buffer_p2,read2);

					strncpy(rb->fname,filename,strlen(filename));

					strncpy(rb->fold,"/DFS4/",strlen("/DFS4/"));
					if(fo_put == NULL)
						strncpy(rb->sub_fold,"NULL",strlen("NULL"));
					else
						strncpy(rb->sub_fold,fo_put,strlen(fo_put));
					rb->part_number = dfs4_part1;

					/*---- Create the socket. The three arguments are: ----*/
 					/* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
 					clientSocket = socket(PF_INET, SOCK_STREAM, 0);
 					/*---- Configure settings of the server address struct ----*/
 					/* Address family = Internet */
 					serverAddr.sin_family = AF_INET;
 					/* Set port number, using htons function to use proper byte order */
 					serverAddr.sin_port = htons(atoi(port4));
 					/* Set IP address to localhost */
 					serverAddr.sin_addr.s_addr = inet_addr(ip_address);
 					/* Set all bits of the padding field to 0 */ 
 					memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
 					/*---- Connect the socket to the server using the address struct ----*/
 					addr_size = sizeof serverAddr;
 					connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

 					printf("Packet length to send = %d\n", rb->packet_length);
    				printf("Packet request_method to send = %s\n", rb->request_method);

    				int bytes = send(clientSocket, up, sizeof(struct username_password), MSG_NOSIGNAL);
 					//printf("Bytes sent = %d\n",bytes);
 					printf("Username and Password sent\n");

 					bytes = recv(clientSocket, &msg_int, sizeof(msg_int), 0);

 					if(msg_int == 1)
 					{
 						bytes = send(clientSocket, rb, sizeof(struct reliable_packet), MSG_NOSIGNAL);
 						//printf("Bytes sent = %d\n",bytes);
 						printf("Packet sent\n");
 						int count = 0;
 						for(int i=0;i<read2;i++)
 						{
 							data[i] = data[i] ^ PASSWORD[count];
 							count++;
 							if(count == strlen(PASSWORD))
 								count = 0;
 						}
 						printf("ENCRYPTED\n");
 						bytes = send(clientSocket, data, read2, MSG_NOSIGNAL);
 						printf("Bytes sent = %d\n",bytes);
 						printf("Data sent\n");
 					}
 					else if(msg_int == -1)
 					{
 						bytes = recv(clientSocket, &message_receive, sizeof(message_receive), 0);
 						printf("MESSAGE RECEIVED = %s\n", message_receive);
 						bzero(message_receive,strlen(message_receive));
 						printf("USERNAME AND PASSWORD NOT VERIFIED\n");
 					}

 					free(up);
 					free(rb);
 					free(data);

				}
				else if(dfs4_part1 == 3)
				{
					printf("\nCLIENT REACHES dfs4_part1 == 3\n");

					struct username_password *up = malloc(sizeof(struct username_password));
					memset(up,0,sizeof(struct username_password));

					printf("USERNAME = %s\n",username);
 					strcpy(up->user,username);
 					printf("up->user = %s\n",up->user);
 					printf("PASSWORD = %s\n",password);
 					strncpy(up->passwd,password,strlen(password));
 					printf("up->passwd = %s\n",up->passwd);

					struct reliable_packet *rb = malloc(sizeof(struct reliable_packet));
					memset(rb,0,sizeof(struct reliable_packet));

					memcpy(rb->request_method, req_method, strlen(req_method));
					printf("rb->request_method = %s\n",rb->request_method);
					
					rb->packet_length = read3;
					printf("rb->packet_length = %d\n",rb->packet_length);

					char *data = (char *)malloc(read3);
					memcpy(data,buffer_p3,read3);

					strncpy(rb->fname,filename,strlen(filename));

					strncpy(rb->fold,"/DFS4/",strlen("/DFS4/"));
					if(fo_put == NULL)
						strncpy(rb->sub_fold,"NULL",strlen("NULL"));
					else
						strncpy(rb->sub_fold,fo_put,strlen(fo_put));
					rb->part_number = dfs4_part1;

					/*---- Create the socket. The three arguments are: ----*/
 					/* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
 					clientSocket = socket(PF_INET, SOCK_STREAM, 0);
 					/*---- Configure settings of the server address struct ----*/
 					/* Address family = Internet */
 					serverAddr.sin_family = AF_INET;
 					/* Set port number, using htons function to use proper byte order */
 					serverAddr.sin_port = htons(atoi(port4));
 					/* Set IP address to localhost */
 					serverAddr.sin_addr.s_addr = inet_addr(ip_address);
 					/* Set all bits of the padding field to 0 */ 
 					memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
 					/*---- Connect the socket to the server using the address struct ----*/
 					addr_size = sizeof serverAddr;
 					connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

 					printf("Packet length to send = %d\n", rb->packet_length);
    				printf("Packet request_method to send = %s\n", rb->request_method);

    				int bytes = send(clientSocket, up, sizeof(struct username_password), MSG_NOSIGNAL);
 					//printf("Bytes sent = %d\n",bytes);
 					printf("Username and Password sent\n");

 					bytes = recv(clientSocket, &msg_int, sizeof(msg_int), 0);

 					if(msg_int == 1)
 					{
 						bytes = send(clientSocket, rb, sizeof(struct reliable_packet), MSG_NOSIGNAL);
 						//printf("Bytes sent = %d\n",bytes);
 						printf("Packet sent\n");
 						int count = 0;
 						for(int i=0;i<read3;i++)
 						{
 							data[i] = data[i] ^ PASSWORD[count];
 							count++;
 							if(count == strlen(PASSWORD))
 								count = 0;
 						}
 						printf("ENCRYPTED\n");
 						bytes = send(clientSocket, data, read3, MSG_NOSIGNAL);
 						printf("Bytes sent = %d\n",bytes);
 						printf("Data sent\n");
 					}
 					else if(msg_int == -1)
 					{
 						bytes = recv(clientSocket, &message_receive, sizeof(message_receive), 0);
 						printf("MESSAGE RECEIVED = %s\n", message_receive);
 						bzero(message_receive,strlen(message_receive));
 						printf("USERNAME AND PASSWORD NOT VERIFIED\n");
 					}

 					free(up);
 					free(rb);
 					free(data);
				}
				else if(dfs4_part1 == 4)
				{
					printf("\nCLIENT REACHES dfs4_part1 == 4\n");

					struct username_password *up = malloc(sizeof(struct username_password));
					memset(up,0,sizeof(struct username_password));

					printf("USERNAME = %s\n",username);
 					strcpy(up->user,username);
 					printf("up->user = %s\n",up->user);
 					printf("PASSWORD = %s\n",password);
 					strncpy(up->passwd,password,strlen(password));
 					printf("up->passwd = %s\n",up->passwd);

					struct reliable_packet *rb = malloc(sizeof(struct reliable_packet));
					memset(rb,0,sizeof(struct reliable_packet));

					memcpy(rb->request_method, req_method, strlen(req_method));
					printf("rb->request_method = %s\n",rb->request_method);
					
					rb->packet_length = read4;
					printf("rb->packet_length = %d\n",rb->packet_length);

					char *data = (char *)malloc(read1);
					memcpy(data,buffer_p4,read4);

					strncpy(rb->fname,filename,strlen(filename));

					strncpy(rb->fold,"/DFS4/",strlen("/DFS4/"));
					if(fo_put == NULL)
						strncpy(rb->sub_fold,"NULL",strlen("NULL"));
					else
						strncpy(rb->sub_fold,fo_put,strlen(fo_put));
					rb->part_number = dfs4_part1;

					/*---- Create the socket. The three arguments are: ----*/
 					/* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
 					clientSocket = socket(PF_INET, SOCK_STREAM, 0);
 					/*---- Configure settings of the server address struct ----*/
 					/* Address family = Internet */
 					serverAddr.sin_family = AF_INET;
 					/* Set port number, using htons function to use proper byte order */
 					serverAddr.sin_port = htons(atoi(port4));
 					/* Set IP address to localhost */
 					serverAddr.sin_addr.s_addr = inet_addr(ip_address);
 					/* Set all bits of the padding field to 0 */ 
 					memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
 					/*---- Connect the socket to the server using the address struct ----*/
 					addr_size = sizeof serverAddr;
 					connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

 					printf("Packet length to send = %d\n", rb->packet_length);
    				printf("Packet request_method to send = %s\n", rb->request_method);

    				int bytes = send(clientSocket, up, sizeof(struct username_password), MSG_NOSIGNAL);
 					//printf("Bytes sent = %d\n",bytes);
 					printf("Username and Password sent\n");

 					bytes = recv(clientSocket, &msg_int, sizeof(msg_int), 0);

 					if(msg_int == 1)
 					{
 						bytes = send(clientSocket, rb, sizeof(struct reliable_packet), MSG_NOSIGNAL);
 						//printf("Bytes sent = %d\n",bytes);
 						printf("Packet sent\n");
 						int count = 0;
 						for(int i=0;i<read4;i++)
 						{
 							data[i] = data[i] ^ PASSWORD[count];
 							count++;
 							if(count == strlen(PASSWORD))
 								count = 0;
 						}
 						printf("ENCRYPTED\n");
 						bytes = send(clientSocket, data, read4, MSG_NOSIGNAL);
 						printf("Bytes sent = %d\n",bytes);
 						printf("Data sent\n");
 					}
 					else if(msg_int == -1)
 					{
 						bytes = recv(clientSocket, &message_receive, sizeof(message_receive), 0);
 						printf("MESSAGE RECEIVED = %s\n", message_receive);
 						bzero(message_receive,strlen(message_receive));
 						printf("USERNAME AND PASSWORD NOT VERIFIED\n");
 					}

 					free(up);
 					free(rb);
 					free(data);
				}
				printf("***************************************************************** DFS 4 - Part 2 *****************************************************************\n");
				if(dfs4_part2 == 1)
				{
					printf("\nCLIENT REACHES dfs4_part2 == 1\n");

					struct username_password *up = malloc(sizeof(struct username_password));
					memset(up,0,sizeof(struct username_password));

					printf("USERNAME = %s\n",username);
 					strcpy(up->user,username);
 					printf("up->user = %s\n",up->user);
 					printf("PASSWORD = %s\n",password);
 					strncpy(up->passwd,password,strlen(password));
 					printf("up->passwd = %s\n",up->passwd);

					struct reliable_packet *rb = malloc(sizeof(struct reliable_packet));
					memset(rb,0,sizeof(struct reliable_packet));

					memcpy(rb->request_method, req_method, strlen(req_method));
					printf("rb->request_method = %s\n",rb->request_method);
					
					rb->packet_length = read1;
					printf("rb->packet_length = %d\n",rb->packet_length);

					char *data = (char *)malloc(read1);
					memcpy(data,buffer_p1,read1);

					strncpy(rb->fname,filename,strlen(filename));

					strncpy(rb->fold,"/DFS4/",strlen("/DFS4/"));
					if(fo_put == NULL)
						strncpy(rb->sub_fold,"NULL",strlen("NULL"));
					else
						strncpy(rb->sub_fold,fo_put,strlen(fo_put));
					rb->part_number = dfs4_part2;

					/*---- Create the socket. The three arguments are: ----*/
 					/* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
 					clientSocket = socket(PF_INET, SOCK_STREAM, 0);
 					/*---- Configure settings of the server address struct ----*/
 					/* Address family = Internet */
 					serverAddr.sin_family = AF_INET;
 					/* Set port number, using htons function to use proper byte order */
 					serverAddr.sin_port = htons(atoi(port4));
 					/* Set IP address to localhost */
 					serverAddr.sin_addr.s_addr = inet_addr(ip_address);
 					/* Set all bits of the padding field to 0 */ 
 					memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
 					/*---- Connect the socket to the server using the address struct ----*/
 					addr_size = sizeof serverAddr;
 					connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

 					printf("Packet length to send = %d\n", rb->packet_length);
    				printf("Packet request_method to send = %s\n", rb->request_method);

    				int bytes = send(clientSocket, up, sizeof(struct username_password), MSG_NOSIGNAL);
 					//printf("Bytes sent = %d\n",bytes);
 					printf("Username and Password sent\n");

 					bytes = recv(clientSocket, &msg_int, sizeof(msg_int), 0);

 					if(msg_int == 1)
 					{
 						bytes = send(clientSocket, rb, sizeof(struct reliable_packet), MSG_NOSIGNAL);
 						//printf("Bytes sent = %d\n",bytes);
 						printf("Packet sent\n");
 						int count = 0;
 						for(int i=0;i<read1;i++)
 						{
 							data[i] = data[i] ^ PASSWORD[count];
 							count++;
 							if(count == strlen(PASSWORD))
 								count = 0;
 						}
 						printf("ENCRYPTED\n");
 						bytes = send(clientSocket, data, read1, MSG_NOSIGNAL);
 						printf("Bytes sent = %d\n",bytes);
 						printf("Data sent\n");
 					}
 					else if(msg_int == -1)
 					{
 						bytes = recv(clientSocket, &message_receive, sizeof(message_receive), 0);
 						printf("MESSAGE RECEIVED = %s\n", message_receive);
 						bzero(message_receive,strlen(message_receive));
 						printf("USERNAME AND PASSWORD NOT VERIFIED\n");
 					}

 					free(up);
 					free(rb);
 					free(data);
				}
				else if(dfs4_part2 == 2)
				{
					printf("\nCLIENT REACHES dfs4_part2 == 2\n");

					struct username_password *up = malloc(sizeof(struct username_password));
					memset(up,0,sizeof(struct username_password));

					printf("USERNAME = %s\n",username);
 					strcpy(up->user,username);
 					printf("up->user = %s\n",up->user);
 					printf("PASSWORD = %s\n",password);
 					strncpy(up->passwd,password,strlen(password));
 					printf("up->passwd = %s\n",up->passwd);

					struct reliable_packet *rb = malloc(sizeof(struct reliable_packet));
					memset(rb,0,sizeof(struct reliable_packet));

					memcpy(rb->request_method, req_method, strlen(req_method));
					printf("rb->request_method = %s\n",rb->request_method);
					
					rb->packet_length = read2;
					printf("rb->packet_length = %d\n",rb->packet_length);

					char *data = (char *)malloc(read2);
					memcpy(data,buffer_p2,read2);

					strncpy(rb->fname,filename,strlen(filename));

					strncpy(rb->fold,"/DFS4/",strlen("/DFS4/"));
					if(fo_put == NULL)
						strncpy(rb->sub_fold,"NULL",strlen("NULL"));
					else
						strncpy(rb->sub_fold,fo_put,strlen(fo_put));
					rb->part_number = dfs4_part2;

					/*---- Create the socket. The three arguments are: ----*/
 					/* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
 					clientSocket = socket(PF_INET, SOCK_STREAM, 0);
 					/*---- Configure settings of the server address struct ----*/
 					/* Address family = Internet */
 					serverAddr.sin_family = AF_INET;
 					/* Set port number, using htons function to use proper byte order */
 					serverAddr.sin_port = htons(atoi(port4));
 					/* Set IP address to localhost */
 					serverAddr.sin_addr.s_addr = inet_addr(ip_address);
 					/* Set all bits of the padding field to 0 */ 
 					memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
 					/*---- Connect the socket to the server using the address struct ----*/
 					addr_size = sizeof serverAddr;
 					connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

 					printf("Packet length to send = %d\n", rb->packet_length);
    				printf("Packet request_method to send = %s\n", rb->request_method);

    				int bytes = send(clientSocket, up, sizeof(struct username_password), MSG_NOSIGNAL);
 					//printf("Bytes sent = %d\n",bytes);
 					printf("Username and Password sent\n");

 					bytes = recv(clientSocket, &msg_int, sizeof(msg_int), 0);

 					if(msg_int == 1)
 					{
 						bytes = send(clientSocket, rb, sizeof(struct reliable_packet), MSG_NOSIGNAL);
 						//printf("Bytes sent = %d\n",bytes);
 						printf("Packet sent\n");
 						int count = 0;
 						for(int i=0;i<read2;i++)
 						{
 							data[i] = data[i] ^ PASSWORD[count];
 							count++;
 							if(count == strlen(PASSWORD))
 								count = 0;
 						}
 						printf("ENCRYPTED\n");
 						bytes = send(clientSocket, data, read2, MSG_NOSIGNAL);
 						printf("Bytes sent = %d\n",bytes);
 						printf("Data sent\n");
 					}
 					else if(msg_int == -1)
 					{
 						bytes = recv(clientSocket, &message_receive, sizeof(message_receive), 0);
 						printf("MESSAGE RECEIVED = %s\n", message_receive);
 						bzero(message_receive,strlen(message_receive));
 						printf("USERNAME AND PASSWORD NOT VERIFIED\n");
 					}

 					free(up);
 					free(rb);
 					free(data);

				}
				else if(dfs4_part2 == 3)
				{
					printf("\nCLIENT REACHES dfs4_part2 == 3\n");

					struct username_password *up = malloc(sizeof(struct username_password));
					memset(up,0,sizeof(struct username_password));

					printf("USERNAME = %s\n",username);
 					strcpy(up->user,username);
 					printf("up->user = %s\n",up->user);
 					printf("PASSWORD = %s\n",password);
 					strncpy(up->passwd,password,strlen(password));
 					printf("up->passwd = %s\n",up->passwd);

					struct reliable_packet *rb = malloc(sizeof(struct reliable_packet));
					memset(rb,0,sizeof(struct reliable_packet));

					memcpy(rb->request_method, req_method, strlen(req_method));
					printf("rb->request_method = %s\n",rb->request_method);
					
					rb->packet_length = read3;
					printf("rb->packet_length = %d\n",rb->packet_length);

					char *data = (char *)malloc(read3);
					memcpy(data,buffer_p3,read3);

					strncpy(rb->fname,filename,strlen(filename));

					strncpy(rb->fold,"/DFS4/",strlen("/DFS4/"));
					if(fo_put == NULL)
						strncpy(rb->sub_fold,"NULL",strlen("NULL"));
					else
						strncpy(rb->sub_fold,fo_put,strlen(fo_put));
					rb->part_number = dfs4_part2;

					/*---- Create the socket. The three arguments are: ----*/
 					/* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
 					clientSocket = socket(PF_INET, SOCK_STREAM, 0);
 					/*---- Configure settings of the server address struct ----*/
 					/* Address family = Internet */
 					serverAddr.sin_family = AF_INET;
 					/* Set port number, using htons function to use proper byte order */
 					serverAddr.sin_port = htons(atoi(port4));
 					/* Set IP address to localhost */
 					serverAddr.sin_addr.s_addr = inet_addr(ip_address);
 					/* Set all bits of the padding field to 0 */ 
 					memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
 					/*---- Connect the socket to the server using the address struct ----*/
 					addr_size = sizeof serverAddr;
 					connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

 					printf("Packet length to send = %d\n", rb->packet_length);
    				printf("Packet request_method to send = %s\n", rb->request_method);

    				int bytes = send(clientSocket, up, sizeof(struct username_password), MSG_NOSIGNAL);
 					//printf("Bytes sent = %d\n",bytes);
 					printf("Username and Password sent\n");

 					bytes = recv(clientSocket, &msg_int, sizeof(msg_int), 0);

 					if(msg_int == 1)
 					{
 						bytes = send(clientSocket, rb, sizeof(struct reliable_packet), MSG_NOSIGNAL);
 						//printf("Bytes sent = %d\n",bytes);
 						printf("Packet sent\n");
 						int count = 0;
 						for(int i=0;i<read3;i++)
 						{
 							data[i] = data[i] ^ PASSWORD[count];
 							count++;
 							if(count == strlen(PASSWORD))
 								count = 0;
 						}
 						printf("ENCRYPTED\n");
 						bytes = send(clientSocket, data, read3, MSG_NOSIGNAL);
 						printf("Bytes sent = %d\n",bytes);
 						printf("Data sent\n");
 					}
 					else if(msg_int == -1)
 					{
 						bytes = recv(clientSocket, &message_receive, sizeof(message_receive), 0);
 						printf("MESSAGE RECEIVED = %s\n", message_receive);
 						bzero(message_receive,strlen(message_receive));
 						printf("USERNAME AND PASSWORD NOT VERIFIED\n");
 					}

 					free(up);
 					free(rb);
 					free(data);
				}
				else if(dfs4_part2 == 4)
				{
					printf("\nCLIENT REACHES dfs4_part2 == 4\n");

					struct username_password *up = malloc(sizeof(struct username_password));
					memset(up,0,sizeof(struct username_password));

					printf("USERNAME = %s\n",username);
 					strcpy(up->user,username);
 					printf("up->user = %s\n",up->user);
 					printf("PASSWORD = %s\n",password);
 					strncpy(up->passwd,password,strlen(password));
 					printf("up->passwd = %s\n",up->passwd);

					struct reliable_packet *rb = malloc(sizeof(struct reliable_packet));
					memset(rb,0,sizeof(struct reliable_packet));

					memcpy(rb->request_method, req_method, strlen(req_method));
					printf("rb->request_method = %s\n",rb->request_method);
					
					rb->packet_length = read4;
					printf("rb->packet_length = %d\n",rb->packet_length);

					char *data = (char *)malloc(read1);
					memcpy(data,buffer_p4,read4);

					strncpy(rb->fname,filename,strlen(filename));

					strncpy(rb->fold,"/DFS4/",strlen("/DFS4/"));
					if(fo_put == NULL)
						strncpy(rb->sub_fold,"NULL",strlen("NULL"));
					else
						strncpy(rb->sub_fold,fo_put,strlen(fo_put));
					rb->part_number = dfs4_part2;

					/*---- Create the socket. The three arguments are: ----*/
 					/* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
 					clientSocket = socket(PF_INET, SOCK_STREAM, 0);
 					/*---- Configure settings of the server address struct ----*/
 					/* Address family = Internet */
 					serverAddr.sin_family = AF_INET;
 					/* Set port number, using htons function to use proper byte order */
 					serverAddr.sin_port = htons(atoi(port4));
 					/* Set IP address to localhost */
 					serverAddr.sin_addr.s_addr = inet_addr(ip_address);
 					/* Set all bits of the padding field to 0 */ 
 					memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
 					/*---- Connect the socket to the server using the address struct ----*/
 					addr_size = sizeof serverAddr;
 					connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

 					printf("Packet length to send = %d\n", rb->packet_length);
    				printf("Packet request_method to send = %s\n", rb->request_method);

    				int bytes = send(clientSocket, up, sizeof(struct username_password), MSG_NOSIGNAL);
 					//printf("Bytes sent = %d\n",bytes);
 					printf("Username and Password sent\n");

 					bytes = recv(clientSocket, &msg_int, sizeof(msg_int), 0);

 					if(msg_int == 1)
 					{
 						bytes = send(clientSocket, rb, sizeof(struct reliable_packet), MSG_NOSIGNAL);
 						//printf("Bytes sent = %d\n",bytes);
 						printf("Packet sent\n");
 						int count = 0;
 						for(int i=0;i<read4;i++)
 						{
 							data[i] = data[i] ^ PASSWORD[count];
 							count++;
 							if(count == strlen(PASSWORD))
 								count = 0;
 						}
 						printf("ENCRYPTED\n");
 						bytes = send(clientSocket, data, read4, MSG_NOSIGNAL);
 						printf("Bytes sent = %d\n",bytes);
 						printf("Data sent\n");
 					}
 					else if(msg_int == -1)
 					{
 						bytes = recv(clientSocket, &message_receive, sizeof(message_receive), 0);
 						printf("MESSAGE RECEIVED = %s\n", message_receive);
 						bzero(message_receive,strlen(message_receive));
 						printf("USERNAME AND PASSWORD NOT VERIFIED\n");
 					}

 					free(up);
 					free(rb);
 					free(data);
				}
				free(buffer_p1);
				free(buffer_p2);
				free(buffer_p3);
				free(buffer_p4);
            }
		}

		else if(checker(req_method,"LIST") == 1)
        {
        	char list_buffer[MAXBUFSIZE];
        	bzero(list_buffer, MAXBUFSIZE);
        	bzero(final_list, MAXBUFSIZE);

        	printf("List files\n");

        	char *fo_list = strtok(NULL,"\n");
			printf("FO = %s\n",fo_list);

        	struct username_password *up = malloc(sizeof(struct username_password));
			memset(up,0,sizeof(struct username_password));

			printf("USERNAME = %s\n",username);
 			strcpy(up->user,username);
 			printf("up->user = %s\n",up->user);
 			printf("PASSWORD = %s\n",password);
 			strncpy(up->passwd,password,strlen(password));
 			printf("up->passwd = %s\n",up->passwd);

 			struct reliable_packet *rb = malloc(sizeof(struct reliable_packet));
			memset(rb,0,sizeof(struct reliable_packet));

			memcpy(rb->request_method, req_method, strlen(req_method));
			printf("rb->request_method = %s\n",rb->request_method);

			printf("\n");
 			printf("/*------------------------------------------------------------------------------ For DFS1 ------------------------------------------------------------------------------*/");
 			printf("\n");
			bzero(rb->fold,MAXBUFSIZE);
			strncpy(rb->fold,"/DFS1/",strlen("/DFS1/"));
			if(fo_list == NULL)
				strncpy(rb->sub_fold,"NULL",strlen("NULL"));
			else
				strncpy(rb->sub_fold,fo_list,strlen(fo_list));

 			/*---- Create the socket. The three arguments are: ----*/
 			/* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
 			clientSocket = socket(PF_INET, SOCK_STREAM, 0);
 			/*---- Configure settings of the server address struct ----*/
 			/* Address family = Internet */
 			serverAddr.sin_family = AF_INET;
 			/* Set port number, using htons function to use proper byte order */
 			serverAddr.sin_port = htons(atoi(port1));
 			/* Set IP address to localhost */
 			serverAddr.sin_addr.s_addr = inet_addr(ip_address);
 			/* Set all bits of the padding field to 0 */ 
 			memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
 			/*---- Connect the socket to the server using the address struct ----*/
 			addr_size = sizeof serverAddr;
 			connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

 			int bytes = send(clientSocket, up, sizeof(struct username_password), MSG_NOSIGNAL);
 			//printf("Username and Password sent\n");

 			bytes = recv(clientSocket, &msg_int, sizeof(msg_int), 0);

 			if(msg_int == 1)
 			{
 				bytes = send(clientSocket, rb, sizeof(struct reliable_packet), MSG_NOSIGNAL);
 				//printf("Packet sent\n");
 				bzero(dfs1_list,MAXBUFSIZE);
 				bytes = recv(clientSocket, &dfs1_list, sizeof(dfs1_list), 0);
 				bytes_rec_dfs1 = bytes;
 				//strncat(final_list,dfs1_list,strlen(dfs1_list));
 				printf("Received list:\n");
 				printf("%s",dfs1_list);
 				printf("\n");
 			}
 			else if(msg_int == -1)
 			{
 				bytes = recv(clientSocket, &message_receive, sizeof(message_receive), 0);
 				bzero(message_receive,strlen(message_receive));
 				printf("USERNAME AND PASSWORD NOT VERIFIED\n");
 			}

 			printf("\n");
 			printf("/*------------------------------------------------------------------------------ For DFS2 ------------------------------------------------------------------------------*/");
 			printf("\n");
			bzero(rb->fold,MAXBUFSIZE);
			strncpy(rb->fold,"/DFS2/",strlen("/DFS2/"));
			if(fo_list == NULL)
				strncpy(rb->sub_fold,"NULL",strlen("NULL"));
			else
				strncpy(rb->sub_fold,fo_list,strlen(fo_list));

 			/*---- Create the socket. The three arguments are: ----*/
 			/* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
 			clientSocket = socket(PF_INET, SOCK_STREAM, 0);
 			/*---- Configure settings of the server address struct ----*/
 			/* Address family = Internet */
 			serverAddr.sin_family = AF_INET;
 			/* Set port number, using htons function to use proper byte order */
 			serverAddr.sin_port = htons(atoi(port2));
 			/* Set IP address to localhost */
 			serverAddr.sin_addr.s_addr = inet_addr(ip_address);
 			/* Set all bits of the padding field to 0 */ 
 			memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
 			/*---- Connect the socket to the server using the address struct ----*/
 			addr_size = sizeof serverAddr;
 			connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

 			bytes = send(clientSocket, up, sizeof(struct username_password), MSG_NOSIGNAL);
 			//printf("Username and Password sent\n");

 			bytes = recv(clientSocket, &msg_int, sizeof(msg_int), 0);

 			if(msg_int == 1)
 			{
 				bytes = send(clientSocket, rb, sizeof(struct reliable_packet), MSG_NOSIGNAL);
 				printf("Packet sent\n");
 				bzero(dfs2_list,MAXBUFSIZE);
 				bytes = recv(clientSocket, &dfs2_list, sizeof(dfs2_list), 0);
 				bytes_rec_dfs2 = bytes;
 				//strncat(final_list,dfs2_list,strlen(dfs2_list));
 				printf("Received list:\n");
 				printf("%s",dfs2_list);
 				printf("\n");
 			}
 			else if(msg_int == -1)
 			{
 				bytes = recv(clientSocket, &message_receive, sizeof(message_receive), 0);
 				bzero(message_receive,strlen(message_receive));
 				printf("USERNAME AND PASSWORD NOT VERIFIED\n");
 			}

 			printf("\n");
 			printf("/*------------------------------------------------------------------------------ For DFS3 ------------------------------------------------------------------------------*/");
 			printf("\n");
			bzero(rb->fold,MAXBUFSIZE);
			strncpy(rb->fold,"/DFS3/",strlen("/DFS3/"));
			if(fo_list == NULL)
				strncpy(rb->sub_fold,"NULL",strlen("NULL"));
			else
				strncpy(rb->sub_fold,fo_list,strlen(fo_list));

 			/*---- Create the socket. The three arguments are: ----*/
 			/* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
 			clientSocket = socket(PF_INET, SOCK_STREAM, 0);
 			/*---- Configure settings of the server address struct ----*/
 			/* Address family = Internet */
 			serverAddr.sin_family = AF_INET;
 			/* Set port number, using htons function to use proper byte order */
 			serverAddr.sin_port = htons(atoi(port3));
 			/* Set IP address to localhost */
 			serverAddr.sin_addr.s_addr = inet_addr(ip_address);
 			/* Set all bits of the padding field to 0 */ 
 			memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
 			/*---- Connect the socket to the server using the address struct ----*/
 			addr_size = sizeof serverAddr;
 			connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

 			bytes = send(clientSocket, up, sizeof(struct username_password), MSG_NOSIGNAL);
 			//printf("Username and Password sent\n");

 			bytes = recv(clientSocket, &msg_int, sizeof(msg_int), 0);

 			if(msg_int == 1)
 			{
 				bytes = send(clientSocket, rb, sizeof(struct reliable_packet), MSG_NOSIGNAL);
 				//printf("Packet sent\n");
 				bzero(dfs3_list,MAXBUFSIZE);
 				bytes = recv(clientSocket, &dfs3_list, sizeof(dfs3_list), 0);
 				bytes_rec_dfs3 = bytes;
 				//strncat(final_list,dfs3_list,strlen(dfs3_list));
 				printf("Received list:\n");
 				printf("%s",dfs3_list);
 				printf("\n");
 			}
 			else if(msg_int == -1)
 			{
 				bytes = recv(clientSocket, &message_receive, sizeof(message_receive), 0);
 				bzero(message_receive,strlen(message_receive));
 				printf("USERNAME AND PASSWORD NOT VERIFIED\n");
 			}

 			printf("\n");
 			printf("/*------------------------------------------------------------------------------ For DFS4 ------------------------------------------------------------------------------*/");
 			printf("\n");
			bzero(rb->fold,MAXBUFSIZE);
			strncpy(rb->fold,"/DFS4/",strlen("/DFS4/"));
			if(fo_list == NULL)
				strncpy(rb->sub_fold,"NULL",strlen("NULL"));
			else
				strncpy(rb->sub_fold,fo_list,strlen(fo_list));

 			/*---- Create the socket. The three arguments are: ----*/
 			/* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
 			clientSocket = socket(PF_INET, SOCK_STREAM, 0);
 			/*---- Configure settings of the server address struct ----*/
 			/* Address family = Internet */
 			serverAddr.sin_family = AF_INET;
 			/* Set port number, using htons function to use proper byte order */
 			serverAddr.sin_port = htons(atoi(port4));
 			/* Set IP address to localhost */
 			serverAddr.sin_addr.s_addr = inet_addr(ip_address);
 			/* Set all bits of the padding field to 0 */ 
 			memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
 			/*---- Connect the socket to the server using the address struct ----*/
 			addr_size = sizeof serverAddr;
 			connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

 			bytes = send(clientSocket, up, sizeof(struct username_password), MSG_NOSIGNAL);
 			//printf("Username and Password sent\n");

 			bytes = recv(clientSocket, &msg_int, sizeof(msg_int), 0);

 			if(msg_int == 1)
 			{
 				bytes = send(clientSocket, rb, sizeof(struct reliable_packet), MSG_NOSIGNAL);
 				//printf("Packet sent\n");
 				bzero(dfs4_list,MAXBUFSIZE);
 				bytes = recv(clientSocket, &dfs4_list, sizeof(dfs4_list), 0);
 				bytes_rec_dfs4 = bytes;
 				printf("Received list:\n");
 				printf("%s",dfs4_list);
 				printf("\n");
 			}
 			else if(msg_int == -1)
 			{
 				bytes = recv(clientSocket, &message_receive, sizeof(message_receive), 0);
 				bzero(message_receive,strlen(message_receive));
 				printf("USERNAME AND PASSWORD NOT VERIFIED\n");
 			}

 			free(up);
 			free(rb);

 			if(bytes_rec_dfs1 > 0)
 			{
 				bzero(temp,MAXBUFSIZE);
 				strncpy(temp,dfs1_list,strlen(dfs1_list));
 			}
 			else if(bytes_rec_dfs2 > 0)
 			{
 				bzero(temp,MAXBUFSIZE);
 				strncpy(temp,dfs2_list,strlen(dfs2_list));
 			}
 			else if(bytes_rec_dfs3 > 0)
 			{
 				bzero(temp,MAXBUFSIZE);
 				strncpy(temp,dfs3_list,strlen(dfs3_list));
 			}
 			else if(bytes_rec_dfs4 > 0)
 			{
 				bzero(temp,MAXBUFSIZE);
 				strncpy(temp,dfs4_list,strlen(dfs4_list));
 			}
 			printf("TEMP = %s\n",temp);

 			char *end_token_1;
 			char *end_token_2;
 			char *end_token_3;
			char *l = strtok_r(temp,"\n", &end_token_1);
			char character[MAXBUFSIZE];
			int num_zero = 0;
			char name[MAXBUFSIZE];

 			while(l != NULL)
 			{
 				flag_1=0;
 				flag_2=0;
 				flag_3=0;
 				flag_4=0;

 				bzero(character,MAXBUFSIZE);
 				num_zero = 0;
 				strncpy(character,l,strlen(l));
 				char *c = strtok_r(character,".", &end_token_2);

 				while(c != NULL)
 				{
 					num_zero++;
 					c = strtok_r(NULL, ".", &end_token_2);
 				}
 				bzero(character,MAXBUFSIZE);
 				strncpy(character,l,strlen(l));

 				if(num_zero == 2)
 				{
 					bzero(fn, MAXBUFSIZE);
 					sprintf(fn,".");
 					c = strtok_r(character, ".", &end_token_3);
 					bzero(name,MAXBUFSIZE);
 					strncpy(name,c,strlen(c));
 					strncat(fn,c,strlen(c));
 
 					bzero(f,MAXBUFSIZE);
 					strncpy(f,fn,strlen(fn));

 					strncat(f,".",strlen("."));
 					strncat(f,"1",strlen("1"));
 					

 					if((strstr(dfs1_list,f) != NULL) || (strstr(dfs2_list,f) != NULL) || (strstr(dfs3_list,f) != NULL) || (strstr(dfs4_list,f) != NULL))
 						flag_1 = 1;
 					else
 						flag_1 = -1;

 					bzero(f,MAXBUFSIZE);
 					strncpy(f,fn,strlen(fn));
 					strncat(f,".",strlen("."));
 					strncat(f,"2",strlen("1"));

 					if((strstr(dfs1_list,f) != NULL) || (strstr(dfs2_list,f) != NULL) || (strstr(dfs3_list,f) != NULL) || (strstr(dfs4_list,f) != NULL))
 						flag_2 = 1;
 					else
 						flag_2 = -1;

 					bzero(f,MAXBUFSIZE);
 					strncpy(f,fn,strlen(fn));
 					strncat(f,".",strlen("."));
 					strncat(f,"3",strlen("1"));

 					if((strstr(dfs1_list,f) != NULL) || (strstr(dfs2_list,f) != NULL) || (strstr(dfs3_list,f) != NULL) || (strstr(dfs4_list,f) != NULL))
 						flag_3 = 1;
 					else
 						flag_3 = -1;

 					bzero(f,MAXBUFSIZE);
 					strncpy(f,fn,strlen(fn));
 					strncat(f,".",strlen("."));
 					strncat(f,"4",strlen("1"));

 					if((strstr(dfs1_list,f) != NULL) || (strstr(dfs2_list,f) != NULL) || (strstr(dfs3_list,f) != NULL) || (strstr(dfs4_list,f) != NULL))
 						flag_4 = 1;
 					else
 						flag_4 = -1;

 					if((flag_1 == -1) || (flag_2 == -1) || (flag_3 == -1) || (flag_4 == -1))
 					{
 						if(strstr(final_list,name) == NULL)
 						{
 							strncat(name," [incomplete]",strlen(" [incomplete]"));
 							strncat(final_list,name,strlen(name));
 							strncat(final_list,"\n",strlen("\n"));
 						}
 					}
 					else
 					{
 						if(strstr(final_list,name) == NULL)
 						{
 							strncat(final_list,name,strlen(name));
 							strncat(final_list,"\n",strlen("\n"));
 						}
 					}
 				}
 				else if(num_zero == 3)
 				{
 					bzero(fn, MAXBUFSIZE);
 					sprintf(fn,".");
 					c = strtok_r(character, ".", &end_token_3);
 					bzero(name,MAXBUFSIZE);
 					strncpy(name,c,strlen(c));
 					strncat(fn,c,strlen(c));
 					c = strtok_r(NULL, ".", &end_token_3);
 					strncat(fn,".",strlen("."));
 					strncat(fn,c,strlen(c));
 					strncat(name,".",strlen("."));
 					strncat(name,c,strlen(c));

 					bzero(f,MAXBUFSIZE);
 					strncpy(f,fn,strlen(fn));
 					strncat(f,".",strlen("."));
 					strncat(f,"1",strlen("1"));

 					if((strstr(dfs1_list,f) != NULL) || (strstr(dfs2_list,f) != NULL) || (strstr(dfs3_list,f) != NULL) || (strstr(dfs4_list,f) != NULL))
 						flag_1 = 1;
 					else
 						flag_1 = -1;

 					bzero(f,MAXBUFSIZE);
 					strncpy(f,fn,strlen(fn));
 					strncat(f,".",strlen("."));
 					strncat(f,"2",strlen("1"));


 					if((strstr(dfs1_list,f) != NULL) || (strstr(dfs2_list,f) != NULL) || (strstr(dfs3_list,f) != NULL) || (strstr(dfs4_list,f) != NULL))
 						flag_2 = 1;
 					else
 						flag_2 = -1;

 					bzero(f,MAXBUFSIZE);
 					strncpy(f,fn,strlen(fn));
 					strncat(f,".",strlen("."));
 					strncat(f,"3",strlen("1"));

 					if((strstr(dfs1_list,f) != NULL) || (strstr(dfs2_list,f) != NULL) || (strstr(dfs3_list,f) != NULL) || (strstr(dfs4_list,f) != NULL))
 						flag_3 = 1;
 					else
 						flag_3 = -1;

 					bzero(f,MAXBUFSIZE);
 					strncpy(f,fn,strlen(fn));
 					strncat(f,".",strlen("."));
 					strncat(f,"4",strlen("1"));

 					if((strstr(dfs1_list,f) != NULL) || (strstr(dfs2_list,f) != NULL) || (strstr(dfs3_list,f) != NULL) || (strstr(dfs4_list,f) != NULL))
 						flag_4 = 1;
 					else
 						flag_4 = -1;

 					if((flag_1 == -1) || (flag_2 == -1) || (flag_3 == -1) || (flag_4 == -1))
 					{
 						if(strstr(final_list,name) == NULL)
 						{
 							strncat(name," [incomplete]",strlen(" [incomplete]"));
 							strncat(final_list,name,strlen(name));
 							strncat(final_list,"\n",strlen("\n"));
 						}
 					}
 					else
 					{
 						if(strstr(final_list,name) == NULL)
 						{
 							strncat(final_list,name,strlen(name));
 							strncat(final_list,"\n",strlen("\n"));
 						}
 					}
 				}

 				l = strtok_r(NULL,"\n", &end_token_1);
 			}
 			printf("FINAL LIST = \n");
 			printf("%s",final_list);
        }

        else if(checker(req_method,"GET") == 1)
        {
        	printf("GET files\n");

        	flag_1=0;
        	flag_2=0;
        	flag_3=0;
        	flag_4=0;

        	read_1=0;
        	read_2=0;
        	read_3=0;
        	read_4=0;

        	bzero(what_i_have,MAXBUFSIZE);

        	char message[MAXBUFSIZE];
			char *filename = strtok(NULL," ");
			printf("FILENAME = %s\n", filename);
			char *fo_get = strtok(NULL,"\n");
			printf("FO = %s\n",fo_get);

			printf("***************************************************************** DFS 1 *****************************************************************\n");

			struct username_password *up = malloc(sizeof(struct username_password));
			memset(up,0,sizeof(struct username_password));

			printf("USERNAME = %s\n",username);
 			strcpy(up->user,username);
 			printf("up->user = %s\n",up->user);
 			printf("PASSWORD = %s\n",password);
 			strncpy(up->passwd,password,strlen(password));
 			printf("up->passwd = %s\n",up->passwd);

 			bzero(PASSWORD,MAXBUFSIZE);
 			strncpy(PASSWORD,up->passwd,strlen(up->passwd));

			struct reliable_packet *rb = malloc(sizeof(struct reliable_packet));
			memset(rb,0,sizeof(struct reliable_packet));

			memcpy(rb->request_method, req_method, strlen(req_method));
			printf("rb->request_method = %s\n",rb->request_method);
					
			strncpy(rb->fname,filename,strlen(filename));

			strncpy(rb->fold,"/DFS1/",strlen("/DFS1/"));

			if(fo_get == NULL)
				strncpy(rb->sub_fold,"NULL",strlen("NULL"));
			else
				strncpy(rb->sub_fold,fo_get,strlen(fo_get));

			/*---- Create the socket. The three arguments are: ----*/
 			/* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
 			clientSocket = socket(PF_INET, SOCK_STREAM, 0);
 			/*---- Configure settings of the server address struct ----*/
 			/* Address family = Internet */
 			serverAddr.sin_family = AF_INET;
 			/* Set port number, using htons function to use proper byte order */
 			serverAddr.sin_port = htons(atoi(port1));
 			/* Set IP address to localhost */
 			serverAddr.sin_addr.s_addr = inet_addr(ip_address);
 			/* Set all bits of the padding field to 0 */ 
 			memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
 			/*---- Connect the socket to the server using the address struct ----*/
 			addr_size = sizeof serverAddr;
 			connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

    		int bytes = send(clientSocket, up, sizeof(struct username_password), MSG_NOSIGNAL);
 			printf("Bytes sent = %d\n",bytes);
 			printf("Username and Password sent\n");

 			bytes = recv(clientSocket, &msg_int, sizeof(msg_int), 0);

 			is_file = -1;

 			if(msg_int == 1)
 			{
 				printf("Positive message received\n");
 				bytes = send(clientSocket, rb, sizeof(struct reliable_packet), MSG_NOSIGNAL);
 				printf("Bytes sent = %d\n",bytes);
 				printf("Packet sent\n");

 				bytes = recv(clientSocket, &is_file, sizeof(is_file), 0);

 				if(is_file == 5)
 				{
 					printf("Server has file %s\n",rb->fname);
 					send_file = 5;
 					bytes = send(clientSocket, &send_file, sizeof(send_file), MSG_NOSIGNAL);

 					int index = 2;
 					while(index !=0 )
 					{
 						struct get_packet *gp = malloc(sizeof(struct get_packet));
                    	memset(gp,0,sizeof(struct get_packet));

 						bytes = recv(clientSocket, gp, sizeof(struct get_packet), 0);
 						printf("HERE\n");
 						printf("gp->number = %d\n",gp->number);
 						printf("gp->size = %d\n",gp->size);

 						char n[1];
 						bzero(n,1);

 						if(gp->number == 1)
 							strncpy(n,"1",strlen("1"));
 						else if(gp->number == 2)
 							strncpy(n,"2",strlen("2"));
 						else if(gp->number == 3)
 							strncpy(n,"3",strlen("3"));
 						else if(gp->number == 4)
 							strncpy(n,"4",strlen("4"));

 						bzero(get_temp,MAXBUFSIZE);
 						strncpy(get_temp,filename,strlen(filename));
 						strncat(get_temp,".",strlen("."));
 						strncat(get_temp,n,strlen(n));

 						printf("what_i_have = %s\n",what_i_have);
 						printf("get_temp = %s\n",get_temp);

 						if(strstr(what_i_have,get_temp) == NULL)
 						{
 							strncat(what_i_have,get_temp,strlen(get_temp));

 							if(gp->number == 1)
 								flag_1 = 55;
 							else if(gp->number == 2)
 								flag_2 = 55;
 							else if(gp->number == 3)
 								flag_3 = 55;
 							else if(gp->number == 4)
 								flag_4 = 55;

 							// send ack
 							bzero(M,3);
 							strncpy(M,"ACK",strlen("ACK"));
 							bytes = send(clientSocket, &M, sizeof(M), MSG_NOSIGNAL);

 							if(gp->number == 1)
 							{
 								buf_1 = (char *)malloc(gp->size);
 								bytes = recv(clientSocket, buf_1, gp->size, 0);
 								read_1 = bytes;
 								printf("DECRYPTED\n");
 								int count = 0;
 								for(int i=0;i<bytes;i++)
 								{
 									buf_1[i] = buf_1[i] ^ PASSWORD[count];
 									count++;
 									if(count == strlen(PASSWORD))
 										count = 0;
 								}
 								printf("buffer 1 received\n");
 								printf("No. of bytes received = %d\n",bytes);
 							}
 							else if(gp->number == 2)
 							{
 								buf_2 = (char *)malloc(gp->size);
 								bytes = recv(clientSocket, buf_2, gp->size, 0);
 								read_2 = bytes;
 								printf("DECRYPTED\n");
 								int count = 0;
 								for(int i=0;i<bytes;i++)
 								{
 									buf_2[i] = buf_2[i] ^ PASSWORD[count];
 									count++;
 									if(count == strlen(PASSWORD))
 										count = 0;
 								}
 								printf("buffer 2 received\n");
 								printf("No. of bytes received = %d\n",bytes);
 							}
 							else if(gp->number == 3)
 							{
 								buf_3 = (char *)malloc(gp->size);
 								bytes = recv(clientSocket, buf_3, gp->size, 0);
 								read_3 = bytes;
 								printf("DECRYPTED\n");
 								int count = 0;
 								for(int i=0;i<bytes;i++)
 								{
 									buf_3[i] = buf_3[i] ^ PASSWORD[count];
 									count++;
 									if(count == strlen(PASSWORD))
 										count = 0;
 								}
 								printf("buffer 3 received\n");
 								printf("No. of bytes received = %d\n",bytes);
 							}
 							else if(gp->number == 4)
 							{
 								buf_4 = (char *)malloc(gp->size);
 								bytes = recv(clientSocket, buf_4, gp->size, 0);
 								read_4 = bytes;
 								printf("DECRYPTED\n");
 								int count = 0;
 								for(int i=0;i<bytes;i++)
 								{
 									buf_4[i] = buf_4[i] ^ PASSWORD[count];
 									count++;
 									if(count == strlen(PASSWORD))
 										count = 0;
 								}
 								printf("buffer 4 received\n");
 								printf("No. of bytes received = %d\n",bytes);
 							}
 						}
 						else
 						{
 							// send nak
 							bzero(M,3);
 							strncpy(M,"NAK",strlen("NAK"));
 							bytes = send(clientSocket, &M, sizeof(M), MSG_NOSIGNAL);
 						}
 						index--;
 						free(gp);
 					}
 				}
 			}
 			else if(msg_int == -1)
 			{
 				bytes = recv(clientSocket, &message_receive, sizeof(message_receive), 0);
 				printf("MESSAGE RECEIVED = %s\n", message_receive);
 				bzero(message_receive,strlen(message_receive));
 				printf("USERNAME AND PASSWORD NOT VERIFIED\n");
 			}

			printf("***************************************************************** DFS 2 *****************************************************************\n");

			memset(up,0,sizeof(struct username_password));

			printf("USERNAME = %s\n",username);
 			strcpy(up->user,username);
 			printf("up->user = %s\n",up->user);
 			printf("PASSWORD = %s\n",password);
 			strncpy(up->passwd,password,strlen(password));
 			printf("up->passwd = %s\n",up->passwd);

			//struct reliable_packet *rb = malloc(sizeof(struct reliable_packet));
			memset(rb,0,sizeof(struct reliable_packet));

			memcpy(rb->request_method, req_method, strlen(req_method));
			printf("rb->request_method = %s\n",rb->request_method);
					
			strncpy(rb->fname,filename,strlen(filename));

			strncpy(rb->fold,"/DFS2/",strlen("/DFS2/"));

			if(fo_get == NULL)
				strncpy(rb->sub_fold,"NULL",strlen("NULL"));
			else
				strncpy(rb->sub_fold,fo_get,strlen(fo_get));

			/*---- Create the socket. The three arguments are: ----*/
 			/* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
 			clientSocket = socket(PF_INET, SOCK_STREAM, 0);
 			/*---- Configure settings of the server address struct ----*/
 			/* Address family = Internet */
 			serverAddr.sin_family = AF_INET;
 			/* Set port number, using htons function to use proper byte order */
 			serverAddr.sin_port = htons(atoi(port2));
 			/* Set IP address to localhost */
 			serverAddr.sin_addr.s_addr = inet_addr(ip_address);
 			/* Set all bits of the padding field to 0 */ 
 			memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
 			/*---- Connect the socket to the server using the address struct ----*/
 			addr_size = sizeof serverAddr;
 			connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

    		bytes = send(clientSocket, up, sizeof(struct username_password), MSG_NOSIGNAL);
 			printf("Bytes sent = %d\n",bytes);
 			printf("Username and Password sent\n");

 			bytes = recv(clientSocket, &msg_int, sizeof(msg_int), 0);

 			is_file = -1;

 			if(msg_int == 1)
 			{
 				printf("Positive message received\n");
 				bytes = send(clientSocket, rb, sizeof(struct reliable_packet), MSG_NOSIGNAL);
 				printf("Bytes sent = %d\n",bytes);
 				printf("Packet sent\n");

 				bytes = recv(clientSocket, &is_file, sizeof(is_file), 0);

 				if(is_file == 5)
 				{
 					printf("Server has file %s\n",rb->fname);
 					send_file = 5;
 					bytes = send(clientSocket, &send_file, sizeof(send_file), MSG_NOSIGNAL);

 					int index = 2;
 					while(index !=0 )
 					{
 						struct get_packet *gp = malloc(sizeof(struct get_packet));
                    	memset(gp,0,sizeof(struct get_packet));

 						bytes = recv(clientSocket, gp, sizeof(struct get_packet), 0);
 						printf("HERE\n");
 						printf("gp->number = %d\n",gp->number);
 						printf("gp->size = %d\n",gp->size);

 						char n[1];
 						bzero(n,1);

 						if(gp->number == 1)
 							strncpy(n,"1",strlen("1"));
 						else if(gp->number == 2)
 							strncpy(n,"2",strlen("2"));
 						else if(gp->number == 3)
 							strncpy(n,"3",strlen("3"));
 						else if(gp->number == 4)
 							strncpy(n,"4",strlen("4"));

 						bzero(get_temp,MAXBUFSIZE);
 						strncpy(get_temp,filename,strlen(filename));
 						strncat(get_temp,".",strlen("."));
 						strncat(get_temp,n,strlen(n));

 						printf("what_i_have = %s\n",what_i_have);
 						printf("get_temp = %s\n",get_temp);

 						if(strstr(what_i_have,get_temp) == NULL)
 						{
 							strncat(what_i_have,get_temp,strlen(get_temp));

 							if(gp->number == 1)
 								flag_1 = 55;
 							else if(gp->number == 2)
 								flag_2 = 55;
 							else if(gp->number == 3)
 								flag_3 = 55;
 							else if(gp->number == 4)
 								flag_4 = 55;

 							// send ack
 							bzero(M,3);
 							strncpy(M,"ACK",strlen("ACK"));
 							bytes = send(clientSocket, &M, sizeof(M), MSG_NOSIGNAL);

 							if(gp->number == 1)
 							{
 								buf_1 = (char *)malloc(gp->size);
 								bytes = recv(clientSocket, buf_1, gp->size, 0);
 								read_1 = bytes;
 								printf("buffer 1 received\n");
 								printf("No. of bytes received = %d\n",bytes);
 								printf("DECRYPTED\n");
 								int count = 0;
 								for(int i=0;i<bytes;i++)
 								{
 									buf_1[i] = buf_1[i] ^ PASSWORD[count];
 									count++;
 									if(count == strlen(PASSWORD))
 										count = 0;
 								}
 							}
 							else if(gp->number == 2)
 							{
 								buf_2 = (char *)malloc(gp->size);
 								bytes = recv(clientSocket, buf_2, gp->size, 0);
 								read_2 = bytes;
 								printf("buffer 2 received\n");
 								printf("No. of bytes received = %d\n",bytes);
 								printf("DECRYPTED\n");
 								int count = 0;
 								for(int i=0;i<bytes;i++)
 								{
 									buf_2[i] = buf_2[i] ^ PASSWORD[count];
 									count++;
 									if(count == strlen(PASSWORD))
 										count = 0;
 								}
 							}
 							else if(gp->number == 3)
 							{
 								buf_3 = (char *)malloc(gp->size);
 								bytes = recv(clientSocket, buf_3, gp->size, 0);
 								read_3 = bytes;
 								printf("buffer 3 received\n");
 								printf("No. of bytes received = %d\n",bytes);
 								printf("DECRYPTED\n");
 								int count = 0;
 								for(int i=0;i<bytes;i++)
 								{
 									buf_3[i] = buf_3[i] ^ PASSWORD[count];
 									count++;
 									if(count == strlen(PASSWORD))
 										count = 0;
 								}
 							}
 							else if(gp->number == 4)
 							{
 								buf_4 = (char *)malloc(gp->size);
 								bytes = recv(clientSocket, buf_4, gp->size, 0);
 								read_4 = bytes;
 								printf("buffer 4 received\n");
 								printf("No. of bytes received = %d\n",bytes);
 								printf("DECRYPTED\n");
 								int count = 0;
 								for(int i=0;i<bytes;i++)
 								{
 									buf_4[i] = buf_4[i] ^ PASSWORD[count];
 									count++;
 									if(count == strlen(PASSWORD))
 										count = 0;
 								}
 							}
 						}
 						else
 						{
 							// send nak
 							bzero(M,3);
 							strncpy(M,"NAK",strlen("NAK"));
 							bytes = send(clientSocket, &M, sizeof(M), MSG_NOSIGNAL);
 						}
 						index--;
 						free(gp);
 					}
 				}
 			}
 			else if(msg_int == -1)
 			{
 				bytes = recv(clientSocket, &message_receive, sizeof(message_receive), 0);
 				printf("MESSAGE RECEIVED = %s\n", message_receive);
 				bzero(message_receive,strlen(message_receive));
 				printf("USERNAME AND PASSWORD NOT VERIFIED\n");
 			}
 			printf("***************************************************************** DFS 3 *****************************************************************\n");

			memset(up,0,sizeof(struct username_password));

			printf("USERNAME = %s\n",username);
 			strcpy(up->user,username);
 			printf("up->user = %s\n",up->user);
 			printf("PASSWORD = %s\n",password);
 			strncpy(up->passwd,password,strlen(password));
 			printf("up->passwd = %s\n",up->passwd);

			//struct reliable_packet *rb = malloc(sizeof(struct reliable_packet));
			memset(rb,0,sizeof(struct reliable_packet));

			memcpy(rb->request_method, req_method, strlen(req_method));
			printf("rb->request_method = %s\n",rb->request_method);
					
			strncpy(rb->fname,filename,strlen(filename));

			strncpy(rb->fold,"/DFS3/",strlen("/DFS3/"));

			if(fo_get == NULL)
				strncpy(rb->sub_fold,"NULL",strlen("NULL"));
			else
				strncpy(rb->sub_fold,fo_get,strlen(fo_get));

			/*---- Create the socket. The three arguments are: ----*/
 			/* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
 			clientSocket = socket(PF_INET, SOCK_STREAM, 0);
 			/*---- Configure settings of the server address struct ----*/
 			/* Address family = Internet */
 			serverAddr.sin_family = AF_INET;
 			/* Set port number, using htons function to use proper byte order */
 			serverAddr.sin_port = htons(atoi(port3));
 			/* Set IP address to localhost */
 			serverAddr.sin_addr.s_addr = inet_addr(ip_address);
 			/* Set all bits of the padding field to 0 */ 
 			memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
 			/*---- Connect the socket to the server using the address struct ----*/
 			addr_size = sizeof serverAddr;
 			connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

    		bytes = send(clientSocket, up, sizeof(struct username_password), MSG_NOSIGNAL);
 			printf("Bytes sent = %d\n",bytes);
 			printf("Username and Password sent\n");

 			bytes = recv(clientSocket, &msg_int, sizeof(msg_int), 0);

 			is_file = -1;

 			if(msg_int == 1)
 			{
 				printf("Positive message received\n");
 				bytes = send(clientSocket, rb, sizeof(struct reliable_packet), MSG_NOSIGNAL);
 				printf("Bytes sent = %d\n",bytes);
 				printf("Packet sent\n");

 				bytes = recv(clientSocket, &is_file, sizeof(is_file), 0);

 				if(is_file == 5)
 				{
 					printf("Server has file %s\n",rb->fname);
 					send_file = 5;
 					bytes = send(clientSocket, &send_file, sizeof(send_file), MSG_NOSIGNAL);

 					int index = 2;
 					while(index !=0 )
 					{
 						struct get_packet *gp = malloc(sizeof(struct get_packet));
                    	memset(gp,0,sizeof(struct get_packet));

 						bytes = recv(clientSocket, gp, sizeof(struct get_packet), 0);
 						printf("HERE\n");
 						printf("gp->number = %d\n",gp->number);
 						printf("gp->size = %d\n",gp->size);

 						char n[1];
 						bzero(n,1);

 						if(gp->number == 1)
 							strncpy(n,"1",strlen("1"));
 						else if(gp->number == 2)
 							strncpy(n,"2",strlen("2"));
 						else if(gp->number == 3)
 							strncpy(n,"3",strlen("3"));
 						else if(gp->number == 4)
 							strncpy(n,"4",strlen("4"));

 						bzero(get_temp,MAXBUFSIZE);
 						strncpy(get_temp,filename,strlen(filename));
 						strncat(get_temp,".",strlen("."));
 						strncat(get_temp,n,strlen(n));

 						printf("what_i_have = %s\n",what_i_have);
 						printf("get_temp = %s\n",get_temp);

 						if(strstr(what_i_have,get_temp) == NULL)
 						{
 							strncat(what_i_have,get_temp,strlen(get_temp));

 							if(gp->number == 1)
 								flag_1 = 55;
 							else if(gp->number == 2)
 								flag_2 = 55;
 							else if(gp->number == 3)
 								flag_3 = 55;
 							else if(gp->number == 4)
 								flag_4 = 55;

 							// send ack
 							bzero(M,3);
 							strncpy(M,"ACK",strlen("ACK"));
 							bytes = send(clientSocket, &M, sizeof(M), MSG_NOSIGNAL);

 							if(gp->number == 1)
 							{
 								buf_1 = (char *)malloc(gp->size);
 								bytes = recv(clientSocket, buf_1, gp->size, 0);
 								read_1 = bytes;
 								printf("buffer 1 received\n");
 								printf("No. of bytes received = %d\n",bytes);
 								printf("DECRYPTED\n");
 								int count = 0;
 								for(int i=0;i<bytes;i++)
 								{
 									buf_1[i] = buf_1[i] ^ PASSWORD[count];
 									count++;
 									if(count == strlen(PASSWORD))
 										count = 0;
 								}
 							}
 							else if(gp->number == 2)
 							{
 								buf_2 = (char *)malloc(gp->size);
 								bytes = recv(clientSocket, buf_2, gp->size, 0);
 								read_2 = bytes;
 								printf("buffer 2 received\n");
 								printf("No. of bytes received = %d\n",bytes);
 								printf("DECRYPTED\n");
 								int count = 0;
 								for(int i=0;i<bytes;i++)
 								{
 									buf_2[i] = buf_2[i] ^ PASSWORD[count];
 									count++;
 									if(count == strlen(PASSWORD))
 										count = 0;
 								}
 							}
 							else if(gp->number == 3)
 							{
 								buf_3 = (char *)malloc(gp->size);
 								bytes = recv(clientSocket, buf_3, gp->size, 0);
 								read_3 = bytes;
 								printf("buffer 3 received\n");
 								printf("No. of bytes received = %d\n",bytes);
 								printf("DECRYPTED\n");
 								int count = 0;
 								for(int i=0;i<bytes;i++)
 								{
 									buf_3[i] = buf_3[i] ^ PASSWORD[count];
 									count++;
 									if(count == strlen(PASSWORD))
 										count = 0;
 								}
 							}
 							else if(gp->number == 4)
 							{
 								buf_4 = (char *)malloc(gp->size);
 								bytes = recv(clientSocket, buf_4, gp->size, 0);
 								read_4 = bytes;
 								printf("buffer 4 received\n");
 								printf("No. of bytes received = %d\n",bytes);
 								printf("DECRYPTED\n");
 								int count = 0;
 								for(int i=0;i<bytes;i++)
 								{
 									buf_4[i] = buf_4[i] ^ PASSWORD[count];
 									count++;
 									if(count == strlen(PASSWORD))
 										count = 0;
 								}
 							}
 						}
 						else
 						{
 							// send nak
 							bzero(M,3);
 							strncpy(M,"NAK",strlen("NAK"));
 							bytes = send(clientSocket, &M, sizeof(M), MSG_NOSIGNAL);
 							printf("DECRYPTED\n");
 							int count = 0;
 								for(int i=0;i<bytes;i++)
 								{
 									buf_4[i] = buf_4[i] ^ PASSWORD[count];
 									count++;
 									if(count == strlen(PASSWORD))
 										count = 0;
 								}
 						}
 						index--;
 						free(gp);
 					}
 				}
 			}
 			else if(msg_int == -1)
 			{
 				bytes = recv(clientSocket, &message_receive, sizeof(message_receive), 0);
 				printf("MESSAGE RECEIVED = %s\n", message_receive);
 				bzero(message_receive,strlen(message_receive));
 				printf("USERNAME AND PASSWORD NOT VERIFIED\n");
 			}
 			printf("***************************************************************** DFS 4 *****************************************************************\n");

			memset(up,0,sizeof(struct username_password));

			printf("USERNAME = %s\n",username);
 			strcpy(up->user,username);
 			printf("up->user = %s\n",up->user);
 			printf("PASSWORD = %s\n",password);
 			strncpy(up->passwd,password,strlen(password));
 			printf("up->passwd = %s\n",up->passwd);

			//struct reliable_packet *rb = malloc(sizeof(struct reliable_packet));
			memset(rb,0,sizeof(struct reliable_packet));

			memcpy(rb->request_method, req_method, strlen(req_method));
			printf("rb->request_method = %s\n",rb->request_method);
					
			strncpy(rb->fname,filename,strlen(filename));

			strncpy(rb->fold,"/DFS4/",strlen("/DFS4/"));

			if(fo_get == NULL)
				strncpy(rb->sub_fold,"NULL",strlen("NULL"));
			else
				strncpy(rb->sub_fold,fo_get,strlen(fo_get));

			/*---- Create the socket. The three arguments are: ----*/
 			/* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
 			clientSocket = socket(PF_INET, SOCK_STREAM, 0);
 			/*---- Configure settings of the server address struct ----*/
 			/* Address family = Internet */
 			serverAddr.sin_family = AF_INET;
 			/* Set port number, using htons function to use proper byte order */
 			serverAddr.sin_port = htons(atoi(port4));
 			/* Set IP address to localhost */
 			serverAddr.sin_addr.s_addr = inet_addr(ip_address);
 			/* Set all bits of the padding field to 0 */ 
 			memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
 			/*---- Connect the socket to the server using the address struct ----*/
 			addr_size = sizeof serverAddr;
 			connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

    		bytes = send(clientSocket, up, sizeof(struct username_password), MSG_NOSIGNAL);
 			printf("Bytes sent = %d\n",bytes);
 			printf("Username and Password sent\n");

 			bytes = recv(clientSocket, &msg_int, sizeof(msg_int), 0);

 			is_file = -1;

 			if(msg_int == 1)
 			{
 				printf("Positive message received\n");
 				bytes = send(clientSocket, rb, sizeof(struct reliable_packet), MSG_NOSIGNAL);
 				printf("Bytes sent = %d\n",bytes);
 				printf("Packet sent\n");

 				bytes = recv(clientSocket, &is_file, sizeof(is_file), 0);

 				if(is_file == 5)
 				{
 					printf("Server has file %s\n",rb->fname);
 					send_file = 5;
 					bytes = send(clientSocket, &send_file, sizeof(send_file), MSG_NOSIGNAL);

 					int index = 2;
 					while(index !=0 )
 					{
 						struct get_packet *gp = malloc(sizeof(struct get_packet));
                    	memset(gp,0,sizeof(struct get_packet));

 						bytes = recv(clientSocket, gp, sizeof(struct get_packet), 0);
 						printf("HERE\n");
 						printf("gp->number = %d\n",gp->number);
 						printf("gp->size = %d\n",gp->size);

 						char n[1];
 						bzero(n,1);

 						if(gp->number == 1)
 							strncpy(n,"1",strlen("1"));
 						else if(gp->number == 2)
 							strncpy(n,"2",strlen("2"));
 						else if(gp->number == 3)
 							strncpy(n,"3",strlen("3"));
 						else if(gp->number == 4)
 							strncpy(n,"4",strlen("4"));

 						bzero(get_temp,MAXBUFSIZE);
 						strncpy(get_temp,filename,strlen(filename));
 						strncat(get_temp,".",strlen("."));
 						strncat(get_temp,n,strlen(n));

 						printf("what_i_have = %s\n",what_i_have);
 						printf("get_temp = %s\n",get_temp);

 						if(strstr(what_i_have,get_temp) == NULL)
 						{
 							strncat(what_i_have,get_temp,strlen(get_temp));

 							if(gp->number == 1)
 								flag_1 = 55;
 							else if(gp->number == 2)
 								flag_2 = 55;
 							else if(gp->number == 3)
 								flag_3 = 55;
 							else if(gp->number == 4)
 								flag_4 = 55;

 							// send ack
 							bzero(M,3);
 							strncpy(M,"ACK",strlen("ACK"));
 							bytes = send(clientSocket, &M, sizeof(M), MSG_NOSIGNAL);

 							if(gp->number == 1)
 							{
 								buf_1 = (char *)malloc(gp->size);
 								bytes = recv(clientSocket, buf_1, gp->size, 0);
 								read_1 = bytes;
 								printf("buffer 1 received\n");
 								printf("No. of bytes received = %d\n",bytes);
 								printf("DECRYPTED\n");
 								int count = 0;
 								for(int i=0;i<bytes;i++)
 								{
 									buf_1[i] = buf_1[i] ^ PASSWORD[count];
 									count++;
 									if(count == strlen(PASSWORD))
 										count = 0;
 								}
 							}
 							else if(gp->number == 2)
 							{
 								buf_2 = (char *)malloc(gp->size);
 								bytes = recv(clientSocket, buf_2, gp->size, 0);
 								read_2 = bytes;
 								printf("buffer 2 received\n");
 								printf("No. of bytes received = %d\n",bytes);
 								printf("DECRYPTED\n");
 								int count = 0;
 								for(int i=0;i<bytes;i++)
 								{
 									buf_2[i] = buf_2[i] ^ PASSWORD[count];
 									count++;
 									if(count == strlen(PASSWORD))
 										count = 0;
 								}
 							}
 							else if(gp->number == 3)
 							{
 								buf_3 = (char *)malloc(gp->size);
 								bytes = recv(clientSocket, buf_3, gp->size, 0);
 								read_3 = bytes;
 								printf("buffer 3 received\n");
 								printf("No. of bytes received = %d\n",bytes);
 								printf("DECRYPTED\n");
 								int count = 0;
 								for(int i=0;i<bytes;i++)
 								{
 									buf_3[i] = buf_3[i] ^ PASSWORD[count];
 									count++;
 									if(count == strlen(PASSWORD))
 										count = 0;
 								}
 							}
 							else if(gp->number == 4)
 							{
 								buf_4 = (char *)malloc(gp->size);
 								bytes = recv(clientSocket, buf_4, gp->size, 0);
 								read_4 = bytes;
 								printf("buffer 4 received\n");
 								printf("No. of bytes received = %d\n",bytes);
 								printf("DECRYPTED\n");
 								int count = 0;
 								for(int i=0;i<bytes;i++)
 								{
 									buf_4[i] = buf_4[i] ^ PASSWORD[count];
 									count++;
 									if(count == strlen(PASSWORD))
 										count = 0;
 								}
 							}
 						}
 						else
 						{
 							// send nak
 							bzero(M,3);
 							strncpy(M,"NAK",strlen("NAK"));
 							bytes = send(clientSocket, &M, sizeof(M), MSG_NOSIGNAL);
 						}
 						index--;
 						free(gp);
 					}
 				}
 				printf("flag_1 = %d\n",flag_1);
 				printf("flag_2 = %d\n",flag_2);
 				printf("flag_3 = %d\n",flag_3);
 				printf("flag_4 = %d\n",flag_4);

 				printf("rb->fname = %s\n",rb->fname);

 				char p[MAXBUFSIZE] = "/home/srishti/Netsys/Assignment3/Client/";
 				strncat(p,rb->fname,strlen(rb->fname));
 				printf("p = %s\n",p);

 				FILE *ff;
 				ff = fopen(p,"w+");

 				if(ff == NULL)
 					printf("ERROR. Could not create receive file\n");
 				else
 				{
 					if(buf_1 != NULL)
 					{
 						fwrite(buf_1,sizeof(char),read_1,ff);
 						free(buf_1);
 					}
 					if(buf_2 != NULL)
 					{
 						fwrite(buf_2,sizeof(char),read_2,ff);
 						free(buf_2);
 					}
 					if(buf_3 != NULL)
 					{
 						fwrite(buf_3,sizeof(char),read_3,ff);
 						free(buf_3);
 					}
 					if(buf_4 != NULL)
 					{
 						fwrite(buf_4,sizeof(char),read_4,ff);
 						free(buf_4);
 					}
 				}
 				fclose(ff);
 			}
 			else if(msg_int == -1)
 			{
 				bytes = recv(clientSocket, &message_receive, sizeof(message_receive), 0);
 				printf("MESSAGE RECEIVED = %s\n", message_receive);
 				bzero(message_receive,strlen(message_receive));
 				printf("USERNAME AND PASSWORD NOT VERIFIED\n");
 			}
			free(up);
			free(rb);
        }

        else if(checker(req_method,"MKDIR") == 1)
        {
        	printf("Create subfolder\n");

        	char *fo_mkdir = strtok(NULL,"\n");
			printf("FO = %s\n",fo_mkdir);

			struct username_password *up = malloc(sizeof(struct username_password));
			memset(up,0,sizeof(struct username_password));

			printf("USERNAME = %s\n",username);
 			strcpy(up->user,username);
 			printf("up->user = %s\n",up->user);
 			printf("PASSWORD = %s\n",password);
 			strncpy(up->passwd,password,strlen(password));
 			printf("up->passwd = %s\n",up->passwd);

 			struct reliable_packet *rb = malloc(sizeof(struct reliable_packet));
			memset(rb,0,sizeof(struct reliable_packet));

			memcpy(rb->request_method, req_method, strlen(req_method));
			printf("rb->request_method = %s\n",rb->request_method);

 			/*------------------------------------------------------------------------------ For DFS1 ------------------------------------------------------------------------------*/
			bzero(rb->fold,MAXBUFSIZE);
			strncpy(rb->fold,"/DFS1/",strlen("/DFS1/"));
			strncpy(rb->sub_fold,fo_mkdir,strlen(fo_mkdir));

 			/*---- Create the socket. The three arguments are: ----*/
 			/* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
 			clientSocket = socket(PF_INET, SOCK_STREAM, 0);
 			/*---- Configure settings of the server address struct ----*/
 			/* Address family = Internet */
 			serverAddr.sin_family = AF_INET;
 			/* Set port number, using htons function to use proper byte order */
 			serverAddr.sin_port = htons(atoi(port1));
 			/* Set IP address to localhost */
 			serverAddr.sin_addr.s_addr = inet_addr(ip_address);
 			/* Set all bits of the padding field to 0 */ 
 			memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
 			/*---- Connect the socket to the server using the address struct ----*/
 			addr_size = sizeof serverAddr;
 			connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

 			int bytes = send(clientSocket, up, sizeof(struct username_password), MSG_NOSIGNAL);
 			//printf("Username and Password sent\n");

 			bytes = recv(clientSocket, &msg_int, sizeof(msg_int), 0);

 			if(msg_int == 1)
 			{
 				bytes = send(clientSocket, rb, sizeof(struct reliable_packet), MSG_NOSIGNAL);

 			}
 			else if(msg_int == -1)
 			{
 				bytes = recv(clientSocket, &message_receive, sizeof(message_receive), 0);
 				bzero(message_receive,strlen(message_receive));
 				printf("USERNAME AND PASSWORD NOT VERIFIED\n");
 			}

 			/*------------------------------------------------------------------------------ For DFS2 ------------------------------------------------------------------------------*/
			bzero(rb->fold,MAXBUFSIZE);
			strncpy(rb->fold,"/DFS2/",strlen("/DFS2/"));
			strncpy(rb->sub_fold,fo_mkdir,strlen(fo_mkdir));

 			/*---- Create the socket. The three arguments are: ----*/
 			/* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
 			clientSocket = socket(PF_INET, SOCK_STREAM, 0);
 			/*---- Configure settings of the server address struct ----*/
 			/* Address family = Internet */
 			serverAddr.sin_family = AF_INET;
 			/* Set port number, using htons function to use proper byte order */
 			serverAddr.sin_port = htons(atoi(port2));
 			/* Set IP address to localhost */
 			serverAddr.sin_addr.s_addr = inet_addr(ip_address);
 			/* Set all bits of the padding field to 0 */ 
 			memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
 			/*---- Connect the socket to the server using the address struct ----*/
 			addr_size = sizeof serverAddr;
 			connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

 			bytes = send(clientSocket, up, sizeof(struct username_password), MSG_NOSIGNAL);
 			//printf("Username and Password sent\n");

 			bytes = recv(clientSocket, &msg_int, sizeof(msg_int), 0);

 			if(msg_int == 1)
 			{
 				bytes = send(clientSocket, rb, sizeof(struct reliable_packet), MSG_NOSIGNAL);

 			}
 			else if(msg_int == -1)
 			{
 				bytes = recv(clientSocket, &message_receive, sizeof(message_receive), 0);
 				bzero(message_receive,strlen(message_receive));
 				printf("USERNAME AND PASSWORD NOT VERIFIED\n");
 			}

 			/*------------------------------------------------------------------------------ For DFS3 ------------------------------------------------------------------------------*/
			bzero(rb->fold,MAXBUFSIZE);
			strncpy(rb->fold,"/DFS3/",strlen("/DFS3/"));
			strncpy(rb->sub_fold,fo_mkdir,strlen(fo_mkdir));

 			/*---- Create the socket. The three arguments are: ----*/
 			/* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
 			clientSocket = socket(PF_INET, SOCK_STREAM, 0);
 			/*---- Configure settings of the server address struct ----*/
 			/* Address family = Internet */
 			serverAddr.sin_family = AF_INET;
 			/* Set port number, using htons function to use proper byte order */
 			serverAddr.sin_port = htons(atoi(port3));
 			/* Set IP address to localhost */
 			serverAddr.sin_addr.s_addr = inet_addr(ip_address);
 			/* Set all bits of the padding field to 0 */ 
 			memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
 			/*---- Connect the socket to the server using the address struct ----*/
 			addr_size = sizeof serverAddr;
 			connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

 			bytes = send(clientSocket, up, sizeof(struct username_password), MSG_NOSIGNAL);
 			//printf("Username and Password sent\n");

 			bytes = recv(clientSocket, &msg_int, sizeof(msg_int), 0);

 			if(msg_int == 1)
 			{
 				bytes = send(clientSocket, rb, sizeof(struct reliable_packet), MSG_NOSIGNAL);

 			}
 			else if(msg_int == -1)
 			{
 				bytes = recv(clientSocket, &message_receive, sizeof(message_receive), 0);
 				bzero(message_receive,strlen(message_receive));
 				printf("USERNAME AND PASSWORD NOT VERIFIED\n");
 			}

 			/*------------------------------------------------------------------------------ For DFS4 ------------------------------------------------------------------------------*/
			bzero(rb->fold,MAXBUFSIZE);
			strncpy(rb->fold,"/DFS4/",strlen("/DFS4/"));
			strncpy(rb->sub_fold,fo_mkdir,strlen(fo_mkdir));

 			/*---- Create the socket. The three arguments are: ----*/
 			/* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
 			clientSocket = socket(PF_INET, SOCK_STREAM, 0);
 			/*---- Configure settings of the server address struct ----*/
 			/* Address family = Internet */
 			serverAddr.sin_family = AF_INET;
 			/* Set port number, using htons function to use proper byte order */
 			serverAddr.sin_port = htons(atoi(port4));
 			/* Set IP address to localhost */
 			serverAddr.sin_addr.s_addr = inet_addr(ip_address);
 			/* Set all bits of the padding field to 0 */ 
 			memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
 			/*---- Connect the socket to the server using the address struct ----*/
 			addr_size = sizeof serverAddr;
 			connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

 			bytes = send(clientSocket, up, sizeof(struct username_password), MSG_NOSIGNAL);

 			bytes = recv(clientSocket, &msg_int, sizeof(msg_int), 0);

 			if(msg_int == 1)
 			{
 				bytes = send(clientSocket, rb, sizeof(struct reliable_packet), MSG_NOSIGNAL);

 			}
 			else if(msg_int == -1)
 			{
 				bytes = recv(clientSocket, &message_receive, sizeof(message_receive), 0);
 				bzero(message_receive,strlen(message_receive));
 				printf("USERNAME AND PASSWORD NOT VERIFIED\n");
 			}
        }
 	}
	return 0;
}