#include "inet.h"

#include <arpa/inet.h>

#include <stdio.h>
#include <unistd.h>

int __socket(int domain, int type, int protocol)
{
    int fd = socket(domain, type, protocol);
    
    if (fd == -1) __err(socket);
    
    return fd;
}
// port is in host format
// s_addr is set to INADDR_ANY
addr_t *cons(in_port_t port)
{
    addr_t *addr = calloc(1, INET_ADDRSTRLEN);
    
    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);

    return addr;
}
void __bind(int fd, void *addr, socklen_t len)
{
    if (bind(fd, (__CONST_SOCKADDR_ARG) addr, len) == -1)
	__err(bind);
}
void __listen(int fd, int n)
{
    if (listen(fd, n) == -1) __err(listen);
}
int __accept (int fd, void *addr, socklen_t *len)
{
    int confd = accept(fd, (__SOCKADDR_ARG) addr, len);

    if (confd == -1) __err(accept);

    return confd;
}
void __close(int fd)
{
    if (close(fd) == -1) __err(close);
}
void __connect(int fd, void *addr, socklen_t len)
{
    if (connect(fd, (__CONST_SOCKADDR_ARG) addr, len) == -1)
	__err(connect);
}
void __dumpsock(char *str, int fd, getname_t getname)
{
    struct sockaddr buf = {0};
    socklen_t len = sizeof buf;
    
    if ((getname(fd, &buf, &len)) == -1) __err(__dumpsock);
    switch (buf.sa_family)
    {
	case AF_INET: {
	    addr_t *addr = (addr_t *) &buf;
	    printf("%s: AF_INET %5i %s\n", str,
		   ntohs(addr->sin_port),
		   inet_ntoa(addr->sin_addr));
	    break;
	}
	default: break;
    }
}
