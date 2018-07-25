#include "i4d_ucase.h"
int main(int argc, char *argv[])
{
    struct sockaddr_in svaddr;
    int sfd,j;
    size_t msglen;
    ssize_t numBytes;
    char resp[BUF_SIZE];
    int addr_len = sizeof(struct sockaddr_in);
    sfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sfd == -1)
        return -1;
    memset(&svaddr, 0, sizeof(struct sockaddr_in));
    svaddr.sin_family = AF_INET;
    svaddr.sin_port = htons(PORT_NUM);
    if(inet_pton(AF_INET, argv[1], &svaddr.sin_addr)<=0)
        return -1;
    for(j=2;j<argc;j++)
    {
        msglen = strlen(argv[j]);
        if(sendto(sfd,argv[j],msglen,0,(struct sockaddr *)&svaddr,sizeof(struct sockaddr_in))!=msglen)
            return -1;
        numBytes = recvfrom(sfd,resp,BUF_SIZE,0,(struct sockaddr *)&svaddr, &addr_len);

        if(numBytes == -1)
            return -1;
        printf("Response %d: %.*s\n",j-1,(int)numBytes,resp);
    }
}
