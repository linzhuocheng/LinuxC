#define _BSD_SOURCE
#include <signal.h>
#include <stdio.h>
#define _GNU_SOURCE
#include <string.h>

int cnt = 0;
void handler(int sig)
{
    printf("receive :%s\n",strsignal(sig));
    cnt++;
}
int main()
{
    signal(SIGINT,handler);
    while(cnt!=3);
}
