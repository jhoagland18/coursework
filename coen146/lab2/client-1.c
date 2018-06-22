 /**************************
	socket example, client
	spring 2018
 **************************/

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 

int main (int, char *[]);


/********************
 * main
 ********************/
int main (int argc, char *argv[])
{
	int i;
	int sockfd = 0, n = 0;
	char buff[10];
	char *p;
	struct sockaddr_in serv_addr; 

	if (argc != 5)
	{
		printf ("Usage: %s <input file>, <output file name>, <ip of server>, <port of server> \n",argv[0]);
		return 1;
	}

	// set up
	memset (buff, '0', sizeof (buff));
	memset (&serv_addr, '0', sizeof (serv_addr)); 

	// open socket
	if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf ("Error : Could not create socket \n");
		return 1;
	}

	int port = atoi(argv[4]);

	// set address
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port); 

	if (inet_pton (AF_INET, argv[3], &serv_addr.sin_addr) <= 0)
	{
		printf ("inet_pton error occured\n");
		return 1;
	} 

	// connect
	if (connect (sockfd, (struct sockaddr *)&serv_addr, sizeof (serv_addr)) < 0)
	{
		printf ("Error : Connect Failed \n");
		return 1;
	}

	char outputf[256];
	strcpy(outputf,argv[2]);
	outputf[strlen(outputf)]='\0';


	write(sockfd, outputf, strlen(outputf)+1);
	
	FILE *fsource = fopen(argv[1], "r");
	size_t data;
	// input, send to server, receive it back, and output it
	while((data=fread(buff, 1, sizeof(buff), fsource)) > 0)
	{
		write(sockfd,buff,data);
	}

	return 0;
}
