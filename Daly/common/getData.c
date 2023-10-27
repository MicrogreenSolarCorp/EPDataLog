// File: getData.c
// Description: Implementation of functions for setting getting and parsing data for Daly BMS.
#include "getData.h"  // Include the corresponding header file
#include <stdio.h>
#include <time.h>
#include <string.h>

int g_number_of_battery_cells = -1;
int g_number_of_temp_sensors = -1;

BMSData bmsData; // Declare a BMSData struct variable
// Forward declarations of static functions
void getDateTime();
int getBMSData(CommHandle hComm, int requestType);
void parseBmsResponseSoc(unsigned char *pResponse);
void parseBmsResponseHighestLowestVoltage(unsigned char *pResponse);
void parseBmsResponseMaxMinTemp(unsigned char *pResponse);
void parseBmsResponseChargeDischargeMosStatus(unsigned char *pResponse);
void parseBmsResponseStatusInfo1(unsigned char *pResponse);
void parseBmsResponseSingleCellVoltage(unsigned char *pResponse);
void parseBmsResponseSingleCellTemp(unsigned char *pResponse);
void parseBmsResponseSingleCellBalancingStatus(unsigned char *pResponse);
void parseBmsResponseBatteryFailureStatus(unsigned char *pResponse);


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
    const int REQ_LEN = 13;
    const unsigned char REQUEST_TOTAL_VOLTAGE_CURRENT_SOC[13] = {0XA5, 0X40, 0X90, 0X08, 0X00, 0X00, 0x00, 0x00, 0X00, 0X00, 0X00, 0X00, 0x7D};
    const unsigned char REQUEST_HIGHEST_LOWEST_VOLTAGE[13] = {0XA5, 0X40, 0X91, 0X08, 0X00, 0X00, 0x00, 0x00, 0X00, 0X00, 0X00, 0X00, 0x7E};
    const unsigned char REQUEST_MAX_MIN_TEMP[13] = {0XA5, 0X40, 0X92, 0X08, 0X00, 0X00, 0x00, 0x00, 0X00, 0X00, 0X00, 0X00, 0x7F};
    const unsigned char REQUEST_CHARGE_DISCHARGE_MOS_STATUS[13] = {0XA5, 0X40, 0X93, 0X08, 0X00, 0X00, 0x00, 0x00, 0X00, 0X00, 0X00, 0X00, 0x80};
    const unsigned char REQUEST_STATUS_INFO_1[13] = {0XA5, 0X40, 0X94, 0X08, 0X00, 0X00, 0x00, 0x00, 0X00, 0X00, 0X00, 0X00, 0x81};
    const unsigned char REQUEST_SINGLE_CELL_VOLTAGE[13] = {0XA5, 0X40, 0X95, 0X08, 0X00, 0X00, 0x00, 0x00, 0X00, 0X00, 0X00, 0X00, 0x82};
    const unsigned char REQUEST_SINGLE_CELL_TEMP[13] = {0XA5, 0X40, 0X96, 0X08, 0X00, 0X00, 0x00, 0x00, 0X00, 0X00, 0X00, 0X00, 0x83};
    const unsigned char REQUEST_SINGLE_CELL_BALANCE_STATUS[13] = {0XA5, 0X40, 0X97, 0X08, 0X00, 0X00, 0x00, 0x00, 0X00, 0X00, 0X00, 0X00, 0x84};
    const unsigned char REQUEST_SINGLE_CELL_FAILURE_STATUS[13] = {0XA5, 0X40, 0X98, 0X08, 0X00, 0X00, 0x00, 0x00, 0X00, 0X00, 0X00, 0X00, 0x85};

    const unsigned char *pRequest;
    unsigned char pResponse[300];

    switch( requestType ) { 
        case READ_BAT_TOTAL_VOLTAGE_CURRENT_SOC:
            pRequest = REQUEST_TOTAL_VOLTAGE_CURRENT_SOC;
            break;
        case READ_BAT_HIGHEST_LOWEST_VOLTAGE:
            pRequest = REQUEST_HIGHEST_LOWEST_VOLTAGE;
            break;
        case READ_BAT_MAX_MIN_TEMP:
            pRequest = REQUEST_MAX_MIN_TEMP;
            break;
        case READ_BAT_CHARGE_DISCHARGE_MOS_STATUS:
            pRequest = REQUEST_CHARGE_DISCHARGE_MOS_STATUS;
            break;
        case READ_BAT_STATUS_INFO_1:
            pRequest = REQUEST_STATUS_INFO_1;
            break;
        case READ_BAT_SINGLE_CELL_VOLTAGE:
            pRequest = REQUEST_SINGLE_CELL_VOLTAGE;
            break;
        case READ_BAT_SINGLE_CELL_TEMP:
            pRequest = REQUEST_SINGLE_CELL_TEMP;
            break;
        case READ_BAT_SINGLE_CELL_BALANCE_STATUS:
            pRequest = REQUEST_SINGLE_CELL_BALANCE_STATUS;
            break;
        case READ_BAT_SINGLE_CELL_FAILURE_STATUS:
            pRequest = REQUEST_SINGLE_CELL_FAILURE_STATUS;
            break;
        default:
            return 0;
    }

    const int MAX_RETRY = 1;

    for (int i = 0; i < MAX_RETRY; i++) {
        int nBytesWritten, nBytesRead;

        #ifdef _WIN32
        DWORD bytesWritten;
        WriteFile(hComm, pRequest, REQ_LEN, &bytesWritten, NULL);
        nBytesWritten = bytesWritten;
        #else  // macOS and other Unix-like systems
        nBytesWritten = (int) write(hComm, pRequest, REQ_LEN);
        #endif

        if (nBytesWritten > 0) {
            printf("Data written to port, %d bytes\n", nBytesWritten);
        } else {
            printf("Could not write data to port\n");
        }

        #ifdef _WIN32
        DWORD bytesRead;
        ReadFile(hComm, pResponse, sizeof(pResponse), &bytesRead, NULL);
        nBytesRead = bytesRead;
        #else  // macOS and other Unix-like systems
        usleep(100000);  // Sleep for 100 ms to reduce CPU usage, adjust as necessary
        nBytesRead = (int) read(hComm, pResponse, sizeof(pResponse));
        usleep(100000);  // Sleep for 100 ms to reduce CPU usage, adjust as necessary
        #endif

        if (nBytesRead > 0) {
            printf("Data read from port: ");
            for (int i = 0; i < nBytesRead; i++) {
                printf("%02X ", pResponse[i]);
            }
            printf("\n");
        } else {
            printf("Could not read data from port\n");
        }

        switch( pResponse[2] ) {
            case READ_BAT_TOTAL_VOLTAGE_CURRENT_SOC:
                parseBmsResponseSoc(pResponse);
                break;
            case READ_BAT_HIGHEST_LOWEST_VOLTAGE:
                parseBmsResponseHighestLowestVoltage(pResponse);
                break;
            case READ_BAT_MAX_MIN_TEMP:
                parseBmsResponseMaxMinTemp(pResponse);
                break;
            case READ_BAT_CHARGE_DISCHARGE_MOS_STATUS:
                parseBmsResponseChargeDischargeMosStatus(pResponse);
                break;
            case READ_BAT_STATUS_INFO_1:
                parseBmsResponseStatusInfo1(pResponse);
                break;
            case READ_BAT_SINGLE_CELL_VOLTAGE:
                parseBmsResponseSingleCellVoltage(pResponse);
                break;
            case READ_BAT_SINGLE_CELL_TEMP:
                parseBmsResponseSingleCellTemp(pResponse);
                break;
            case READ_BAT_SINGLE_CELL_BALANCE_STATUS:
                parseBmsResponseSingleCellBalancingStatus(pResponse);
                break;
            case READ_BAT_SINGLE_CELL_FAILURE_STATUS:
                parseBmsResponseBatteryFailureStatus(pResponse);
                break;
            default:
                return 0;
        }
    }
    return 0;

}

