                      ////
////////////////////////////////////////////////
# include <sys/types.h>
# include <ifaddrs.h> // interface addresses
# include <netdb.h>
////////////////////////////////////////////////
# include <stdio.h>
////////////////////////////////////////////////
int main()
{
    struct ifaddrs *ifaddrs;
    if( getifaddrs( &ifaddrs) == -1) {
        puts( "getifaddrs: Boom");
        return 1;
    }
    for( struct ifaddrs *p = ifaddrs;
         p != NULL; p = p->ifa_next) {
        socklen_t len;
        int version;
        switch( p->ifa_addr->sa_family) {
            case AF_INET: // 2
            {
                len = sizeof( struct sockaddr_in);
                version = 4;
                break;
            }
            case AF_INET6: // 10
            {
                len = sizeof( struct sockaddr_in6);
                version = 6;
                break;
            }
            default: continue;
        }
        char host[ 64];
        getnameinfo( p->ifa_addr, len,
                     host, sizeof host,
                     0, 0, NI_NUMERICHOST);
        printf( "%-20s IPv%d %s\n",
                p->ifa_name, version, host);
    }
    freeifaddrs( ifaddrs);
}
////////////////////////////////////////////////
// log: gcc -pedantic listing.c && ./a.out
// +----------------+-----------------+
// | <ifaddr.h>     | <bits/socket.h> |
// +----------------+-----------------+
// | struct ifaddrs | struct sockaddr |
// | getifaddrs()   |                 |
// | freeifaddr()   |                 |
// +----------------+----+------------+--+
// | <netinet/in.h>      | <netdb.h>     |
// +---------------------+---------------+
// | struct sockaddr_in  | getnameinfo() |
// | struct sockaddr_in6 |               |
// +---------------------+---------------+
////////////////////////////////////////////////
    ////                                ////
