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
#include <sys/time.h>
#include <sys/select.h>
#include <fcntl.h>

char getChecksum(PACKET packet);

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
	
		struct timeval tv;
	int rv=0;

	fd_set readfds;
	fcntl(sockfd,F_SETFL,O_NONBLOCK);

	FD_ZERO(&readfds);
	FD_SET(sockfd, &readfds);

	tv.tv_sec = 1;
	tv.tv_usec = 0;

	int packet_num = 0;
	HEADER header;
	PACKET packet;
	packet.header = header;
	header.seq_ack = packet_num;

	i = 0;
	for(i=0; i<strlen(outputf); i++)
		packet.data[i] = outputf[i];
	char cs = getChecksum(packet);	
	char last_ack=0;
	PACKET response;
	int nBytes;
	int headerRecieved = 0;

	do {
		packet.header.checksum = getChecksum(packet);
		sendto(sockfd, &packet, sizeof(packet), 0, (struct sockaddr *)&serv_addr, sizeof serv_addr);
		last_ack = packet.header.seq_ack = 0;
		packet.header.seq_ack = 0;

		tv.tv_sec = 1;
		tv.tv_usec = 0;	
		rv = select(sockfd+1,&readfds,NULL,NULL,&tv);

		if(rv==1) {
			nBytes = recvfrom(sockfd, &response, sizeof(response), 0, (struct sockaddr *)&serv_addr, sizeof serv_addr);
		
			char responseChecksum = response.header.checksum;
			response.header.checksum == 0;

			if(responseChecksum == getChecksum(response) && response.header.seq_ack == packet.header.seq_ack) {
				headerRecieved = 1;
			}
		}


		//printf("Received ACK %i\n",response.header.seq_ack);
	} while(headerRecieved == 0 && rv == 0);

	FILE *fsource = fopen(argv[1], "r");
	size_t data;

	// input, send to server, receive it back, and output it
	while((data=fread(buff, 1, 10, fsource)) > 0)
	{
		char responseChecksum = 0;
		packet.header.seq_ack = (packet.header.seq_ack+1)%2;
		
		printf(".............seq is %i\n\n",packet.header.seq_ack);
		
		do {
			//packet_num++;
			//packet.header.seq_ack = (response.header.seq_ack+1) % 2;

			for(i=0; i<strlen(buff)+1; i++)
				packet.data[i] = buff[i];
			packet.header.checksum = 0;
			packet.header.checksum = getChecksum(packet);
			printf("");	

			

			if(getRand()<7) { //getRand returns a number 0-9
				if(getRand()<7) {
					packet.header.checksum = 0;
				}
				sendto(sockfd, &packet, sizeof(packet), 0, (struct sockaddr *)&serv_addr, sizeof serv_addr);
				//printf("sending packet from client\n");
				printf("sending ack %i\n",packet.header.seq_ack);
			} else {
				printf("dropped packet from client\n");
			}

			last_ack = packet.header.seq_ack;	

			FD_ZERO(&readfds);
			FD_SET(sockfd, &readfds);

			
			tv.tv_sec = 1;
			tv.tv_usec = 0;
			rv = select(sockfd+1,&readfds,NULL,NULL,&tv);



			if(rv==1) {
				nBytes = recvfrom(sockfd, &response, sizeof(response), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

				printf("Recieved ACK %i\n",response.header.seq_ack);
			
				responseChecksum = response.header.checksum;
				response.header.checksum = 0;

				printf("response checksum is %i and calculated checksum is %i\n", responseChecksum, getChecksum(response));

				printf("response seq is %i, expected seq is %i\n",response.header.seq_ack,packet.header.seq_ack);
			} else {
				printf("no response\n");
				response.header.seq_ack = 2;
			}
			



			printf("\n");
			//sleep(5);
		} while(rv == 0 || responseChecksum != getChecksum(response) || response.header.seq_ack != packet.header.seq_ack); //resend packet while checksum and seq_ack and there is no response not equal to expected
		





	}

	HEADER finHeader;
	PACKET finPacket;
	finPacket.header = finHeader;
	finPacket.header.seq_ack = (response.header.seq_ack+1) % 2;
	finPacket.header.fin = 1;
	finPacket.header.checksum = 0;
	finPacket.header.checksum = getChecksum(finPacket);


	int numTries = 0;
	char responseChecksum = 0;

	while(numTries<3) {
		printf("sending fin\n");

		sendto(sockfd, &finPacket, sizeof(finPacket),0,(struct sockaddr *)&serv_addr, sizeof serv_addr);
		
		tv.tv_sec = 1;
		tv.tv_usec = 0;	

		rv = select(sockfd+1,&readfds,NULL,NULL,&tv);
		if(rv==1) {
			nBytes = recvfrom(sockfd, &response, sizeof(response), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr)); 
			responseChecksum = response.header.checksum;
			response.header.checksum = 0;
			if(responseChecksum == getChecksum(response)) {
				printf("fin checksums match\n");
				if(response.header.seq_ack == finPacket.header.seq_ack) {
					printf("seq_acks match. Exiting.\n");
					return 0;
				}
			}

		}
		numTries++;
	}

	return 1;
}

char getChecksum(PACKET packet) {
	char checkSum = 0;
	int i=0;
	int size = sizeof(packet);
	char * packetPointer = &packet;
	//printf("size of packet is %i\n",size);

	for(i=0;i<size;i++) {
		checkSum = checkSum ^ *(packetPointer+i);
	}

	//printf("checkSum is %i\n",checkSum);
	return checkSum;
}

int getRand() {
	return rand()%10;
}

void printTime() {
	printf("%l",time(NULL));
}
