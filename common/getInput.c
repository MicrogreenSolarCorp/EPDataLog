// File: getInput.c
// Description: Implementation of functions for reading input from the user.

#include "getInput.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int getInput(int *serialPortNumber, int *delayTimeMs) {
    char useDefaultSettings;
    char inputBuffer[256];

    printf("Would you like to use default settings? (y/n): ");
    scanf(" %c", &useDefaultSettings);

    while(getchar() != '\n');  // Clear newline from buffer

    if (useDefaultSettings == 'y' || useDefaultSettings == 'Y') {
        *serialPortNumber = NO_COM_PORT_NUMBER_SUPPLIED;
        *delayTimeMs = DEFAULT_DELAY_TIME_MS;
        return 0;
    } else if (useDefaultSettings == 'n' || useDefaultSettings == 'N') {
        printf("Enter custom port number (%d-%d) or press Enter for auto port detection: ", MIN_COM_PORT_NUMBER, MAX_COM_PORT_NUMBER);
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        if (strlen(inputBuffer) > 1) {
            *serialPortNumber = atoi(inputBuffer);
            if (*serialPortNumber < MIN_COM_PORT_NUMBER || *serialPortNumber > MAX_COM_PORT_NUMBER) {
                printf("Error: COM port number must be between %d and %d. Aborting.\n", MIN_COM_PORT_NUMBER, MAX_COM_PORT_NUMBER);
                return -1;
            }
        } else {
            *serialPortNumber = NO_COM_PORT_NUMBER_SUPPLIED;
        }

        printf("Enter logging interval time (s, >= %d) or press Enter for default (%d s): ", MIN_DELAY_TIME, DEFAULT_DELAY_TIME_MS / 1000);
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        if (strlen(inputBuffer) > 1) {
            *delayTimeMs = atoi(inputBuffer) * 1000; // Input in s, convert to ms
            if (*delayTimeMs < MIN_DELAY_TIME) {
                printf("Error: logging interval time must be greater than or equal to %d. Aborting.\n", MIN_DELAY_TIME);
                return -1;
            }
        } else {
            *delayTimeMs = DEFAULT_DELAY_TIME_MS;
        }

        return 0;
    } else {
        printf("Error: Invalid choice. Aborting.\n");
        return -1;
    }
}