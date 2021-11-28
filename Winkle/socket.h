                              ////
////////////////////////////////////////////////////////////////
# include <sys/types.h>
# include <sys/socket.h> // socket, bind, listen
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h> // getnameinfo, getaddrinfo
# include <unistd.h> // close
# include <errno.h>
////////////////////////////////////////////////////////////////
// Socket APIs
// - socket() Creates endpoint 4 comunication( file descriptor).
// - bind() Bind socket fd with socket addr.
// - listen() Marks fd as passive socket.
// - connect()
// - accept()
// - send() and recv()
// - sendto() and recvfrom()
// - close() Closes fd.
// - shutdown()
// - select()
// - getnameinfo() and getaddrinfo() 172.217.169.110 <> google.com
// - setsockopt()
// - fcntl()
////////////////////////////////////////////////////////////////
// log:
// TCP flow => getaddrinfo, socket =>
// Client:=> connect =>
// Server:=> bind => listen => accept =>
// => send, recv => close
////////////////////////////////////////////////////////////////
////                                                        ////
