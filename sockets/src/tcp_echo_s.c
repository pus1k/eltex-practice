#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define MESSAGE_SIZE 30
#define PORT 8080

static int count_of_clients_served = 1;

int check(int result, char* message)
{
	if (result < 0) { 
		printf("SERVER:\033[1m\033[31m%s\033[0m", message), perror(""), exit(EXIT_FAILURE); 
	}
	printf("SERVER:\033[1m\033[32m%s\033[0m\n", message);
	return result;
}

void rip() { while (waitpid(-1, NULL, WNOHANG) > 0); count_of_clients_served++;}

void work(int client_socket)
{
	char message[MESSAGE_SIZE];
	memset(&message, 0, MESSAGE_SIZE);
	check(recv(client_socket, message, MESSAGE_SIZE, 0), "RECEVE");
	printf("SERVER:CLIENT MESSAGE %s\n", message);
	memset(&message, 0, MESSAGE_SIZE);
	strcpy(message, "Hello Client!");
	printf("SERVER:MY MESSAGE %s\n", message);
	check(send(client_socket, message, strlen(message), 0), "SEND");
}
int main(void)
{
	int server_socket = check(socket(AF_INET, SOCK_STREAM, 0), "SOCKET");

	struct sockaddr_in server_address, client_address;
	memset(&server_address, 0, sizeof(server_address)), memset(&client_address, 0, sizeof(client_address));

	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(PORT);

	check(bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)), "BIND");
	listen(server_socket, 3);

	signal(SIGCHLD, rip);
	check(0, "READY");
	while(count_of_clients_served != 5)
	{
		socklen_t addr_size = sizeof(client_address);
		memset(&client_address, 0, sizeof(client_address));
		int client_socket = accept(server_socket, (struct sockaddr*)&client_address, &addr_size);
		if (client_socket != -1)
		{
			switch(fork()) {
				case 0:
					check(0, "NEW CONNECTION");
					close(server_socket);
					work(client_socket);
					check(0, "DISCONNECTION");
					close(client_socket);
					printf("\n");
					exit(EXIT_SUCCESS);
					break;
				case -1: check(-1, "FORK");
			}
			close(client_socket);
		}
	}
	close(server_socket);
	return 0;
}