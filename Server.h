/************************************************************************
File Description :Server Utility
Project          :Remote Directory Monitoring System
Purpose          :Project Deliverable Submitted By Ayokunle Ade-Aina to
Reference        :xxx
Date             : May 2016
************************************************************************/
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef SERVER_H_	
#define SERVER_H


#undef UNICODE
#define WIN32_LEAN_AND_MEAN
/*C++ headers*/
#include <string>
#include <iostream>
/*c type headers*/
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"


WSADATA wsaData;
int iResult;

SOCKET ListenSocket = INVALID_SOCKET;
SOCKET ClientSocket = INVALID_SOCKET;

struct addrinfo *result = NULL;
struct addrinfo hints;

int iSendResult;
char recvbuf[DEFAULT_BUFLEN];
int recvbuflen = DEFAULT_BUFLEN;
char  sendbuf[512] = "logging event:";
bool proceedStatus = true;

bool initializeSocket();
void setup();
bool resolveAddress();
bool establishConnection();
bool receiveMessage();
bool sendMessage();
bool shutDownSocket();
void tearDown();




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

void setup()
{
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

}

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

void tearDown()
{
	closesocket(ClientSocket);
	WSACleanup();
}

#endif
