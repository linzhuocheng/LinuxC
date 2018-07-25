#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

#define PORT 12345

int g_listenfd = 0;

int main()
{
    g_listenfd = ::socket(AF_INET, SOCK_STREAM, 0);
    if(g_listenfd == -1)
    {
        return false;
    }
    int on = 1;
    ::setsockopt(g_listenfd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
    ::setsockopt(g_listenfd, SOL_SOCKET, SO_REUSEPORT, (char *)&on, sizeof(on));

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("0.0.0.0");
    servaddr.sin_port = htons(PORT);
    if(::bind(g_listenfd, (sockaddr *)&servaddr, sizeof(servaddr)) == -1)
        return false;
	if(::listen(g_listenfd,50)==-1)
		return false;

	struct sockaddr_in clientaddr;
	socklen_t addrlen;
    int newfd = ::accept(g_listenfd,(struct sockaddr *)&clientaddr, &addrlen);
	if(newfd == -1)
		return false;

	int oldflag = ::fcntl(newfd,F_GETFL,0);
	int newflag = oldflag | O_NONBLOCK;
	::fcntl(newfd,F_SETFL,newflag);

	std::cout << "new client connected: "<< inet_ntoa(clientaddr.sin_addr) << std::endl;

	std::cout<<std::endl;

	std::string clientmsg;
	char buff[256];
	bool bError = false;
	while(true)
	{
	while(true)
	{
		memset(buff, 0, sizeof(buff));
		int nRecv = ::recv(newfd,buff,255,0);
		if(nRecv == -1)
		{
			if(errno == EWOULDBLOCK)
				break;
			else
			{
				std::cout<<"recv error, client disconnected, fd = " << newfd << std::endl;
				::close(newfd);
				bError = true;
				break;
			}
		}
		else if(nRecv==0)
		{
			std::cout<<"peer closed,client disconnected,fd = "<< newfd << std::endl;
			close(newfd);
			bError = true;
			break;
		}
		clientmsg += buff;
		//if(buff[nRecv] == '\0')
		//	break;
	}
	if(bError)
		return -1;
	if(clientmsg.size()!=0)
		std::cout << clientmsg <<std::endl;
	while(true)
	{
		int nSent = ::send(newfd, clientmsg.c_str(),clientmsg.length(), 0);
		if(nSent == -1)
		{
			if(errno == EWOULDBLOCK)
			{
				::sleep(10);
				continue;
			}
			else
			{
				std::cout << "send error, fd = " << newfd << std::endl;
				close(newfd);
				break;
			}
		}
if(clientmsg.size()!=0)
	std::cout << "send: " << clientmsg << std::endl;
		clientmsg.erase(0,nSent);

		if(clientmsg.empty())
			break;
	}
	}
}
