// File: connection.c
// Description: Implementation of functions for setting up the Serial port connection for MacOS.

#include "connection.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>

// Global Vars
int g_delay_time_ms = 2000;

// Function to check if a string is an integer
int isInteger(char *str) {
    char *endptr;
    strtol(str, &endptr, 10);
    if (endptr == str) {
        return 0;  // No conversion performed
    }
    if (*endptr != '\0') {
        return 0;  // String is partially a valid integer
    }
    return 1;
}

int setupSerialPort(const int portNumber, const int baudRate, const unsigned char *queryData, const unsigned char *expectedResponse, const int dataLength) {
    char portName[30];  // Enough space for "/dev/tty.usbserial-" + up to 2 digits + null terminator

    if (portNumber != -1) {
        // Format the specified port name
        sprintf(portName, "/dev/tty.usbserial-%d", portNumber);
        int fd = connectToSerialPort(portName, baudRate, queryData, expectedResponse, dataLength);
        if (fd != -1) {
            // Write the query
            int nBytesWritten = (int) write(fd, queryData, dataLength);
            if (nBytesWritten > 0) {
                printf("Data written to port %s, %d bytes\n", portName, nBytesWritten);
            } else {
                printf("Could not write data to port %s\n", portName);
                close(fd);
                return INVALID_PORT_VALUE;
            }

            // Sleep for a short duration to allow data to be received
            usleep(100000);  // Sleep for 100 ms, adjust as necessary

            // Read the response
            unsigned char buffer[300];
            int nBytesRead = (int) read(fd, buffer, sizeof(buffer));
            if (nBytesRead > 0) {
                printf("Read %d bytes from port %s\n", nBytesRead, portName);
            } else {
                printf("Could not read data from port %s\n", portName);
                close(fd);
                return INVALID_PORT_VALUE;
            }

            // Check the response
            if (nBytesRead >= 3 && buffer[0] == expectedResponse[0] && buffer[1] == expectedResponse[1] && buffer[2] == expectedResponse[2]) {
                printf("Found the target serial port: %s\n", portName);
                return fd;  // Return the file descriptor if this is the target port
            }

            // Close the file descriptor if this isn't the correct port
            close(fd);
        }
        printf("Unable to connect to the specified serial port: %s. Aborting.\n", portName);
        return -1;
    } else {
        printf("No serial port number supplied. Searching for a serial port... \n");
        for (int i = 1; i <= 3; i++) { // Run through 3 scans from COM1 to COM10
            printf("\nScan #%d \n", i);

            for (int i = 1; i <= 20; ++i) {
                sprintf(portName, "/dev/tty.usbserial-%d", i);
                printf("Trying port %s\n", portName);
                int fd = connectToSerialPort(portName, baudRate, queryData, expectedResponse, dataLength);
                usleep(500000);  // Sleep for 500 ms, adjust as necessary
                if (fd != -1) {
                    // Write the query
                    int nBytesWritten = (int) write(fd, queryData, dataLength);
                    if (nBytesWritten > 0) {
                        printf("Data written to port %s, %d bytes\n", portName, nBytesWritten);
                    } else {
                        printf("Could not write data to port %s\n", portName);
                        close(fd);
                        continue;  // Skip to the next iteration if writing failed
                    }

                    // Sleep for a short duration to allow data to be received
                    usleep(100000);  // Sleep for 100 ms, adjust as necessary

                    // Read the response
                    unsigned char buffer[300];
                    int nBytesRead = (int) read(fd, buffer, sizeof(buffer));
                    if (nBytesRead > 0) {
                        printf("Read %d bytes from port %s\n", nBytesRead, portName);
                    } else {
                        printf("Could not read data from port %s\n", portName);
                        close(fd);
                        continue;  // Skip to the next iteration if reading failed
                    }

                    // Check the response
                    if (nBytesRead >= 3 && buffer[0] == expectedResponse[0] && buffer[1] == expectedResponse[1] && buffer[2] == expectedResponse[2]) {
                        printf("Found the target serial port: %s\n", portName);
                        return fd;  // Return the file descriptor if this is the target port
                    }

                    // Close the file descriptor if this isn't the correct port
                    close(fd);
                }
            }
        }
    }

    printf("Unable to find the target serial port. Aborting.\n");
    return INVALID_PORT_VALUE;
}


