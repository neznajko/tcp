################################################################
################################################################
################################################################
################################################################
import paramiko
################################################################
from dataclasses import dataclass, astuple
################################################################
@dataclass
class Server:
    host: str
    port: int = 22
################################################################
@dataclass
class Client:
    username: str
    password: str
################################################################
# Execute a command on a remote host via ssh
################################################################
def ssh_exec_command( server, client, cmd ):
    ssh = paramiko.SSHClient()
    ssh.set_missing_host_key_policy( paramiko.AutoAddPolicy())
    ssh.connect( *astuple( server ), *astuple( client ))
    stdin, stdout, stderr = ssh.exec_command( cmd )
    for line in stdout:
        print( line.strip())
    for line in stderr:
        print( line.strip())
################################################################
# drwx------ 2 demo users          0 Mar 31  2023 .
# drwx------ 2 demo users          0 Mar 31  2023 ..
# drwx------ 2 demo users          0 Mar 31  2023 pub
# -rw------- 1 demo users        379 Sep 19  2023 readme.txt
server = Server( "test.rebex.net" )
client = Client( "demo", "password" )
ssh_exec_command( server, client,  "ls -al" )
################################################################
# <> 2.1.32: Determine the f o r c e acting on the vertical wall
# from the s i d e of the wedge if a load of mass m is placed on
# it. The angle at the base o f the wedge is α.  The coefficient
# of friction between the load and the surface o f the w e d g e
# μ. There i s no friction b e t w e e n the floor and the wedge.
####                   _ The f  o  r  c  e  s acting o n m a r e:
####                   _ μN    N, h e r e N is perpendicular t o
####                   _  \   /   t h e plane surface, a n d the
####.                  _   \ /    force of f r i c t i o n μN is
####. .   .`.          _    m     parallel. Becoz of t h e third
####. . .` m `.        _    |     law o f Newton the same forces
####. . .`.   .`       _    |     N and μN will act on the wedge
####. . . .`.`         _    |     but in opposite directions, so
####. . . . . .        _    |     the force with which the wedge
####. . . . . . .      _    |     will act o n the vertical wall
####. . . . . α . .    _    |     w i l l b e t  h  e horizontal 
####. . . . . . . . .  _   mg     component o f t h o s e forces
################################################################
################################################################
#### N = mgcosα; Nₓ = mgcosαsinα
#### f = μN; fₓ = μmgcosαcosα
#### F = Nₓ - fₓ = mgcosα(sinα - μcosα), the wall can't push the
#### wedge o f course, so when sinα - μcosα i s negative that is
#### tgα < μ, F is zero                                        «
################################################################
################################################################
################################################################
################################################################
## 2.1.33: Why does the s p e e d of raindrops not depend on the
## height of t h e clouds and strongly depend on the size of the
## drops?
##
## The reason for this is the so called resistance or drag force
## in fluids. T h i s is a bit complicated but roughly while the
## usual dry friction i s explained at atomic scale by molecular
## w e a k dipole interactions( Van der Waals forces ) in fluids
## and especially i n air the resistance f o r c e is due mainly
## becoz of the kinetic energy t r a n s f e r between colliding
## molecules and in general h a s the form f = αv², where α is a
## coefficient depending o n object's shape and mass, while v is
## the r e l a t i v e speed. To be precise we have to write the
## differential equation o f t h e raindrops: mdv/dt = -αv² + mg,
## and solve it but intuitively due to the resistance factor the
## speed w i l l initially increase and then reach some constant
## asymptotic value, regardless of t h e initial conditions( the
## h e i g h t of the clouds ), b u t we can effectively write a
## formula for the asymptotic s p e e d by balancing the gravity
## force with the drag force: mg = αv², v = √(mg/α)            «
##
################################################################
################################################################
################################################################
################################################################
