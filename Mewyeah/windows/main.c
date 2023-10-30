// File: main.c
// Description: Main file for Mewyeah Windows program.

#include "../../common/readProgramParams.h"
#include "../../common/windows/connection.h"
#include "../../common/windows/outputToCsv.h"
#include "../common/getData.h"

#include <windows.h>
#include <stdio.h>


// Global variables:
#define BAUD_RATE 38400

// Use pRequestVolt as queryData
unsigned char connectionQueryData[] = {0XEB, 0X90, 0X1F, 0X04, 0X07, 0x9A, 0x00, 0x00, 0x79};
unsigned char expectedConnectionQueryResponse[] = {0XEB, 0X90, 0X1F};


int main(int argc, char *argv[]) {
    bmsData.lineNumber = 1;
    
    int comPortNumber = NO_COM_PORT_NUMBER_SUPPLIED;
    int delayTimeMs = DEFAULT_DELAY_TIME_MS;
    int readProgramParamsReturnCode = readProgramParams(argc, argv, &comPortNumber, &delayTimeMs);

    if (comPortNumber == INVALID_COM_PORT_NUMBER || comPortNumber == INVALID_DELAY_TIME_SUPPLIED) {
        return 1; // Invalid COM port number or delay time supplied.
    }

    HANDLE hComm = setupCOMPort(comPortNumber, BAUD_RATE, connectionQueryData, expectedConnectionQueryResponse, sizeof(connectionQueryData));

    if (hComm == INVALID_HANDLE_VALUE) return 1; // Could not open COM port.

    printf("Opening serial port successful\n");

    FILE *fp = openCsvFile();
    printCsvHeader(fp);

    while (1) {
        getDateTime();
        getBMSData(hComm, READ_BAT_VOLTAGE);
        getBMSData(hComm, READ_BAT_OTHER);
        getBMSData(hComm, READ_BAT_BAL);
        getBMSData(hComm, READ_BAT_TEMP);

        outputBMSDataToCsv(fp);
        Sleep(delayTimeMs);
    }

    // Close the file
    fclose(fp);
    // Close the COM port
    CloseHandle(hComm);
    return 0;
}