int connectToSerialPort(const char *device, const int baudRate, const unsigned char *queryData, const unsigned char *expectedResponse, const int dataLength) {
    int fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1) {
        perror("open_port: Unable to open the serial port");
        return -1;
    }

    struct termios options;
    tcgetattr(fd, &options);

    // Set the baud rate
    cfsetispeed(&options, baudRate);
    cfsetospeed(&options, baudRate);

    // Set 8N1 mode
    options.c_cflag &= ~PARENB; // No parity
    options.c_cflag &= ~CSTOPB; // 1 stop bit
    options.c_cflag &= ~CSIZE;  // Mask the character size bits
    options.c_cflag |= CS8;     // 8 data bits

    // Enable the receiver and set local mode
    options.c_cflag |= (CLOCAL | CREAD);

    // Set the new options for the port
    tcsetattr(fd, TCSANOW, &options);

    return fd;
}


// Returns the supplied COM port number, or -1 if none was supplied
int readProgramParams(int argc, char *argv[]) {
    int suppliedComPortNumber = NO_COM_PORT_NUMBER_SUPPLIED;
    int suppliedDelayTime = NO_DELAY_TIME_SUPPLIED;

    // Try to read the COM port number from the command line
    for (int i = 1; i < argc; i++) {  // Starting at 1 to skip the program name itself
        if (strcmp(argv[i], "-c") == 0) {
            if (i + 1 < argc) {  // Make sure we don't go out of bounds
                if (!isInteger(argv[i + 1])) {
                    printf("Error: Invalid value for -c option. Aborting\n");
                    return INVALID_COM_PORT_NUMBER;
                }
                suppliedComPortNumber = atoi(argv[++i]);  // Convert next argument to integer and assign to t_value
                // Handle Error checking for com port number time
                if (suppliedComPortNumber < MIN_COM_PORT_NUMBER) {
                    printf("Error: COM port number cannot be less than %d. Aborting\n", MIN_COM_PORT_NUMBER);
                    suppliedComPortNumber = INVALID_COM_PORT_NUMBER;
                } else if (suppliedComPortNumber > MAX_COM_PORT_NUMBER) {
                    printf("Error: COM port number cannot be greater than %d. Aborting\n", MAX_COM_PORT_NUMBER);
                    suppliedComPortNumber = INVALID_COM_PORT_NUMBER;
                }
            } else {
                printf("Error: Missing value for -c option\n");
            }
    // Try to read the delay time from the command line
        } else if (strcmp(argv[i], "-t") == 0) {
            if (i + 1 < argc) {  // Make sure we don't go out of bounds
                if (!isInteger(argv[i + 1])) {
                    printf("Error: Invalid value for -t option\n");
                    continue;
                }
                suppliedDelayTime = atoi(argv[++i]);  // Convert next argument to integer and assign to c_value
                // Handle Error checking for delay time
                if (suppliedDelayTime < MIN_DELAY_TIME) {
                    printf("Error: Delay time cannot be less than %d. Aborting\n", MIN_DELAY_TIME);
                    return INVALID_DELAY_TIME_SUPPLIED;
                } else {
                    g_delay_time_ms = suppliedDelayTime;
                    printf("Success: Delay time of %dms set\n", g_delay_time_ms);
                }

            } else {
                printf("Error: Missing value for -t option\n");
            }
        }
    }

    if (suppliedDelayTime == NO_DELAY_TIME_SUPPLIED) {
        printf("No delay time supplied. Using default delay time of %dms\n", g_delay_time_ms);
    }

    return suppliedComPortNumber;

}
