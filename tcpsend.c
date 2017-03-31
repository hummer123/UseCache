#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>


int Tcp_connect(const char *host, const char *serv)
{
	return(tcp_connect(host, serv));
}

int tcp_connect(const char *host, const char *serv)
{
	int sockfd, n;
	struct addrinfo hints, *res, *ressave;
	
	bzero(&hints, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	
	if( (n = getaddrinfo(host, serv, &hints, &res)) != 0)
	{
		printf("tcp_conect error for %s, %s: %s\n", host, serv, gai_strerror(n));
		exit(-2);
	}
	
	ressave = res;
	do{
		sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if(sockfd < 0)
			continue;
		if(connect(sockfd, res->ai_addr, res->ai_addrlen) == 0)
			break;
		close(sockfd);
	}while((res = res->ai_next) != NULL);
	
	if(res == NULL)
	{
		printf("tcp_connect error for %s, %s\n", host, serv);
		exit(-2);
	}
	freeaddrinfo(ressave);
	
	return (sockfd);
}

int main(int argc, char **argv)
{
	int sockfd, size;
	char buff[16384];
	
	if(argc != 3)
	{
        printf("usage: tcpsend <host> <port#>\n");
        exit(-1);
	}

    sockfd = Tcp_connect(argv[1], argv[2]);
    
/*
	size = 32768;
	if(setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &size, sizeof(size)) < 0)
	{
		printf("setsockopt error\n");
		exit(-2);
	}
		
    write(sockfd, buff, 16384);
    printf("wrote 16384 bytes of normal data\n");
    sleep(5);

    send(sockfd, "a", 1, MSG_OOB);    
    printf("wrote 1 bytes of OOB data\n");
    
    write(sockfd, buff, 1024);
    printf("wrote 1024 bytes of normal data\n");
*/

    write(sockfd, "123", 3);
    printf("wrote 3 bytes of normal data\n");
    //sleep(1);

    send(sockfd, "4", 1, MSG_OOB);    
    printf("wrote 1 bytes of OOB data\n");
    //sleep(1);
    
    write(sockfd, "56", 2);
    printf("wrote 2 bytes of normal data\n");
    //sleep(1);
    
    send(sockfd, "7", 1, MSG_OOB);    
    printf("wrote 1 bytes of OOB data\n");
    //sleep(1);

    write(sockfd, "89", 2);
    printf("wrote 2 bytes of normal data\n");
    //sleep(1);

    exit(0);    	
}

