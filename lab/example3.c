////////////////////////////////////////////////////////////////
// This is example 3 from phrack49/14.txt, here is the original
// code:
// ORIG:
// void function(int a, int b, int c) {
//    char buffer1[5];
//    char buffer2[10];
//    int *ret;
// 
//    ret = buffer1 + 12;
//    (*ret) += 8;
// }
// 
// void main() {
//   int x;
// 
//   x = 0;
//   function(1,2,3);
//   x = 1;
//   printf("%d\n",x);
// }
// The idea is that the line x = 1; will be skipped. I've tried
// compile it with -m32, but the addresses are still 8 bytes, so
// have decided to give it a try as a 64-bit program. There are
// some differences how parameters and locl variables are placed
// on the stack, so one has to use -S to view the assembly code
// or use gdb.
////////////////////////////////////////////////////////////////
# include <stdio.h>
# include <sys/types.h>
typedef int64_t i64;
void function( int a, int b, int c) {
   char buffer1[ 5];
   char buffer2[ 10];
   i64 *ret;
//      [ buffer1 ][ ret ][ sfp ][ RET ]
//      <----5----><--8--><--8--><--8-->
//                        rbp   
   ret = ( i64*)( buffer1 + 21);
   *ret += 7;
   printf( "ret: 0x%lx %lu\n", *ret, sizeof( i64));
}
int main() {
    int x = 8;
    function( 1, 2, 3);
    x = 1; // disassembly reveals that this is a simple mov
    // instruction with size 7
    printf( "%d\n", x); // prints 8
}
////////////////////////////////////////////////////////////////
//
//
