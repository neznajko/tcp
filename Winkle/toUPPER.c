                              ////
////////////////////////////////////////////////////////////////
# include <sys/types.h> //                                     L
# include <sys/socket.h> //                                    O
# include <netdb.h> //                                         T
# include <sys/select.h> //                                    O
# include <unistd.h> //                                        F
# include <ctype.h> //                                         S
# include <stdio.h> //                                         T
# include <stdlib.h> //                                        O
# include <err.h> //                                           F
////////////////////////////////////////////////////////////////
# define SUCCESS 0 // cykcec
# define FAILURE 1
# define boom( MSG) err( FAILURE, MSG)
# define BACKLOG 8 // listen waiting queue max length
////////////////////////////////////////////////////////////////
struct addrinfo *cons_ai( const char srvc[]) {
    struct addrinfo *ai = NULL; // artificial intelligence
    // Setting up wildcard server address for incoming requests.
    // - Wont chck calloc for errors, this time.
    struct addrinfo *hints = calloc( 1, sizeof *hints);
    hints->ai_flags    = AI_PASSIVE; // ctpBep
    hints->ai_family   = AF_INET;
    hints->ai_socktype = SOCK_STREAM;
    if( getaddrinfo( NULL, srvc, hints, &ai) != SUCCESS) {
        boom( "o-o");
    }
    free( hints);
    return ai;
}
////////////////////////////////////////////////////////////////
# define NODELEN 64 // getnameinfo stff
# define SRVCLEN 32
////////////////////////////////////////////////////////////////
int cons_pass( struct addrinfo *ai) {
    // Creating listening( passive) socket.
    int sockfd = socket( ai->ai_family,
                         ai->ai_socktype,
                         ai->ai_protocol);
    if( sockfd == -1) boom( "x_o");
    // Bind socket fd and socket address.
    if( bind( sockfd, ai->ai_addr, ai->ai_addrlen) != SUCCESS) {
        boom( ":o");
    }
    // Mark socket as passive.
    if( listen( sockfd, BACKLOG) != SUCCESS) boom( "boom");
    // Dmp somfng.
    char node[ NODELEN]; // 123.45.6.789
    char srvc[ SRVCLEN]; // http
    if( getnameinfo( ai->ai_addr, ai->ai_addrlen,
                     node, sizeof node,
                     srvc, sizeof srvc,
                     NI_NUMERICHOST) != SUCCESS) {
        boom( "._.");
    }
    printf( "Socket %d at %s listening on %s for requests.\n",
            sockfd, node, srvc);
    return sockfd;
}
////////////////////////////////////////////////////////////////
int main() {
    printf( "Starting d'toUPPER server..\n");
    struct addrinfo *ai = cons_ai( "5000");
    int kibitzing = cons_pass( ai); // listening socket
    freeaddrinfo( ai);
    // dj selecta
    fd_set fd_read; // master class
    FD_ZERO( &fd_read);
    FD_SET( kibitzing, &fd_read);
    int nfds = kibitzing + 1; // upper bound
    char buf[ BUFSIZ];
    for( ;;) { /////////////////////////////////////////////////
        fd_set copy = fd_read; // backup, I said --b-a-c-k-u-p--
        // DJ SELECTA is modifying fd_set's at once zo make sure
        // to call Copy Ninja Kakashi.
        if( select( nfds, &copy, NULL, NULL, NULL) == -1) {
            boom( "3:(");
        }
        // - Mr. Holmes what shell we do?
        // - Upperfy at once!
        for( int fd = kibitzing + 1; fd < nfds; fd++) {
            if( FD_ISSET( fd, &copy)) {
                // Now read, convyort nd send, f peer has closed
                // the connection close socket and rm from d'set
                int n = read( fd, buf, BUFSIZ);
                if( n < 1) { // peer has closed the connection
                    printf( "Bye:)\n");
                    FD_CLR( fd, &fd_read);
                    close( fd);
                } else {
                    printf( "IEEAH\n");
                    buf[ n] = '\0';
                    char *c = buf - 1;
                    while( *++c) *c = toupper( *c);
                    write( fd, buf, c - buf);
                }
            }
        }
        // Chck for new clients willing to UPPER THEIR MESSAGES.
        if( FD_ISSET( kibitzing, &copy)) {
            printf( "A Client!\n");
            // thats the same as in time_server.c
            struct sockaddr_storage client;
            socklen_t socklen = sizeof client;
            int fd = accept( kibitzing,
                             ( struct sockaddr *) &client,
                             &socklen);
            if( fd == -1) boom( "kaboom");
            getnameinfo(( struct sockaddr *) &client,
                        socklen, buf, BUFSIZ, 0, 0,
                        NI_NUMERICHOST);
            printf( "That is %s here.\n", buf);
            // Opdate fd_set and stoff
            if( fd >= nfds) nfds = fd + 1;
            FD_SET( fd, &fd_read);
        }
    }
}
////////////////////////////////////////////////////////////////
// log: https://youtu.be/cKuEMD275Mg?t=2698
////////////////////////////////////////////////////////////////
////                                                        ////
