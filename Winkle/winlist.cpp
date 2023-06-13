////////////////////////////////////////////////////////////////
# include <WinSock2.h> // WSADATA, WSAStartup, WSACleanup
# include <iphlpapi.h> // PIP_ADAPTER_ADDRESSES ..
# include <ws2tcpip.h> // getnameinfo
# pragma comment( lib, "ws2_32.lib" )
# pragma comment( lib, "iphlpapi.lib")
# include <iostream>
////////////////////////////////////////////////////////////////
using std::endl;
////////////////////////////////////////////////////////////////
std::ostream& operator<<( std::ostream& os, WSADATA wsa_data ){
    return os << wsa_data.szDescription;
}
////////////////////////////////////////////////////////////////
# define au_revoir( EXIT_CODE ){                               \
    delete[] bufr;                                             \
    WSACleanup();                                              \
    return EXIT_CODE;                                          \
}
////////////////////////////////////////////////////////////////
int GetAdapters( PIP_ADAPTER_ADDRESSES& bufr ){
    DWORD adapta_size{};
    bufr = nullptr; // to figure adapta_size
    for( ;; ){
        int rslt = GetAdaptersAddresses(
            AF_UNSPEC,               // both 4 and 6
            GAA_FLAG_INCLUDE_PREFIX, // wtf's thus?
            nullptr,                 // dummy stuff( figured! )
            bufr, 
            &adapta_size );
        if( rslt == ERROR_BUFFER_OVERFLOW ){
            bufr = ( PIP_ADAPTER_ADDRESSES ) new char[ adapta_size ];
        } else {
            return rslt;
        }
    }
 }
////////////////////////////////////////////////////////////////
int main(){
    // Initialize winsock library
    WSADATA wsa_data; // WSA = WinSock API
    const BYTE MAJOR{ 2 };
    const BYTE MINOR{ 2 };
    const WORD VERSION{ MAKEWORD( MAJOR, MINOR )};
    if( WSAStartup( VERSION, &wsa_data ) != 0 ){
        std::cout << "WinSock Startup Error.\n";
        return 1;
    } else {
        std::cout << wsa_data << " successfully launched!\n";
    }
    PIP_ADAPTER_ADDRESSES bufr{}; 
    if( GetAdapters( bufr ) != ERROR_SUCCESS ){
        std::cout << "NEVA_BEGINZE!\n";
        au_revoir( 1 );
    }
    for( auto adapta{ bufr }; adapta; adapta = adapta->Next ){
        // PWCHAR
        printf( "\nAdapter name: %s( 0x%llX ): %S", 
            adapta->AdapterName, 
            adapta->FriendlyName,
            adapta->FriendlyName ); 
        for( auto* addr{ adapta->FirstUnicastAddress }; 
            addr; addr = addr->Next ){
            char rax[ 64 ];
            getnameinfo( 
                addr->Address.lpSockaddr,
                addr->Address.iSockaddrLength,
                rax, sizeof rax, 0, 0,
                NI_NUMERICHOST );
            std::cout << endl << rax;
        }
    }
    au_revoir( 0 );
}
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////// 