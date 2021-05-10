#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int sendfile(int sock)
{
    FILE *file = NULL;
    char buf[BUFSIZ];

    size_t filesize, sendsize = 0;
    int file_pointer_size;

    system("sudo docker commit nx nx2");
    system("rm nx2.tar");
    system("sudo docker save -o nx2.tar nx2");
    //sleep(5);
    system("sudo chmod 777 nx2.tar");

    if ((file = fopen("nx2.tar", "rb")) == NULL)
    {
        perror("file open error");
    }

    fseek(file, 0, SEEK_END);
    filesize = ftell(file);
    printf("filesize :%ld\n", filesize);
    fseek(file, 0, SEEK_SET);

    while (sendsize != filesize)
    {
        memset(buf, '\0', sizeof(buf));
        file_pointer_size = fread(buf, sizeof(char), sizeof(buf), file);
        sendsize += file_pointer_size;
        send(sock, buf, file_pointer_size, 0);
        printf("sendsize:%ld\n", sendsize);
    }

    sleep(2);

    fclose(file);

    return 0;
}