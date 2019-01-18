#ifndef INET_H
#define INET_H

#include <netinet/ip.h>

#include <err.h>
#include <stdlib.h>

#define __err(NAME) err(EXIT_FAILURE, "%s", #NAME)

typedef struct sockaddr_in addr_t;
typedef int (*getname_t)(int, struct sockaddr *, socklen_t *);

int __socket(int domain, int type, int protocol);
addr_t *cons(in_port_t port);
void __bind(int fd, void *addr, socklen_t len);
void __listen(int fd, int n);
int __accept (int fd, void *addr, socklen_t *len);
void __close(int fd);
void __connect(int fd, void *addr, socklen_t len);
void __dumpsock(char *str, int fd, getname_t getname);

#ifdef DEBUG
#define dumpsock(STR, FD, GETNAME) __dumpsock(STR, FD, GETNAME)
#else
#define dumpsock(STR, FD, GETNAME)
#endif

#endif // INET_H
