#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define SERVERIP "192.168.0.8"

extern int recvfile(int sock);

int connect_con_sock;
int len;
struct sockaddr_in addr;
char recvMsg[100];
char *sendMsg = "VM2_DONE";

int access_sock, accept_sock;
int access_len, accept_len;
struct sockaddr_in access_addr, accept_addr;

int main(int argc, char *argv[])
{
    //connect controller
    if ((connect_con_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("sock error");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(SERVERIP);
    addr.sin_port = htons(atoi(argv[1]));
    len = sizeof(addr);

    if ((connect(connect_con_sock, (struct sockaddr *)&addr, len)) == -1)
    {
        perror("connect error");
        exit(1);
    }

    //recvfile server from vm1
    if ((access_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("server sock error");
        exit(1);
    }

    access_addr.sin_family = AF_INET;
    access_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    access_addr.sin_port = htons(atoi(argv[2]));
    access_len = sizeof(access_addr);

    if ((bind(access_sock, (struct sockaddr *)&access_addr, access_len)) == -1)
    {
        perror("bind error");
        exit(1);
    }

    if (listen(access_sock, 5) == -1)
    {
        perror("listen error");
        exit(1);
    }

    accept_len = sizeof(accept_addr);

    if ((accept_sock = accept(access_sock, (struct sockaddr *)&accept_addr, &accept_len)) == -1)
    {
        perror("accept error");
        exit(1);
    }
    else
    {
        printf("connected vm1\n");
    }

    recvfile(accept_sock);

    printf("ok done\n");
    close(accept_sock);

    send(connect_con_sock, sendMsg, strlen(sendMsg) + 1, 0);
    printf("sendMsg : %s\n", sendMsg);

    close(connect_con_sock);

    return 0;
}