// File: main.c
// Description: Main file for Daly MacOS program.
#include "../../common/macos/connection.h"
#include "../../common/macos/outputToCsv.h"
#include "../common/getData.h"
#include <stdio.h>

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>


// Global variables:
#define BAUD_RATE 9600

// Use pRequestVolt as queryData
const unsigned char connectionQueryData[] = {0XA5, 0X40, 0X90, 0X08, 0X00, 0X00, 0x00, 0x00, 0X00, 0X00, 0X00, 0X00, 0x7D};
const unsigned char expectedConnectionQueryResponse[] = {0XA5, 0X01, 0X90};


int main(int argc, char *argv[]) {
    bmsData.lineNumber = 1;

    int serialPortNumber = NO_COM_PORT_NUMBER_SUPPLIED;
    serialPortNumber = readProgramParams(argc, argv);
    if (serialPortNumber == INVALID_COM_PORT_NUMBER || serialPortNumber == INVALID_DELAY_TIME_SUPPLIED) {
        return 1; // Invalid serial port number or delay time supplied.
    }

    int fd = setupSerialPort(serialPortNumber, BAUD_RATE, connectionQueryData, expectedConnectionQueryResponse, sizeof(connectionQueryData));
    if (fd == -1) {
        return 1;
    }

    printf("Opening serial port successful\n");

    FILE *fp = openCsvFile();
    printCsvHeader(fp);

    // while (1) {
    for (int i = 0; i < 3; i++) {
        getDateTime();
        getBMSData(fd, READ_BAT_TOTAL_VOLTAGE_CURRENT_SOC);
        getBMSData(fd, READ_BAT_HIGHEST_LOWEST_VOLTAGE);
        getBMSData(fd, READ_BAT_MAX_MIN_TEMP);
        getBMSData(fd, READ_BAT_CHARGE_DISCHARGE_MOS_STATUS);
        getBMSData(fd, READ_BAT_STATUS_INFO_1);
        getBMSData(fd, READ_BAT_SINGLE_CELL_VOLTAGE);
        getBMSData(fd, READ_BAT_SINGLE_CELL_TEMP);
        getBMSData(fd, READ_BAT_SINGLE_CELL_BALANCE_STATUS);
        getBMSData(fd, READ_BAT_SINGLE_CELL_FAILURE_STATUS);

        outputBMSDataToCsv(fp);
        usleep(g_delay_time_ms * 1000); // usleep takes sleep time in us (1 millionth of a second)
    }

    // Close the file
    fclose(fp);
    // Close the COM port
    close(fd);
    return 0;
}