void parseBmsResponseSoc(unsigned char *pResponse) {
    // Data bits start at index 4
    float cumulative_total_voltage = ((pResponse[4] << 8) + pResponse[5]) * 0.1f;
    float collect_total_voltage = ((pResponse[6] << 8) + pResponse[7]) * 0.1f;
    float current = (((pResponse[8] << 8) + pResponse[9]) - 30000) * 0.1f;

    float soc = ((pResponse[10] << 8) + pResponse[11]) * 0.1f;

    bmsData.voltage = cumulative_total_voltage;
    bmsData.current = current;
    bmsData.stateOfCharge = soc;

    printf("cumulative_total_voltage: %.2fV\n", cumulative_total_voltage);
    printf("collect_total_voltage: %.2fV\n", collect_total_voltage);
    printf("current: %.2fA\n", current);
    printf("soc: %.2f%%\n", soc);
}

void parseBmsResponseHighestLowestVoltage(unsigned char *pResponse) {
    // Data bits start at index 4
    float highest_single_voltage = (pResponse[4] << 8) + pResponse[5];
    int highest_voltage_cell_number = pResponse[6];
    float lowest_single_voltage = (pResponse[7] << 8) + pResponse[8];
    int lowest_voltage_cell_number = pResponse[9];

    bmsData.highestCellVoltage = highest_single_voltage;
    bmsData.lowestCellVoltage = lowest_single_voltage;

    printf("highest_single_voltage: %.2fmV\n", highest_single_voltage);
    printf("highest_voltage_cell_number: %d\n", highest_voltage_cell_number);
    printf("lowest_single_voltage: %.2fmV\n", lowest_single_voltage);
    printf("lowest_voltage_cell_number: %d\n", lowest_voltage_cell_number);
}

