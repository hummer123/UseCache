#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>

#define LISTENQ 5

int listenfd, connfd;

void sig_urg(int);

int Tcp_listen(const char *host, const char *serv, socklen_t *addrlenp)
{
    return (tcp_listen(host, serv, addrlenp));
}

int tcp_listen(const char *host, const char *serv, socklen_t *addrlenp)
{
    int listenfd, n;
    const int on = 1;
    struct addrinfo hints, *res, *ressave;

    bzero(&hints, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_socktype = SOCK_STREAM;

    if( (n = getaddrinfo(host, serv, &hints, &res)) != 0)
    {
        printf("tcp_listen error for %s, %s: %s\n", host, serv, gai_strerror(n));
        exit(-2);
    }
    ressave = res;

    do{
        listenfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if(listenfd < 0)
            continue;
        if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
        {
            printf("setsockopt error\n");
            exit(-2);
        }
        if(bind(listenfd, res->ai_addr, res->ai_addrlen) == 0)
            break;
        close(listenfd);
    }while( (res = res->ai_next) != NULL);

    if(res == NULL)
    {
        printf("tcp_listen error for %s, %s\n", host, serv);
        exit(-2);
    }
    
    if(listen(listenfd, LISTENQ) < 0)
    {
        printf("listen error\n");
        exit(-2);
    }
    
    if(addrlenp)
        *addrlenp = res->ai_addrlen;

    freeaddrinfo(ressave);

    return(listenfd);
}




int main(int argc, char **argv)
{
    int size;
    char buff[100];

    if(argc == 2)
    {
        listenfd = Tcp_listen(NULL, argv[1], NULL);
    }
    else if(argc == 3)
    {
        listenfd = Tcp_listen(argv[1], argv[2], NULL);
    }
    else
    {
        printf("usage: tcprecv [ <host> ] <port#>\n");
        exit(-2);
    }
	
	size = 4096;
	if(setsockopt(listenfd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size)) < 0)
	{
		printf("setsockopt error\n");
		exit(-2);
	}
		
    if( (connfd = accept(listenfd, NULL, NULL)) < 0)
    {
        printf("accept error\n");
        exit(-2);
    }

    signal(SIGURG, sig_urg);
    fcntl(connfd, F_SETOWN, getpid());

    for(; ;)
    {
		pause();
		/*
        if( (n = read(connfd, buff, sizeof(buff)-1)) == 0)
        {
            printf("received EOF\n");
            exit(0);
        }
        buff[n] = 0;
        printf("read: %d bytes: %s\n", n, buff);
		*/
	}
}

void sig_urg(int signo)
{
    int n;
    char buf[2048];

    printf("SIGURG received\n");
    n = recv(connfd, buf, sizeof(buf)-1, MSG_OOB);
    buf[n] = 0;
    printf("read: %d OOB byte: %s\n", n, buf);
}

