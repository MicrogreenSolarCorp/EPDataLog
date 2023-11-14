// File: main.c
// Description: Main file for Mewyeah Windows program.

#include "../../common/getInput.h"
#include "../../common/windows/connection.h"
#include "../../common/windows/outputToCsv.h"
#include "../common/getData.h"

#include <windows.h>
#include <stdio.h>


// Global variables:
#define BAUD_RATE 9600

// Use pRequestVolt as queryData
unsigned char *connectionQueryData = ":0322F00D\n";
unsigned char *expectedConnectionQueryResponse = ":05";


int main(int argc, char *argv[]) {
    bmsData.lineNumber = 1;
    
    int comPortNumber = NO_COM_PORT_NUMBER_SUPPLIED;
    int delayTimeMs = DEFAULT_DELAY_TIME_MS;

    if (getInput(&comPortNumber, &delayTimeMs) == 0) {
        printf("Configuration: COM port number = %d, delay time = %d ms\n", comPortNumber, delayTimeMs);
    } else {
        printf("Failed to read program parameters.\n");

        // Used to not immediately close the command window after the program is done.
        printf("Press Enter to close the window...");
        getchar();

        return 1;
    }

    HANDLE hComm = setupCOMPort(comPortNumber, BAUD_RATE, connectionQueryData, expectedConnectionQueryResponse, strlen(connectionQueryData), ORION_BMS);

    if (hComm == INVALID_HANDLE_VALUE) return 1; // Could not open COM port.

    // Print handle
    printf("HCOMM: %d\n", hComm);

    printf("Opening serial port successful\n");

    FILE *fp = openCsvFile();
    printCsvHeader(fp);

    while (1) {
        getDateTime();
		for(int i = 0; i < 10; i++){ //Get current multiple times to calculate an accurate average current
			getBMSData(hComm, READ_UNSIGNED_PACK_CURRENT);
		}
        getBMSData(hComm, READ_POPULATED_CELL_COUNT);
		getBMSData(hComm, READ_PACK_VOLTAGE);
		getBMSData(hComm, READ_PACK_STATE_OF_CHARGE);
		getBMSData(hComm, READ_PACK_AMPHOURS);
		getBMSData(hComm, READ_HIGHEST_PACK_TEMPERTURE);
		getBMSData(hComm, READ_LOWEST_PACK_TEMPERTURE);
		getBMSData(hComm, READ_LOW_CELL_VOLTAGE);
		getBMSData(hComm, READ_HIGH_CELL_VOLTAGE);
		getBMSData(hComm, READ_CELL_VOLTAGES_1_TO_12);
		getBMSData(hComm, READ_CELL_VOLTAGES_13_TO_24);

        outputBMSDataToCsv(fp);
        Sleep(delayTimeMs);
    }

    // Close the file
    fclose(fp);
    // Close the COM port
    CloseHandle(hComm);

    // Used to not immediately close the command window after the program is done.
    printf("Press Enter to close the window...");
    getchar();

    return 0;
}
