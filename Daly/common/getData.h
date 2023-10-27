// File: getData.h
// Description: Headers of functions for setting getting and parsing data for Daly BMS.
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

#define READ_BAT_TOTAL_VOLTAGE_CURRENT_SOC		    0x90
#define READ_BAT_HIGHEST_LOWEST_VOLTAGE		        0x91
#define READ_BAT_MAX_MIN_TEMP		                0x92
#define READ_BAT_CHARGE_DISCHARGE_MOS_STATUS		0x93
#define READ_BAT_STATUS_INFO_1		                0x94
#define READ_BAT_SINGLE_CELL_VOLTAGE		        0x95
#define READ_BAT_SINGLE_CELL_TEMP		            0x96
#define READ_BAT_SINGLE_CELL_BALANCE_STATUS		    0x97 
#define READ_BAT_SINGLE_CELL_FAILURE_STATUS		    0x98

#define G_MAX_NUMBER_OF_CELLS       16
#define G_MAX_NUMBER_OF_TEMP_SENSORS 4


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
int getBMSData(CommHandle hComm, int requestType);
static void parseBmsResponseSoc(unsigned char *pResponse);
static void parseBmsResponseHighestLowestVoltage(unsigned char *pResponse);
static void parseBmsResponseMaxMinTemp(unsigned char *pResponse);
static void parseBmsResponseChargeDischargeMosStatus(unsigned char *pResponse);
static void parseBmsResponseStatusInfo1(unsigned char *pResponse);
static void parseBmsResponseSingleCellVoltage(unsigned char *pResponse);
static void parseBmsResponseSingleCellTemp(unsigned char *pResponse);
static void parseBmsResponseSingleCellBalancingStatus(unsigned char *pResponse);
static void parseBmsResponseBatteryFailureStatus(unsigned char *pResponse);

#endif // GETDATA_H
