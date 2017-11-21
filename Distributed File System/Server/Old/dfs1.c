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

char username[10];
char password[10];
char folder[10];
char port[10];

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

void load_config(void)
{
	FILE* file = fopen("dfs.conf", "r"); /* should check the result */
    char *line = NULL;
    size_t len = 0;
    int index = 0;

    while(-1 != getline(&line, &len, file))
	{
		char* input = strtok(line, " ");

        /* Retrieve ip address and port numbers from dfc.conf file */
        if(checker(input,"Srishti") == 1)
        {
        	strncpy(username,input,strlen(input));
        	printf("%s\n",username);
        	input = strtok(NULL, " ");
        	strip(input);
        	strncpy(password,input,strlen(input));
        	printf("%s\n",password);
        }
    }
    fclose(file);
}

int main(int argc, char *argv[])
{
    strncpy(folder,argv[1],strlen(argv[1]));
    strip(folder);
    strncpy(port,argv[2],strlen(argv[2]));
    strip(port);

	load_config();

	return 0;
}