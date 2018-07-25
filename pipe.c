#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#define K 1024
#define WRITELEN (64*K)
int main()
{
    int result=1;
    int fd[2],nbytes;
    pid_t pid;
    char string[WRITELEN]="hello world my pipe 2018!";
    char readbuffer[10*K];

    int *read_fd=&fd[0];
    int *write_fd=&fd[1];

    result=pipe(fd);
    pid=fork();
    if(0==pid)
    {
        int write_size=WRITELEN;
        result=0;
        close(*read_fd);
        while(write_size>=0)
        {
            result=write(*write_fd,string,write_size);
            if(result>0)
            {
                write_size-=result;
                printf("write %d bytes data,the rest is %d bytes\n",result,write_size);
            }
            else
            {
                printf("sleep!\n");
                sleep(10);
            }
        }
        return 0;
    }
    else
    {
        close(*write_fd);
        while(1)
        {
            nbytes=read(*read_fd,readbuffer,sizeof(readbuffer));
            if(nbytes<=0)
            {
                printf("no data to write\n");
                break;
            }
            printf("receive %d bytes data:%s\n",nbytes,readbuffer);
        }
        return 0;
    }
}
