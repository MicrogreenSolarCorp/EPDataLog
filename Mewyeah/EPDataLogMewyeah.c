#include <windows.h>
#include <stdio.h>
#include <time.h>

#define READ_BAT_VOLTAGE		    0x9A
#define READ_BAT_OTHER		        0x9B
#define READ_BAT_BAL		        0x9C
#define READ_BAT_TEMP		        0x9D


#define G_MAX_NUMBER_OF_CELLS                       16
#define G_MAX_NUMBER_OF_TEMP_SENSORS                2

#define HMI_COMM_BAUD       38400 

// Global Vars
int g_number_of_battery_cells = -1;
int g_number_of_temp_sensors = -1;
int g_delay_time_ms = 2000;
const int MIN_DELAY_TIME = 0;
const int MAX_COM_PORT_NUMBER = 256;
const int MIN_COM_PORT_NUMBER = 1;

const int INVALID_COM_PORT_NUMBER = -2;
const int INVALID_DELAY_TIME_SUPPLIED = -3;
const int NO_COM_PORT_NUMBER_SUPPLIED = -1;
const int NO_DELAY_TIME_SUPPLIED = -1;

int readProgramParams(int argc, char *argv[]);
int isInteger(char *str);
HANDLE setupCOMPort(int portNumber);
HANDLE connectToCOMPort(const char *portName);
int getDateTime();
int getBMSData(HANDLE hComm, int requestType);

int parseBmsResponseVolt( char *pResponse, int len );
int parseBmsResponseOther( char *pResponse, int len );
int parseBmsResponseBal( char *pResponse, int len );
int parseBmsResponseTemp( char *pResponse, int len );

FILE *openCsvFile();
int printCsvHeader(FILE *fp);
int outputBMSDataToCsv(FILE *fp);

// BMS Data Structure
typedef struct {
    int lineNumber;
    char dateTime[20];  // Enough to hold "YYYY-MM-DD HH:MM:SS" and '\0'
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
    // Balance Status' are stored in reverse order. I.e. balance_status[0] is the balance status of the last cell
    int cellBalancingStatus[G_MAX_NUMBER_OF_CELLS];
    // Alarms' are stored in reverse bit order. I.e. alarms[i][0] is the alarm of the ith byte, last bit
    char alarms[8][9];
} BMSData;

BMSData bmsData; // Declare a BMSData struct variable


// Returns the supplied COM port number, or -1 if none was supplied
int readProgramParams(int argc, char *argv[]) {
    int suppliedComPortNumber = NO_COM_PORT_NUMBER_SUPPLIED;
    int suppliedDelayTime = NO_DELAY_TIME_SUPPLIED;

    // Try to read the COM port number from the command line
    for (int i = 1; i < argc; i++) {  // Starting at 1 to skip the program name itself
        if (strcmp(argv[i], "-c") == 0) {
            if (i + 1 < argc) {  // Make sure we don't go out of bounds
                if (!isInteger(argv[i + 1])) {
                    printf("Error: Invalid value for -c option. Aborting\n");
                    return INVALID_COM_PORT_NUMBER;
                }
                suppliedComPortNumber = atoi(argv[++i]);  // Convert next argument to integer and assign to t_value
                // Handle Error checking for com port number time
                if (suppliedComPortNumber < MIN_COM_PORT_NUMBER) {
                    printf("Error: COM port number cannot be less than %d. Aborting\n", MIN_COM_PORT_NUMBER);
                    suppliedComPortNumber = INVALID_COM_PORT_NUMBER;
                } else if (suppliedComPortNumber > MAX_COM_PORT_NUMBER) {
                    printf("Error: COM port number cannot be greater than %d. Aborting\n", MAX_COM_PORT_NUMBER);
                    suppliedComPortNumber = INVALID_COM_PORT_NUMBER;
                }
            } else {
                printf("Error: Missing value for -c option\n");
            }
    // Try to read the delay time from the command line
        } else if (strcmp(argv[i], "-t") == 0) {
            if (i + 1 < argc) {  // Make sure we don't go out of bounds
                if (!isInteger(argv[i + 1])) {
                    printf("Error: Invalid value for -t option\n");
                    continue;
                }
                suppliedDelayTime = atoi(argv[++i]);  // Convert next argument to integer and assign to c_value
                // Handle Error checking for delay time
                if (suppliedDelayTime < MIN_DELAY_TIME) {
                    printf("Error: Delay time cannot be less than %d. Aborting\n", MIN_DELAY_TIME);
                    return INVALID_DELAY_TIME_SUPPLIED;
                } else {
                    g_delay_time_ms = suppliedDelayTime;
                    printf("Success: Delay time of %dms set\n", g_delay_time_ms);
                }

            } else {
                printf("Error: Missing value for -t option\n");
            }
        }
    }

    if (suppliedDelayTime == NO_DELAY_TIME_SUPPLIED) {
        printf("No delay time supplied. Using default delay time of %dms\n", g_delay_time_ms);
    }

    return suppliedComPortNumber;

}

