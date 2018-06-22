
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
	

	while (1)
	{
		PACKET packet_rcv;
		nBytes = recvfrom(sock, &packet_rcv, sizeof(packet_rcv), 0, (struct sockaddr *)&serverStorage, &addr_size);
		printf("title is %s\n",packet_rcv.data);	

		char file_name[20];
		int i=0;
		for(i=0; i<strlen(packet_rcv.data)+1; i++) {
			file_name[i]=packet_rcv.data[i];
		}

		FILE *fdest = fopen(file_name,"w");
		fclose(fdest);

		sendto(sock, &packet_rcv, sizeof(packet_rcv), 0, (struct sockaddr *)&serverStorage, addr_size);

		// receive  datagrams
		while(nBytes = recvfrom (sock, &packet_rcv, sizeof(packet_rcv), 0, (struct sockaddr *)&serverStorage, &addr_size)) {

			fdest = fopen(file_name,"a");
			fwrite(packet_rcv.data,1,10,fdest);

			// convert message
			printf("%s\n",packet_rcv.data);
			// send message back
			sendto (sock, &packet_rcv, sizeof(packet_rcv), 0, (struct sockaddr *)&serverStorage, addr_size);
			fclose(fdest);
		}
	}

	return 0;
}
