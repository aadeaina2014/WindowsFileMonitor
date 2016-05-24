/************************************************************************
File Description :Server Application Driver
Project          :Remote Directory Monitoring System
Purpose          :Project Deliverable Submitted By Ayokunle Ade-Aina to
Reference        :xxx
Date             : May 2016
************************************************************************/

/* Server Application must be running before client application can connect*/

/*
Instruction
1. Run  Server.exe application compile from the folloing source files  main_server.cpp, FileWatcher.h, Server.h
2. Run Client.exe  /SERVER IP ADDRESS/ application compiled from main_client and client.h using

*/
#undef UNICODE
#define WIN32_LEAN_AND_MEAN
#include "Server.h"
#include "FileWatcher.h"
#include <locale>

int __cdecl main(void)
{
	char * init = ""; //initialial conditions

	printf("log event :Initializing file spy server\n");
	printf("log event :Setting up Socket for client  server communication\n");

	///////////////////////////////////////////////////
	//  Mark: initiate Socket Communication Procedures
	//////////////////////////////////////////////////
	proceedStatus = initializeSocket();
	if (!proceedStatus)return EXIT_FAILURE;
	setup();
	printf("log event :Resolving Addresses and ports\n");
	proceedStatus = resolveAddress();
	if (!proceedStatus)return EXIT_FAILURE;
	printf("log event :Establishing Connection\n");
	proceedStatus = establishConnection();
	if (!proceedStatus)return EXIT_FAILURE;
	printf("log event :waiting for client to request Directory to be Monitored\n");
	iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);

	printf("log event : Directory to be Monitored is %s\n", recvbuf);
	printf("log event : Logging File system activity\n");

	/////////////////////////////////////////////////////////
	//Continuously watch and log changes to given directory
	/////////////////////////////////////////////////////////
	while (1)
	{
		WatchDirChanges(recvbuf);

		if (clear2send)
		{
			wcstombs(sendbuf, message, sizeof(sendbuf));	/// convert from wchar_t* to char[]	
			send(ClientSocket, sendbuf, 512, 0);
		}
		else
		{
			printf("log event: waiting for text file event\n");
		}


	}
	//////////////////////////////////////////////////////
	// Mark: TearDown and Cleanup all memory Allocations
	///////////////////////////////////////////////////////
	proceedStatus = shutDownSocket();
	if (!proceedStatus)return EXIT_FAILURE;
	tearDown();

	return EXIT_SUCCESS;
}
