#ifndef CONNECTION_H
#define CONNECTION_H

#include <stdlib.h>

#define HMI_COMM_BAUD       38400 
#define MIN_DELAY_TIME 0
#define MAX_COM_PORT_NUMBER 20
#define MIN_COM_PORT_NUMBER 1

#define INVALID_COM_PORT_NUMBER -2
#define INVALID_DELAY_TIME_SUPPLIED -3
#define NO_COM_PORT_NUMBER_SUPPLIED -1
#define NO_DELAY_TIME_SUPPLIED -1
#define INVALID_PORT_VALUE -4


// Global Variables
extern int g_delay_time_ms;

int isInteger(char *str);
int setupSerialPort(const int portNumber, const int baudRate, const unsigned char *queryData, const unsigned char *expectedResponse, const int dataLength);
int connectToSerialPort(const char *device, const int baudRate, const unsigned char *queryData, const unsigned char *expectedResponse, const int dataLength);
int readProgramParams(int argc, char *argv[]);

#endif // CONNECTION_H
