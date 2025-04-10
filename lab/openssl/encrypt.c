////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
// #############################################################
// # CRT Mode ##################################################
// #############################################################
// nonce                                    64 bit random number
// ctr                                            64 bit counter
// crt-block                       [ nonce, ctr ], 128 bit block
// key                                     256 bit random number
// plaintext                            128 bit plain text block
// ciphertext                          128 bit cipher text block
// AES                                      256 bit block cipher
// #####/###########################################/###########
// ####/######## crt-block ########################/####### crt-
// ###/##########           ######################/#########    
// ##/################|##########################/##############
// #/#################|#########################/###############
// /##################v########################/################
// ### key -------> AES ######################/#################
// ####     ########     ####################/### key -------> A
// ###################|#####################/#####     ######## 
// ###################|####################/####################
// ###################v###################/#####################
// ### plaintext --> xor ################/####### plaintext --> 
// ####           ###     ##############/#########           ###
// ###################|################/########################
// ###################|###############/#########################
// ###################v##############/##########################
// ############## ciphertext #######/####################### cip
// ###############            #####/#########################   
// ###############################/#############################
// ##############################/##############################
// #############################/###############################
// ############################/################################
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
// #############################################################
// # GCM Mode ( Galois/Counter mode ) ##########################
// #############################################################
// utf-8 #######################################################
// #############################################################
// #############################################################
// #############################################################
// #############################################################
// #############################################################
////////////////////////////////////////////////////////////////
# include "types.h"                  //
////////////////////////////////////////////////////////////////
# include <stdio.h>                //
# include <stdlib.h>              //
# include <libgen.h>             //                     basename
////////////////////////////////////////////////////////////////
# include <openssl/rand.h>     // 
# include <openssl/ssl.h>     // 
////////////////////////////////////////////////////////////////
# define BUFSIZE 64 * 1024  //                             64 KB
# define KEYLENGTH 32      //                           256 bits
# define BLOCKSIZE 16     //                            128 bits
# define IVSIZE 12       //                              96 bits
////////////////////////////////////////////////////////////////
enum { IFILE = 1, OFILE, KEY, NARGS };
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
// > seq -s ' ' -w 20000 > plain.txt
// > ./encrypt plain.txt cipher.txt \
// d3b2bd22bc5ae5039a21587fd2b9cf2072be32b60ef8a09ec0d5544a923b80a2
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
void encrypt( FILE* ifile, FILE* ofile, u8_t* key ){
    // Generate the Initialization Vector
    u8_t iv[ IVSIZE ];
    RAND_bytes( iv, IVSIZE );
    // store it alongside cipher code
    fwrite( iv, 1, IVSIZE, ofile );
    // Set up the cipher context for the AES-256-GCM cipher
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit( ctx, EVP_aes_256_gcm(), key, iv );
    ////////////////////////////////////////////////////////////
    u8_t ibuf[ BUFSIZE ];
    u8_t obuf[ BUFSIZE ];
    while( !feof( ifile )){
        // load 1*BUFSIZE bytes from ifile to ibuf
        size_t ibytes = fread( ibuf, 1, BUFSIZE, ifile );
        int obytes = 0;
        // encrypt ibuf to obuf
        EVP_EncryptUpdate( ctx, obuf, &obytes, ibuf, ibytes );
        // store 1*obytes from obuf to ofile
        fwrite( obuf, 1, obytes, ofile );
    }
    // finalize encryption( write padding bytes )
    int outlen = 0;
    EVP_EncryptFinal( ctx, obuf, &outlen );
    fwrite( obuf, 1, outlen, ofile );
    // store the gcm tag
    u8_t tag[ BLOCKSIZE ];
    EVP_CIPHER_CTX_ctrl( ctx,
                         EVP_CTRL_GCM_GET_TAG,
                         BLOCKSIZE,
                         tag );
    fwrite( tag, 1, BLOCKSIZE, ofile );
    EVP_CIPHER_CTX_free( ctx );
}
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
// 9. Prove that the cube of any integer a has to be exactly one
// of these forms: 9k or 9k + 1 or 9k + 8 for some integer k.
//
// Ok let's try as before and divide an integer n by 9:
// n = q9 + r, 0 <= r < 9, using the formula:
// (a + b)³ = a³ + 3a²b + 3ab² + b³, we get for n³:
// n³ = q³9³ + 3q²9²r + 3q9r² + r³ = 9( ... ) + r³, now we have
// to look at all cases for r = 0, 1, 2, 3, 4, 5, 6, 7, 8
// When r = 0, 1, 2 we get exactly the forms:
// 9k, 9k + 1 and 9k + 8, for the others we have to calculate
// r³ and devide it by 9: r³ = Q9 + R, here we can put Q in
// the ( ... ), so the remainder will be R, and this should be
// either 0, 1, or 8
// r = 3, r³ = 27 = 3*9, R = 0
// r = 4, r³ = 64 = 7*9 + 1, R = 1
// r = 5, r³ = 125 = 13*9 + 8, R = 8
// r = 6, r³ = 216 = 24*9, R = 0
// r = 7, r³ = 343 = 38*9 + 1, R = 1
// r = 8, r³ = 512 = 56*9 + 8, R = 8
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
void usage( char *path ){
    printf( "Usage: %s ifile ofile key\n", basename( path ));
    exit( EXIT_FAILURE );
}
////////////////////////////////////////////////////////////////
u8_t *decode( u8_t *key ){
    ////////////////////////////////////////////////////////////
    // convert the input key hex string to memory representation
    // hexstr: ABCD, as a string this takes 4 bytes, one byte
    // for each character, the ascii code for A is 65 in decimal
    // 65 = 64 + 1 = 2**6 + 2**0 = 0100 0001, so the memory will
    // look like: |0100 0001|0100 0010|0100 0011|0100 0100|
    //             A         B         C         D
    // but what ABCD as a key is meant to be is its numerical
    // value A16**3 + B16**2 + C16**1 + D16**0, the values of
    // A,B,C and D as binary bits are:
    // A = 10 = 8 + 2 = 2**3 + 2**1 => 1010
    // B = 11 = 8 + 2 + 1 = ...     => 1011
    // C = 12 = ...                 => 1100
    // D = 13 = ...                 => 1101, so
    // the memory should look like, assuming little endian byte
    // ordering( Least Significant Byte (LSB) is in fyorst
    // memory location ), we'll have this:
    // 0xCD|0xAB = 1100 1101 | 1010 1011, but it seems
    // OPENSSL_hexstr2buf is converting in big endian that is
    // the string "ABCD" will be decoded as 0xab 0xcd, so the
    // value will be 0xcdab, ok never mind, let's decode:
    ////////////////////////////////////////////////////////////
    long len;
    key = OPENSSL_hexstr2buf( key, &len );
    // if len is 0 that means an error, for example a non-hex
    // character or odd number of character, the len is the
    // decoded buffer length, and cos we are supplying 256 bits
    // key it must be equal to 32
    if( len == 0 || len != KEYLENGTH ){
        printf( "boom\n" );
        exit( EXIT_FAILURE );
    }
    return key;
}
////////////////////////////////////////////////////////////////
int main( int argc, char* argv[]){
    if( argc != NARGS ){ usage( *argv );}
    FILE* ifile = fopen( argv[ IFILE ], "rb" );
    FILE* ofile = fopen( argv[ OFILE ], "wb" );
    u8_t* key = decode( argv[ KEY ]);
    ////////////////////////////////////////////////////////////
    encrypt( ifile, ofile, key );
    ////////////////////////////////////////////////////////////
    OPENSSL_free(key);
    
    fclose( ifile );
    fclose( ofile );
}
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
// 7. Prove that the square of any integer a is either of the
// form 3k or of the form 3k + 1 for some integer k.
// 
// Let's divide a by 3: a = 3q + r, 0 <= r < 3
//
// a² = (3q + r)² = 9q² + 6qr + r² = 3q(3q + 2r) + r²
//
// That is a² has the form 3k + r², where r = 0, 1, 2
// Now we have to consider each case for r:
//
// a) r = 0, a² = 3k
// b) r = 1, a² = 3k + 1
// c) r = 2, a² = 3k + 4 = 3(k + 1) + 1                        «
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
// 8. Use the Devision Algorithm to prove that every odd integer
// is either of the form 4k + 1 or of the form 4k + 3 for some
// integer k.
// Let a is odd, and let's divide it by 4:
// a = 4k + r, 0 <= r < 4, but r can't be neither 0 or 2 be cos
// a will be even, so r is either 1 or 3                       «
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
// 10. Let n be a positive integer. Prove that a and c leave the
// same remainder when divided by n if and only if a - c = nk,
// for some integer k
// => a and c leave the same remainder:
// a = qn + r, c = Qn + r; a - c = n(q - Q)
// <= a - c = nk, let a = qn + r, 0 <= r < n, then
// c = a - nk = (q - k)n + r, 0 <= r < n. That is r is the
// same remainder                                              «
////////////////////////////////////////////////////////////////
