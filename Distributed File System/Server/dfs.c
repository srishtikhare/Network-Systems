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
#include <sys/stat.h>
#include <dirent.h>

#define LISTENQ          (1024)
#define MAXBUFSIZE 1024
#define TIMEOUT 10

char username[MAXBUFSIZE];
char password[MAXBUFSIZE];
char folder[MAXBUFSIZE];
char port[MAXBUFSIZE];
int msg_int = 5;
char string[MAXBUFSIZE];
char path[MAXBUFSIZE];
char buffer[MAXBUFSIZE];

struct stat st;

char msg[45] = "Invalid Username/Password. Please try again.";
int is_file = -1;
int send_file = -1;
char M[3];
int flag = 0;
char name_1[MAXBUFSIZE];
char name_2[MAXBUFSIZE];
char n_1[MAXBUFSIZE];
char n_2[MAXBUFSIZE];
char buffer_1[MAXBUFSIZE];
char buffer_2[MAXBUFSIZE];
char buffer_3[MAXBUFSIZE];
char buffer_4[MAXBUFSIZE];
char final_list[MAXBUFSIZE];
char buf_2[3000000];
char PASSWORD[MAXBUFSIZE];

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

void Error_Quit(char const * msg) {
    fprintf(stderr, "WEBSERV: %s\n", msg);
    exit(EXIT_FAILURE);
}

void load_config(void)
{
	FILE* file = fopen("dfs.conf", "r"); /* should check the result */
    char *line = NULL;
    size_t len = 0;
    int index = 0;

    bzero(username,MAXBUFSIZE);
    bzero(password,MAXBUFSIZE);

    while(-1 != getline(&line, &len, file))
	{
		char* input = strtok(line, " ");

        strncpy(username,input,strlen(input));
        input = strtok(NULL, " ");
        strip(input);
        strncpy(password,input,strlen(input));
    }
    fclose(file);
}

