#!                                          /usr/bin/env python3
##,#########################################-------<<-----------
from socket import socket, SOCK_DGRAM
#######======#####################_#########]]]]]] ]]]]]]]]]]]](
# Get local address, by making DNS request.
s = socket( type=SOCK_DGRAM )
s.connect(( "8.8.8.8", 53 ))
host = s.getsockname()[ 0 ] # what.e.v.a
s.close()
# Chose server's port number.
pot = 5000 # port five thousand
# Create the server
s = socket()
s.bind(( host, pot )) 
s.listen()
print( "Server", host, "listening on port", pot, ".." )
conn, addr = s.accept() # what is conn?
# accept(self)
# accept() -> (socket object, address info)
# Wait for an incoming connection.  Return a new socket
# representing the connection, and the address of the client.
# For IP sockets, the address info is a pair (hostaddr, port).
print( addr, " has just connected!" )
print( "Client: ", conn.recv( 64 ))
conn.send( b"Thanks for the connection." )
conn.close()
################################################################
