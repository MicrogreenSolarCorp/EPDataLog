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

void getDateTime();
int getBMSData(CommHandle hComm, int requestType);
void parseBmsResponseVolt(unsigned char *pResponse, int len);
void parseBmsResponseOther(unsigned char *pResponse, int len);
void parseBmsResponseBal(unsigned char *pResponse, int len);
void parseBmsResponseTemp(unsigned char *pResponse, int len);

#endif // GETDATA_H
