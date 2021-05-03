#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

#define SERVERIP "127.0.0.1"
#define VMTWOIP "192.168.0.9"
#define RECV_MSG "migrate crazy_orange 1 2"

extern int sendfile(int sock);

int main(int argc, char *argv[])
{
    int con_sock, sendfile2VM2_sock;
    int con_len, sendfile2VM2_len;
    struct sockaddr_in con_addr, sendfile2VM2_addr;
    char recvMsg[100];

    con_len = sizeof(con_addr);

    if ((con_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("sock1 error");
        exit(1);
    }

    con_addr.sin_family = AF_INET;
    con_addr.sin_addr.s_addr = inet_addr(SERVERIP);
    con_addr.sin_port = htons(atoi(argv[1]));

    if ((connect(con_sock, (struct sockaddr *)&con_addr, con_len)) == -1)
    {
        perror("connect fail");
        exit(1);
    }

    recv(con_sock, recvMsg, sizeof(recvMsg), 0);
    printf("recvMsg : %s\n", recvMsg);

    // access to vm2

    if (strncmp(recvMsg, RECV_MSG, strlen(RECV_MSG)) == 0)
    {
        sendfile2VM2_len = sizeof(sendfile2VM2_addr);

        if ((sendfile2VM2_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
            perror("sock2 error");
            exit(1);
        }

        sendfile2VM2_addr.sin_family = AF_INET;
        sendfile2VM2_addr.sin_addr.s_addr = inet_addr(VMTWOIP);
        sendfile2VM2_addr.sin_port = htons(atoi(argv[2]));

        if ((connect(sendfile2VM2_sock, (struct sockaddr *)&sendfile2VM2_addr, sendfile2VM2_len)) == -1)
        {
            perror("connect2 error");
            exit(1);
        }
        else
        {
            printf("connected vm2\n");
        }

        sendfile(sendfile2VM2_sock);
        close(sendfile2VM2_sock);
    }
    else
    {
        printf("please, Try again\n");
    }

    close(con_sock);

    return 0;
}