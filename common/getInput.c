// File: getInput.c
// Description: Implementation of functions for reading input from the user.

#include "getInput.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"

int getInput(int *serialPortNumber, int *delayTimeMs) {
    char useDefaultSettings;
    char inputBuffer[256];

    printf(ANSI_COLOR_RED "Would you like to use default settings? (y/n): " ANSI_COLOR_GREEN);
    scanf(" %c", &useDefaultSettings);
    printf(ANSI_COLOR_RESET);

    while(getchar() != '\n');  // Clear newline from buffer

    if (useDefaultSettings == 'y' || useDefaultSettings == 'Y') {
        *serialPortNumber = NO_COM_PORT_NUMBER_SUPPLIED;
        *delayTimeMs = DEFAULT_DELAY_TIME_MS;
        return 0;
    } else if (useDefaultSettings == 'n' || useDefaultSettings == 'N') {
        printf(ANSI_COLOR_RED "Enter custom port number (%d-%d) or press Enter for auto port detection: " ANSI_COLOR_GREEN, MIN_COM_PORT_NUMBER, MAX_COM_PORT_NUMBER);
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        printf(ANSI_COLOR_RESET);
        if (strlen(inputBuffer) > 1) {
            *serialPortNumber = atoi(inputBuffer);
            if (*serialPortNumber < MIN_COM_PORT_NUMBER || *serialPortNumber > MAX_COM_PORT_NUMBER) {
                printf("Error: COM port number must be between %d and %d. Aborting.\n", MIN_COM_PORT_NUMBER, MAX_COM_PORT_NUMBER);
                return -1;
            }
        } else {
            *serialPortNumber = NO_COM_PORT_NUMBER_SUPPLIED;
        }

        printf(ANSI_COLOR_RED "Enter logging interval time (s, >= %d) or press Enter for default (%d s): " ANSI_COLOR_GREEN, MIN_DELAY_TIME, DEFAULT_DELAY_TIME_MS / 1000);
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        printf(ANSI_COLOR_RESET);
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