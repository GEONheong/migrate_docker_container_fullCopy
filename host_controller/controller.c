#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int access_sock, accept_sock[3];
    int access_len, accept_len;
    struct sockaddr_in access_addr, accept_addr;

    access_len = sizeof(access_addr);
    accept_len = sizeof(accept_addr);

    char recvMsg[100];

    if ((access_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("sock error");
        exit(1);
    }

    access_addr.sin_family = AF_INET;
    access_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    access_addr.sin_port = htons(atoi(argv[1]));

    if ((bind(access_sock, (struct sockaddr *)&access_addr, access_len)) == -1)
    {
        perror("bind error");
        exit(1);
    }

    if ((listen(access_sock, 5)) == -1)
    {
        perror("listen error");
        exit(1);
    }

    while (1)
    {
        printf("Server Wating...\n");

        for (int i = 0; i < 3; i++)
        {
            if ((accept_sock[i] = accept(access_sock, (struct sockaddr *)&accept_addr, &accept_len)) == -1)
            {
                perror("accept error");
                exit(1);
            }
            else
            {
                printf("%d-conneted\n", i);
            }
        }
        // 0 : user
        // 1 : vm1
        // 2 : vm2

        recv(accept_sock[2], recvMsg, sizeof(recvMsg), 0);
        printf("recvMsg from user: %s\n", recvMsg);
        send(accept_sock[0], recvMsg, sizeof(recvMsg), 0);
        printf("sendMsg to VM1 : %s\n", recvMsg);
        recv(accept_sock[1], recvMsg, sizeof(recvMsg), 0);
        printf("recvMsg from VM2: %s\n", recvMsg);
        send(accept_sock[2], recvMsg, sizeof(recvMsg), 0);
        printf("sendMsg to user : %s\n", recvMsg);

        close(accept_sock[0]);
        close(accept_sock[1]);
        close(accept_sock[2]);
    }

    
    return 0;
}
