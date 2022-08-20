################################################################
import os
from datetime import datetime
################################################################
# 192.168.1.1
net = input( "Enter Network Address: ").split( '.' )
net.pop()
net = ".".join( net )
# 192.168.1
i = int( input( "Enter Starting Number: "))
k = int( input( "Enter Last Number: "))
ping = "ping -c 1 -w 1"
t1 = datetime.now()
print( "Scanning in Progress." )
for j in range( i , k + 1):
    ip = f"{net}.{j}"
    print( ip, end=' ' )
    com = f"{ping} {ip} >/dev/null"
    exit_code = os.system( com )
    status = ""
    if exit_code == 0: status = "Ok"
    print( status )
t2 = datetime.now()
total = t2 - t1
print( "Scanning complete in", total.seconds, "sec." )
################################################################
# log: - yea!!?
# $ python3 ping_sweep.py 
# Enter Network Address: 142.250.187.174 (google.com)
# Enter Starting Number: 170
# Enter Last Number: 180
# Scanning in Progress.
# 142.250.187.170 Ok
# 142.250.187.171 Ok
# 142.250.187.172 Ok
# 142.250.187.173 Ok
# 142.250.187.174 Ok
# 142.250.187.175 Ok
# 142.250.187.176 Ok
# 142.250.187.177 Ok
# 142.250.187.178 Ok
# 142.250.187.179 Ok
# 142.250.187.180 Ok
# Scanning complete in 0 sec.
# 
