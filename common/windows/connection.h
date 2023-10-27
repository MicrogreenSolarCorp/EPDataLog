// File: connection.h
// Description: Headers of functions for setting up the COM port connection for Windows.
#ifndef CONNECTION_H
#define CONNECTION_H

#include <windows.h>
#include <stdio.h>

// Constants
#define MIN_DELAY_TIME 0
#define MAX_COM_PORT_NUMBER 10
#define MIN_COM_PORT_NUMBER 1

#define INVALID_COM_PORT_NUMBER -2
#define INVALID_DELAY_TIME_SUPPLIED -3
#define NO_COM_PORT_NUMBER_SUPPLIED -1
#define NO_DELAY_TIME_SUPPLIED -1


// Global Variables
extern int g_delay_time_ms;

// Function Prototypes
int isInteger(char *str);
HANDLE setupCOMPort(int portNumber, const int baudRate, const unsigned char *queryData, const unsigned char *expectedResponse, const int dataLength);
HANDLE connectToCOMPort(const char *portName, const int baudRate);
int readProgramParams(int argc, char *argv[]);


#endif // CONNECTION_H
