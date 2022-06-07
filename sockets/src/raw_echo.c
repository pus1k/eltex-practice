#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define PORT 8080
#define MSG_SIZE 256

struct PacketSend {
    struct udphdr udp_header;
    char mes[256];
};

struct PacketRecv {
    struct iphdr ip_header;
    struct udphdr udp_header;
    char mes[256];
};

int check(int ret, const char* opt)
{
    if (ret < 0) 
    { perror(opt), exit(EXIT_FAILURE); }
    else
    { printf("%s:SUCCKES\n", opt); return ret; }
}
int main(void)
{
    int sockd = check(socket(AF_INET, SOCK_RAW, IPPROTO_UDP), "SOCKET");

    struct sockaddr_in s_addr, c_addr;
	memset(&s_addr, 0, sizeof(s_addr));
	memset(&c_addr, 0, sizeof(c_addr));

    s_addr.sin_family = AF_INET;
	s_addr.sin_addr.s_addr = INADDR_ANY;
	s_addr.sin_port = htons(PORT);

	check(bind(sockd, (struct sockaddr*)&c_addr, sizeof(c_addr)), "BIND");

    char let[MSG_SIZE] = "Hello world!";
    packet_send.udp_header.source = htons(8080);
    packet_send.udp_header.dest = htons(atoi(argv[1]));
    packet_send.udp_header.len = htons(sizeof(struct PacketSend));
    packet_send.udp_header.check = 0;

    check(sendto(sockd, &packet_send, sizeof(struct PacketSend), 0,
               (struct sockaddr *)(&server_addr), sizeof(struct sockaddr_in)), "SEND");
    socklen_t length = sizeof(s_addr);
    recvfrom(sockd, msg, MSG_SIZE, 0, (struct sockaddr*)&s_addr, &length);

	printf("MSG: %s\n", msg);
    close(sockd);
    return 0;
}