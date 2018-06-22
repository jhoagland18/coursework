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
#include "./tfv1.h"

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

	int port = atoi(argv[4]);

	// set address
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port); 
	inet_pton (AF_INET, argv[3], &serv_addr.sin_addr.s_addr);
	memset(serv_addr.sin_zero, '\0', sizeof (serv_addr.sin_zero));
	socklen_t addr_size = sizeof serv_addr;

	sockfd = socket(PF_INET, SOCK_DGRAM, 0);

	char outputf[20];
	
	strcpy(outputf,argv[2]);
	//outputf[strlen(outputf)]='\0';

	int packet_num = 0;
	HEADER header;
	PACKET packet;
	packet.header = header;
	header.seq_ack = packet_num;

	i = 0;
	for(i=0; i<strlen(outputf); i++)
		packet.data[i] = outputf[i];

	sendto(sockfd, &packet, sizeof(packet), 0, (struct sockaddr *)&serv_addr, sizeof serv_addr);
	
	PACKET response;
	int nBytes = recvfrom(sockfd, &response, sizeof(response), 0, (struct sockaddr *)&serv_addr, sizeof serv_addr);

	printf("Received ACKs %i\n",response.header.seq_ack);

	if(response.header.seq_ack!=0) {
		printf("received incorrect ack. Exiting.\n");
		return 1;	
	}

	FILE *fsource = fopen(argv[1], "r");
	size_t data;

	// input, send to server, receive it back, and output it
	while((data=fread(buff, 1, 10, fsource)) > 0)
	{
		packet_num++;
		packet.header.seq_ack = packet_num % 2;

		for(i=0; i<strlen(buff)+1; i++)
			packet.data[i] = buff[i];
		
		sendto(sockfd, &packet, sizeof(packet), 0, (struct sockaddr *)&serv_addr, sizeof serv_addr);

		nBytes = recvfrom(sockfd, &response, sizeof(response), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

		if(response.header.seq_ack != packet.header.seq_ack) {
			printf("Recieved bad ACK. Exiting.");
			return 1;
		}
		
		printf("Recieved ACK %i\n",response.header.seq_ack);
	}

	return 0;
}