void parseBmsResponseMaxMinTemp(unsigned char *pResponse) {
    // Data bits start at index 4
    float max_temp = pResponse[4] - 40;
    int max_temp_cell_number = pResponse[5];
    float min_temp = pResponse[6] - 40;
    int min_temp_cell_number = pResponse[7];

    printf("max_temp: %.2fC\n", max_temp);
    printf("max_temp_cell_number: %d\n", max_temp_cell_number);
    printf("min_temp: %.2fC\n", min_temp);
    printf("min_temp_cell_number: %d\n", min_temp_cell_number);
}

void parseBmsResponseChargeDischargeMosStatus(unsigned char *pResponse) {
    // Data bits start at index 4
    int charge_discharge_status = pResponse[4];
    int mos_tube_charging_status = pResponse[5];
    int mos_tube_discharging_status = pResponse[6];
    int bms_life = pResponse[7];
    int remaining_capacity = (int)(((pResponse[8] << 24) | (pResponse[9] << 16) | (pResponse[10] << 8) | pResponse[11]) / 1000.0);

    bmsData.chargingDischargingStatus = charge_discharge_status;
    bmsData.chargingMOSStatus = mos_tube_charging_status;
    bmsData.dischargingMOSStatus = mos_tube_discharging_status;

    bmsData.remainingCapacity = remaining_capacity;

    printf("charge_discharge_status: %d\n", charge_discharge_status);
    printf("mos_tube_charging_status: %d\n", mos_tube_charging_status);
    printf("mos_tube_discharging_status: %d\n", mos_tube_discharging_status);
    printf("bms_life: %d\n", bms_life);
    printf("remaining_capacity: %dAH\n", remaining_capacity);
}

void parseBmsResponseStatusInfo1(unsigned char *pResponse) {
    // Data bits start at index 4
    int battery_strings = pResponse[4];
    int number_of_temperature = pResponse[5];
    int charger_status = pResponse[6];
    int load_status = pResponse[7];
    int states = pResponse[8];

    int DI1_state = (states >> 0) & 1;
    int DI2_state = (states >> 1) & 1;
    int DI3_state = (states >> 2) & 1;
    int DI4_state = (states >> 3) & 1;
    int DO1_state = (states >> 4) & 1;
    int DO2_state = (states >> 5) & 1;
    int DO3_state = (states >> 6) & 1;
    int DO4_state = (states >> 7) & 1;

    g_number_of_battery_cells = battery_strings;
    g_number_of_temp_sensors = number_of_temperature;

    printf("battery_strings: %d\n", battery_strings);
    printf("number_of_temperature: %d\n", number_of_temperature);
    printf("charger_status: %d\n", charger_status);
    printf("load_status: %d\n", load_status);
    printf("DI1_state: %d\n", DI1_state);
    printf("DI2_state: %d\n", DI2_state);
    printf("DI3_state: %d\n", DI3_state);
    printf("DI4_state: %d\n", DI4_state);
    printf("DO1_state: %d\n", DO1_state);
    printf("DO2_state: %d\n", DO2_state);
    printf("DO3_state: %d\n", DO3_state);
    printf("DO4_state: %d\n", DO4_state);
}

