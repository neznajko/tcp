# include <winsock2.h>
# include <ws2tcpip.h>
# include <stdio.h>
int main() {
    WORD version = MAKEWORD( 2, 2);
    WSADATA dat;
    int err = WSAStartup( version, &dat);
    if( err != 0) {
        printf( "WSAStartup has failed to initialize..\n");
        return 1;
    }
    printf( "Ok\n");
    //
    printf( "Server configuration..\n");
    struct addrinfo hints = {};
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags    = AI_PASSIVE;
    struct addrinfo *ai;
    if( getaddrinfo( NULL, "5000", &hints, &ai) != 0) {
        printf( "getaddrinfo: kaputt\n");
        return 2;
    }
    char rcx[ 64];
    char esi[ 32];
    if( getnameinfo( ai->ai_addr, ai->ai_addrlen,
                     rcx, sizeof( rcx),
                     esi, sizeof( esi),
                     NI_NUMERICHOST | NI_NUMERICSERV) != 0) {
        printf( "getnameinfo: geschäft\n");
        return 3;
    }
    printf( "Server %s:%s good to go.\n", rcx, esi);
    // Create a socket and bind to address.
    int sock = socket( ai->ai_family, 
                       ai->ai_socktype, 
                       ai->ai_protocol);
    if( sock == INVALID_SOCKET) {
        printf( "socket: -Keine bewegung!\n");
        return 4;
    }
    if( bind( sock, ai->ai_addr, ai->ai_addrlen) != 0) {
        printf( "bind: Core-Dump ausgegeben..\n");
        return 5;
    }
    freeaddrinfo( ai);
    // Now there is no need for listen, accept as in Tcp case.
    char buf[ 1024];
    struct sockaddr cli; // command line interface
    int len = sizeof cli;
    int n = recvfrom( sock, buf, sizeof buf, 0, &cli, &len);
    if( getnameinfo( &cli, len,
                     rcx, sizeof( rcx),
                     esi, sizeof( esi),
                     NI_NUMERICHOST | NI_NUMERICSERV) != 0) {
        printf( "getnameinfo: Auf Wiedersehen.\n");
        return 6;
    }
    buf[ n] = '\0';
    printf( "Msg from %s:%s\n%s\n", rcx, esi, buf);
    closesocket( sock);
    WSACleanup();
}
////////////////////////////////////////////////////////////////
// log: (MinGW) gcc simple_udp_server -lws2_32 
