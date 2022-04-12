#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MESSAGE_SIZE 30
#define PORT 8080

int check(int result, char* message)
{
	if (result < 0) { 
		printf("CLIENT:\033[1m\033[31m%s\033[0m", message), perror(""), exit(EXIT_FAILURE); 
	}
	printf("CLIENT:\033[1m\033[32m%s\033[0m\n", message);
	return result;
}

int main(void)
{
	int client_socket = check(socket(AF_INET, SOCK_STREAM, 0), "SOCKET");

	struct sockaddr_in server_address, client_address;
	memset(&server_address, 0, sizeof(server_address)), memset(&client_address, 0, sizeof(client_address));

	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(PORT);

	check(connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)), "CONNECT");
	check(0, "READY");
	
	char message[MESSAGE_SIZE] = "Hello Server!";
	
	check(send(client_socket, message, strlen(message), 0), "SEND");
	
	printf("CLIENT:MY MESSAGE %s\n", message);
	
	check(recv(client_socket, message, MESSAGE_SIZE, 0), "RECEVE");
	
	printf("CLIENT:SERVER MESSAGE %s\n", message);
	
	close(client_socket);
	check(0, "EXIT");
	return 0;
}