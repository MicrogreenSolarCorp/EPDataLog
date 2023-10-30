// File: connection.h
// Description: Headers of functions for setting up the COM port connection for Windows.
#ifndef CONNECTION_H
#define CONNECTION_H

#include <windows.h>
#include <stdio.h>
#include "../../constants.h"


// Function Prototypes
HANDLE setupCOMPort(int portNumber, const int baudRate, const unsigned char *queryData, const unsigned char *expectedResponse, const int dataLength);
HANDLE connectToCOMPort(const char *portName, const int baudRate);


#endif // CONNECTION_H