int ServiceReq(int connection)
{
    struct reliable_packet *rb = malloc(sizeof(struct reliable_packet));
    struct username_password *up = malloc(sizeof(struct username_password));

    int bytes = recv(connection, up, sizeof(struct username_password), 0);
    printf("Username and Password received\n");
    printf("Bytes received = %d\n", bytes);
    printf("Username received = %s\n", up->user);
    printf("Password received = %s\n", up->passwd);

    bzero(PASSWORD,MAXBUFSIZE);
    strncpy(PASSWORD,up->passwd,strlen(up->passwd));

    if((checker(up->user,username) == 1) && (checker(up->passwd,password) == 1))
    {
        printf("USERNAME AND PASSWORD VERIFIED\n");
        msg_int = 1;
        bytes = send(connection, &msg_int, sizeof(msg_int), MSG_NOSIGNAL);

        bytes = recv(connection, rb, sizeof(struct reliable_packet), 0);
        printf("Packet received\n");

        printf("Packet request_method received = %s\n", rb->request_method);
        printf("Bytes received = %d\n", bytes);
        printf("Packet length received = %d\n", rb->packet_length);

        if(checker(rb->request_method,"PUT") == 1)
        {
            printf("REQUEST METHOD PUT VERIFIED\n");
            
            printf("Folder received = %s\n", rb->fold);
            printf("Subfolder received = %s\n", rb->sub_fold);
            printf("Filename received = %s\n", rb->fname);
            printf("Part number received = %d\n", rb->part_number);

            char pwd[MAXBUFSIZE] = "/home/srishti/Netsys/Assignment3/Server";
            char fn[MAXBUFSIZE];

            char *data = (char *)malloc(rb->packet_length);
            int data_len = recv(connection, data, rb->packet_length, 0);

            printf("Data received\n");
            printf("Data bytes received = %d\n",data_len);

            int count = 0;
            for(int i=0;i<data_len;i++)
            {
                data[i] = data[i] ^ PASSWORD[count];
                count++;
                if(count == strlen(PASSWORD))
                    count = 0;
            }
            printf("DECRYPTED\n");

            strncat(pwd,rb->fold,strlen(rb->fold));
            strncat(pwd,up->user,strlen(up->user));

            if(checker(rb->sub_fold,"NULL") != 1)
            {
                strncat(pwd,"/",strlen("/"));
                char *t = strtok(rb->sub_fold,"/");
                strncat(pwd,t,strlen(t));
            }
            printf("NOW MKDIR\n");

            printf("PATH AFTER = %s\n",pwd);
            char temp_mkdir[MAXBUFSIZE];
            sprintf(temp_mkdir,"mkdir %s",pwd);
            system(temp_mkdir);
            printf("HERE 1\n");
            strncat(pwd,"/",strlen("/"));

            bzero(fn,MAXBUFSIZE);
            sprintf(fn, ".");
            strncat(fn,rb->fname,strlen(rb->fname));
            strncat(pwd,fn,strlen(fn));
            printf("HERE 2\n");
            bzero(string,MAXBUFSIZE);
            sprintf(string, ".%d",rb->part_number);
            strncat(pwd,string,strlen(string));
            printf("PWD = %s\n",pwd);
            printf("HERE 3\n");
            FILE* fpointer = fopen(pwd, "w"); /* should check the result */

            if(fpointer == NULL)
            {
                printf("ERROR. Could not create file\n");
                //exit(-1);
            }
            else
            {
                printf("File created\n");
                printf("Writing into file\n");

                fwrite(data, 1, rb->packet_length, fpointer);
                printf("Written in file\n");

                fclose(fpointer);
            }
            free(rb);
        }
        else if(checker(rb->request_method,"LIST") == 1)
        {
            printf("REQUEST METHOD LIST VERIFIED\n");
            printf("Folder received = %s\n", rb->fold);

            if(checker(rb->fold,"/DFS1/") == 1)
            {
                bzero(path,MAXBUFSIZE);
                strncpy(path,"/home/srishti/Netsys/Assignment3/Server",strlen("/home/srishti/Netsys/Assignment3/Server"));

                bzero(buffer,MAXBUFSIZE);

                strncat(path,rb->fold,strlen(rb->fold));
                strncat(path,up->user,strlen(up->user));
                if(checker(rb->sub_fold,"NULL") != 1)
                {
                    strncat(path,"/",strlen("/"));
                    //char *t = strtok(rb->sub_fold,"/");
                    strncat(path,rb->sub_fold,strlen(rb->sub_fold));
                }
                printf("NOW PATH = %s\n",path);

                struct dirent *de;  // Pointer for directory entry
 
                // opendir() returns a pointer of DIR type. 
                DIR *dr = opendir(path);
 
                if (dr == NULL)  // opendir returns NULL if couldn't open directory
                {
                    printf("Could not open current directory" );
                }
                else
                {
                    printf("Directory successfully opened\n");
                    printf("\n");

                    const char* extension = "\n";

                    while ((de = readdir(dr)) != NULL)
                    {
                        char* name = de->d_name;
                        if((checker(name,".") != 1) || (checker(name,"..") != 1))
                        {
                            strcat(buffer, name); /* add the extension */
                            strcat(buffer, extension); /* add the extension */
                        }
                    }
                    printf("LIST in %s is:\n%s\n", rb->fold, buffer);

                    bytes = send(connection, &buffer, sizeof(buffer), MSG_NOSIGNAL);

                    printf("Sent the list of files to client\n");
 
                    closedir(dr);
                }
            }
            else if(checker(rb->fold,"/DFS2/") == 1)
            {
                bzero(path,MAXBUFSIZE);
                strncpy(path,"/home/srishti/Netsys/Assignment3/Server",strlen("/home/srishti/Netsys/Assignment3/Server"));

                bzero(buffer,MAXBUFSIZE);

                strncat(path,rb->fold,strlen(rb->fold));
                strncat(path,up->user,strlen(up->user));
                if(checker(rb->sub_fold,"NULL") != 1)
                {
                    strncat(path,"/",strlen("/"));
                    //char *t = strtok(rb->sub_fold,"/");
                    strncat(path,rb->sub_fold,strlen(rb->sub_fold));
                }
                printf("NOW PATH = %s\n",path);

                struct dirent *de;  // Pointer for directory entry
 
                // opendir() returns a pointer of DIR type. 
                DIR *dr = opendir(path);
 
                if (dr == NULL)  // opendir returns NULL if couldn't open directory
                {
                    printf("Could not open current directory" );
                }
                else
                {
                    printf("Directory successfully opened\n");
                    printf("\n");

                    const char* extension = "\n";

                    while ((de = readdir(dr)) != NULL)
                    {
                        char* name = de->d_name;
                        if((checker(name,".") != 1) || (checker(name,"..") != 1))
                        {
                            strcat(buffer, name); /* add the extension */
                            strcat(buffer, extension); /* add the extension */
                        }
                    }
                    printf("LIST in %s is:\n%s\n", rb->fold, buffer);

                    bytes = send(connection, &buffer, sizeof(buffer), MSG_NOSIGNAL);

                    printf("Sent the list of files to client\n");
 
                    closedir(dr);
                }
            }
            else if(checker(rb->fold,"/DFS3/") == 1)
            {
                bzero(path,MAXBUFSIZE);
                strncpy(path,"/home/srishti/Netsys/Assignment3/Server",strlen("/home/srishti/Netsys/Assignment3/Server"));

                bzero(buffer,MAXBUFSIZE);

                strncat(path,rb->fold,strlen(rb->fold));
                strncat(path,up->user,strlen(up->user));
                if(checker(rb->sub_fold,"NULL") != 1)
                {
                    strncat(path,"/",strlen("/"));
                    //char *t = strtok(rb->sub_fold,"/");
                    strncat(path,rb->sub_fold,strlen(rb->sub_fold));
                }
                printf("NOW PATH = %s\n",path);

                struct dirent *de;  // Pointer for directory entry
 
                // opendir() returns a pointer of DIR type. 
                DIR *dr = opendir(path);
 
                if (dr == NULL)  // opendir returns NULL if couldn't open directory
                {
                    printf("Could not open current directory" );
                }
                else
                {
                    printf("Directory successfully opened\n");
                    printf("\n");

                    const char* extension = "\n";

                    while ((de = readdir(dr)) != NULL)
                    {
                        char* name = de->d_name;
                        if((checker(name,".") != 1) || (checker(name,"..") != 1))
                        {
                            strcat(buffer, name); /* add the extension */
                            strcat(buffer, extension); /* add the extension */
                        }
                    }
                    printf("LIST in %s is:\n%s\n", rb->fold, buffer);

                    bytes = send(connection, &buffer, sizeof(buffer), MSG_NOSIGNAL);

                    printf("Sent the list of files to client\n");
 
                    closedir(dr);
                }
            }
            else if(checker(rb->fold,"/DFS4/") == 1)
            {
                bzero(path,MAXBUFSIZE);
                strncpy(path,"/home/srishti/Netsys/Assignment3/Server",strlen("/home/srishti/Netsys/Assignment3/Server"));

                bzero(buffer,MAXBUFSIZE);

                strncat(path,rb->fold,strlen(rb->fold));
                strncat(path,up->user,strlen(up->user));

                if(checker(rb->sub_fold,"NULL") != 1)
                {
                    strncat(path,"/",strlen("/"));
                    //char *t = strtok(rb->sub_fold,"/");
                    strncat(path,rb->sub_fold,strlen(rb->sub_fold));
                }
                printf("NOW PATH = %s\n",path);

                struct dirent *de;  // Pointer for directory entry
 
                // opendir() returns a pointer of DIR type. 
                DIR *dr = opendir(path);
 
                if (dr == NULL)  // opendir returns NULL if couldn't open directory
                {
                    printf("Could not open current directory" );
                }
                else
                {
                    printf("Directory successfully opened\n");
                    printf("\n");

                    const char* extension = "\n";

                    while ((de = readdir(dr)) != NULL)
                    {
                        char* name = de->d_name;
                        if((checker(name,".") != 1) || (checker(name,"..") != 1))
                        {
                            strcat(buffer, name); /* add the extension */
                            strcat(buffer, extension); /* add the extension */
                        }
                    }
                    printf("LIST in %s is:\n%s\n", rb->fold, buffer);

                    bytes = send(connection, &buffer, sizeof(buffer), MSG_NOSIGNAL);

                    printf("Sent the list of files to client\n");
 
                    closedir(dr);
                }
            }
        }
        else if(checker(rb->request_method,"GET") == 1)
        {
            printf("REQUEST METHOD GET VERIFIED\n");
            
            printf("Folder received = %s\n", rb->fold);
            printf("Subfolder received = %s\n", rb->sub_fold);
            printf("Filename received = %s\n", rb->fname);
            printf("Part number received = %d\n", rb->part_number);

            int read_bytes = 0;
            char *num;
            char *num_2;

            char file_path[MAXBUFSIZE] = "/home/srishti/Netsys/Assignment3/Server";
            char file_path_2[MAXBUFSIZE];
            char fn[MAXBUFSIZE];
            bzero(buffer_1,MAXBUFSIZE);
            bzero(buffer_2,MAXBUFSIZE);
            bzero(buffer_3,MAXBUFSIZE);
            bzero(buffer_4,MAXBUFSIZE);

            /**************************************************************************************************************************************/
            strncat(file_path,rb->fold,strlen(rb->fold));
            strncat(file_path,up->user,strlen(up->user));

            if(checker(rb->sub_fold,"NULL") != 1)
                {
                    strncat(file_path,"/",strlen("/"));
                    //char *t = strtok(rb->sub_fold,"/");
                    strncat(file_path,rb->sub_fold,strlen(rb->sub_fold));
                }
                printf("NOW PATH = %s\n",file_path);
                bzero(file_path_2,MAXBUFSIZE);
                strncpy(file_path_2,file_path,strlen(file_path));

                struct dirent *de;  // Pointer for directory entry
 
                DIR *dr = opendir(file_path);
 
                if (dr == NULL)  // opendir returns NULL if couldn't open directory
                {
                    printf("Could not open current directory" );
                }
                else
                {
                    printf("Directory successfully opened\n");
                    printf("\n");

                    const char* extension = "\n";
                    flag = 0;
                    while ((de = readdir(dr)) != NULL)
                    {
                        char* name = de->d_name;
                        if(strstr(name, rb->fname) != NULL)
                        {
                            printf("File found\n");
                            if(flag != 5)
                            {
                                bzero(name_1,MAXBUFSIZE);
                                strncpy(name_1,name,strlen(name));
                                flag = 5;
                            }
                            else
                            {
                                bzero(name_2,MAXBUFSIZE);
                                strncpy(name_2,name,strlen(name));
                            }

                            is_file = 5;
                        }
                    }
                    closedir(dr);
                    bytes = send(connection, &is_file, sizeof(is_file), MSG_NOSIGNAL);
                    bytes = recv(connection, &send_file, sizeof(send_file), 0);

                    if(send_file == 5)
                    {
                        printf("name_1 = %s\n", name_1);
                        printf("name_2 = %s\n", name_2);
                        int num_zero = 0;
                        bzero(n_1,MAXBUFSIZE);
                        bzero(n_2,MAXBUFSIZE);
                        strncpy(n_1,name_1,strlen(name_1));
                        strncpy(n_2,name_2,strlen(name_2));

                        struct get_packet *gp = malloc(sizeof(struct get_packet));
                        memset(gp,0,sizeof(struct get_packet));

                        num_zero = 0;
                        for(int i=0; i<strlen(name_1); i++)
                        {
                            if(name_1[i]=='.')
                                num_zero++;
                        }
                        if(num_zero == 2)
                        {
                            num = strtok(name_1,".");
                            num = strtok(NULL,".");
                            printf("num = %s\n",num);

                            for(int i=0; i<strlen(num); i++)
                            {
                                if(num[i]=='1')
                                    gp->number = 1;
                                else if(num[i]=='2')
                                    gp->number = 2;
                                else if(num[i]=='3')
                                    gp->number = 3;
                                else if(num[i]=='4')
                                    gp->number = 4;
                            }
                        }
                        else if(num_zero == 3)
                        {
                            char *num = strtok(name_1,".");
                            num = strtok(NULL,".");
                            num = strtok(NULL, ".");
                            printf("num = %s\n",num);

                            for(int i=0; i<strlen(num); i++)
                            {
                                if(num[i]=='1')
                                    gp->number = 1;
                                else if(num[i]=='2')
                                    gp->number = 2;
                                else if(num[i]=='3')
                                    gp->number = 3;
                                else if(num[i]=='4')
                                    gp->number = 4;
                            }
                        }

                        strncat(file_path,"/",strlen("/"));
                        strncat(file_path,n_1,strlen(n_1));

                        FILE *fp;
                        unsigned long fsize = 0, last_part=0;

                        printf("file_path = %s\n",file_path);

                        fp = fopen(file_path, "r");

                        if(fp == NULL)
                            printf("File does not exist at server side\n");
                        else
                        {
                            fseek(fp, 0, SEEK_END);
                            fsize = ftell(fp);
                            fseek(fp,0,SEEK_SET);

                            printf("Size of %s = %ld\n",n_1,fsize);

                            char *buf_1 = (char*)malloc(fsize);
                            read_bytes = fread(buf_1,1,fsize,fp);
                            fseek(fp,0,SEEK_SET);
                            printf("read bytes = %d\n",read_bytes);

                            gp->size = read_bytes;
                            printf("HERE\n");
                            printf("gp->number = %d\n",gp->number);
                            printf("gp->size = %d\n",gp->size);

                            bytes = send(connection, gp, sizeof(struct get_packet), MSG_NOSIGNAL);

                            bzero(M,strlen(M));
                            bytes = recv(connection, &M, sizeof(M), 0);
                            printf("M = %s\n",M);

                            if(checker(M,"ACK") == 1)
                            {
                                int count = 0;
                                for(int i=0;i<gp->size;i++)
                                {
                                    buf_1[i] = buf_1[i] ^ PASSWORD[count];
                                    count++;
                                    if(count == strlen(PASSWORD))
                                        count = 0;
                                }
                                printf("ENCRYPTED\n");
                                bytes = send(connection, buf_1, gp->size, MSG_NOSIGNAL);
                                printf("No. of bytes sent = %d\n",bytes);
                            }
                            free(buf_1);
                        }
                        fclose(fp);
                        /*********************************************** 2nd part ***********************************************/
                        memset(gp,0,sizeof(struct get_packet));

                        num_zero = 0;
                        for(int i=0; i<strlen(name_2); i++)
                        {
                            if(name_2[i]=='.')
                                num_zero++;
                        }
                        if(num_zero == 2)
                        {
                            num_2 = strtok(name_2,".");
                            num_2 = strtok(NULL,".");
                            printf("num_2 = %s\n",num_2);

                            for(int i=0; i<strlen(num_2); i++)
                            {
                                if(num_2[i]=='1')
                                    gp->number = 1;
                                else if(num_2[i]=='2')
                                    gp->number = 2;
                                else if(num_2[i]=='3')
                                    gp->number = 3;
                                else if(num_2[i]=='4')
                                    gp->number = 4;
                            }
                        }
                        else if(num_zero == 3)
                        {
                            char *num_2 = strtok(name_2,".");
                            num_2 = strtok(NULL,".");
                            num_2 = strtok(NULL, ".");
                            printf("num_2 = %s\n",num_2);

                            for(int i=0; i<strlen(num_2); i++)
                            {
                                if(num_2[i]=='1')
                                    gp->number = 1;
                                else if(num_2[i]=='2')
                                    gp->number = 2;
                                else if(num_2[i]=='3')
                                    gp->number = 3;
                                else if(num_2[i]=='4')
                                    gp->number = 4;
                            }
                        }

                        strncat(file_path_2,"/",strlen("/"));
                        strncat(file_path_2,n_2,strlen(n_2));

                        printf("file_path_2 = %s\n",file_path_2);

                        fp = fopen(file_path_2, "r");

                        if(fp == NULL)
                            printf("File does not exist at server side\n");
                        else
                        {
                            printf("File exists\n");

                            printf("gp->number = %d\n",gp->number);
                            printf("gp->size = %d\n",gp->size);
                            printf("Filename = %s\n",n_2);

                            fseek(fp, 0, SEEK_END);
                            fsize = ftell(fp);
                            fseek(fp,0,SEEK_SET);

                            printf("Size of %s = %ld\n",n_2,fsize);

                            char *buf_2 = (char*)malloc(fsize);
                            read_bytes = fread(buf_2,1,fsize,fp);
                            fseek(fp,0,SEEK_SET);
                            printf("read bytes = %d\n",read_bytes);

                            gp->size = read_bytes;
                            printf("HERE\n");
                            printf("gp->number = %d\n",gp->number);
                            printf("gp->size = %d\n",gp->size);

                            bytes = send(connection, gp, sizeof(struct get_packet), MSG_NOSIGNAL);

                            bzero(M,strlen(M));
                            bytes = recv(connection, &M, sizeof(M), 0);
                            printf("M = %s\n",M);

                            if(checker(M,"ACK") == 1)
                            {
                                int count = 0;
                                for(int i=0;i<gp->size;i++)
                                {
                                    buf_2[i] = buf_2[i] ^ PASSWORD[count];
                                    count++;
                                    if(count == strlen(PASSWORD))
                                        count = 0;
                                }
                                printf("ENCRYPTED\n");
                                bytes = send(connection, buf_2, gp->size, MSG_NOSIGNAL);
                                printf("No. of bytes sent = %d\n",bytes);
                            }
                            free(buf_2);
                        }
                        fclose(fp);
                        free(gp);
                    }
                }
            free(rb);
        }

        else if(checker(rb->request_method,"MKDIR") == 1)
        {
            printf("MKDIR METHOD PUT VERIFIED\n");
            
            printf("Folder received = %s\n", rb->fold);

            char pwd[MAXBUFSIZE] = "/home/srishti/Netsys/Assignment3/Server";

            strncat(pwd,rb->fold,strlen(rb->fold));
            strncat(pwd,up->user,strlen(up->user));
            //printf("PATH BEFORE = %s\n",pwd);
            if(checker(rb->sub_fold,"NULL") != 1)
            {
                strncat(pwd,"/",strlen("/"));
                char *t = strtok(rb->sub_fold,"/");
                strncat(pwd,t,strlen(t));
            }
            printf("NOW MKDIR\n");

            printf("PATH AFTER = %s\n",pwd);
            char temp_mkdir[MAXBUFSIZE];
            sprintf(temp_mkdir,"mkdir %s",pwd);
            system(temp_mkdir);
            printf("FOLDER CREATED\n");
        }
    }
    else
    {
        msg_int = -1;
        bytes = send(connection, &msg_int, sizeof(msg_int), MSG_NOSIGNAL);
        bytes = send(connection, &msg, sizeof(msg), MSG_NOSIGNAL);
        printf("USERNAME AND PASSWORD NOT VERIFIED\n");
    }
    free(up);
    return 0;
}

int main(int argc, char *argv[])
{
    bzero(folder,strlen(folder));
    bzero(port,strlen(port));
    strncpy(folder,argv[1],strlen(argv[1]));
    strip(folder);
    printf("%s\n",folder);
    strncpy(port,argv[2],strlen(argv[2]));
    strip(port);
    printf("%s\n",port);

	load_config();

    int    listener, connection;
    pid_t  pid;
    struct sockaddr_in servaddr;
    int result=0;

    /*  Create socket  */
    if ( (listener = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
        Error_Quit("Couldn't create listening socket.");
    else
        printf("Created socket\n");

    /* Populate socket address structure */
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(atoi(port));

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
        printf("Received request\n");
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

   return 0;
}