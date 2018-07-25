#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
int main()
{
    pid_t pid;
    printf("fork!");
    printf("%d",getpid());
    pid = fork();
    if(pid==0)
    {
        printf("I am child process,id is: %d,and my parent's id is: %d\n ",getpid(),getppid());
    }
    else{
        printf("I am parent process,id is: %d,and my parent's id is: %d\n ",getpid(),getppid());
    }
    printf("nihao!\n");
}
