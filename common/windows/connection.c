// File: connection.c
// Description: Implementation of functions for setting up the COM port connection for Windows.

#include "connection.h"  // Include the header file for this source file

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

// Function to set up the COM port
HANDLE setupCOMPort(int portNumber, const int baudRate, const unsigned char *queryData, const unsigned char *expectedResponse, const int dataLength) {
    if (portNumber != -1) {
        char comPortName[10];  // Enough space for "COM" + up to 3 digits + null terminator
        sprintf(comPortName, "COM%d", portNumber);  // Format the COM port name
        printf("Attempting to use COM port %d\n", portNumber);
        HANDLE hComm = connectToCOMPort(comPortName, baudRate);
        Sleep(500);
        if (hComm != INVALID_HANDLE_VALUE) {
            DWORD bytesWritten;

            // Send a query
            if (!WriteFile(hComm, queryData, dataLength, &bytesWritten, NULL)) {
                printf("Error in writing to COM port. Aborting.\n");
                CloseHandle(hComm);
                return INVALID_HANDLE_VALUE;
            }

            unsigned char buffer[300];
            DWORD bytesRead;

            // Read the response
            if (!ReadFile(hComm, buffer, sizeof(buffer), &bytesRead, NULL)) {
                printf("Error in reading from COM port. Aborting.\n");
                CloseHandle(hComm);
                return INVALID_HANDLE_VALUE;
            }

            // Check the response
            if (buffer[0] == expectedResponse[0] && buffer[1] == expectedResponse[1] && buffer[2] == expectedResponse[2]) {
                printf("Found the target COM port: %s\n", comPortName);
                return hComm;
            }

            // Close the handle if this isn't the correct one
            CloseHandle(hComm);
        } else {
            printf("Error: Unable to open COM port %s. Aborting.\n", comPortName);
            return INVALID_HANDLE_VALUE;
        }
    } else {
        printf("No COM port supplied. Searching for a COM port... \n");

        for (int i = 1; i <= 3; i++) { // Run through 3 scans from COM1 to COM10
            printf("\nScan #%d \n", i);

            for (int i = 1; i <= 10; ++i) {
                char comPortName[10];  // Enough space for "COM" + up to 3 digits + null terminator
                sprintf(comPortName, "COM%d", i);  // Format the COM port name
                HANDLE hComm = connectToCOMPort(comPortName, baudRate);
                Sleep(500);
                if (hComm != INVALID_HANDLE_VALUE) {
                    printf("Found a COM port: %s\n", comPortName);
                    DWORD bytesWritten;

                    // Send a query
                    if (!WriteFile(hComm, queryData, dataLength, &bytesWritten, NULL)) {
                        printf("Error in writing to COM port\n");
                        CloseHandle(hComm);
                        continue;
                    }

                    unsigned char buffer[300];
                    DWORD bytesRead;

                    // Read the response
                    if (!ReadFile(hComm, buffer, sizeof(buffer), &bytesRead, NULL)) {
                        printf("Error in reading from COM port\n");
                        CloseHandle(hComm);
                        continue;
                    }

                    printf("Read %d bytes\n", bytesRead);
                    // Print buffer
                    for (int i = 0; i < bytesRead; ++i) {
                        printf("%02X ", buffer[i]);
                    }

                    // Check the response
                    if (buffer[0] == expectedResponse[0] && buffer[1] == expectedResponse[1] && buffer[2] == expectedResponse[2]) {
                        printf("Found the target COM port: %s\n", comPortName);
                        return hComm;
                    }
                    
                    // Close the handle if this isn't the correct one
                    CloseHandle(hComm);
                } else {
                    printf("Unable to open COM port %s\n", comPortName);
                }
            }
        }
        printf("Unable to find the target COM port. Aborting.\n");
        return INVALID_HANDLE_VALUE;
    }
}

// Function to connect to the COM port
HANDLE connectToCOMPort(const char *portName, const int baudRate) {
    printf("Trying port %s\n", portName);
    DCB dcbSerialParams = {0};
    COMMTIMEOUTS timeouts = {0};
    
    HANDLE hComm = CreateFile(portName,
                              GENERIC_READ | GENERIC_WRITE,
                              0,
                              NULL,
                              OPEN_EXISTING,
                              0,
                              NULL);

    if (hComm == INVALID_HANDLE_VALUE) {
        return INVALID_HANDLE_VALUE;
    }

    dcbSerialParams.DCBlength = sizeof(DCB);

    if (!GetCommState(hComm, &dcbSerialParams)) {
        printf("Error getting current DCB settings\n");
        CloseHandle(hComm);
        return INVALID_HANDLE_VALUE;
    }

    
    dcbSerialParams.BaudRate = baudRate;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity   = NOPARITY;

    if (!SetCommState(hComm, &dcbSerialParams)) {
        printf("Could not set serial port parameters\n");
        CloseHandle(hComm);
        return INVALID_HANDLE_VALUE;
    }

    timeouts.ReadIntervalTimeout         = 50;
    timeouts.ReadTotalTimeoutConstant    = 50;
    timeouts.ReadTotalTimeoutMultiplier  = 10;
    timeouts.WriteTotalTimeoutConstant   = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(hComm, &timeouts)) {
        printf("Could not set serial port timeouts\n");
        CloseHandle(hComm);
        return INVALID_HANDLE_VALUE;
    }
    return hComm;
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
