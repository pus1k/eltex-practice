#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MESSAGE_SIZE 81
#define PORT 8080

#define GREEN 32
#define RED 31

void color_print(int color, char* str) { printf("SERVER: \033[1m\033[%dm%s\033[0m", color, str); }

int check(int result, char* err)
{
	if (result < 0) { color_print(RED, err), perror("\n"), exit(EXIT_FAILURE); }
	return result;
}

void udp()
{
	int server_socket = check(socket(AF_INET, SOCK_DGRAM, 0), "SOCKET");

	struct sockaddr_in server_address, client_address;
	memset(&server_address, 0, sizeof(server_address));
	memset(&client_address, 0, sizeof(client_address));

	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(PORT);

	check(bind(server_socket, (const struct sockaddr*)&server_address, sizeof(server_address)), "BIND");
	color_print(GREEN, "READY\n");
	
	char message[MESSAGE_SIZE];
	memset(&message, 0, sizeof(message));

	int length = sizeof(client_address);
	recvfrom(server_socket, message, MESSAGE_SIZE, 0, (struct sockaddr*)&client_address, (socklen_t*)&length);
	
	printf("SERVER: [\033[1m\033[34m%d\033[0m] %s\n", ntohs(client_address.sin_port), message);
	strcat(message, "_SERVER");
	
	sendto(server_socket, message, MESSAGE_SIZE, MSG_WAITALL, (struct sockaddr*)&client_address, sizeof(client_address));
	close(server_socket);
}
void tcp()
{
	int client_socket = -1, server_socket = check(socket(AF_INET, SOCK_STREAM, 0), "SOCKET");

	struct sockaddr_in server_address, client_address;
	memset(&server_address, 0, sizeof(server_address));
	memset(&client_address, 0, sizeof(client_address));

	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(PORT);

	check(bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)), "BIND");
	listen(server_socket, 3);
	color_print(GREEN, "READY\n");

	socklen_t addr_size = sizeof(client_address);
	while (client_socket == -1) { client_socket = accept(server_socket, (struct sockaddr*)&client_address, &addr_size); }
	
	char message[MESSAGE_SIZE];
	memset(&message, 0, sizeof(message));
	
	check(recv(client_socket, message, MESSAGE_SIZE, 0), "RECV");
	
	printf("SERVER: [\033[1m\033[34m%d\033[0m] %s\n", ntohs(client_address.sin_port), message);
	strcat(message, "_SERVER");

	check(send(client_socket, message, strlen(message), 0), "SEND");
	close(server_socket);
}
int main(void)
{
	udp();
	tcp();
	return 0;
}