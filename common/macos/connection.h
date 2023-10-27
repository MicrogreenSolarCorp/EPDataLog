#ifndef CONNECTION_H
#define CONNECTION_H

#define INVALID_COM_PORT_NUMBER -2
#define INVALID_DELAY_TIME_SUPPLIED -3
#define NO_COM_PORT_NUMBER_SUPPLIED -1
#define NO_DELAY_TIME_SUPPLIED -1

#define INVALID_PORT_VALUE -1

// Global Variables
extern int g_delay_time_ms;

int setupSerialPort(const int portNumber, const int baudRate, const unsigned char *queryData, const unsigned char *expectedResponse, const int dataLength);

int connectToSerialPort(const char *device, const int baudRate, const unsigned char *queryData, const unsigned char *expectedResponse, const int dataLength);

#endif // CONNECTION_H
