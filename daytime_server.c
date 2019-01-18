#include "inet.h"

#include <time.h>

#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int fd = __socket(AF_INET, SOCK_STREAM, 0);
    addr_t *addr = cons(IPPORT_DAYTIME);
    int confd; // connecion file descriptor
    time_t t;
    char msg[BUFSIZ] = {0};

    __bind(fd, addr, INET_ADDRSTRLEN); // bind to all local interfaces
    __listen(fd, SOMAXCONN);
    dumpsock("listen", fd, getsockname);
    for (;;) {
    	confd = __accept(fd, NULL, NULL);
	dumpsock("accept   local", confd, getsockname);
	dumpsock("accept foregin", confd, getpeername);
	t = time(NULL);
	sprintf(msg, "\n%s", ctime(&t));
	write(confd, msg, BUFSIZ);
    	__close(confd);
    }
    __close(fd);
    free(addr);
    return 0;
}
