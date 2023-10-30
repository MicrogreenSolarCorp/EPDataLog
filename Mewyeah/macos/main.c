// File: main.c
// Description: Main file for Mewyeah MacOS program.

#include "../../common/readProgramParams.h"
#include "../../common/macos/connection.h"
#include "../../common/macos/outputToCsv.h"
#include "../common/getData.h"

#include <stdio.h>

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>


// Global variables:
#define BAUD_RATE 38400

// Use pRequestVolt as queryData
unsigned char connectionQueryData[] = {0XEB, 0X90, 0X1F, 0X04, 0X07, 0x9A, 0x00, 0x00, 0x79};
unsigned char expectedConnectionQueryResponse[] = {0XEB, 0X90, 0X1F};


int main(int argc, char *argv[]) {
    bmsData.lineNumber = 1;

    int serialPortNumber = NO_COM_PORT_NUMBER_SUPPLIED;
    int delayTimeMs = DEFAULT_DELAY_TIME_MS;
    int readProgramParamsReturnCode = readProgramParams(argc, argv, &serialPortNumber, &delayTimeMs);

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

    while (1) {
        getDateTime();
        getBMSData(fd, READ_BAT_VOLTAGE);
        getBMSData(fd, READ_BAT_OTHER);
        getBMSData(fd, READ_BAT_BAL);
        getBMSData(fd, READ_BAT_TEMP);

        outputBMSDataToCsv(fp);
        usleep(delayTimeMs * 1000); // usleep takes sleep time in us (1 millionth of a second)
    }

    // Close the file
    fclose(fp);
    // Close the COM port
    close(fd);
    return 0;
}
