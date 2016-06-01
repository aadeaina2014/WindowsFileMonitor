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




#endif
