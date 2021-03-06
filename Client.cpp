/************************************************************************
File Description :Client Utility
Project          :Remote Directory Monitoring System
Purpose          :Project Deliverable Submitted By Ayokunle Ade-Aina to
Reference        :xxx
************************************************************************/
#include "Client.h"

/////////////////////////////////////////////////////////
// Mark: Implementation of functions for socke Connection
/////////////////////////////////////////////////////////


//////////////////////////////////
// Mark: Validate the parameters
//////////////////////////////////
bool validateParameters(int argc, char **argv)
{
	
	if (argc != 2)
	{
		printf("usage: %s server-name\n", argv[0]);
		return false;
	}
	else
	{
		return true;
	}
}

//////////////////////////////////
// Mark: Initialize Socket
//////////////////////////////////
bool initializeSocket()
{
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return false;
	}
	else
	{
		return true;
	}
}

//////////////////////////////////////
// Mark: Specify Socket Setup Options
//////////////////////////////////////

void setup()
{
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
}


//////////////////////////////////////
// Mark: Resolve Addressses 
// for local simulation on single 
// system use loop back address 
// 127.0.0.1
//////////////////////////////////////


bool resolveAddress(char**argv)
{
	// Resolve the server address and port
	iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return false;
	}
	else
	{
		return true;
	}
}


//////////////////////////////////////
// Mark: Connect Socket end points
//////////////////////////////////////
bool establishConnection()
{

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET)
		{
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return false;
		}

		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR)
		{
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET)
	{
		printf("Unable to connect to server!\n");
		WSACleanup();
		return false;
	}


	return true;
}


//////////////////////////////////////
// Mark: send ascii data over socket
//////////////////////////////////////

bool sendMessage()
{
	// Send an initial buffer
	iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
	if (iResult == SOCKET_ERROR)
	{
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return false;
	}
	else
	{
		return true;
	}
}

bool receiveMessage()
{


	iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
	if (iResult > 0)
	{
		//wprintf(L"log event: %s '%s'\n", action, filename);
		printf("%s\n", recvbuf);
		return true;
	}
	else
	{
		printf("recv failed with error: %d\n", WSAGetLastError());
		return false;
	}
}




bool shutDownSocket()
{
	// shutdown the connection since no more data will be sent
	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return false;
	}
	else
	{
		return true;
	}
}
void tearDown()
{
	closesocket(ConnectSocket);
	WSACleanup();
}
