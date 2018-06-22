#include <stdio.h>
#include <pthread.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <limits.h>


//myid, neighbor, cost
//gcc -o lab5.out lab5.c -lpthread

int N = 4; //default number of nodes
int localId;
pthread_mutex_t lock;
int dataSend[3];
int dataRecieve[3];
int mySocket;

typedef struct {
	char name[64]; //appease the binary gods
	char ip[15];
	int port;
	int costs[256];
} ROUTER;

const int NUM_READ_CHANGES = 3;

int update_data[3];

ROUTER hosts[256]; //appease the binary gods


void *receiveUpdates();
void *updateLinkState();

int main(int argc, char* argv[]) {
	if(argc != 5) {
		printf("Invalid args.\n Proper usage: <id> <num_hosts> <costs_file> <hosts_file>\n");
		return 0;
	}
	
	sscanf(argv[1],"%d",&localId);
	sscanf(argv[2],"%d",&N);

	FILE *costsFile = fopen(argv[3],"r");
	FILE *hostsFile = fopen(argv[4], "r");


	readFiles(costsFile, hostsFile);


	struct sockaddr_in myAddr, otherAddr;
	struct sockaddr_storage myStorage;
	socklen_t addr_size, other_addr_size;
	
//set up local port
	myAddr.sin_family = AF_INET;
	myAddr.sin_port = htons ((short)hosts[localId].port);
	myAddr.sin_addr.s_addr = htonl (INADDR_ANY);
	memset ((char *)myAddr.sin_zero, '\0', sizeof (myAddr.sin_zero));  
	addr_size = sizeof (myStorage);
	
	printf("listening on port %d\n",hosts[localId].port);

	//create socket
	if ((mySocket = socket (AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		printf ("ERROR creating socket\n");
		return 1;
	}
	
	//bind socket
	if (bind (mySocket, (struct sockaddr *)&myAddr, sizeof (myAddr)) != 0)
	{
		printf ("ERROR binding socket\n");
		return 1;
	}

	//initialize threads
	pthread_t recieve_t, linkState_t;
	pthread_create(&recieve_t, NULL, receiveUpdates, NULL);
	pthread_create(&linkState_t, NULL, updateLinkState, NULL);
	
	int i=0;
	for(i=0; i<NUM_READ_CHANGES; i++) {
		readChanges();
		sleep(10);
	}

	return 0;
}

void readFiles(FILE* costsFile, FILE* hostsFile) {
	//load hosts
	int i=0;
	for(i=0; i<N; i++) {
		if(fscanf(hostsFile,"%s %s %d", &(hosts[i].name),&(hosts[i].ip), &(hosts[i].port)) < 1) {
			break;
		}
		printf("Loaded host %s %s %d\n", (hosts[i].name),hosts[i].ip, (hosts[i].port));
	}

	//load costs
	int j=0;
	for(i=0; i<N; i++) {
		for(j=0; j<N; j++) {
			if(fscanf(costsFile, "%d", &(hosts[i].costs[j])) < 1) {
				break;
			}	
			printf("%d ", hosts[i].costs[j]);	
		}
		printf("\n");
	}
	return;
}

void readChanges() {
	int neighbor;
	int newCost;

	scanf("%d %d",&neighbor,&newCost);

	//aquire lock
	pthread_mutex_lock(&lock);
	hosts[localId].costs[neighbor] = newCost;
	hosts[neighbor].costs[localId] = newCost;

	//notify other hosts of costs change
	updateNodes(neighbor,newCost);

	//print new cost matrix
	int i=0,j=0;
	printf("Updated cost matrix:\n");
	for(i=0; i<N; i++) {
		for(j=0; j<N; j++) {
			printf("%d\t",hosts[i].costs[j]);
		}
		printf("\n");
	}
	pthread_mutex_unlock(&lock);
}

void updateNodes(int otherHost, int cost) {
	int i;
	int data[3] = {localId, otherHost, cost};

	int sock;
	struct sockaddr_in destAddr;
	socklen_t addr_size;

	//iterate over all hosts
	for(i=0; i<N; i++) {
		if(i != localId) {
			destAddr.sin_family = AF_INET;
			destAddr.sin_port = htons(hosts[i].port);
			inet_pton (AF_INET, hosts[i].ip, &destAddr.sin_addr.s_addr);
			memset(destAddr.sin_zero, '\0', sizeof(destAddr.sin_zero));
			addr_size = sizeof destAddr;

			sock = socket (PF_INET, SOCK_DGRAM, 0);

			printf("Sending update to %d at %s:%d\n",i,hosts[i].ip,hosts[i].port);
			sendto(sock, &data, sizeof(data),0,(struct sockaddr *)&(destAddr), addr_size);
		}
	}
}

//thread 2
void *receiveUpdates() {
	printf("Starting receive updates.\n");
	while(1) {
		int nBytes = recvfrom(mySocket, &update_data, sizeof(update_data), 0, NULL, NULL);
		printf("Received update.\n");

		int hostA = update_data[0];
		int hostB = update_data[1];
		int cost = update_data[2];

		pthread_mutex_lock(&lock);

		hosts[hostA].costs[hostB] = cost;
		hosts[hostB].costs[hostA] = cost;

		int i;
		for(i=0; i<N; i++) {
			int j;
			for(j=0; j<N; j++) {
				printf("%d\t",hosts[i].costs[j]);
			}
			printf("\n");
		}
		pthread_mutex_unlock(&lock);
	}
}

//dijkstra's
void *updateLinkState() {
	printf("Starting update link state.\n");
	for(;;) {
		sleep(10);

		int distance[N];
		int i;

		int source;
		int visited[N];

		pthread_mutex_lock(&lock);

		//for each host (source) node
		for(source = 0; source < N; source++) {
			//initialize temporary variables
			for(i=0; i<N; i++) {
				distance[i] = INT_MAX;
				visited[i]=0;
			}

			distance[source]=0;

			//for each node that is not source
			for(i=0; i<N-1; i++) {
				int d = minDistance(distance, visited);	
				visited[d] = 1;
				
				int j;
				for(j=0; j<N; j++) {
					if(visited[j]==0 && hosts[d].costs[j] != INT_MAX && distance[d]+hosts[d].costs[j] < distance[j]) {
						distance[j] = distance[d] + hosts[d].costs[j];
					}
				}
			}

			printf("Distances from host %d: ",source);
			for(i=0; i<N; i++) {
				printf("%d ",distance[i]);
			}
			printf("\n");

		}
		printf("\n");
		pthread_mutex_unlock(&lock);
	}
}

int minDistance(int dist[], int sptSet[])
{
   // Initialize min value
   int min = INT_MAX, min_index;
  
   int v;

   //for each machine
   for (v = 0; v < N; v++)
     if (sptSet[v] == 0 && dist[v] <= min) {
         min = dist[v], min_index = v;
	 }
  
   return min_index;
}
