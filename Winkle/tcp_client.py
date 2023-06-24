###############################################################>
import socket
import sys
import select # Dj Selecta!
###############################################################_
def main( argv ):
 ## get program arguments
    if len( argv ) < 3:
        print( f"Usg: { argv[0] } <host> <service>" )
        return
    host, port = argv[1:]
 ## get target address info for establishing tcp connection
    ai = socket.getaddrinfo( host, port,
                             type=socket.SOCK_STREAM )[0]
    ( family, type_, col, cannonball, sockaddr ) = ai
    print( "Remote address( " +
           ":".join( map( str, sockaddr )) + " )")
 ## create socket for the communication      proto___
    remote_socket = socket.socket( family, type_, col )
 ## make conectn
    remote_socket.connect( sockaddr )
 ## In Vindovs it's not possible to use socket for monitoring
 ## stdin, because all socets should originate from WinSock
 ## library. Use timeout as in the book, and msvcrt.kbhit().
    if sys.platform == "win32":
        import msvcrt
    sockets_to_monitor = [ remote_socket ]
    if sys.platform != "win32":
        sockets_to_monitor.append( sys.stdin )
    timeout = 1
    print( "To send msg, type somfng and hit Enter." )
    ##
    while True:
        ready = select.select( sockets_to_monitor,
                               (), (), timeout )[ 0 ]
        if remote_socket in ready:
            dat = remote_socket.recv( 1024 )
            print( dat.decode())
        else:
            if(( sys.platform == "win32" and msvcrt.kbhit()) or
               sys.stdin in ready ):
                msg = sys.stdin.readline()
                if msg:
                    remote_socket.send( msg.encode())
                else: # ^D 
                    print( "Bye" )
                    return
###############################################################`
if __name__ == '__main__':
    main( sys.argv )
###############################################################,
# I fink I've developed a skill to debug while sleeping:)
###############################################################=
###############################################################/
