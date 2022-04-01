////////////////////////////////////////////////////////////////
# include <sys/types.h>
////////////////////////////////////////////////////////////////
// This is the code for getting the machine instructions:
//
//    asm( "mov  %0, %%rdi\n\t" // fst arg
//         "mov  %1, %%rsi\n\t" // snd
//         "mov  $0, %%rdx\n\t" // NULL
//         "mov $59, %%rax\n\t" // sys_execve
//         "syscall"
//         :
//         : "r"( argv[ 1]), "r"( argv + 1));
//
// The program to be executed is put on the command line for
// example: ./shellcode "/bin/sh" will give the classical
// prompt, and ./shellcode "/bin/ls" "-al" will list direcotry.
// By the way I have found an interesting site, have to read it:
// https://nekosecurity.com/x86-64-shellcoding/
// part-1-introduction-to-x86_64-shellcoding
////////////////////////////////////////////////////////////////
int main( int arc, char *argv[]) {
    u_char bufr[64] =
        "\x48\x8b\x45\xb0"
        "\x48\x83\xc0\x08"
        "\x48\x8b\x00"
        "\x48\x8b\x55\xb0"
        "\x48\x83\xc2\x08"
        "\x48\x89\xc7"
        "\x48\x89\xd6"
        "\x48\xc7\xc2\x00\x00\x00\x00"
        "\x48\xc7\xc0\x3b\x00\x00\x00"
        "\x0f\x05";
    ((void(*)())bufr)(); // warning: ISO C forbids conv..
}
////////////////////////////////////////////////////////////////
// 0x48    0x8b    0x45    0xb0,   0x48    0x83
// 0x48    0x83    0xc0    0x08,   0x48    0x8b
// 0x48    0x8b    0x00,   0x48    0x8b    0x55
// 0x48    0x8b    0x55    0xb0,   0x48    0x83
// 0x48    0x83    0xc2    0x08,   0x48    0x89
// 0x48    0x89    0xc7,   0x48    0x89    0xd6
// 0x48    0x89    0xd6,   0x48    0xc7    0xc2
// 0x48    0xc7    0xc2    0x00    0x00    0x00    0x00,   0x48
// 0x48    0xc7    0xc0    0x3b    0x00    0x00    0x00,   0x0f
// 0x0f    0x05,   0x48    0xc7
