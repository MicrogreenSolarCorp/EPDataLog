#include "getData.h"  // Include the corresponding header file
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <windows.h>

// Use cross platform code for COM port handling
#ifdef _WIN32  // This is defined for both 32 and 64 bit Windows
#include <windows.h>
typedef HANDLE CommHandle;
#else  // macOS and other Unix-like systems
#include <unistd.h>
#include <fcntl.h>
typedef int CommHandle;
#endif

BMSData bmsData; // Declare a BMSData struct variable
// Forward declarations of static functions
void getDateTime();
int getBMSData(CommHandle hComm, int requestType);
static void parseBmsResponseVolt(unsigned char *pResponse, int len);
static void parseBmsResponseOther(unsigned char *pResponse, int len);
static void parseBmsResponseBal(unsigned char *pResponse, int len);
static void parseBmsResponseTemp(unsigned char *pResponse, int len);

void getDateTime() {
    // Get current date and time
    time_t raw_time;
    struct tm *tm_info;

    time(&raw_time);
    tm_info = localtime(&raw_time);

    // Store date and time in BMSData struct
    snprintf(bmsData.dateTime, sizeof(bmsData.dateTime), "%04d-%02d-%02d %02d:%02d:%02d", 
             tm_info->tm_year + 1900, 
             tm_info->tm_mon + 1, 
             tm_info->tm_mday, 
             tm_info->tm_hour, 
             tm_info->tm_min, 
             tm_info->tm_sec);

    printf("\n\nCurrent Time: %s\n", bmsData.dateTime);
}

int getBMSData(CommHandle hComm, int requestType) {
    const int REQ_LEN = 9;
    const unsigned char pRequestVolt[9]  = {0XEB, 0X90, 0X1F, 0X04, 0X07, 0x9A, 0x00, 0x00, 0x79};
    const unsigned char pRequestOther[9] = {0XEB, 0X90, 0X1F, 0X04, 0X07, 0x9B, 0x00, 0x00, 0x78};
    const unsigned char pRequestBal[9]   = {0XEB, 0X90, 0X1F, 0X04, 0X07, 0x9C, 0x00, 0x00, 0x7F};	
    const unsigned char pRequestTemp[9]  = {0XEB, 0X90, 0X1F, 0X04, 0X07, 0x9D, 0x00, 0x00, 0x7E};
    const unsigned char *pRequest;
    unsigned char pResponse[300];


    switch( requestType ) { 
        case READ_BAT_VOLTAGE:
            pRequest = pRequestVolt;
            break;
        case READ_BAT_OTHER:
            pRequest = pRequestOther;
            break;
        case READ_BAT_BAL:
            pRequest = pRequestBal;
            break;
        case READ_BAT_TEMP:
            pRequest = pRequestTemp;
            break;
        default:
            return 0;
    }

    const int MAX_RETRY = 3;

    for (int i = 0; i < MAX_RETRY; i++) {
        
        int nBytesWritten, nBytesRead;

        #ifdef _WIN32
        DWORD bytesWritten;
        WriteFile(hComm, pRequest, REQ_LEN, &bytesWritten, NULL);
        nBytesWritten = bytesWritten;
        #else  // macOS and other Unix-like systems
        // REPLACE WITH MAC WRITE
        #endif

        if (bytesWritten > 0) {
            printf("Data written to port, %lu bytes\n", bytesWritten);
        } else {
            printf("Could not write data to port\n");
        }

        #ifdef _WIN32
        DWORD bytesRead;
        ReadFile(hComm, pResponse, sizeof(pResponse), &bytesRead, NULL);
        nBytesRead = bytesRead;
        #else  // macOS and other Unix-like systems
        // REPLACE WITH MAC WRITE
        #endif

        if (nBytesRead > 0) {
            printf("Data read from port: ");
            for (int i = 0; i < bytesRead; i++) {
                printf("%02X ", pResponse[i]);
            }
            printf("\n");

            switch( pResponse[7] ) {
                case READ_BAT_VOLTAGE:
                    parseBmsResponseVolt(pResponse, bytesRead);
                    break;
                case READ_BAT_OTHER:
                    parseBmsResponseOther(pResponse, bytesRead);
                    break;
                case READ_BAT_BAL:
                    parseBmsResponseBal(pResponse, bytesRead);
                    break;
                case READ_BAT_TEMP:
                    parseBmsResponseTemp(pResponse, bytesRead);
                    break;
                default:
                    return 0;
            }
            break;

        } else {
            printf("Could not read data from port\n");
        }
    }
    return 0;
}

