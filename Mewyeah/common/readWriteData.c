// File: readWriteData.c
// Description: Implementation of functions for reading and writing serial data for windows.
#include "readWriteData.h"
#include <windows.h>

// Returns the number of bytes read
int windowsReadData(const HANDLE hComm, unsigned char* pResponse) {
    int bytesRead;
    BOOL status = ReadFile(hComm, pResponse, sizeof(pResponse), &bytesRead, NULL);

    if (status) {
        printf("Data read from port, %lu bytes\n", bytesRead);
    } else {
        printf("Could not read data from port\n");
    }
    return bytesRead;
}

// Returns the number of bytes written
int windowsWriteData(const HANDLE hComm, const unsigned char* pRequest, const int request_length) {
    int bytesWritten;
    
    BOOL status = WriteFile(hComm, pRequest, request_length, &bytesWritten, NULL);

    if (status) {
        printf("Data written to port, %lu bytes\n", bytesWritten);
    } else {
        printf("Could not write data to port\n");
    }

    return bytesWritten;
}