int isInteger(char *str) {
    char *endptr;
    strtol(str, &endptr, 10);
    if (endptr == str) {
        return 0;  // No conversion performed
    }
    if (*endptr != '\0') {
        return 0;  // String is partially a valid integer
    }
    return 1;
}

HANDLE setupCOMPort(int portNumber) {
    if (portNumber != -1) {
        char comPortName[10];  // Enough space for "COM" + up to 3 digits + null terminator
        sprintf(comPortName, "COM%d", portNumber);  // Format the COM port name
        printf("Attempting to use COM port %d\n", portNumber);
        HANDLE hComm = connectToCOMPort(comPortName);
        Sleep(500);
        if (hComm != INVALID_HANDLE_VALUE) {
            // Use pRequestVolt as queryData
            const unsigned char queryData[9] = {0XEB, 0X90, 0X1F, 0X04, 0X07, 0x9A, 0x00, 0x00, 0x79};
            DWORD bytesWritten;

            // Send a query
            if (!WriteFile(hComm, queryData, sizeof(queryData), &bytesWritten, NULL)) {
                printf("Error in writing to COM port. Aborting.\n");
                CloseHandle(hComm);
                return INVALID_HANDLE_VALUE;
            }

            unsigned char buffer[300];
            DWORD bytesRead;

            // Read the response
            if (!ReadFile(hComm, buffer, sizeof(buffer), &bytesRead, NULL)) {
                printf("Error in reading from COM port. Aborting.\n");
                CloseHandle(hComm);
                return INVALID_HANDLE_VALUE;
            }

            // Check the response
            if (buffer[0] == 0xEB && buffer[1] == 0x90 && buffer[2] == 0x1F) {
                printf("Found the target COM port: %s\n", comPortName);
                return hComm;
            }

            // Close the handle if this isn't the correct one
            CloseHandle(hComm);
        } else {
            printf("Error: Unable to open COM port %s. Aborting.\n", comPortName);
            return INVALID_HANDLE_VALUE;
        }
    } else {
        printf("No COM port supplied. Searching for a COM port... \n");
        for (int i = 1; i <= 256; ++i) {
            char comPortName[10];  // Enough space for "COM" + up to 3 digits + null terminator
            sprintf(comPortName, "COM%d", i);  // Format the COM port name
            HANDLE hComm = connectToCOMPort(comPortName);
            Sleep(500);
            if (hComm != INVALID_HANDLE_VALUE) {
                // Use pRequestVolt as queryData
                const unsigned char queryData[9] = {0XEB, 0X90, 0X1F, 0X04, 0X07, 0x9A, 0x00, 0x00, 0x79};
                DWORD bytesWritten;

                // Send a query
                if (!WriteFile(hComm, queryData, sizeof(queryData), &bytesWritten, NULL)) {
                    printf("Error in writing to COM port\n");
                    CloseHandle(hComm);
                    continue;
                }

                unsigned char buffer[300];
                DWORD bytesRead;

                // Read the response
                if (!ReadFile(hComm, buffer, sizeof(buffer), &bytesRead, NULL)) {
                    printf("Error in reading from COM port\n");
                    CloseHandle(hComm);
                    continue;
                }

                // Check the response
                if (buffer[0] == 0xEB && buffer[1] == 0x90 && buffer[2] == 0x1F) {
                    printf("Found the target COM port: %s\n", comPortName);
                    return hComm;
                }

                // Close the handle if this isn't the correct one
                CloseHandle(hComm);
            } else {
                printf("Unable to open COM port %s\n", comPortName);
            }
        }
    
    }
}

HANDLE connectToCOMPort(const char *portName) {
    printf("Trying port %s\n", portName);
    DCB dcbSerialParams = {0};
    COMMTIMEOUTS timeouts = {0};
    
    HANDLE hComm = CreateFile(portName,
                              GENERIC_READ | GENERIC_WRITE,
                              0,
                              NULL,
                              OPEN_EXISTING,
                              0,
                              NULL);

    if (hComm == INVALID_HANDLE_VALUE) {
        return INVALID_HANDLE_VALUE;
    }

    dcbSerialParams.DCBlength = sizeof(DCB);

    if (!GetCommState(hComm, &dcbSerialParams)) {
        printf("Error getting current DCB settings\n");
        CloseHandle(hComm);
        return INVALID_HANDLE_VALUE;
    }

    dcbSerialParams.BaudRate = HMI_COMM_BAUD;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity   = NOPARITY;

    if (!SetCommState(hComm, &dcbSerialParams)) {
        printf("Could not set serial port parameters\n");
        CloseHandle(hComm);
        return INVALID_HANDLE_VALUE;
    }

    timeouts.ReadIntervalTimeout         = 50;
    timeouts.ReadTotalTimeoutConstant    = 50;
    timeouts.ReadTotalTimeoutMultiplier  = 10;
    timeouts.WriteTotalTimeoutConstant   = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(hComm, &timeouts)) {
        printf("Could not set serial port timeouts\n");
        CloseHandle(hComm);
        return INVALID_HANDLE_VALUE;
    }
    return hComm;
}

