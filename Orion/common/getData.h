// File: getData.h
// Description: Headers of functions for parsing BMS data.
#ifndef GETDATA_H
#define GETDATA_H

#ifdef _WIN32  // This is defined for both 32 and 64 bit Windows
#include <windows.h>
typedef HANDLE CommHandle;
#else  // macOS and other Unix-like systems
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
typedef int CommHandle;
#endif

#include "../../constants.h"

#define READ_RELAY_STATUS    	  			":0322F004\n"
#define READ_POPULATED_CELL_COUNT  			":0322F007\n"
#define READ_PACK_CHARGE_CURRENT_LIMIT  	":0322F00A\n"
#define READ_PACK_DISCHARGE_CURRENT_LIMIT   ":0322F00B\n"
#define READ_UNSIGNED_PACK_CURRENT		    ":0322F015\n"
#define READ_PACK_VOLTAGE       			":0322F00D\n"
#define READ_PACK_OPEN_VOLTAGE      		":0322F00E\n"
#define READ_PACK_STATE_OF_CHARGE     		":0322F00F\n"
#define READ_PACK_AMPHOURS     				":0322F010\n"
#define READ_PACK_RESISTANCE   				":0322F011\n"
#define READ_PACK_DEPTH_OF_DISCHARGE     	":0322F012\n"
#define READ_PACK_HEALTH     				":0322F013\n"
#define READ_HIGHEST_PACK_TEMPERTURE     	":0322F028\n"
#define READ_LOWEST_PACK_TEMPERTURE    		":0322F029\n"
#define READ_LOW_CELL_VOLTAGE    			":0322F032\n"
#define READ_LOW_CELL_VOLTAGE_ID   			":0322F03E\n"
#define READ_HIGH_CELL_VOLTAGE    			":0322F033\n"
#define READ_HIGH_CELL_VOLTAGE_ID   		":0322F03D\n"
#define READ_CELL_VOLTAGES_1_TO_12			":0322F100\n"
#define READ_CELL_VOLTAGES_13_TO_24			":0322F101\n"

typedef struct {
    int lineNumber;
    char dateTime[20];
    int batteryID;
    float current;
    float voltage;
    float stateOfCharge;
    float totalCapacity;
    float remainingCapacity;
    float cellVoltage[G_MAX_NUMBER_OF_CELLS];
    float highestCellVoltage;
    float lowestCellVoltage;
    float temperatures[G_MAX_NUMBER_OF_TEMP_SENSORS];
    int chargingDischargingStatus;
    int chargingMOSStatus;
    int dischargingMOSStatus;
    int balancingStatus;
    int cellBalancingStatus[G_MAX_NUMBER_OF_CELLS];
    char alarms[8][9];
} BMSData;

extern BMSData bmsData;

void getDateTime();
int getBMSData(CommHandle hComm, char *request);
int parseBmsResponse(unsigned char *pResponse);
unsigned int hex2int(unsigned char *a, unsigned int len);
void parseBmsCellVolt( unsigned char *pResponse, int len );
int getAveragedCurrent(int current);

#endif // GETDATA_H
