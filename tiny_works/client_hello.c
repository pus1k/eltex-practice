#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MESSAGE_SIZE 81
#define PORT 8080

#define GREEN 32
#define RED 31

void color_print(int color, char* str) { printf("CLIENT: \033[1m\033[%dm%s\033[0m", color, str); }

int check(int result, char* err)
{
	if (result < 0) { color_print(RED, err), perror("\n"), exit(EXIT_FAILURE); }
	return result;
}
void udp()
{
	int client_socket = check(socket(AF_INET, SOCK_DGRAM, 0), "SOCKET");

	struct sockaddr_in server_address, client_address;
	memset(&server_address, 0, sizeof(server_address));
	memset(&client_address, 0, sizeof(client_address));

	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(PORT);

	check(bind(client_socket, (struct sockaddr*)&client_address, sizeof(client_address)), "BIND");

	color_print(GREEN, "READY\n");

	char message[MESSAGE_SIZE];
	memset(&message, 0, sizeof(message));
	strcpy(message, "Hello world!");
	
	socklen_t addr_size = sizeof(client_address);
	getsockname(client_socket, (struct sockaddr*)&client_address, &addr_size);
	printf("CLIENT: MY PORT [\033[1m\033[34m%d\033[0m]\n", ntohs(client_address.sin_port));

	sendto(client_socket, message, MESSAGE_SIZE, MSG_WAITALL, (struct sockaddr*)&server_address, sizeof(server_address));
	recvfrom(client_socket, message, MESSAGE_SIZE, 0, (struct sockaddr*) & server_address, (socklen_t*)sizeof(server_address));
	
	color_print(33, "MESSAGE FROM SERVER "), printf("%s\n", message);
	close(client_socket);
	sleep(3);
}
void tcp()
{
	int client_socket = check(socket(AF_INET, SOCK_STREAM, 0), "SOCKET");
	
	struct sockaddr_in server_address, client_address;
	memset(&server_address, 0, sizeof(server_address));
	memset(&client_address, 0, sizeof(client_address));

	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(PORT);

	check(connect(client_socket, (const struct sockaddr*)&server_address, sizeof(server_address)), "CONNECT");

	socklen_t addr_size = sizeof(client_address);
	getsockname(client_socket, (struct sockaddr*)&client_address, &addr_size);

	color_print(GREEN, "READY\n");
	printf("CLIENT: MY PORT [\033[1m\033[34m%d\033[0m]\n", ntohs(client_address.sin_port));
	
	char message[MESSAGE_SIZE];
	memset(&message, 0, sizeof(message));
	strcpy(message, "Hello world!");

	check(send(client_socket, message, strlen(message), 0), "SEND");
	check(recv(client_socket, message, MESSAGE_SIZE, 0), "RECV");

	color_print(33, "MESSAGE FROM SERVER "), printf("%s\n", message);
	close(client_socket);
}
int main(void)
{
	udp();
	tcp();
	return 0;
}