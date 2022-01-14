# include <WinSock2.h>
# include <iphlpapi.h>
# include <ws2tcpip.h>
# include <iostream>
# pragma comment( lib, "ws2_32.lib")
# pragma comment( lib, "iphlpapi.lib")
////////////////////////////////////////////////////////////////////////
// clean the mess
# define au_revoir( EXIT_CODE) {\
	WSACleanup();\
	free( bufr);\
	return( EXIT_CODE);\
}
int main() {
	// Initialize winsock library.
	WSADATA d; // WSA = WinSock API
	if( WSAStartup( MAKEWORD( 2, 2), &d)) {
		std::cout << "Hello World!\n";
		return 1;
	}
	std::cout << "Ok.\n";
	// get adapters( adapta)
	DWORD adpt_size = 16384; // 16kB( 2**14)
	PIP_ADAPTER_ADDRESSES bufr;
	// Let's try without the do loop
	bufr = (PIP_ADAPTER_ADDRESSES) malloc( adpt_size);
	// Neva was looking for malloc errors before,
	// I'm going to skip << out of memory check << or whateva.
	int r = GetAdaptersAddresses( 
		AF_UNSPEC,               // both 4 and 6
		GAA_FLAG_INCLUDE_PREFIX, // not extremly clear
		nullptr,                 // dummy stuff
		bufr,
		&adpt_size);
	if( r != ERROR_SUCCESS) {
		std::cout << "NEVA_BEGINZE!\n";
		au_revoir( 1);
	}
	PIP_ADAPTER_ADDRESSES adapta = bufr;
	while( adapta) {
		std::cout << "Adapter's Friendly Name: " 
			      << adapta->FriendlyName << std::endl;
		IP_ADAPTER_UNICAST_ADDRESS* adr = adapta->FirstUnicastAddress;
		while( adr) {
			int family = adr->Address.lpSockaddr->sa_family;
			if( family == AF_INET) {
				std::cout << "4\n";
			} else {
				std::cout << "6\n";
			}
			char rax[ 64];
			getnameinfo( adr->Address.lpSockaddr,
				adr->Address.iSockaddrLength,
				rax, sizeof rax, 0, 0,
				NI_NUMERICHOST);
			std::cout << rax << std::endl;
			adr = adr->Next;
		}
		adapta = adapta->Next;
	}
	au_revoir( 0);
}
// log: Microsoft Visual Studio (Not Responding)