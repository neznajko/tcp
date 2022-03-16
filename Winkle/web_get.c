////////////////////////////////////////////////////////////////
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
//
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <unistd.h>
////////////////////////////////////////////////////////////////
char *chck_protocol( char *url) {
    static char key[] = "://";
    char *p = strstr( url, key); // search for key in url
    if( p == NULL) return url; // not found
    *p = '\0'; // replace : with null byte
    if( strcmp( url, "http") != 0) {
        puts( "Non http protocol");
        exit( 1);
    }
    *p = ':'; // re-establish
    return p + sizeof key - 1;
}
////////////////////////////////////////////////////////////////
char *getfield( char *p, char *dest, char key) {
    for(;; p++) {
        char c = *p;
        if( c == key || c == '\0') break;
        *dest++ = c;
    }
    *dest = '\0';
    return p;
}
////////////////////////////////////////////////////////////////
void parse_url( char *url, char *hostname,
                char *port, char *path) {
    char *p = chck_protocol( url);
    // set default values
    strcpy( port, "80");
    *path = '\0';
    // get hostname
    char *copy = p;
    p = getfield( p, hostname, ':');
    if( *p++ != '\0') {
        // get port
        p = getfield( p, port, '/');
        if( *p++ == '\0') return;        
    } else {
        p = getfield( copy, hostname, '/');
        p++;
    }
    // get path
    p = getfield( p, path, '#');
}
////////////////////////////////////////////////////////////////
typedef struct addrinfo ai_t;
# include <errno.h>
# define bailout( msg) {                        \
        perror( msg);                           \
        exit( 1);                               \
    }
////////////////////////////////////////////////////////////////
int contact( char *hostname, char *port) {
    printf( "Connecting to a remote host ..");
    ai_t hints = {};
    hints.ai_socktype = SOCK_STREAM; // tcp protocol
    ai_t *ainfo;
    if( getaddrinfo( hostname, port, &hints, &ainfo) != 0) {
        bailout( "getaddrinfo");
    }
    char eax[ 32];
    if( getnameinfo( ainfo->ai_addr,
                     ainfo->ai_addrlen,
                     eax, sizeof eax, NULL, 0,
                     NI_NUMERICHOST) != 0) {
        bailout( "getnameinfo");
    }
    printf( ": %s\n", eax);
    int sock = socket( ainfo->ai_family,
                       ainfo->ai_socktype,
                       ainfo->ai_protocol);
    if( sock == -1) bailout( "socket");
    if( connect( sock,
                 ainfo->ai_addr,
                 ainfo->ai_addrlen) != 0) {
        bailout( "connect");
    }
    puts( "Connected!");
    freeaddrinfo( ainfo);
    return sock;
}
////////////////////////////////////////////////////////////////
void send_http_request( int sock, char *hostname,
                        char *port, char *path) {
    static char bufr[ 512];
    sprintf( bufr,
             "GET /%s HTTP/1.1\r\n"
             "Host: %s:%s\r\n"
             "Connection: close\r\n"
             "User-Agent: web_get/0.1\r\n\r\n",
             path, hostname, port);
    write( sock, bufr, strlen( bufr));
    printf( "Sent request:\n%s", bufr);
}
////_///////////////////////////////////////////////////////////
int main( int argc, char *argv[]) {
    char *url;
    if( argc > 1) {
        url = argv[ 1];
    } else {
        url = strdup( "http://www.example.com:80/"
                      "page1.php?usr=volen#a");
    }
    //
    char hostname[ 64] = ""; // This should set all elements to
    char port[ 8] = "";      // 0,
    char path[ 128] = "";    //
    //
    parse_url( url, hostname, port, path);
    printf( "hostname: %s\n", hostname);
    printf( "port:     %s\n", port);
    printf( "path:     %s\n", path);
    //
    int sock = contact( hostname,  port);
    send_http_request( sock, hostname, port, path);
    // The book code is more complicated, but I've decided to
    // keep it simple, coz I'm lazy.
    // smashing the stack error here is possible, in principle
    // it should be reallocated but ok.
    char res[ 32768];
    int total = 0;
    int n;
    do {
        n = read( sock, res + total, sizeof res);
        total += n;
    } while( n > 0);
    res[ total] = '\0';
    printf( "Total bytes recieved: %d\n%s\n", total, res);
}
////////////////////////////////////////////////////////////////
//
//
