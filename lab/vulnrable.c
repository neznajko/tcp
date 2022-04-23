// -*- c-file-style: "linux" -*-
//  ĵ   Ģ   ğ   Ÿ   ƈ   ɟ   ȕ   Ʌ   _   Ū
//  `   `   `   `   `   `   `   `   `   `
//  `   `   `   `   `   `   `   `   `   `
////////--------````````________=========
//                m  m   _ _ _   . . . .
# include <sys/types.h>
//        - - -   m      ` `     .   . .      
//______________========================= 
typedef u_int64_t u64; // // // // // //
//              , 
u64 rsp() { //  ,________________________
        u64 addr = 0;         // > > > > >
        asm( "mov %%rsp, %0"  //  
             : "=rm" ( addr ) //       ..
             :);              //__________
        return addr; //------------------            
} //            , , , , , , , , , , , , ,
# include <stdio.h>
# include <string.h>
# define BUFRLEN 512
////////////////////////////////////////////////////////////////
int main( int argc, char* argv[]                               )
                                                               {
        char bufr[ BUFRLEN ]                                   ;
        if( argc == 1                                          )
                                                               {
                printf( "%p\n", rsp() )                        ;
        } else                                                 {
                memcpy( bufr, argv[ 1 ], BUFRLEN )             ;
                                                               }
                                                               }
  /////////////////////////////////////////
 // /proc/sys/kernel/randomize_va_space //
//_____________________________________//
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
