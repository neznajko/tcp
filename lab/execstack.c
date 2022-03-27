////////////////////////////////////////////////////////////////
# include <sys/types.h>
////////////////////////////////////////////////////////////////
typedef u_int64_t u64;
////////////////////////////////////////////////////////////////
// The shellcode examples from phrack and other sources are
// written for 32-bit machines with older kernel and compiler
// versions and definitely do not run on my box. For example the
// machine instructions code were saved in a global variable,
// but newer kernels seems to forbid execution from data section
// even if it's read only( const). Fortunately there is a hidden
// --not-documented-- gcc option -z execstack which allows for
// compatibility reasons instructions from the stack to be
// fetched. This code will triger SIGSEGV and should be run with
// gdb in asm layout stepping through instructions with ni( next
// instruction). After the main returns it will jumb back to the
// address of ::bufr and after the third instruction will exit
// dumping -Program received signal SIGSEGV, Segmentation fault.
////////////////////////////////////////////////////////////////
int main() {
    asm( "mov $39, %rax\n\t" // sys_getpid     (1)
         "syscall\n\t"       // Hello, Kernel! (2)
         "mov %rax, %rdi");  // copy           (3)
    // Repeat with .byte
    asm( ".byte 0x48,0xc7,0xc0,0x27,0x00,0x00,0x00\n\t"
         ".byte 0x0f,0x05\n\t"
         ".byte 0x48,0x89,0xc7\n\t");
    // Let's try jumping here after returning from main
    u_char bufr[ 16] = "\x48\xc7\xc0\x27\x00\x00\x00"
                       "\x0f\x05"
                       "\x48\x89\xc7";
    // Without allocating new variables on the stack, find the
    // address of the saved %rip instruction pointer, and set
    // its value to the address of ::bufr
    *(u64 *)(bufr + 16 + 8) = (u64)bufr;
}
////////////////////////////////////////////////////////////////
// 0x48    0xc7    0xc0,    0x27    0x00    0x00    0x00; (1)
// 0x0f    0x05;                                          (2)
// 0x48    0x89    0xc7;                                  (3)
//
// 0x48,0xc7,0xc0,0x27,0x00,0x00,0x00 
// 0x0f,0x05
// 0x48,0x89,0xc7
