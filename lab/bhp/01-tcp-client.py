################################################################
import socket
################################################################
target_host = "example.com"
target_port = 80
################################################################
ipv4_tcp = socket.AF_INET, socket.SOCK_STREAM
client = socket.socket( *ipv4_tcp )
################################################################
target_server = target_host, target_port
################################################################
client.connect( target_server )
################################################################
get_request = f"GET / HTTP/1.1\r\n" \
              f"Host: { target_host }\r\n\r\n"               
################################################################
client.send( get_request.encode())
################################################################
responce = client.recv( 4096 )
################################################################
print( responce.decode())
################################################################
client.close()
################################################################
#        する( su ru )
#        #######################################################
#   fare i         いい( i i )
#  hacer s         #############################################
#  faire f     Gut g          うち( u chi )
#    Tun g   добре u          ##################################  
# робити u    bien f   maison f        いつ( i tsu )
#     do e    buen s     Haus g        #########################
#        #    bene i     casa s        f                   quand
#        #    good e   додому u        g                    Wann
#        #         #     casa i        i                  quando
#        #         #     home e        u                    коли
#        #         #          #        s                  cuando
#        #         #          #        e                    when
################################################################
# log: