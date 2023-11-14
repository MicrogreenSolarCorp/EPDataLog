// File: main.c
// Description: Main file for Orion MacOS program.

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
unsigned char *connectionQueryData = (unsigned char *)":0322F00D\n";
unsigned char *expectedConnectionQueryResponse = (unsigned char *)":05";


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

    int fd = setupSerialPort(serialPortNumber, BAUD_RATE, connectionQueryData, expectedConnectionQueryResponse, sizeof(connectionQueryData), MEWYEAH_BMS);
    if (fd == -1) {
        return 1;
    }

    printf("Opening serial port successful\n");

    FILE *fp = openCsvFile();
    printCsvHeader(fp);

    while (1) {
        getDateTime();
		for(int i = 0; i < 10; i++){ //Get current multiple times to calculate an accurate average current
			getBMSData(fd, READ_UNSIGNED_PACK_CURRENT);
		}
        getBMSData(fd, READ_POPULATED_CELL_COUNT);
		getBMSData(fd, READ_PACK_VOLTAGE);
		getBMSData(fd, READ_PACK_STATE_OF_CHARGE);
		getBMSData(fd, READ_PACK_AMPHOURS);
		getBMSData(fd, READ_HIGHEST_PACK_TEMPERTURE);
		getBMSData(fd, READ_LOWEST_PACK_TEMPERTURE);
		getBMSData(fd, READ_LOW_CELL_VOLTAGE);
		getBMSData(fd, READ_HIGH_CELL_VOLTAGE);
		getBMSData(fd, READ_CELL_VOLTAGES_1_TO_12);
		getBMSData(fd, READ_CELL_VOLTAGES_13_TO_24);

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
