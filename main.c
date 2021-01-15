// include Library : 
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "toolkit/main.h"
#include "sqlite3/sqlite3mgt.h"
#include "sqlite3/sqlite3mgt_default.h"
// Finish.

// Defines : 
#define MAX_CLIENTS 100
#define BUFFER_SZ 2048
// Finish.

// Global Variebles : 
static _Atomic unsigned int client_count = 0;
static int uid = 10;
// Finish.

// Structures : 
typedef struct {
	struct sockaddr_in address;
	int sockfd;
	int uid;
	char name[32];
} client_t;
// Finish;

client_t* clients[MAX_CLIENTS];

pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

// OverWrite stdout : 
void str_overwrite_stdout()
{
	printf("\r%s", "> ");
	fflush(stdout);
}
//Finish.

// String Trim : 
void str_trim_lf(arr, length)
char* arr;
int length;
{
	int i;
	for (i = 0; i < length; i++) { // trim \n
		if (arr[i] == '\n') {
			arr[i] = '\0';
			break;
		}
	}
}
// Finish.

void print_ip_address(struct sockaddr_in addr) {
	printf("%d.%d.%d.%d",
		addr.sin_addr.s_addr & 0xff,
		(addr.sin_addr.s_addr & 0xff00) >> 8,
		(addr.sin_addr.s_addr & 0xff0000) >> 16,
		(addr.sin_addr.s_addr & 0xff000000) >> 24);
}

// Add clients to queue : 
void queue_add(client_t* cl) {
	pthread_mutex_lock(&clients_mutex);

	for (int i = 0; i < MAX_CLIENTS; ++i) {
		if (!clients[i]) {
			clients[i] = cl;
			break;
		}
	}

	pthread_mutex_unlock(&clients_mutex);
}

// Remove clients to queue : 
void queue_remove(int uid) {
	pthread_mutex_lock(&clients_mutex);

	for (int i = 0; i < MAX_CLIENTS; ++i) {
		if (clients[i]) {
			if (clients[i]->uid == uid) {
				clients[i] = NULL;
				break;
			}
		}
	}

	pthread_mutex_unlock(&clients_mutex);
}

// Send message to all clients except sender : 
void send_message(char* s, int uid) {
	pthread_mutex_lock(&clients_mutex);

	for (int i = 0; i < MAX_CLIENTS; ++i) {
		if (clients[i]) {
			if (clients[i]->uid != uid) {
				if (write(clients[i]->sockfd, s, strlen(s)) < 0) {
					perror("ERROR: write to descriptor failed");
					break;
				}
			}
		}
	}

	pthread_mutex_unlock(&clients_mutex);
}

// Handle all communication with the client : 
void* handle_client(void* arg) {
	char buff_out[BUFFER_SZ];
	char name[32];
	int leave_flag = 0;

	client_count++;
	client_t* cli = (client_t*)arg;

	// Name : 
	if (recv(cli->sockfd, name, 32, 0) <= 0 || strlen(name) < 2 || strlen(name) >= 32 - 1) {
		printf("Didn't enter the name.\n");
		leave_flag = 1;
	}
	else {
		strcpy(cli->name, name);
		sprintf(buff_out, "%s has joined\n", cli->name);
		printf("%s", buff_out);
		send_message(buff_out, cli->uid);
	}

	bzero(buff_out, BUFFER_SZ);

	while (1) {
		if (leave_flag) {
			break;
		}

		int receive = recv(cli->sockfd, buff_out, BUFFER_SZ, 0);
		if (receive > 0) {
			if (strlen(buff_out) > 0) {
				send_message(buff_out, cli->uid);

				str_trim_lf(buff_out, strlen(buff_out));
				printf("%s -> %s\n", buff_out, cli->name);
			}
		}
		else if (receive == 0 || strcmp(buff_out, "exit") == 0) {
			sprintf(buff_out, "%s has left\n", cli->name);
			printf("%s", buff_out);
			send_message(buff_out, cli->uid);
			leave_flag = 1;
		}
		else {
			printf("ERROR: -1\n");
			leave_flag = 1;
		}

		bzero(buff_out, BUFFER_SZ);
	}

	// Delete client from queue and yield thread : 
	close(cli->sockfd);
	queue_remove(cli->uid);
	free(cli);
	client_count--;
	pthread_detach(pthread_self());

	return NULL;
}

int main()// (int argc, char** argv)
{
	// Variebles : 
	FILE *fp;
	char buff[255], * array[3];
	if(folder_exists("config"))
	{
		fp = fopen("config/settings.txt", "r");
		readfile(buff, sizeof(buff), fp);
		split(buff, ",", array);
	}
	else
	{
		mkdir("config", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		fp = fopen("config/settings.txt", "w+");
		fprintf(fp, "%s,%s,%s", "127.0.0.1", "8080", "messenger.db");
		readfile(buff, sizeof(buff), fp);
		split(buff, ",", array);
		printf("Settings File And DataBase File Check And Created But Please ReOpen Program.");
		return EXIT_SUCCESS;
	}
	printf("Your DataBase And Settings File Created Your Settings Is :\nYour Ip : %s\nYour Port : %s", array[0], array[1]);
	printf("\nYour DataBase Name : %s\n", array[2]);
	Sqlite3_MGT Messengerdb = Sqlite3_Default;
	strcpy(Messengerdb.databasename, array[2]);
	strcpy(Messengerdb.columnname, "Name, lastmessage, messagetime");
	strcpy(Messengerdb.value, "'test', 'test', 'test'");
	sqlite3mgt(Messengerdb);
	strcpy(Messengerdb.tablename, "Clients");
	strcpy(Messengerdb.condition, "4");
	sqlite3mgt(Messengerdb);
	printf("\ntest finished please check database.\n");
	struct sockaddr_in server_address, client_address;
	pthread_t ptid;
	char* ip = array[0];
	int port = atoi(array[1]), option = 1, listenfd = 0, connfd = 0;

	// Socket Settings : 
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr(ip);
	server_address.sin_port = htons(port);
	// Finish.

	// Linux Supported.
	// Signals : 
	/*if (setsockopt(listenfd, SOL_SOCKET, (SO_REUSEPORT | SO_REUSEADDR), (char*)&option, sizeof(option)) < 0)
	{
		printf("ERROR: setsockopt\n");
		return EXIT_FAILURE;
	}*/
	// Finish.

	// Bind : 
	if (bind(listenfd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0)
	{
		printf("ERROR: bind\n");
		return EXIT_FAILURE;
	}
	// Finish.

	// Listen : 
	if (listen(listenfd, 10) < 0)
	{
		printf("ERROR: listen\n");
		return EXIT_FAILURE;
	}
	// Finish.

	while (1)
	{
		socklen_t clientlen = sizeof(client_address);
		connfd = accept(listenfd, (struct sockaddr*)&client_address, &clientlen);

		// Check if max clients is reached : 
		if((client_count + 1) == MAX_CLIENTS)
		{
			printf("Maximum Clients Connected. Please Wait Connection Rejected : ");
			print_ip_address(client_address);
			printf(":%d\n", client_address.sin_port);
			close(connfd);
			continue;
		}

		// Client settings : 
		client_t* cli = (client_t*)malloc(sizeof(client_t));
		cli->address = client_address;
		cli->sockfd = connfd;
		cli->uid = uid++;

		// Add client to the queue and fork thread : 
		queue_add(cli);
		pthread_create(&ptid, NULL, &handle_client, (void*)cli);

		// Reduce CPU usage : 
		sleep(1);
	}
	return EXIT_SUCCESS;
}