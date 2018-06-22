
/********************
 * COEN 146, UDP example, server
 ********************/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include "./tfv1.h"

/********************
 * main
 ********************/

char getChecksum(PACKET packet);

int main (int argc, char *argv[])
{
	int sock, nBytes;
	char buffer[1024];
	struct sockaddr_in serverAddr, clientAddr;
	struct sockaddr_storage serverStorage;
	socklen_t addr_size, client_addr_size;
	int i;

    if (argc != 2)
    {
        printf ("need the port number\n");
        return 1;
    }

	// init 
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons ((short)atoi (argv[1]));
	serverAddr.sin_addr.s_addr = htonl (INADDR_ANY);
	memset ((char *)serverAddr.sin_zero, '\0', sizeof (serverAddr.sin_zero));  
	addr_size = sizeof (serverStorage);

	// create socket
	if ((sock = socket (AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		printf ("socket error\n");
		return 1;
	}

	// bind
	if (bind (sock, (struct sockaddr *)&serverAddr, sizeof (serverAddr)) != 0)
	{
		printf ("bind error\n");
		return 1;
	}

	PACKET packet_rcv;
	char rcvChecksum;
	char calcChecksum;
	FILE *fdest;
	char file_name[20];
	
	int last_ack=1;
	while (1)
	{
		int recievedTitle = 0;

		while(recievedTitle == 0) { //recieve title
		nBytes = recvfrom(sock, &packet_rcv, sizeof(packet_rcv), 0, (struct sockaddr *)&serverStorage, &addr_size);
		
		rcvChecksum = packet_rcv.header.checksum;
		packet_rcv.header.checksum = 0;
		calcChecksum = getChecksum(packet_rcv);

		if(calcChecksum == rcvChecksum && packet_rcv.header.seq_ack == (last_ack+1))%2) {

			printf("recieved checksum was %i and calculated is %i\n",rcvChecksum,calcChecksum);

			printf("title is %s\n",packet_rcv.data);	
			
			int i=0;
			for(i=0; i<strlen(packet_rcv.data)+1; i++) {
				file_name[i]=packet_rcv.data[i];
			}

			fdest = fopen(file_name,"w");
			fclose(fdest);
			
			sendto(sock, &packet_rcv, sizeof(packet_rcv), 0, (struct sockaddr *)&serverStorage, addr_size);
			recievedTitle = 1;	
		} else {
			packet_rcv.header.seq_ack = (packet_rcv.header.seq_ack+1) % 2;
			packet_rcv.header.checksum = 0;
			packet_rcv.header.checksum = getChecksum(packet_rcv);
			printf("server recieved bad packet. resending previous ack\n");
			sendto(sock, &packet_rcv, sizeof(packet_rcv), 0, (struct sockaddr *) &serverStorage, addr_size);
		}
			last_ack = packet_rcv.header.seq_ack;
		}




		printf("exited title block\n");
		// receive  datagrams
		while(nBytes = recvfrom (sock, &packet_rcv, sizeof(packet_rcv), 0, (struct sockaddr *)&serverStorage, &addr_size)) {

			rcvChecksum = packet_rcv.header.checksum;
			packet_rcv.header.checksum = 0;
			calcChecksum = getChecksum(packet_rcv);

			printf("recieved checksum is %i and calculated checksum is %i\n",rcvChecksum,calcChecksum);

			
			//if uncorrupted
			if(calcChecksum != rcvChecksum) {
				printf("Checksums invalid\n");
				//if ack is out of order
				packet_rcv.header.seq_ack = (packet_rcv.header.seq_ack + 1) % 2;
				sendto (sock, &packet_rcv, sizeof(packet_rcv), 0, (struct sockaddr *)&serverStorage, addr_size);

			} else if(packet_rcv.header.seq_ack != (last_ack + 1) % 2) {
				//incorrect seq_ack
				packet_rcv.header.seq_ack = (packet_rcv.header.seq_ack + 1) % 2;
				sendto (sock, &packet_rcv, sizeof(packet_rcv), 0, (struct sockaddr *)&serverStorage, addr_size);

			} else {
				if(packet_rcv.header.fin==1) {
					sendto (sock, &packet_rcv, sizeof(packet_rcv), 0, (struct sockaddr *)&serverStorage, addr_size);
					printf("EOF reached.\n");
					break;
				} else {

					fdest = fopen(file_name,"a");
					fwrite(packet_rcv.data,1,10,fdest);

					sendto (sock, &packet_rcv, sizeof(packet_rcv), 0, (struct sockaddr *)&serverStorage, addr_size);
					fclose(fdest);
				}
					
			}
				last_ack = packet_rcv.header.seq_ack;
		}
	}

	return 0;
}


char getChecksum(PACKET packet) {
	char checkSum = 0;
	int i=0;
	int size = sizeof(packet);
	char * packetPointer = &packet;

	for(i=0;i<size;i++) {
		checkSum = checkSum ^ *(packetPointer+i);
	}
	return checkSum;
}
