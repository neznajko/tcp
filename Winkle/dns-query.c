                              ////
////////////////////////////////////////////////////////////////
# include <stdio.h>
# include <libgen.h> // basename
# include <stdlib.h>
# include <string.h> // strcmp
////////////////////////////////////////////////////////////////
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
////////////////////////////////////////////////////////////////
# define NARGS 3 // minimum arguments
# define panic( ATTACK) {                       \
        printf( "panic: %s\n", ATTACK);         \
        exit( 1);                               \
    }
////////////////////////////////////////////////////////////////
typedef struct addrinfo ai_t;
////////////////////////////////////////////////////////////////
void usg( char *prog) {
    printf( "usg: %s hostname record-type\n", basename( prog));
    printf( " Vhere record-type is:\n");
    printf( " a) A    - IPv4 address,         1\n");
    printf( " b) AAAA - IPv6 address,        28\n");
    printf( " c) MX   - Mail exchange,       15\n");
    printf( " d) TXT  - Text record,         16\n");
    printf( " e) *    - All cached records, 255\n");
    exit( 1);
}
////////////////////////////////////////////////////////////////
int get_type( char *str) {
    static char *key[] = { "A", "AAAA", "MX", "TXT", "*"};
    static int value[] = {  1,   28,     15,   16,   255};
    static int n = sizeof value;
    //
    for( int j = 0; j < n; j++) {
        if(! strcmp( str, key[j])) return value[j];
    }
    return 0;
}
////////////////////////////////////////////////////////////////
int make_socket( ai_t *ai) {
    int sock = socket( ai->ai_family,
                       ai->ai_socktype,
                       ai->ai_protocol);
    if( sock == -1) {
        panic( "socket( #:) - Boris Johnson)");
    }
    return sock;
}
////////////////////////////////////////////////////////////////
ai_t *get_dns_ai( char *dns_server) {
    ai_t hnts = {};
    hnts.ai_socktype = SOCK_DGRAM;
    ai_t *ai = NULL;
    if( getaddrinfo( dns_server, "53", &hnts, &ai)) {
        panic( "getaddrinfo( @;' - Smoking Lady)");
    }
    return ai;
}
////////////////////////////////////////////////////////////////
// Wikipedia:
// The header section consists of the following fields:
// Identification,
// Flags,
// Number of questions,
// Number of answers,
// Number of authority resource records (RRs), and
// Number of additional RRs. Each field is 16 bits long, and
// appears in the order given. The identification field is used
// to match responses with queries. The flag field consists of
// sub-fields as follows:
////////////////////////////////////////////////////////////////
// 1 - Sub-Fields
// 2 - Description
// 3 - Length( bits)
// +--------+----------------------------------------------+---+
// |      1 |                                            2 | 3 |
// +--------+----------------------------------------------+---+
// | QR     | Indicates if the message is a query (0) or a | 1 |
// |        | a reply (1)                                  |   |
// +--------+----------------------------------------------+---+
// | OPCODE | The type can be QUERY (standard query, 0),   | 4 |
// |        | IQUERY (inverse query, 1), or STATUS (server |   |
// |        | status request, 2)                           |   |
// +--------+----------------------------------------------+---+
// | AA     | Authoritative Answer, in a response,         | 1 |
// |        | indicates if the DNS server is authoritative |   |
// |        | for the queried hostname                     |   |
// +--------+----------------------------------------------+---+
// | TC     | TrunCation, indicates that this message was  | 1 |
// |        | truncated due to excessive length            |   |
// +--------+----------------------------------------------+---+
// | RD     | Recursion Desired, indicates if the client   | 1 |
// |        | means a recursive query                      |   |
// +--------+----------------------------------------------+---+
// | RA     | Recursion Available, in a response,          | 1 |
// |        | indicates if the replying DNS server         |   |
// |        | supports recursion                           |   |
// +--------+----------------------------------------------+---+
// | Z      | Zero, reserved for future use                | 3 |
// +--------+----------------------------------------------+---+
// | RCODE  | Response code, can be NOERROR (0), FORMERR   | 4 |
// |        | (1, Format error), SERVFAIL (2), NXDOMAIN    |   |
// |        | (3, Nonexistent domain), etc.[34]            |   |
// +--------+----------------------------------------------+---+
// In the Book RCODE is 3-bit, bcoz of etc.[34] I take 4.
# define HEADER_SIZE 12
# define MAX_QUERY_SIZE 1024
u_char *get_header() {
// It's probbably good to notice that third field as unsigned
// short is 256, that is 1 in Big Endian Byte Order.
    static u_char header[] =
        { 0x00, 0x00, // Identification
          0x01, 0x00, // Flags( Recursion)
          0x00, 0x01, // No. questions( Big Endian)
          0x00, 0x00, // No. answers
          0x00, 0x00, // No. authority RRs
          0x00, 0x00, // No. additional RRs
        };
    (*(u_short *)header)++; // increment identification
    return header;
}
////////////////////////////////////////////////////////////////
// name convertor: convert hostname src into dns question format
// www.example.com => 3,"www",7,"example",3,"com"
void namecnv( char *src, u_char *dst) {
    do {
        dst++; // make space for lable length
        int len = 0; // lable lenght
        while( *src != '.') { // copy and count
            if(! *src) break; // chck for the null byte
            *dst++ = *src++; // copy and mov forward
            len++; // count label characters
        }
        // store length before label
        *( dst - len - 1) = len;
    } while( *src++); // null byte chck and increment
}
////////////////////////////////////////////////////////////////
// query constructor
void qcons( char *hostname, int type, u_char *query, int *qlen)
{
    u_char *orig = query;
    memcpy( query, get_header(), HEADER_SIZE);
    query += HEADER_SIZE;
    namecnv( hostname, query);
    int n = 1 + strlen( hostname);
    query += n;
    // add null byte after name
    *query++ = '\0';
    // type
    *query++ = 0; *query++ = type;
    // class
    *query++ = 0; *query++ = 1;
    // query length
    *qlen = query - orig;
}
////////////////////////////////////////////////////////////////
# define dmp_word( msg, j) {\
    printf( "%02x %02x\n", msg[j], msg[j+1]);\
}
void qdmp( u_char *query, int qlen) {
    puts( "-Raw query dump:");
    for( int j = 0; j < qlen; j += 2) {
        dmp_word( query, j);
    }
}
// Big to Little Endian
# define B2L( msg, j) (msg[j + 1] + (msg[j] << 8))
////////////////////////////////////////////////////////////////
off_t dunf_name( u_char *msg, off_t off) {
    int len = msg[ off];
    if( len == 0xc0) { // pointer
        int ptr = msg[ off + 1];
        printf( "pointer(%d): %s\n", ptr, &msg[ ptr + 1]);
        return off + 2;
    }
    off_t copy = off;
    do {
        msg[ off] = '.';
        off += len + 1;
        len = msg[ off];
    } while( len);
    puts( &msg[ copy + 1]);
    return off + 1;
}
// Yeea
void chck_ir_out( u_char *msg, int len) {
///////////////////////////////////////////////////////// Header
    printf( "Id:    %02X %02X\n", msg[0], msg[1]);
    printf( "Flags: %02X %02X\n", msg[2], msg[3]);
    int qr = (msg[2] & 0x80) > 0;
    printf( "QR     - %s\n", qr ? "answer" : "question");
    printf( "OPCODE - ");
    int opcode = (msg[2] & 0x78) >> 3;
    if( opcode == 0)      puts( "standard query");
    else if( opcode == 1) puts( "inverse query");
    else                  puts( "server status request");
    // AA
    printf( "AA     - ");
    if(!( msg[2] & 0x04)) {
        printf( "non-");
    }
    puts( "authoritative answer");    
    // TC
    printf( "TC     - ");
    if(!( msg[2] & 0x02)) {
        printf( "not ");
    }
    puts( "truncated message");
    // RD
    printf( "RD     - with");
    if(!( msg[2] & 0x01)) {
        printf( "out");
    }
    puts( " recursion");
    // RCODE
    int rcode = msg[3] & 0x0f;
    if( rcode) panic( "RCODE( =:== - Robinson Crusoe)");
    puts("RCODE  - Ok");
    // Counts
    puts( "Number of :");
    // Number of questons
    printf( ": questions ------ %d\n", B2L( msg, 4));
    // Number of answers
    printf( ": answers -------- %d\n", B2L( msg, 6));
    // Number of authority resource records (RRs)
    printf( ": authority RRs -- %d\n", B2L( msg, 8));
    // Number of additional RRs 
    printf( ": additional RRs - %d\n", B2L( msg, 10));
    /////////////////////////////////////////////////// question
    puts( "Question:");
    off_t off = dunf_name( msg, 12);
    printf( "type:  %d\n", B2L( msg, off));
    printf( "class: %d\n", B2L( msg, off + 2));
    off += 4;
    ///////////////////////////////////////////////////// answer
    puts( "Answer:");
    off = dunf_name( msg, off);
    printf( "type:    %d\n", B2L( msg, off));
    printf( "class:   %d\n", B2L( msg, off + 2));
    off += 4;
    int ttl = B2L( msg, off + 2) + ((B2L( msg, off)) << 16);
    printf( "TTL:     %d\n", ttl);
    off += 4;
    printf( "length:  %d\n", B2L( msg, off));
    off += 2;
    printf( "Address: %d.%d.%d.%d\n",
            msg[off], msg[off+1], msg[off+2], msg[off+3]);
    puts( "The End");
}
////////////////////////////////////////////////////////////////
int main( int argc, char *argv[]) {
    puts( "dns-query here..");
# ifdef TESTING
    exit( 0);
# endif //STING
    // Chck arguments
    if( argc < NARGS) usg( *argv);
    // Get hostname and record type
    char *hostname = argv[1];
    int type = get_type( argv[2]);
    if(! type) usg( *argv);
    printf( "-Khm-khm,.. making dns query type %d for %s ..\n",
            type, hostname);
    // Configure remote dns server
    ai_t *ai = get_dns_ai( "8.8.8.8"); // -This is not for eating!
    int sock = make_socket( ai);
    // construct dns query
    u_char query[MAX_QUERY_SIZE];
    int qlen;
    qcons( hostname, type, query, &qlen);
    qdmp( query, qlen);
    // sending the query to dns server
    int send_bytes = sendto( sock, query, qlen, 0,
                             ai->ai_addr, ai->ai_addrlen);
    if( send_bytes == -1) {
        panic( "sendto( oo:o - Prima Vera)");
    }
    // receive response
    u_char res[ MAX_QUERY_SIZE] = {};
    int n = recvfrom( sock, res, sizeof res, 0, NULL, NULL);
    if( n == -1) {
        panic( "recvfrom( cC:' - Baby)");
    }
    chck_ir_out( res, n);
}
/////////////////////////////////// https://youtu.be/f7th0sBFpg0
// log: ./a.out example.com A
// dns-query here..
// -Khm-khm,.. making dns query type 1 for example.com ..
// -Raw query dump:
// 01 00
// 01 00
// 00 01
// 00 00
// 00 00
// 00 00
// 07 65
// 78 61
// 6d 70
// 6c 65
// 03 63
// 6f 6d
// 00 00
// 01 00
// 01 00
// Id:    01 00
// Flags: 81 80
// QR     - answer
// OPCODE - standard query
// AA     - non-authoritative answer
// TC     - not truncated message
// RD     - with recursion
// RCODE  - Ok
// Number of :
// : questions ------ 1
// : answers -------- 1
// : authority RRs -- 0
// : additional RRs - 0
// Question:
// example.com
// type:  1
// class: 1
// Answer:
// pointer(12): example.com
// type:    1
// class:   1
// TTL:     11970
// length:  4
// Address: 93.184.216.34
// The End
////////////////////////////////////////////////////////////////
//// c;3 - Hercule Poirot                                   ////
