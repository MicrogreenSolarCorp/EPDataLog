// File: readWriteData.h
// Description: Headers of functions for reading and writing serial data for windows.

#ifndef READWRITEDATA_H
#define READWRITEDATA_H

// Returns the number of bytes read
int windowsReadData(const HANDLE hComm, unsigned char* pResponse);

// Returns the number of bytes written
int windowsWriteData(const HANDLE hComm, const unsigned char* pRequest, const int request_length);


#endif // READWRITEDATA_H