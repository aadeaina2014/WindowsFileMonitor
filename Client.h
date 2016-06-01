/************************************************************************
File Description :Client Utility
Project          :Remote Directory Monitoring System
Purpose          :Project Deliverable Submitted By Ayokunle Ade-Aina to
Reference        :xxx
************************************************************************/
#ifndef CLIENT_H_
#define CLIENT_H_


#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

///////////////////////////////////////////////////////////////
// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
///////////////////////////////////////////////////////////////

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

//////////////////////////////
//Global Variables
/////////////////////////////

WSADATA wsaData;
SOCKET ConnectSocket = INVALID_SOCKET;
struct addrinfo *result = NULL, *ptr = NULL, hints;
char *sendbuf = "C:";
char recvbuf[DEFAULT_BUFLEN];
int iResult;
int recvbuflen = DEFAULT_BUFLEN;
bool proceedStatus = true;

////////////////////////////////////
// function prototypes
////////////////////////////////////
bool validateParameters(int argc, char **argv);
bool initializeSocket();
void setup();
bool resolveAddress(char **argv);
bool establishConnection();
bool sendMessage();
bool receiveMessage();
bool shutDownSocket();
void tearDown();



#endif
