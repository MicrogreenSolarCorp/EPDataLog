// File: outputToCsv.c
// Description: Implementation of functions for outputting BMS data to CSV.

#include "outputToCsv.h"  // Include the header file for this source file
#include "../../Mewyeah/common/getData.h"
#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <mach-o/dyld.h>
#include <time.h>
#include <limits.h>

// Function to open the CSV file
FILE *openCsvFile() {
    // Get the path to the executable
    char pathbuf[PATH_MAX + 1];
    uint32_t pathbufSize = sizeof(pathbuf);
    _NSGetExecutablePath(pathbuf, &pathbufSize);
    
    // Get the directory part of the path
    char *dirpath = dirname(pathbuf);
    
    // Get current date and time
    time_t raw_time;
    struct tm *tm_info;

    time(&raw_time);
    tm_info = localtime(&raw_time);

    char dateTime[14];
    // Store date and time in dateTime array
    snprintf(dateTime, sizeof(dateTime), "%02d%02d%02d_%02d%02d%02d", 
             (tm_info->tm_year + 1900) % 100, 
             tm_info->tm_mon + 1, 
             tm_info->tm_mday, 
             tm_info->tm_hour, 
             tm_info->tm_min, 
             tm_info->tm_sec);

    char filePrefix[7] = "EPData";    
    char fileName[PATH_MAX + 1];

    // Concatenate directory path, file prefix, date time, and file extension
    snprintf(fileName, sizeof(fileName), "%s/%s%s.csv", dirpath, filePrefix, dateTime);

    FILE *fp = fopen(fileName, "w");

    if (fp == NULL) {
        printf("Could not open file for writing.\n");
        return NULL;
    }
    return fp;
}

// Function to print the CSV header
void printCsvHeader(FILE *fp) {
    fprintf(fp, "Line #, Timestamp, Battery ID, Current (A), Voltage (V), State Of Charge (%%), Total Capacity, Remaining Capacity (AH),");
    
    // Print cell voltage headers
    for (int i = 1; i <= G_MAX_NUMBER_OF_CELLS; ++i) {
        fprintf(fp, "Cell Voltage %d (mV),", i);
    }
    
    fprintf(fp, "Highest Cell Voltage (mV), Lowest Cell Voltage (mV),");
    
    // Print temperature headers
    for (int i = 1; i <= G_MAX_NUMBER_OF_TEMP_SENSORS; ++i) {
        fprintf(fp, "Temperature %d (C),", i);
    }
    
    fprintf(fp, "Charging (1) Discharging (2) Status, Charging MOS Status, Discharging MOS Status, Balancing Status, Cell Balancing Status,");

    // Print alarm headers
    for (int i = 1; i <= 8; ++i) {
        fprintf(fp, "Alarm %d,", i);
    }

    fprintf(fp, "\n");  // New line at the end
}

// Function to output BMS data to CSV
void outputBMSDataToCsv(FILE *fp) {
    // Write data
    fprintf(fp, "%d, %s, %d, %.2f, %.2f, %.2f, %.2f, %.2f, ", 
            bmsData.lineNumber,
            bmsData.dateTime, 
            bmsData.batteryID, 
            bmsData.current, 
            bmsData.voltage, 
            bmsData.stateOfCharge, 
            bmsData.totalCapacity, 
            bmsData.remainingCapacity);
    bmsData.lineNumber++;

    for (int i = 0; i < G_MAX_NUMBER_OF_CELLS; i++) {
        if (i >= G_MAX_NUMBER_OF_CELLS) {
            fprintf(fp, " , ");
        } else {
            fprintf(fp, "%.2f, ", bmsData.cellVoltage[i]);
        }
    }

    fprintf(fp, "%.2f, %.2f, ", 
            bmsData.highestCellVoltage, 
            bmsData.lowestCellVoltage);

    for (int i = 0; i < G_MAX_NUMBER_OF_TEMP_SENSORS; i++) {
        if (i >= G_MAX_NUMBER_OF_TEMP_SENSORS) {
            fprintf(fp, " , ");
        } else {
            fprintf(fp, "%.2f, ", bmsData.temperatures[i]);
        }
    }

    fprintf(fp, "%d, %d, %d, %d, ", 
            bmsData.chargingDischargingStatus, 
            bmsData.chargingMOSStatus, 
            bmsData.dischargingMOSStatus, 
            bmsData.balancingStatus);

    // Combine cellBalancingStatus[i] into a single string
    char cellBalancingStr[G_MAX_NUMBER_OF_CELLS + 1]; // +1 for the null-terminator
    cellBalancingStr[G_MAX_NUMBER_OF_CELLS] = '\0'; // Null-terminate the string

    for (int i = 0; i < G_MAX_NUMBER_OF_CELLS; i++) {
        cellBalancingStr[i] = bmsData.cellBalancingStatus[i] + '0'; // '0' or '1'
    }

    // output the string to the csv file
    fprintf(fp, "\'%s\', ", cellBalancingStr);    

    for (int i = 0; i < 8; i++) {
        fprintf(fp, "\'%s\', ", bmsData.alarms[i]);
    }

    fprintf(fp, "\n");

    printf("Data written to csv file\n");

}
