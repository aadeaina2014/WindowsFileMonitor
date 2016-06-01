/************************************************************************
File Description :Server Utility
Project          :Remote Directory Monitoring System
Purpose          :Project Deliverable Submitted By Ayokunle Ade-Aina to
Reference        :xxx
Date             : May 2016
************************************************************************/

#include "Server.h"

//////////////////////////////////////////////////////////////
// Mark: initialize Variables reqiured for Socket connection
/////////////////////////////////////////////////////////////


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

//////////////////////////////////////////////////////////////
// Mark: setup end to end parameters
/////////////////////////////////////////////////////////////
void setup()
{
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

}

//////////////////////////////////////////////////////////////
// Mark: resolved connection address and ports
/////////////////////////////////////////////////////////////
bool resolveAddress()
{
	// Resolve the server address and port
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
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

//////////////////////////////////////////////////////////////
// Mark: Attempt to connect end socket point
/////////////////////////////////////////////////////////////
bool establishConnection()
{

	// Create a SOCKET for connecting to server
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	// Setup the TCP listening socket
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return false;
	}

	freeaddrinfo(result);

	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return false;
	}

	// Accept a client socket
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return false;
	}

	// No longer need server socket
	//closesocket(ListenSocket);
	return true; // connection has been successfully established
}

//////////////////////////////////////////////////////////////
// Mark: ascii data over Socket connection
/////////////////////////////////////////////////////////////
bool receiveMessage()
{

	iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);

	if (iResult > 0)
	{
		return true;
	}
	else
	{
		printf("recv failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return  false;
	}
}

//////////////////////////////////////////////////////////////
// Mark: send ascii data over Socket connection
/////////////////////////////////////////////////////////////

bool sendMessage()
{
	// Echo the buffer back to the sender
	iSendResult = send(ClientSocket, sendbuf, iResult, 0);
	if (iSendResult == SOCKET_ERROR)
	{
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return false;
	}
	else
	{
		return true;
	}


}

//////////////////////////////////////////////////////////////
// Mark: Disconnect endpoints
/////////////////////////////////////////////////////////////
bool  shutDownSocket()
{

	// shutdown the connection since we're done
	iResult = shutdown(ClientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR)
	{
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return false;
	}
	else
	{
		return true;
	}
}

//////////////////////////////////////////////////////////////
// Mark: cleanup memory allocation
/////////////////////////////////////////////////////////////
void tearDown()
{
	closesocket(ClientSocket);
	WSACleanup();
}
