// File: main.c
// Description: Main file for Daly MacOS program.

#include "../../common/getInput.h"
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
    int delayTimeMs = DEFAULT_DELAY_TIME_MS;
    if (getInput(&serialPortNumber, &delayTimeMs) == 0) {
        printf("Configuration: COM port number = %d, delay time = %d ms\n", serialPortNumber, delayTimeMs);
    } else {
        printf("Failed to read program parameters.\n");
        // Used to not immediately close the command window after the program is done.
        printf("Press Enter to close the window...");
        getchar();
        return 1;
    }

    int fd = setupSerialPort(serialPortNumber, BAUD_RATE, connectionQueryData, expectedConnectionQueryResponse, sizeof(connectionQueryData));
    if (fd == -1) {
        return 1;
    }

    printf("Opening serial port successful\n");

    FILE *fp = openCsvFile();
    printCsvHeader(fp);

    while (1) {
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
        usleep(delayTimeMs * 1000); // usleep takes sleep time in us (1 millionth of a second)
    }

    // Close the file
    fclose(fp);
    // Close the COM port
    close(fd);
    // Used to not immediately close the command window after the program is done.
    printf("Press Enter to close the window...");
    getchar();
    return 0;
}
