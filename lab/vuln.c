// -*- c-file-style: "linux" -*-
//  ĵ   Ģ   ğ   Ÿ   ƈ   ɟ   ȕ   Ʌ   _   Ū
//  `   `   `   `   `   `   `   `   `   `
//  `   `   `   `   `   `   `   `   `   `
////////--------````````________=========
//                m  m   _ _ _   . . . .
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
//        - - -   m      ` `     .   . .      
//______________========================= 
typedef u_int64_t u64; // // // // // //
//              , 
u64 getrsp() { // ,________________________      _______________
        u64 rsp;                 // > > > > >          > > > > >
        asm( "mov %%rsp, %0\n\t" //  -------------        ------
             "// next: chill"    //=============================
             : "=r" ( rsp ));    //__________       ____________
        return rsp; //------------------   -------------      --       
} // , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , 
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# define BUFRLEN 40
# define OVERSIZ 80
////----\\\\````////,,,,----****----____====****````::::....____
u_char overflow[ OVERSIZ ];
////////////////////////////////////////////////////////////////
// This program is like an artificial castle, but the guessing
// example from phrack49/14.txt exploiting strcpy vulnerability
// by saving and passing the shellcode as an environment variable
// won't work here becos there are lot of zeroes in the stack
// pointer address for x86-64. I couldn't make it work with pipe
// so the programs are passing the evil code via file. The
// exploit.c is making a guessing and saves the code. After lots
// of Sementation faults, Bus Errors or Illegal Instructions
// probably with the help of a for loop, one can figure out the
// exact offsets.
//
// sudo bash -c 'echo 0 >/proc/sys/kernel/randomize_va_space'
// TARGET=exploit make
// TARGET=vuln make
// ./exploit -64 80 2>/dev/null > code; ./vuln
// 0x00007fffffffdf30
// $ 
int main( void ){
        u_char bufr[ BUFRLEN ];
        fprintf( stderr, "0x%016lx\n", getrsp());
        read( open( "code", O_RDONLY ), overflow, OVERSIZ );
        memcpy( bufr, overflow, OVERSIZ );
}  ///////////////////////////////////////
  ///////////////////////////////////////
 //proc/sys/kernel/randomize_va_space //
//___________________________________//
// M   s  Tv  i  t  a   f i    n
// a   u  Ha  n  h  b   i s    i
//_k___r__El_____e__o___l______l__________
// e T ei  u a s  p v  fe    1  f  g
//   H  s  e   i  r e  r     4  o  u
//   I         m  o    o     .  r  e
//   S         p  g    m     t     s
//__ ___ __ ___l__r__________x_____i______
//  t   b  o   e  a          t     n   
//  h   u  f      m                g   
//  e   f  f                           
//      f  s
//      e  e
//      r  t
//          
