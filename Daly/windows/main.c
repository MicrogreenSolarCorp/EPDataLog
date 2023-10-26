#include "connection.h"
#include "../common/getData.h"
#include <windows.h>
#include <stdio.h>


int main(int argc, char *argv[]) {
    bmsData.lineNumber = 1;
    int comPort = NO_COM_PORT_NUMBER_SUPPLIED;
    comPort = readProgramParams(argc, argv);
    if (comPort == INVALID_COM_PORT_NUMBER || comPort == INVALID_DELAY_TIME_SUPPLIED) {
        return 1; // Invalid COM port number or delay time supplied.
    }

    HANDLE hComm = setupCOMPort(comPort);

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
