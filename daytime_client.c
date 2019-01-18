#include "inet.h"

#include <arpa/inet.h>

#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int fd = __socket(AF_INET, SOCK_STREAM, 0);
    char *ip = (argc > 1)? argv[1]: "127.0.0.1";
    addr_t *addr = cons(IPPORT_DAYTIME);
    char msg[BUFSIZ];

    inet_aton(ip, &addr->sin_addr); // ascii to network

    __connect(fd, addr, INET_ADDRSTRLEN);
    dumpsock("connect   local", fd, getsockname);
    dumpsock("connect foreign", fd, getpeername);
    read(fd, msg, BUFSIZ);
    puts(msg);

    __close(fd);
    free(addr);
    return 0;
}