// Define static functions; these functions are internal to this file
static void parseBmsResponseVolt( unsigned char *pResponse, int len )
{
	// len should be 251, response 0x9A, Volt
    int i;
    unsigned int lowByte, highByte, cellVolt;
	unsigned int highVolt=0, lowVolt=0;

    for (i = 0; i < G_MAX_NUMBER_OF_CELLS; i++){
        lowByte = pResponse[10 + (2*i)] & 0xFF;
        highByte = pResponse[11 + (2*i)] & 0xFF;
        highByte = highByte & 7;  			// use only 3 bits
        cellVolt = ((highByte*256 + lowByte)) * 2.5;  // this number should be divided by 1000 in the LW display		
        bmsData.cellVoltage[i] = cellVolt;
        printf("cell_voltages[%d]: %.0fmV\n", i, bmsData.cellVoltage[i]);

		if( cellVolt > highVolt )
		{
			highVolt = cellVolt;
		}
		if( ((cellVolt > 0 ) && (cellVolt<lowVolt)) || lowVolt==0)
		{
			lowVolt = cellVolt;
		}
	} // for()

    bmsData.highestCellVoltage = highVolt;
    bmsData.lowestCellVoltage = lowVolt;
    printf("highest_single_voltage: %.2fV\n", bmsData.highestCellVoltage);
    printf("lowest_single_voltage: %.2fV\n", bmsData.lowestCellVoltage);

} // parseBmsResponseVolt()


static void parseBmsResponseBal( unsigned char *pResponse, int len )
{
    // Balancing Current is not needed in the data logger
} // parseBmsResponseBal()

static void parseBmsResponseTemp( unsigned char *pResponse, int len )
{
	// len should be 191, response 0X9D, Temperature
    int temp, i;  

  	for( i = 0; i < G_MAX_NUMBER_OF_TEMP_SENSORS; i++ )
	{
		temp = (pResponse[10+i] & 0xFF) - 40;

        bmsData.temperatures[i] = temp;
        printf("cell_temps[%d]: %.0fC\n", i, bmsData.temperatures[i]);
	}
} // parseBmsResponseTemp()


// Function to get a specific bit from a byte
static int GETBIT(unsigned char byte, int index) {
    return (byte >> index) & 1;
}

