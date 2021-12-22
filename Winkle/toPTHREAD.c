                              ////
////////////////////////////////////////////////////////////////
// [ This] is toUPPER var using threads for each request instead
// of forking the process. It shouldn't be vry difficult 'guess.
# include <pthread.h>
# include <unistd.h>
////////////////////////////////////////////////////////////////
# include <stdio.h>
# include <err.h>
# include <ctype.h>
////////////////////////////////////////////////////////////////
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
////////////////////////////////////////////////////////////////
char eax[ 32]; // CPU registers
char rsi[ 64]; // very dangerous stf
////////////////////////////////////////////////////////////////
# define BACKLOG 8
////////////////////////////////////////////////////////////////
int tcp_server( char *port) {
    struct addrinfo spoilers = {}; // initlze all fields to zero
    spoilers.ai_flags = AI_PASSIVE;     // server
    spoilers.ai_socktype = SOCK_STREAM; // tcp
    spoilers.ai_family = AF_INET;       // 4
    struct addrinfo *res;
    // getaddrinfo( no error chckings)
    getaddrinfo( NULL, port, &spoilers, &res);
    // socket
    int fd = socket( res->ai_family,
                     res->ai_socktype,
                     res->ai_protocol);
    // dmp som stf
    getnameinfo( res->ai_addr, res->ai_addrlen,
                 rsi, sizeof rsi,
                 eax, sizeof eax,
                 NI_NUMERICHOST);
    printf( "Host:    %s\n", rsi);
    printf( "Service: %s\n", eax);
    printf( "Socket:  %d\n", fd);
    // bind
    bind( fd, res->ai_addr, res->ai_addrlen);
    // listen
    listen( fd, BACKLOG);
    // let's go
    printf( "Awaiting for requests..\n");
    freeaddrinfo( res);
    return fd;
}
////////////////////////////////////////////////////////////////
void request( void *arg) {
    int fd = *((int *) arg);
    char buf[ BUFSIZ];
    int n;
    while(( n = read( fd, buf, BUFSIZ)) > 0) {
        buf[ n] = '\0';
        for( int j = 0; j < n; j++) {
            buf[ j] = toupper( buf[ j]);
        }
        write( fd, buf, n);
    }
    printf( "Socket %d has closed connection.\n", fd);
    close( fd);
    pthread_exit( NULL);
}
////////////////////////////////////////////////////////////////
int main() {
    int pas = tcp_server( "5000");
    pthread_t thread;
    int fd;
    for( ;;) {
        // accept requests( blocking)
        struct sockaddr_storage client;
        socklen_t len = sizeof client;
        fd = accept( pas, (struct sockaddr *) &client, &len);
        if( fd == -1) err( 1, "wtf?");
        getnameinfo( (struct sockaddr *) &client, len,
                     rsi, sizeof rsi,
                     NULL, 0, NI_NUMERICHOST);
        printf( "Socket %d from %s has connected.\n", fd, rsi);
        pthread_create( &thread, NULL,
                        (void *) request,
                        (void *) &fd);
    }
}
////////////////////////////////////////////////////////////////
// log: gcc toPTHREAD.c -pthread, Then strt d'server wit ./a.out
// and make request in another terminal with netcat for example:
// let say the server is runing on a local network with wildcard
// address 192.168.x.y, than type ~­-=[ nc 192.168.x.y 5000]=-­~
////////////////////////////////////////////////////////////////
////                                                        ////
