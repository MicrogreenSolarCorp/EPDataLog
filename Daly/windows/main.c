// File: main.c
// Description: Main file for Daly Windows program.
#include "../../common/windows/connection.h"
#include "../../common/windows/outputToCsv.h"
#include "../common/getData.h"
#include <windows.h>
#include <stdio.h>


// Global variables:
#define BAUD_RATE 9600

// Use pRequestVolt as queryData
const unsigned char connectionQueryData[] = {0XA5, 0X40, 0X90, 0X08, 0X00, 0X00, 0x00, 0x00, 0X00, 0X00, 0X00, 0X00, 0x7D};
const unsigned char expectedConnectionQueryResponse[] = {0XA5, 0X01, 0X90};

int main(int argc, char *argv[]) {
    bmsData.lineNumber = 1;
    int comPort = NO_COM_PORT_NUMBER_SUPPLIED;
    comPort = readProgramParams(argc, argv);
    if (comPort == INVALID_COM_PORT_NUMBER || comPort == INVALID_DELAY_TIME_SUPPLIED) {
        return 1; // Invalid COM port number or delay time supplied.
    }

    HANDLE hComm = setupCOMPort(comPort, BAUD_RATE, connectionQueryData, expectedConnectionQueryResponse, sizeof(connectionQueryData));

    if (hComm == INVALID_HANDLE_VALUE) return 1; // Could not open COM port.

    printf("Opening serial port successful\n");

    FILE *fp = openCsvFile();
    printCsvHeader(fp);

    while (1) {
        getDateTime();
        getBMSData(hComm, READ_BAT_TOTAL_VOLTAGE_CURRENT_SOC);
        getBMSData(hComm, READ_BAT_HIGHEST_LOWEST_VOLTAGE);
        getBMSData(hComm, READ_BAT_MAX_MIN_TEMP);
        getBMSData(hComm, READ_BAT_CHARGE_DISCHARGE_MOS_STATUS);
        getBMSData(hComm, READ_BAT_STATUS_INFO_1);
        getBMSData(hComm, READ_BAT_SINGLE_CELL_VOLTAGE);
        getBMSData(hComm, READ_BAT_SINGLE_CELL_TEMP);
        getBMSData(hComm, READ_BAT_SINGLE_CELL_BALANCE_STATUS);
        getBMSData(hComm, READ_BAT_SINGLE_CELL_FAILURE_STATUS);

        outputBMSDataToCsv(fp);
        Sleep(g_delay_time_ms);
    }

    // Close the file
    fclose(fp);
    // Close the COM port
    CloseHandle(hComm);
    return 0;
}
