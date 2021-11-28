                     ////
////////////////////////////////////////////////////////
# include <stdio.h>  // puts!, ..
# include <stdlib.h> // calloc, free
# include <time.h>   // time, ctime
# include <string.h> // strlen
# include "socket.h" // sockets common stuff
////////////////////////////////////////////////////////
# define BACKLOG 10 // Maximum pending connections
# define err( MSG) {                            \
        puts( MSG);                             \
        goto exit;                              \
    }
int main() {
    // server config
    struct addrinfo *hints = calloc( 1, sizeof *hints);
    hints->ai_flags = AI_PASSIVE;     // server
    hints->ai_family = AF_INET;       // 4
    hints->ai_socktype = SOCK_STREAM; // tcp
    struct addrinfo *addr = NULL;
////////////////////////////////////////////////////////
    // I thought this is the loopback interface but its'
    // the private network address 192.168.what.ever, so
    // make sure port number isn't forbidden by firewall.
    char port[] = "8080"; 
    getaddrinfo( NULL, port, hints, &addr); 
    // socket cons
    int fd = socket( addr->ai_family,
                     addr->ai_socktype,
                     addr->ai_protocol);
    // bind fd to address
    if( bind( fd, addr->ai_addr, addr->ai_addrlen)) {
        err( "Bind: o_x");
    }
    // mark fd as passive socket
    if( listen( fd, BACKLOG)) {
        err( "Listen: ._O");
    }
    // accept incoming connection
    struct sockaddr_storage client_addr;
    socklen_t client_len = sizeof client_addr;
    int client_fd = accept( fd,
/* s o m e e m p t y     */ (struct sockaddr *) &client_addr,
/*             s p a c e */ &client_len);
    if( client_fd < 0) err( "kaboom");
    // success[ cykcec ]
    char buf[ BUFSIZ];
    getnameinfo(( struct sockaddr *) &client_addr,
                client_len, buf, BUFSIZ, 0, 0,
                NI_NUMERICHOST);
    puts( buf);
    // request can be made with curl or browser like
    // curl 192.168.your.host:8080
    printf( "%ld bytes received\n", 
            recv( client_fd, buf, BUFSIZ, 0));
    puts( buf);
    // response( Thats first time I see \r\n in code.)
    const char res[] =
        "HTTP/1.1 200 OK\r\n"
        "Connection: close\r\n"
        "Content-Type: text/plain\r\n\r\n"
        "Local time is: ";
    send( client_fd, res, strlen( res), 0);
    time_t tloc = time( NULL);
    char *times = ctime( &tloc);
    send( client_fd, times, strlen( times), 0);
  exit:
    freeaddrinfo( addr);
    free( hints);
    close( fd);
    close( client_fd); /* ifopen */
}
////////////////////////////////////////////////////////
// log: - BpeMeBu ctpBep!
// <bits/socket.h> - struct sockaddr_storage
////////////////////////////////////////////////////////
  ////                                    ////
