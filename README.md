# Socket programming homework
## Winkle
#### listing.c, List Network Interfaces
```C++
+----------------+-----------------+
| <ifaddr.h>     | <bits/socket.h> |
+----------------+-----------------+
| struct ifaddrs | struct sockaddr |
| getifaddrs()   |                 |
| freeifaddr()   |                 |
+----------------+----+------------+--+
| <netinet/in.h>      | <netdb.h>     |
+---------------------+---------------+
| struct sockaddr_in  | getnameinfo() |
| struct sockaddr_in6 |               |
+---------------------+---------------+
Usage: gcc -pedantic listing.c && ./a.out
```
