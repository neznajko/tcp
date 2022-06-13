////////////////////////////////////////////////////////////////
# include <map>          ///////////////////////////////////////
# include <string>       ///////////////////////////////////////
# include <iostream>     ///////////////////////////////////////
# include <sys/types.h>  ///////////////////////////////////////
# include <sys/socket.h> ///////////////////////////////////////
# include <netdb.h>      ///////////////////////////////////////
# include <sys/select.h> /////////////////////////////////////__
# include <cstring>      ///////////////////////////////////////
# include <unistd.h>     ///////////////////////////////////////
////////////////////////////////////////////////////////////////
# define endl "\n"
////////////////////////////////////////////////////////////////
// To run the program compile with g++ webserver.cc, than start
// the server with ./a.out, open a browser and type in the
// address line 192.168.your.address:5000 or 127.0.0.1:5000.
std::string get_file_extension( const std::string& file_path ){
    const auto j = file_path.rfind( '.' );
    // Here if there is no dot ::j will be size_t( -1 ), zo
    // j + 1 will equal to nil.
    return file_path.substr( j + 1 );
}
////////////////////////////////////////////////////////////////
std::string get_content_type( const std::string& file_path ){
    static std::map<std::string, std::string> content_type = {
        { "css",  "text/css"               },
        { "csv",  "text/csv"               },
        { "gif",  "image/gif"              },
        { "htm",  "text/html"              },
        { "html", "text/html"              },
        { "ico",  "image/x-icon"           },
        { "jpeg", "image/jpeg"             },
        { "jpg",  "image/jpeg"             },
        { "js",   "application/javascript" },
        { "json", "application/json"       },
        { "png",  "image/png"              },
        { "pdf",  "application/pdf"        },
        { "svg",  "image/svg+xml"          },
        { "txt",  "text/plain"             },
    };
    const std::string key = get_file_extension( file_path );
    if( content_type.count( key ) == 0 ){ // unknown file type
        return "application/octet-stream";
    }
    return content_type[ key ];
}
////////////////////////////////////////////////////////////////
/////////////////////////_______________________________________
////////////////////////////////////////////////////////////////
// logique: (node,port)->getaddrinfo()->socket()
//                                   \    /
//                                    bind()->listen()
int create_server( char* node, char* port ){
    // A) getaddrinfo
    struct addrinfo hints = {}, *res;
    hints.ai_flags    = AI_PASSIVE;
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if( 0 != getaddrinfo( node, port, &hints, &res )){
        std::cerr << "getaddrinfo: 終了した" << endl;
        exit( 1 );
    }
    // B) socket
    int sock = socket( res->ai_family,
                       res->ai_socktype,
                       res->ai_protocol );
    if( sock < 0 ){
        std::cerr << "socket: 動作しない" << endl;
        exit( 1 );
    }
    // C) bind
    if( -1 == bind( sock, res->ai_addr, res->ai_addrlen )){
        std::cerr << "bind: 閉まっている" << endl;
        exit( 1 );
    }
    // D) listen
    if( -1 == listen( sock, /* backlog= */ 10 )){
        std::cerr << "listen: 失敗した" << endl;
        exit( 1 );
    }
    std::cout << "Server on port " << port
              << " is listening for incoming connections.."
              << endl;
    freeaddrinfo( res );
    return sock;
}
////////////////////////////////////////////////////////////////
const int MAX_REQUEST_LEN = 2048;
typedef struct sockaddr SA;
typedef struct sockaddr_storage SAS;
struct Client {
    static Client ahead; // proper linked list head
    static Client* cons();
    static Client* drop( Client* cli );
    static Client* front() {
        return ahead.next;
    }
    int         socket;
    char        buf[ MAX_REQUEST_LEN ];
    char*       bufptr;
    SAS         addr;
    socklen_t   addrlen;
    std::string name; // numerical name, e.g.: 192.168.1.4
    Client*     next;
    //
    SA* getaddr() const {
        return (SA*)&addr;
    }
    int bufsiz() const {
        return bufptr - buf;
    }
    int read();
}; 
Client Client::ahead = {};
Client* Client::cons() {
    Client* cli = (Client *)calloc( 1, sizeof *cli );
    // This should be initialized, becoz of the accept(2) call.
    cli->bufptr = cli->buf;
    cli->addrlen = sizeof( SAS );
    cli->next = Client::ahead.next;
    return Client::ahead.next = cli;
}
Client* Client::drop( Client* cli ){
    // loop over SLL srch for cli unlink and free memory.
    // Something like veni vidi vici, something like that.
    for( auto p = &Client::ahead; p->next; p = p->next ){
        if( p->next == cli ){
            p->next = cli->next;
            cli->next = nullptr;
            close( cli->socket );
            free( cli );
            return p;
        }
    }
    return nullptr;
}
int Client::read() {
    int count = MAX_REQUEST_LEN - bufsiz();
    int n = ::read( socket, bufptr, count );
    if( n > 0 ){
        bufptr += n;
        *bufptr = '\0';
    }
    return n;
}
int server; // Make server's socket global as well,
///////___//////////////////////////////////////////////////////
void waiting_line( fd_set* rfds ){
// int select(int nfds, fd_set *readfds, fd_set *writefds,
//            fd_set *exceptfds, struct timeval *timeout);
// void FD_SET(int fd, fd_set *set);
// void FD_ZERO(fd_set *set);
    FD_ZERO( rfds ); // clear
    FD_SET( server, rfds );
    int m = server; // maximum file descriptor
    for( auto cli = Client::front(); cli; cli = cli->next ){
        FD_SET( cli->socket, rfds );
        if( cli->socket > m ) m = cli->socket;
    }
    if( -1 == select( m + 1, rfds, NULL, NULL, NULL )){
        std::cerr << "select: 失った" << endl;
        exit( 1 );
    }
}
////////////////////////////////////////////////////////////////
void get_client_name( Client* cli ){
    static char r12[ 64 ];
    getnameinfo( cli->getaddr(),
                 cli->addrlen,
                 r12, sizeof r12,
                 NULL, 0,
                 NI_NUMERICHOST );
    cli->name.append( r12 );
}
////////////////////////_///////////////////////////////////////
void send_400( Client *cli ){
    static const char *res =
        "HTTP/1.1 400 Bad Request\r\n"
        "Connection: close\r\n"
        "Content-Length: 11\r\n\r\nBad Reqvest";
    static const int len = strlen( res );
    write( cli->socket, res, len );
}
////////////////////////////////////////////////////////////////
const char ENDREQ[] = "\r\n\r\n";
std::string getResourcePath( char* buf ){
    for( char* p = buf; *p; p++ ){
        if( *p == ' ' ){
            *p = '\0';
            break;
        }
    }
    return buf;
}
void response( Client* cli ){
    // Serve only GET requests
    static const char GET[] = "GET /";
    static const int LEN = strlen( GET );
    static const int BSIZE = 1024;
    int n = strncmp( cli->buf, GET, LEN );
    if( n != 0 ){
        return send_400( cli );
    }
    auto path = getResourcePath( cli->buf + LEN );
    if( path == "" ) path = "home.html";
    path = "./" + path;
    FILE* fp = fopen( path.c_str(), "rb" );
    if( !fp )return send_400( cli );
    fseek( fp, 0L, SEEK_END );
    size_t cl = ftell( fp );
    rewind( fp );
    auto ct = get_content_type( path );
    char buf[ BSIZE ];
    sprintf( buf, "HTTP/1.1 200 OK\r\n" );
    send( cli->socket, buf, strlen( buf ), 0 );
    sprintf( buf, "Connection: close\r\n" );
    send( cli->socket, buf, strlen( buf ), 0 );
    sprintf( buf, "Content-Length: %lu\r\n", cl );
    send( cli->socket, buf, strlen( buf ), 0 );
    sprintf( buf, "Content-Type: %s\r\n", ct.c_str());
    send( cli->socket, buf, strlen( buf ), 0 );
    sprintf( buf, "\r\n" );
    send( cli->socket, buf, strlen( buf ), 0 );
    int r;
    while( r = fread( buf, 1, BSIZE, fp )){
        send( cli->socket, buf, r, 0 );
    }
    fclose( fp );
}
int main( int argc, char* argv[] ){
    char port[] = "5000";
    server = create_server( NULL, port );
    fd_set rfds; // read fd set
    while( true ){
        waiting_line( &rfds );
        if( FD_ISSET( server, &rfds )){
            // That means a new connection
            Client* cli = Client::cons();
            cli->socket = accept( server,
                                  cli->getaddr(),
                                  &(cli->addrlen));
            if( cli->socket == -1 ){
                std::cerr << "accept: yeeh" << endl;
                return 1;
            }
            get_client_name( cli );
            std::cout << "New Client: " << cli->name << endl;
        }
        // Loop over clients, chck for GET request.
        for( auto cli = Client::front(); cli; cli = cli->next ){
            if( FD_ISSET( cli->socket, &rfds )){
                // Check buffer, and drop clent, if full.
                if( cli->bufsiz() == MAX_REQUEST_LEN ){
                    send_400( cli );
                    cli = Client::drop( cli );
                    continue;
                }
                int n = cli->read();
                if( n < 1 ){
                    std::cerr << cli->name
                              << " has closed the connection."
                              << endl;
                    cli = Client::drop( cli );
                    continue;
                }
                // check vhether \r\n\r\n is present( full request )
                if( strstr( cli->buf, ENDREQ )){
                    response( cli );
                    cli = Client::drop( cli ); // non persistent
                    continue;
                }
            }
        }
    }
}                                                            
////////////////////////////////////////////////////////////____
// log: To Russians: - Read this all_1.ora at https://github.com
// /neznajko/gallery Don't build your identity over evil things.
// You are not for example a country in South Europe named North
// Macedonia, they can be nasty, because they are small. You are
// big and if you think the communism and the war in Ukraine are
// good, this is problem for the World.
// To the comedians with the printers: -Making good jokes is not
// so simple as printing money:)
