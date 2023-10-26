// File: connection.c
// Description: Implementation of functions for setting up the COM port connection.

#include "connection.h"  // Include the header file for this source file
#include "../common/getData.h"

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
HANDLE setupCOMPort(int portNumber, unsigned char *queryData, unsigned char *expectedResponse) {
    if (portNumber != -1) {
        char comPortName[10];  // Enough space for "COM" + up to 3 digits + null terminator
        sprintf(comPortName, "COM%d", portNumber);  // Format the COM port name
        printf("Attempting to use COM port %d\n", portNumber);
        HANDLE hComm = connectToCOMPort(comPortName);
        Sleep(500);
        if (hComm != INVALID_HANDLE_VALUE) {
            DWORD bytesWritten;

            // Send a query
            if (!WriteFile(hComm, queryData, sizeof(queryData), &bytesWritten, NULL)) {
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
            if (buffer[0] == expectedResponse[0] && buffer[1] == expectedResponse[1]  && buffer[2] == expectedResponse[2]) {
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
        for (int i = 1; i <= 256; ++i) {
            char comPortName[10];  // Enough space for "COM" + up to 3 digits + null terminator
            sprintf(comPortName, "COM%d", i);  // Format the COM port name
            HANDLE hComm = connectToCOMPort(comPortName);
            Sleep(500);
            if (hComm != INVALID_HANDLE_VALUE) {
                DWORD bytesWritten;

                // Send a query
                if (!WriteFile(hComm, queryData, sizeof(queryData), &bytesWritten, NULL)) {
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

                // Check the response
                if (buffer[0] == expectedResponse[0] && buffer[1] == expectedResponse[1]  && buffer[2] == expectedResponse[2]) {
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
}

// Function to connect to the COM port
HANDLE connectToCOMPort(const char *portName) {
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

    dcbSerialParams.BaudRate = HMI_COMM_BAUD;
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

// Function to open the CSV file
FILE *openCsvFile() {
    // Get current date and time
    time_t raw_time;
    struct tm *tm_info;

    time(&raw_time);
    tm_info = localtime(&raw_time);

    char dateTime[14];
    // Store date and time in dateTime array
    snprintf(dateTime, sizeof(dateTime), "%02d%02d%02d_%02d%02d%02d", 
             (tm_info->tm_year + 1900) % 100, 
             tm_info->tm_mon + 1, 
             tm_info->tm_mday, 
             tm_info->tm_hour, 
             tm_info->tm_min, 
             tm_info->tm_sec);


    char filePrefix[7] = "EPData";    
    char fileName[30];

    strcpy(fileName, filePrefix);  // Initialize fileName with filePrefix
    strcat(fileName, dateTime);    // Concatenate dateTime to fileName
    strcat(fileName, ".csv");      // Concatenate ".csv" to fileName

    FILE *fp = fopen(fileName, "w");

    if (fp == NULL) {
        printf("Could not open file for writing.\n");
        return NULL;
    }
    return fp;
}

// Function to print the CSV header
int printCsvHeader(FILE *fp) {
    fprintf(fp, "Line #, Timestamp, Battery ID, Current (A), Voltage (V), State Of Charge (%%), Total Capacity, Remaining Capacity (AH),");
    
    // Print cell voltage headers
    for (int i = 1; i <= G_MAX_NUMBER_OF_CELLS; ++i) {
        fprintf(fp, "Cell Voltage %d (mV),", i);
    }
    
    fprintf(fp, "Highest Cell Voltage (mV), Lowest Cell Voltage (mV),");
    
    // Print temperature headers
    for (int i = 1; i <= G_MAX_NUMBER_OF_TEMP_SENSORS; ++i) {
        fprintf(fp, "Temperature %d (C),", i);
    }
    
    fprintf(fp, "Charging (1) Discharging (2) Status, Charging MOS Status, Discharging MOS Status, Balancing Status, Cell Balancing Status,");

    // Print alarm headers
    for (int i = 1; i <= 8; ++i) {
        fprintf(fp, "Alarm %d,", i);
    }

    fprintf(fp, "\n");  // New line at the end
}

// Function to output BMS data to CSV
int outputBMSDataToCsv(FILE *fp) {
    // Write data
    fprintf(fp, "%d, %s, %d, %.2f, %.2f, %.2f, %.2f, %.2f, ", 
            bmsData.lineNumber,
            bmsData.dateTime, 
            bmsData.batteryID, 
            bmsData.current, 
            bmsData.voltage, 
            bmsData.stateOfCharge, 
            bmsData.totalCapacity, 
            bmsData.remainingCapacity);
    bmsData.lineNumber++;

    for (int i = 0; i < G_MAX_NUMBER_OF_CELLS; i++) {
        if (i >= G_MAX_NUMBER_OF_CELLS) {
            fprintf(fp, " , ");
        } else {
            fprintf(fp, "%.2f, ", bmsData.cellVoltage[i]);
        }
    }

    fprintf(fp, "%.2f, %.2f, ", 
            bmsData.highestCellVoltage, 
            bmsData.lowestCellVoltage);

    for (int i = 0; i < G_MAX_NUMBER_OF_TEMP_SENSORS; i++) {
        if (i >= G_MAX_NUMBER_OF_TEMP_SENSORS) {
            fprintf(fp, " , ");
        } else {
            fprintf(fp, "%.2f, ", bmsData.temperatures[i]);
        }
    }

    fprintf(fp, "%d, %d, %d, %d, ", 
            bmsData.chargingDischargingStatus, 
            bmsData.chargingMOSStatus, 
            bmsData.dischargingMOSStatus, 
            bmsData.balancingStatus);

    // Combine cellBalancingStatus[i] into a single string
    char cellBalancingStr[G_MAX_NUMBER_OF_CELLS + 1]; // +1 for the null-terminator
    cellBalancingStr[G_MAX_NUMBER_OF_CELLS] = '\0'; // Null-terminate the string

    for (int i = 0; i < G_MAX_NUMBER_OF_CELLS; i++) {
        cellBalancingStr[i] = bmsData.cellBalancingStatus[i] + '0'; // '0' or '1'
    }

    // output the string to the csv file
    fprintf(fp, "\'%s\', ", cellBalancingStr);    

    for (int i = 0; i < 8; i++) {
        fprintf(fp, "\'%s\', ", bmsData.alarms[i]);
    }

    fprintf(fp, "\n");

    printf("Data written to csv file\n");

    return 0;
}
