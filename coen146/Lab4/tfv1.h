


#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define SIZE    10


typedef struct
{
    int	seq_ack;
	int len;
	char checksum;
	int fin;

} HEADER;

typedef struct
{
    HEADER	header;
    char	data[SIZE];
} PACKET;