void parseBmsResponseSingleCellVoltage(unsigned char *pResponse) {
    // Data bits start at index 4

    float cell_voltages[G_MAX_NUMBER_OF_CELLS];

    const int MESSAGE_LENGTH = 13;
    const int MAX_FRAMES = 16;
    const int CELLS_PER_FRAME = 3;

    int nCellsRead = 0; // number of battery cells read
    int readIndex = 4;

    for (int i = 0; i < MAX_FRAMES; i++) {
        if (nCellsRead == g_number_of_battery_cells) {
            break;
        }

        // check if frame number is correct
        int frame_number = pResponse[4 + i * MESSAGE_LENGTH];
        if (frame_number != i + 1) { // Frames are 1-indexed. i is 0-indexed
            printf("Frame number incorrect\n");
            continue;
        }

        readIndex = i * MESSAGE_LENGTH + 4 + 1; // 4 for the start flage, bms address, command, and data length. 1 for byte 0 being the frame serial number

        for (int j = 0; j < CELLS_PER_FRAME; j++) {
            if (nCellsRead == g_number_of_battery_cells) {
                break;
            }
            cell_voltages[nCellsRead] = (pResponse[readIndex] << 8) + pResponse[readIndex + 1];

            bmsData.cellVoltage[nCellsRead] = cell_voltages[nCellsRead];

            printf("cell_voltages[%d]: %.2fmV\n", nCellsRead, cell_voltages[nCellsRead]);
            nCellsRead++;
            readIndex += 2;
        }
    }
}

void parseBmsResponseSingleCellTemp(unsigned char *pResponse) {
    int cell_temps[G_MAX_NUMBER_OF_CELLS];

    const int MESSAGE_LENGTH = 13;
    const int MAX_FRAMES = 3;
    const int CELLS_PER_FRAME = 7;
    const int TEMPERATURE_OFFSET = 40;

    int nRead = 0; // number of temperature sensors read
    // Data bits start at index 4
    int readIndex = 4;

    for (int i = 0; i < MAX_FRAMES; i++) {
        if (nRead == g_number_of_temp_sensors) {
            break;
        }

        // check if frame number is correct
        int frame_number = pResponse[4 + i * MESSAGE_LENGTH];
        if (frame_number != i + 1) { // Frames are 1-indexed. i is 0-indexed
            printf("Frame number incorrect\n");
            continue;
        }

        readIndex = i * MESSAGE_LENGTH + 4 + 1; // 4 for the start flage, bms address, command, and data length. 1 for byte 0 being the frame serial number

        for (int j = 0; j < CELLS_PER_FRAME; j++) {
            if (nRead == g_number_of_temp_sensors) {
                break;
            }
            cell_temps[nRead] = pResponse[readIndex] - TEMPERATURE_OFFSET;

            bmsData.temperatures[nRead] = cell_temps[nRead];

            printf("cell_temps[%d]: %dC\n", nRead, cell_temps[nRead]);
            nRead++;
            readIndex++;
        }
    }
}

void parseBmsResponseSingleCellBalancingStatus(unsigned char *pResponse) {
    // Data bits start at index 4
    // Balance Status' are stored in reverse order. I.e. balance_status[0] is the balance status of the last cell

    int isBalancing = 0;

    for (int i = g_number_of_battery_cells - 1; i >= 0; i--) {
        isBalancing = isBalancing || pResponse[4 + i];
        bmsData.cellBalancingStatus[i] = pResponse[4 + i];
    }

    for (int i = 0; i < g_number_of_battery_cells; i++) {
        printf("bmsData.cellBalancingStatus[%d]: %d\n", i, bmsData.cellBalancingStatus[i]);
    }


    bmsData.balancingStatus = isBalancing;
}

void parseBmsResponseBatteryFailureStatus(unsigned char *pResponse) {
    // Data bits start at index 4
    // Alarms' are stored in reverse bit order. I.e. alarms[i][0] is the alarm of the ith byte, last bit

    char alarms[8][9];  // 8 bytes, each with 8 bits + null-terminator
    
    for (int i = 0; i < 8; ++i) {
        int byte = pResponse[4 + i];  // start from pResponse[4]
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
