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

////////////////////////////////////
// Global Variables
////////////////////////////////////
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


////////////////////////////////////
// function prototypes
////////////////////////////////////
bool initializeSocket();
void setup();
bool resolveAddress();
bool establishConnection();
bool receiveMessage();
bool sendMessage();
bool shutDownSocket();
void tearDown();

#endif
