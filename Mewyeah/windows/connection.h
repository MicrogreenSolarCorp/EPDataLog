// File: connection.h
// Description: Headers of functions for setting up the COM port connection.
#ifndef CONNECTION_H
#define CONNECTION_H

#include <windows.h>
#include <stdio.h>
#include <time.h>

// Constants
#define HMI_COMM_BAUD       38400 
#define MIN_DELAY_TIME 0
#define MAX_COM_PORT_NUMBER 256
#define MIN_COM_PORT_NUMBER 1

#define INVALID_COM_PORT_NUMBER -2
#define INVALID_DELAY_TIME_SUPPLIED -3
#define NO_COM_PORT_NUMBER_SUPPLIED -1
#define NO_DELAY_TIME_SUPPLIED -1

// Global Variables
extern int g_delay_time_ms;

// Function Prototypes
int isInteger(char *str);
HANDLE setupCOMPort(int portNumber);
HANDLE connectToCOMPort(const char *portName);
int readProgramParams(int argc, char *argv[]);
FILE *openCsvFile();
int printCsvHeader(FILE *fp);
int outputBMSDataToCsv(FILE *fp);

#endif // CONNECTION_H
