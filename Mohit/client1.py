#!                                          /usr/bin/env python3
################################################################
import sys, socket
################################################################
host = sys.argv[ 1]
pot = int( sys.argv[ 2])
print( "Conec4ing to", host, pot )
s = socket.socket() # default tcp 4
s.connect(( host, pot ))
s.send( b"Hello Serv!" )
print( "Server: ", s.recv( 64 ))
s.close()
################################################################