static void parseBmsResponseOther( unsigned char *pResponse, int len )
{
	// len should be 119, response 0X9B, Other Data
	int iValue, i;
	unsigned int highByte, lowByte, uValue;
	
	//-----------------------------------------------------
	// Bat total voltage, byte 36 and 37
    lowByte  = pResponse[36] & 0xFF; // Mask out any sign extension
	highByte = pResponse[37] & 0xFF;
	uValue = (highByte*256 + lowByte);  // this voltage value should be divided by 100 in LW display
    float voltage = uValue / 10.0;
    bmsData.voltage = voltage;
    printf("cumulative_total_voltage: %.2fV\n", bmsData.voltage);

	//-----------------------------------------------------
	// Highest Cell Volt, byte 38 and 39
	lowByte  = pResponse[38] & 0xFF;
	highByte = pResponse[39] & 0xFF;
	uValue = highByte*256 + lowByte;  // this voltage value should be divided by 10 in LW display
	// LocalWord[LW_LCD_CELL_VOLT_MAX] = uValue;

	//-----------------------------------------------------
	// Lowest Cell Volt, byte 40 and 41
	lowByte  = pResponse[40] & 0xFF;
	highByte = pResponse[41] & 0xFF;
	uValue = highByte*256 + lowByte;  // this voltage value should be divided by 10 in LW display
	// LocalWord[LW_LCD_CELL_VOLT_MIN] = uValue;

	//-----------------------------------------------------
	// Bat Current, byte 34 and 35
	lowByte  = pResponse[34] & 0xFF; // Mask out any sign extension
	highByte = pResponse[35] & 0xFF;

	uValue = highByte*256 + lowByte;
	iValue = (int) uValue;

    bmsData.current = ((float) iValue - 32000.0) * -0.1;
    printf("current: %.2fA\n", bmsData.current);

    // Charging (1) Discharging (2)
	if( iValue < 0 )
	{
        bmsData.chargingDischargingStatus = 1;
	}
	else
	{
        bmsData.chargingDischargingStatus = 2;
	}

    printf("charge_discharge_status: %d\n", bmsData.chargingDischargingStatus);

	//-----------------------------------------------------
	// Battery SOC, byte 22
	uValue  = (pResponse[22] & 0xFF) * 0.4; // Mask out any sign extension

    bmsData.stateOfCharge = uValue;

    printf("soc: %.2f%%\n", bmsData.stateOfCharge);

	//-----------------------------------------------------
	// Battery remaining energy KWH, byte 54 and 51
	lowByte  = pResponse[54] & 0xFF;
	highByte = pResponse[51] & 0xFF;
	float kWH = (float)((highByte*256 + lowByte) / 100.0);  // this value should be divided by 100 in LW display
    
    // Convert voltage to kilovolts (kV) for consistency with kWh
    float voltage_kV = bmsData.voltage / 1000.0;

    // Now, calculate the charge in Ah
    int charge_Ah = (int)(kWH / voltage_kV);

    bmsData.remainingCapacity = charge_Ah;
    printf("remaining_capacity: %.0fAH\n", bmsData.remainingCapacity);

	//-----------------------------------------------------
	// Battery Higest Temperature
	iValue  = (pResponse[28] & 0xFF) - 40;
	// LocalWord[LW_LCD_TEMP_MAX] = iValue;

	//-----------------------------------------------------
	// Number of Temperature, byte 10
	uValue  = (pResponse[10] & 0xFF);
	// LocalWord[LW_LCD_NUM_TEMPS] = uValue;


	//-----------------------------------------------------
	// Alarm Status: 
	// Sever Alarm:  Status 1, byte 23;		Status 3, byte 55
	// Normal Alarm: Status 2, byte 33;     Status 4, byte 56
	short b;	

    char alarms[8][9];  // 8 bytes, each with 8 bits + null-terminator


	for( i=0; i<8; i++ )
	{

		// Sever Alarm 1
		b = GETBIT( pResponse[23] & 0xFF, i );
        alarms[0][i] = '0' + b; // Convert 0 or 1 to character '0' or '1'

		// Normal Alarm 2
		b = GETBIT( pResponse[33] & 0xFF, i );
        alarms[1][i] = '0' + b;

		// Sever Alarm 3
		b = GETBIT( pResponse[55] & 0xFF, i );
        alarms[2][i] = '0' + b;

		// Normal Alarm 4
		b = GETBIT( pResponse[56] & 0xFF, i );
        alarms[3][i] = '0' + b;

        alarms[i][8] = '\0';  // null-terminate the string

	} // for()


    // Mewyeah only has 4 Alarm Statuses, so we set the last 4 to '00000000'
    for (int i = 4; i < 8; ++i) {
        strcpy(alarms[i], "00000000");
    }

    for (int i = 0; i < 8; ++i) {
        strcpy(bmsData.alarms[i], alarms[i]);
        printf("Binary string for byte %d: %s\n", i, bmsData.alarms[i]);
    }

} // parseBmsResponseOther()