int getDateTime() {
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


int getBMSData(HANDLE hComm, int requestType) {
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
    DWORD bytesWritten;

    for (int i = 0; i < MAX_RETRY; i++) {
        
        BOOL status = WriteFile(hComm, pRequest, REQ_LEN, &bytesWritten, NULL);

        if (status) {
            printf("Data written to port, %lu bytes\n", bytesWritten);
        } else {
            printf("Could not write data to port\n");
        }


        DWORD bytesRead;
        status = ReadFile(hComm, pResponse, sizeof(pResponse), &bytesRead, NULL);

        if (status) {
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

}

int parseBmsResponseVolt( char *pResponse, int len )
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
		if( (cellVolt > 0 ) && (cellVolt<lowVolt) || lowVolt==0)
		{
			lowVolt = cellVolt;
		}
	} // for()

    bmsData.highestCellVoltage = highVolt;
    bmsData.lowestCellVoltage = lowVolt;
    printf("highest_single_voltage: %.2fV\n", bmsData.highestCellVoltage);
    printf("lowest_single_voltage: %.2fV\n", bmsData.lowestCellVoltage);

} // parseBmsResponseVolt()


int parseBmsResponseBal( char *pResponse, int len )
{
    // Balancing Current is not needed in the data logger
} // parseBmsResponseBal()

int parseBmsResponseTemp( char *pResponse, int len )
{
	// len should be 191, response 0X9D, Temperature
    int temp, i;  

  	for( i = 0; i < G_MAX_NUMBER_OF_TEMP_SENSORS; i++ )
	{
		temp = (pResponse[10+i] & 0xFF) - 40;

        bmsData.temperatures[i] = temp;
        printf("cell_temps[%d]: %.0fC\n", i, bmsData.temperatures[i]);
	}
	return 1;    
} // parseBmsResponseTemp()


// Function to get a specific bit from a byte
int GETBIT(unsigned char byte, int index) {
    return (byte >> index) & 1;
}

int parseBmsResponseOther( char *pResponse, int len )
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

int parseBmsResponseBatteryFailureStatus(unsigned char *pResponse) {
    // Data bits start at index 4
    // Alarms' are stored in reverse bit order. I.e. alarms[i][0] is the alarm of the ith byte, last bit

    char alarms[8][9];  // 8 bytes, each with 8 bits + null-terminator
    
    for (int i = 0; i < 8; ++i) {
        int byte = pResponse[4 + i] & 0xFF;  // start from pResponse[4]
        char *currentString = alarms[i];
        
        for (int j = 7; j >= 0; --j) {  // loop through each bit in byte
            currentString[j] = ((byte >> j) & 1) ? '1' : '0';
        }
        currentString[8] = '\0';  // null-terminate the string
    }
    
    for (int i = 0; i < 8; ++i) {
        strcpy(bmsData.alarms[i], alarms[i]);
        printf("Binary string for byte %d: %s\n", i, alarms[i]);
    }
}

FILE *openCsvFile() {
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
    char fileName[30];

    strcpy(fileName, filePrefix);  // Initialize fileName with filePrefix
    strcat(fileName, dateTime);    // Concatenate dateTime to fileName
    strcat(fileName, ".csv");      // Concatenate ".csv" to fileName

    FILE *fp = fopen(fileName, "w");

    if (fp == NULL) {
        printf("Could not open file for writing.\n");
        return NULL;
    }
    return fp;
}

int printCsvHeader(FILE *fp) {
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

int outputBMSDataToCsv(FILE *fp) {
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

    return 0;
}


int main(int argc, char *argv[]) {
    bmsData.lineNumber = 1;
    int comPort = NO_COM_PORT_NUMBER_SUPPLIED;
    comPort = readProgramParams(argc, argv);
    if (comPort == INVALID_COM_PORT_NUMBER || comPort == INVALID_DELAY_TIME_SUPPLIED) {
        return 1; // Invalid COM port number or delay time supplied.
    }

    HANDLE hComm = setupCOMPort(comPort);

    if (hComm == INVALID_HANDLE_VALUE) return 1; // Could not open COM port.

    printf("Opening serial port successful\n");

    FILE *fp = openCsvFile();
    printCsvHeader(fp);

    while (1) {
        getDateTime();
        getBMSData(hComm, READ_BAT_VOLTAGE);
        getBMSData(hComm, READ_BAT_OTHER);
        getBMSData(hComm, READ_BAT_BAL);
        getBMSData(hComm, READ_BAT_TEMP);

        outputBMSDataToCsv(fp);
        Sleep(g_delay_time_ms);
    }

    // Close the file
    fclose(fp);
    // Close the COM port
    CloseHandle(hComm);
    return 0;
}
