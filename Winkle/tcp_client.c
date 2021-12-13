////
////////////////////////////////////////////////////////
# include "socket.h"
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <err.h>
# define ____( MSG) puts( MSG)
# define p__s( MSG) ____( MSG)
# define _u__( MSG) p__s( MSG)
# define __ts( MSG) _u__( MSG)
# define _err( MSG) err( EXIT_FAILURE, MSG)
void usg( char* prg) {
    printf( "usage: %s host service( port)\n", prg);
    printf( "e.g: %s example.com 80\n", prg);
    exit( EXIT_FAILURE);
}
# define MINARGS 3
# define HOSTLEN 128
# define SERVLEN 32
////////////////////////////////////////////////////////
int main( int argc, char *argv[]) {
    if( argc < MINARGS) usg( *argv);
    // hostname could be numeric as well
    const char* hostname = argv[ 1];
    const char* port = argv[ 2]; // or service( http)
    ____( "Remote address configrtn ..");
    struct addrinfo hints;
    memset( &hints, 0, sizeof hints);
    hints.ai_socktype = SOCK_STREAM; // tcp
    struct addrinfo *addrinfo;
    if( getaddrinfo( hostname, port,
                     &hints, &addrinfo) != 0) {
        _err( "getaddrinfo");
    }
    char host[ HOSTLEN];
    char serv[ SERVLEN];
////////////////////////////////////////////////////////    
    if( getnameinfo( addrinfo->ai_addr,
                     addrinfo->ai_addrlen,
                     host, HOSTLEN, serv, SERVLEN,
                     NI_NUMERICHOST)) {
        _err( "getnameinfo");
    }
    printf( "host address: %s\n", host);
    printf( "service: %s\n", serv);
////////////////////////////////////////////////////////
    p__s( "Conecting..");
    int sockfd = socket( addrinfo->ai_family,
                         addrinfo->ai_socktype,
                         addrinfo->ai_protocol);
    if( sockfd == -1) _err( "socket");
    if( connect( sockfd,
                 addrinfo->ai_addr,
                 addrinfo->ai_addrlen) != 0) {
        _err( "..");
    }
    freeaddrinfo( addrinfo);
    puts( "'DONE");
////////////////////////////////////////////////////////
    _u__( "Type somfng and hit Enter");
    char buf[ BUFSIZ];
    fd_set fds;
    FD_ZERO( &fds);
    FD_SET( sockfd, &fds);
    FD_SET( STDIN_FILENO, &fds);
    for( ;;) {
        fd_set copy = fds;
        if( select( sockfd + 1, &copy, NULL, NULL,
                    NULL) < 0) {
            _err( "selecting");
        }
        if( FD_ISSET( sockfd, &copy)) {
            // There is incoming data from remote peer.
            int n = recv( sockfd, buf, BUFSIZ, 0);
            if( n < 1) {
                __ts( "Connection closed by peer.");
                break;
            }
            buf[ n] = '\0';
            printf( "No. bytes received: %d\n%s\n", n, buf);
        } else if( FD_ISSET( STDIN_FILENO, &copy)) {
            // stdin is active get input line
            // wats gong over here?
            if(! fgets( buf, BUFSIZ, stdin)) break;
            // fgets sets null byte
            if( send( sockfd, buf, strlen( buf), 0) == -1) {
                _err( ":p sending");
            }
        }
    }
    close( sockfd);
}
////////////////////////////////////////////////////////
// log: struct addrinfo
// {
//   int flags;         /* AI_PASSIVE,..              */
//   int family;        /* AF_INET,..                 */
//   int socktype;      /* SOCK_STREAM,..             */
//   int protocol;      /* 0                          */
//   socklen_t addrlen; /*                            */
//   sockaddr *addr;    /*                            */
//   char *canonname;   /* Should've named it ortodox */
//   addrinfo *next;    /*                            */
// };
////////////////////////////////////////////////////////
////                                                ////
