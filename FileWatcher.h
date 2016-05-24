/************************************************************************
File Description :Directory Watch Utility
Project          :Remote Directory Monitoring System
Purpose          :Project Deliverable Submitted By Ayokunle Ade-Aina 
Reference        :xxx
Date             : May 2016
************************************************************************/

#ifndef FILEWATCHER_H_
#define FILEWATCHER_H_
#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <locale>
#include <iostream>
#include <sstream>
using namespace std;

////////////////////////////////////
// Global Variables on Server Side
///////////////////////////////////
wchar_t *message;
bool clear2send; // flag for determimining whether message shold be transmitted  or not


////////////////////////////////
// Function Prototypes
///////////////////////////////
void modifyMessage(wchar_t *fname, bool chk, char *bptr, wchar_t* act, int event); // helper function for string manipulation
void WatchDirChanges(LPCSTR path);// directory watch routine



////////////////////////////////
// Function Implementation
///////////////////////////////
void WatchDirChanges(LPCSTR path)
{
	/*
	FileName member of FILE_NOTIFY_INFORMATION has only one WCHAR according to definition. Most likely, this field will have more characters.
	So the expected size of one item is (sizeof(FILE_NOTIFY_INFORMATION) + MAX_PATH * sizeof(WCHAR)).
	Prepare buffer for 256 items.
	*/

	wchar_t filename[MAX_PATH];
	wchar_t action[256];
	void WatchDirChanges(LPCSTR path);
	int choice;
	int  count;
	char buff[512] = "...";
	char * buffPtr;
	
	
	bool check = true;
	wchar_t  temp[20] = L" fileSize:";

	char buf[256 * (sizeof(FILE_NOTIFY_INFORMATION) + MAX_PATH * sizeof(WCHAR))] = { 0 };
	DWORD bytesReturned = 0;
	BOOL result = FALSE;
	FILE_NOTIFY_INFORMATION *fni = NULL;

	HANDLE hDir = CreateFile(path,
		FILE_LIST_DIRECTORY | STANDARD_RIGHTS_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS,
		NULL);

	if (!hDir || hDir == INVALID_HANDLE_VALUE)
	{
		wprintf(L"CreateFile failed\n");
		return;
	}



	result = ReadDirectoryChangesW(hDir, buf, sizeof(buf) / sizeof(*buf), TRUE,
		(
			FILE_NOTIFY_CHANGE_FILE_NAME |
			FILE_NOTIFY_CHANGE_DIR_NAME |
			FILE_NOTIFY_CHANGE_ATTRIBUTES |
			FILE_NOTIFY_CHANGE_SIZE |
			FILE_NOTIFY_CHANGE_LAST_WRITE |
			FILE_NOTIFY_CHANGE_LAST_ACCESS |
			FILE_NOTIFY_CHANGE_CREATION |
			FILE_NOTIFY_CHANGE_SECURITY), &bytesReturned, NULL, NULL);
	fni = (FILE_NOTIFY_INFORMATION*)buf;


	// if changes in directory is noticed
	if (result && bytesReturned)
	{


	
		switch (fni->Action)
		{
		case FILE_ACTION_ADDED:
			wcscpy_s(action, sizeof(action) / sizeof(*action), L"File added:"); choice = 1;
			check = true;
			break;

		case FILE_ACTION_REMOVED:
			wcscpy_s(action, sizeof(action) / sizeof(*action), L"File removed:"); choice = 2;
			check = false;
			break;

		case FILE_ACTION_MODIFIED:
			wcscpy_s(action, sizeof(action) / sizeof(*action), L"File modified:"); choice = 3;
			check = true;
			break;

		case FILE_ACTION_RENAMED_OLD_NAME:
			wcscpy_s(action, sizeof(action) / sizeof(*action), L"File renamed, was:"); choice = 4;
			check = true;
			break;

		case FILE_ACTION_RENAMED_NEW_NAME:
			wcscpy_s(action, sizeof(action) / sizeof(*action), L"File renamed, now is:"); choice = 5;
			check = true;
			break;

		default:
			swprintf_s(action, sizeof(action) / sizeof(*action), L"Unkonwn action: %ld. File name is:", fni->Action);
		}


		if (fni->FileNameLength)
		{
			wcsncpy_s(filename, MAX_PATH, fni->FileName, fni->FileNameLength);

			//wcsncpy_s(filename, MAX_PATH, fni->FileName, fni->FileNameLength / 2);
			//filename[fni->FileNameLength / 2] = 0;

			wcstombs(buff, filename, sizeof(buff));	/// convert from wchar_t* to char[]
			buffPtr = buff;
			 printf("log event : file event observed\n");
			//wprintf(L"log event : file name is %s\n", filename);
			bool textEventDetected = strstr(buffPtr, ".txt");

			if (textEventDetected) 
			{
				printf("log event: text event detected\n");
				modifyMessage(filename, check, buff,  action, choice);
			}
			else
			{
				printf("log event: text event not detected\n");
				clear2send = false;
			}

		}
		else
		{
			wprintf(L"%s <EMPTY>\n", action);
		}

	}

	else
	{
		wprintf(L"ReadDirectoryChangesW failed\n");
	}
	CloseHandle(hDir);
}



 void modifyMessage(wchar_t *fname, bool chk, char *bptr, wchar_t* act, int event )
{
	system("pause");
	struct stat filestatus;
	wchar_t temp[20] = L"..";

	
	wprintf(L"log event : text file event observed \n");
	//wprintf(L"log event : file name is %s\n", fname);
	if (chk) // filter off size measurement when delete operation  even occurs
	{

		stat(bptr, &filestatus);
		swprintf(temp, 20, L" %ld", filestatus.st_size);
		wcscat(temp, L"Bytes");
		wcscat(fname, temp);
		message = wcscat(act, fname);
		clear2send = true;
	}

	if (event = 2)
	{
		message = wcscat(act, fname); // join action taken to file acted upon
		clear2send = true;
	}

	//074408324359
}
#endif
