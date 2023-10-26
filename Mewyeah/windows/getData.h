// File: getData.h
// Description: Headers of functions for parsing BMS data.
#ifndef GETDATA_H
#define GETDATA_H

#include <windows.h>

#define READ_BAT_VOLTAGE 0x9A
#define READ_BAT_OTHER   0x9B
#define READ_BAT_BAL     0x9C
#define READ_BAT_TEMP    0x9D

#define G_MAX_NUMBER_OF_CELLS       16
#define G_MAX_NUMBER_OF_TEMP_SENSORS 2

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

int getDateTime();
int getBMSData(HANDLE hComm, int requestType);
static int parseBmsResponseVolt(char *pResponse, int len);
static int parseBmsResponseOther(char *pResponse, int len);
static int parseBmsResponseBal(char *pResponse, int len);
static int parseBmsResponseTemp(char *pResponse, int len);
static int parseBmsResponseBatteryFailureStatus(unsigned char *pResponse);

#endif // GETDATA_H
