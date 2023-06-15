////////////////////////////////////////////////////////////////
# include <WinSock2.h>
# include <WS2tcpip.h>
////////////////////////////////////////////////////////////////
# pragma comment( lib, "ws2_32.lib" )
////////////////////////////////////////////////////////////////
# include <iostream>
# include <string>
////////////////////////////////////////////////////////////////
using std::endl;
////////////////////////////////////////////////////////////////
# define NOT_SUCCESS 3
///////////////////////////////////////////////////////////////=
void Leave( const char msg[]){
    std::cout << msg << endl;
    WSACleanup();
    exit( NOT_SUCCESS );
}
////////////////////////////////////////////////////////////////
void iNit(){
    WSADATA wsa_Data;
    if( !WSAStartup( MAKEWORD( 2, 0 ), &wsa_Data )){
        std::cout << "You have successfully installed Firefox!"
                  << endl;
    } else {
        Leave( "Muahaha.." ); 
    }
}
////////////////////////////////////////////////////////////////
addrinfo * GetAI( const char port[] ){
    addrinfo hints{};
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    addrinfo * ai{};
    if( getaddrinfo( nullptr, port, &hints, &ai ) != 0 ){
        Leave( "Poom" );
    }
    return ai;
 }
////////////////////////////////////////////////////////////////
SOCKET Socket( addrinfo * ai ){
    SOCKET listen_Socket{ socket( ai->ai_family,
                                  ai->ai_socktype, 
                                  ai->ai_protocol )};
    if( listen_Socket == INVALID_SOCKET ){
        Leave( "What is thiiis?" );
    }
    return listen_Socket;
}
////////////////////////////////////////////////////////////////
void Bind( SOCKET socket, addrinfo * ai ){
    if( bind( socket, ai->ai_addr, ( int )ai->ai_addrlen ) != 0 ){
        Leave( "-=~_ <- Gun" );
    }
    freeaddrinfo( ai );
}
////////////////////////////////////////////////////////////////
void Listen( SOCKET socket ){
    constexpr int BACKLOG{ 10 };
    if( listen( socket, BACKLOG ) < 0 ){
        Leave( "The Red Sockets" );
    }
    std::cout << "Listening..\n";
}
////////////////////////////////////////////////////////////////
SOCKET accept_Request( SOCKET socket, sockaddr * client ){
    int len{ sizeof *client };
    SOCKET sock{ accept( socket, client, &len )};
    if( sock == INVALID_SOCKET ){
        Leave( "Use Burp Suite Pro" );
    }
    return sock;
}
////////////////////////////////////////////////////////////////
void log( sockaddr * client ){
    printf( "Client is connected..., "
            "watch out for vulneralelalibilities!" );
    char bufr[ 64 ];
    getnameinfo( client, sizeof * client, 
                 bufr, sizeof( bufr ), 0, 0,
                 NI_NUMERICHOST );
    printf( " %s\n", bufr );
}
////////////////////////////////////////////////////////////////
std::string GetRequest( SOCKET sock ){
    char req[ 1024 ];
    recv( sock, req, sizeof req , 0 );
    return std::string( req );
}
////////////////////////////////////////////////////////////////
void Responce( SOCKET sock ){
    const char res[] =
        "HTTP/1.1 200 OK\r\n"
        "Connection: close\r\n"
        "Content-Type: text/plain\r\n\r\n"
        "Local time is: ";
    send( sock, res, ( int )strlen( res ), 0 );
    time_t timer;
    time( &timer );
    char time_msg[ 64 ]; 
    ctime_s( time_msg, sizeof time_msg, &timer );
    send( sock, time_msg, ( int )strlen( time_msg ), 0);
}
////////////////////////////////////////////////////////////////
int main(){
    iNit();
    //
    addrinfo * ai{ GetAI( "8000" )};
    SOCKET listen_Socket{ Socket( ai )};
    Bind( listen_Socket, ai );
    Listen( listen_Socket );
    sockaddr client;
    SOCKET client_Socket{ accept_Request( listen_Socket, &client )};
    log( &client );
    auto req{ GetRequest( client_Socket )};
    std::cout << req.length() << " bytes recieved.\n";
    Responce( client_Socket );
    //
    closesocket( client_Socket );
    WSACleanup();
}
////////////////////////////////////////////////////////////////
//
