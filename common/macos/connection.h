// File: connection.h
// Description: Headers of functions for setting up the Serial port connection for MacOS.
#ifndef CONNECTION_H
#define CONNECTION_H

#include <stdlib.h>
#include "../../constants.h"

int setupSerialPort(const int portNumber, const int baudRate, const unsigned char *queryData, const unsigned char *expectedResponse, const int dataLength);
int connectToSerialPort(const char *device, const int baudRate, const unsigned char *queryData, const unsigned char *expectedResponse, const int dataLength);

#endif // CONNECTION_H
