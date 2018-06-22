 /**************************
 *     socket example, server
 *     spring 2018
 ***************************/

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <ctype.h>

int main (int, char *[]); 


/*********************
 * main
 *********************/
int main (int argc, char *argv[])
{
	int	n;
	char *p; 
	int listenfd = 0, connfd = 0;
	struct sockaddr_in serv_addr; 
	char buff[1025];

	// set up
	memset (&serv_addr, '0', sizeof (serv_addr));
	memset (buff, '0', sizeof (buff)); 
	
	int port = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl (INADDR_ANY);
	serv_addr.sin_port = htons(port); 

	// create socket, bind, and listen
	listenfd = socket (AF_INET, SOCK_STREAM, 0);
	bind (listenfd, (struct sockaddr*)&serv_addr, sizeof (serv_addr)); 
	listen (listenfd, 10); 


	// accept and interact
	while (1)
	{
		connfd = accept (listenfd, (struct sockaddr*)NULL, NULL); 
		char outputf[0];
		// receive data and reply
		while ((n = read (connfd, buff, sizeof (buff))) > 0)
		{
			// change lower to upper case 
			p = buff;
			int i=0;
			while (*p != '\0')
			{
				outputf[i++]=*p;
				p++;
			}
			if(*p=='\0')
				break;
		}

		sleep(1);
		//read file contents and write to file
		FILE *fdest = fopen(outputf,"w");
		while((n=read (connfd,buff,sizeof(buff)))>0)
		{
			fwrite(buff,1,n,fdest);
		}

		fclose(fdest);
		close(connfd);
	}
	return 0;
}
