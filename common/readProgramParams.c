// File: readProgramParams.c
// Description: Implementation of functions for reading program parameters.

#include "readProgramParams.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// Returns -1 if there was an error reading params
int readProgramParams(int argc, char *argv[], int *serialPortNumber, int *delayTimeMs) {
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
                    *delayTimeMs = suppliedDelayTime;
                    printf("Success: Delay time of %dms set\n", *delayTimeMs);
                }

            } else {
                printf("Error: Missing value for -t option\n");
            }
        }
    }

    if (suppliedDelayTime == NO_DELAY_TIME_SUPPLIED) {
        printf("No delay time supplied. Using default delay time of %dms\n", *delayTimeMs);
    }

    *serialPortNumber = suppliedComPortNumber;
    return 0;
}


