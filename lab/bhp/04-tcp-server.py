################################################################
################################################################
################################################################
################################################################
##############
##############  時時
##############  Manchmal                                       # u
##############  parfois                                        # f
##############  a veces                                        # g
##############  іноді                                          # e
##############  A volte                                        # s
##############  sometimes                                      # i
##############  ときどき( to ki do ki )
##############
################################################################
################################################################
################################################################
################################################################
import socket
################################################################
import threading
################################################################
#                                                  p a r f o i s
################################################################
def response( sock ):
################################################################
    req = sock.recv( BUFSIZ ) # manchmal
################################################################
    sock.send( f"You send thus: { req.decode() }".encode())
################################################################
    sock.close() #                                         іноді
################################################################
server = socket.socket( socket.AF_INET, socket.SOCK_STREAM )
################################################################
server_addr = '', 3000 #                                 a veces
################################################################
server.bind( server_addr ) #                            manchmal
################################################################
BACKLOG = 10 #                                             іноді
################################################################
BUFSIZ = 4096 #                                          a volte
################################################################
server.listen( BACKLOG ) #                               parfois
################################################################
print( f"Server listening on port { server_addr[ 1 ]}" )
################################################################
while True: #                                            a veces
################################################################
    client, addr = server.accept() #                     a volte
################################################################
    print( f"Client {addr} has just connected" ) #      manchmal
################################################################
    job = threading.Thread( target=response, args=( client, ))
################################################################
    job.start()
################################################################
# log:     Ok before 20 years or something I found peace and was
#  very happy and excited I quit everything and started learning
# how to draw I have found a pattern that can explain everything
# I didn't gain anything than hate envy humiliation scorn and so
# on, do you think I would risk everything if I had copy someone
#   First time all.ora was written in Bulgarian I didn't plan to
#     write it in English, do you think i would make postures in
#   front of the local audience if I was copying from some local
# hero that would be pretty stupid don't you think, all I wanted
#          was to become a great painter, etc. Now I'm here gave
#      everything that I had and everyone want to gain something
#  from you for his her self I didn't get at least one good word
#     what I have tvs newspapers power friends money what I have
#   nothing. This is my last log i've started logging to respond
#   to the hate to not become crazy it's meaningless I quit from
# this Society I refuse to watch TV and read news, everything is
#                                                          false
