////////////////////////////////////////////////////////////////
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <stdio.h>
////////////////////////////////////////////////////////////////
typedef struct addrinfo ai_t;
////////////////////////////////////////////////////////////////
int main( int argc, char *argv[]) {
    if( argc < 3) {
        printf( "usage: %s hOSt SERViSE\n", argv[ 0]);
        return 1;
    }
    char *host = argv[ 1];
    char *service = argv[ 2];
    ai_t *res;
    // Let's try with NULL hints
    if( getaddrinfo( host, service, NULL, &res) != 0) {
        printf( "Error getting addrinfo: o_x\n");
        return 2;
    }
    for( ai_t *ai = res; ai != NULL; ai = ai->ai_next) {
        char r8[ 64];
        if( getnameinfo( ai->ai_addr, ai->ai_addrlen,
                         r8, sizeof( r8), NULL, 0,
                         NI_NUMERICHOST) != 0) {
            printf( "x_o\n");
            return 4;
        }
        printf( "%s\n", r8);
    }
    freeaddrinfo( res);
}
////////////////////////////////////////////////////////////////
// log: ./a.out yahoo.com ftp
// 74.6.143.25
// 74.6.143.26
// 98.137.11.164
// 98.137.11.163
// 74.6.231.21
// 74.6.231.20
// 2001:4998:24:120d::1:1
// 2001:4998:124:1507::f000
// 2001:4998:44:3507::8001
// 2001:4998:44:3507::8000
// 2001:4998:124:1507::f001
// 2001:4998:24:120d::1:0
////////////////////////////////////////////////////////////////
