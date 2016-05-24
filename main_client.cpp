/************************************************************************
File Description :Client Application Driver
Project          :Remote Directory Monitoring System
Purpose          :Project Deliverable Submitted By Ayokunle Ade-Aina to
Reference        :xxx
Date             : May 2016
************************************************************************/
#include "Client.h"
using namespace std;

int __cdecl main(int argc, char **argv)
{

	printf("log event :Initializing File spy Client\n");
	printf("log event :Setting up Socket for client  server communication\n");

	///////////////////////////////////////////////////
	//  Mark: initiate Socket Communication Procedures
	//////////////////////////////////////////////////

	proceedStatus = validateParameters(argc,argv);
	if (!proceedStatus)return EXIT_FAILURE;
	
	proceedStatus = initializeSocket();
	
	if (!proceedStatus)return EXIT_FAILURE;
	setup();
	printf("log event :Resolving Addresses and ports\n");
	proceedStatus = resolveAddress(argv);
	if (!proceedStatus)return EXIT_FAILURE;
	
	proceedStatus = establishConnection();
	printf("log event :Establishing Connection\n");

	printf("log event :Getting directory of target machine to be monitored\n");
	if (!proceedStatus)return EXIT_FAILURE;
	std::string path, emptybuff;
	cout << "Enter Directory" << endl;
	cin >> path;
	sendbuf =  (char *) path.c_str();
	proceedStatus = sendMessage();


	if (!proceedStatus)return EXIT_FAILURE;
	
	////////////////////////////////////////////////////////////////////////
	//Continuously watch and log changes to given Target Marchine Directory
	////////////////////////////////////////////////////////////////////////
	while (1)
	{
		//recv(ConnectSocket, recvbuf, recvbuflen, 0);
		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		printf("log event :%s\n", recvbuf);
	}

	//////////////////////////////////////////////////////
	// Mark: TearDown and Cleanup all memory Allocations
	///////////////////////////////////////////////////////
	shutDownSocket();
	tearDown();

	return EXIT_SUCCESS;
}
