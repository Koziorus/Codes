#ifndef _WIN32_WIINT 
#define _WIN32_WINNT 0x0600 // 0x0600 means that the libraries below will support min Windows Vista
#endif

#include <winsock2.h> // Windows sockets
#include <iphlpapi.h> // IP Helper API
#include <ws2tcpip.h> // Winsock 2 TCP/IP

#include <stdio.h>
#include <stdlib.h>

#define ADAPTER_MAX_SIZE 20000
#define MAX_IP_LEN 100

int main()
{
	WSADATA wsa_data; // Windows Socket API data
	
	if(WSAStartup(MAKEWORD(2, 2), &wsa_data))
	{
		printf("WSAStartup failed\n");
		return 1;
	}
	
	DWORD adapters_size = ADAPTER_MAX_SIZE;
	
	// linked list of adapters info
	PIP_ADAPTER_ADDRESSES adapters = NULL; // PIP_ADAPTER_ADDRESSES is a pointer type of struct PIP_ADAPTER_ADDRESSES
	
	// when the initial adapters_size is too little the next iteration has the right value of adapters_size
	while(!adapters)
	{
		adapters = (PIP_ADAPTER_ADDRESSES)malloc(adapters_size);
		
		if(!adapters)
		{
			printf("Can't allocate memory for adapters\n");
			return 1;
		}
		
		int result = GetAdaptersAddresses(	AF_UNSPEC, // requesting IPv4 and IPv6 addresses
											GAA_FLAG_INCLUDE_PREFIX,
											0,
											adapters,
											&adapters_size); // writes the right number of bytes required to adapters_size
											
		if(result == ERROR_BUFFER_OVERFLOW)
		{
			printf("%ld bytes not provided!\n", adapters_size); 
			free(adapters);
		}
		else if(result == ERROR_SUCCESS) // Success
		{
			break;
		}
		else
		{
			printf("GetAdaptersAddresses() error: %d\n", result);
			free(adapters);
			WSACleanup();
			return 1;
		}
	}
	
	PIP_ADAPTER_ADDRESSES adapter = adapters;
	
	while(adapter)
	{
		// %S means wchar* instead of char*
		printf("Adapter name: %S\n", adapter->FriendlyName);
		
		// linked list of addresses of an adapter
		PIP_ADAPTER_UNICAST_ADDRESS address = adapter->FirstUnicastAddress;
		while(address)
		{
			// AF_INET - Address Family Internet
			printf("- %s", address->Address.lpSockaddr->sa_family == AF_INET ? "IPv4" : "IPv6");
			
			char ip[MAX_IP_LEN];
			
			// convert addrss to standard notation
			getnameinfo(address->Address.lpSockaddr,
						address->Address.iSockaddrLength,
						ip,
						sizeof(ip),
						0,
						0,
						NI_NUMERICHOST);
						
			printf("%s\n", ip);
						
			address = address->Next;
		}
		
		adapter = adapter->Next;
	}
	
	free(adapters);
	
	WSACleanup();
	return 0;
}