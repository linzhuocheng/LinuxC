#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
int fd = 0;
void create_socket()
{
    fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
}
int main()
{
    create_socket();
    printf("fd:%d\n",fd);
    sleep(60);
    printf("The socket has been closed!\n");
    close(fd);
    while(1);
}
