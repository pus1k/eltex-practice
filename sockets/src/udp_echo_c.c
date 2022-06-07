#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MESSAGE_SIZE 81
#define PORT 8080

#define YELLOW 33
#define GREEN 32
#define RED 31

void color_print(int color, char* str) { printf("CLIENT:\033[1m\033[%dm%s\033[0m", color, str); }

int check(int result, char* err)
{
	if (result < 0) { color_print(RED, err), perror(""), exit(EXIT_FAILURE); }
	return result;
}

int main(void)
{
	int client_socket = socket(AF_INET, SOCK_DGRAM, 0);
	check(client_socket, " SOCKET");

	struct sockaddr_in server_address, client_address;
	memset(&server_address, 0, sizeof(server_address));
	memset(&client_address, 0, sizeof(client_address));

	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(PORT);

	check(bind(client_socket, (const struct sockaddr*)&client_address, sizeof(client_address)), " BIND");

	color_print(GREEN, " READY\n");
	char message[MESSAGE_SIZE];
	sprintf(message, "Hello world!");
	
	sendto(client_socket, message, MESSAGE_SIZE, MSG_WAITALL, (const struct sockaddr*)&server_address, sizeof(server_address));
	recvfrom(client_socket, message, MESSAGE_SIZE, 0, (struct sockaddr * restrict) & server_address, (socklen_t * restrict)sizeof(server_address));
	color_print(33, " MESSAGE FROM SERVER "), printf("%s\n", message);

	close(client_socket);
	return 0;
}