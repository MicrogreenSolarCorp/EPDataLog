#ifndef CONNECTION_H
#define CONNECTION_H

// Global Variables
extern int g_delay_time_ms;

int setupSerialPort(const char *device, int baudRate);

#endif // CONNECTION_H
