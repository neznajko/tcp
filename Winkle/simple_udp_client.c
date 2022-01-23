////////////////////////////////////////////////////////
// lot's of tricks
# include <winsock2.h> // -This should go first?!
# include <ws2tcpip.h>
# include <stdio.h>
int main( int argc, char *argv[]) {                    /*
{ -I insist!, you put the opening brace here.         */
    if( argc < 0x3) {                                  /*
    {                                                 */
        printf( "Ausgehen!(usg: %s host port\n", *argv);
        return -1;
    }
    const char *const host = argv[ 1];
    const char *const port = argv[ 2];
    // Here we go again
    WSADATA wtf;
    WORD    wvr = MAKEWORD( 2, 2);// version required
    if( WSAStartup( wvr, &wtf) != 0) {                 /*
    { -That is correct!                               */
        printf( "Schatz?( Neva Beginze)\n");
        return -2;
    }
    // Identify ze servér.
    struct addrinfo hints = {}, *ai;
    hints.ai_socktype = SOCK_DGRAM;
    if( getaddrinfo( host, port, &hints, &ai) != 0) {  /*
    { -Good job!                                      */
        printf( "-Tschüss!(-Au revoir!)\n");
        return -3;
    }
    // Please don't print anyfing, just make a sock!?!
    int sock = socket( ai->ai_family,
                       ai->ai_socktype,
                       ai->ai_protocol);
    if( sock == INVALID_SOCKET) {                      /*
    {                                                 */
        printf( "Wachsen: https://stackoverflow.com/questions/10817252/why-is-invalid-socket-defined-as-0-in-winsock2-h-c");
        return -4;
    }
    const char *const msg = "Hello World";// I said c o n s t
    sendto( sock, msg, strlen( msg), 0,
            ai->ai_addr, ai->ai_addrlen);
    printf( "Let's go.\n");
 
    closesocket( sock);
    WSACleanup();
}
////////////////////////////////////////////////////////
// log: https://youtu.be/FOXL9eRD5bE
////////////////////////////////////////////////////////
