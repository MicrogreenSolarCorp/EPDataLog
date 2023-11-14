#include "getData.h"  // Include the corresponding header file
#include <stdio.h>
#include <time.h>
#include <string.h>

BMSData bmsData; // Declare a BMSData struct variable

#define NUM_CURRENT_SAMPLES 10

int gPopulatedCellCount = 0;
static int currents[NUM_CURRENT_SAMPLES] = { 0 }; //Static int array for storing the latest 6 currents for getting the average current
int gCurrentCounter = 0;

// Forward declarations of functions
void getDateTime();
int getBMSData(CommHandle hComm, char *request);
int parseBmsResponse(unsigned char *pResponse);
unsigned int hex2int(unsigned char *a, unsigned int len);
void parseBmsCellVolt( unsigned char *pResponse, int len );
int getAveragedCurrent(int current);

unsigned int hex2int(unsigned char  *a, unsigned int len) //Length is in characters, not bytes
{
	int i;
	int val = 0;

    for(i=0;i<len;i++)
    {
		if(a[i] <= 57)  // 0-9
        	val += (a[i]-48)*(1<<(4*(len-1-i)));
       	else if( a[i] <= 70 )  // A-F
		  	val += (a[i]-55)*(1<<(4*(len-1-i)));
       	else  //a-f
        	val += (a[i]-87)*(1<<(4*(len-1-i)));
   }      

    return val;
}

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

int getBMSData(CommHandle hComm, char *request) {
    const int REQ_LEN = 10;
    unsigned char pResponse[300];


    const int MAX_RETRY = 3;

    for (int i = 0; i < MAX_RETRY; i++) {
        
        int nBytesWritten, nBytesRead;

        #ifdef _WIN32
        DWORD bytesWritten;
        WriteFile(hComm, request, REQ_LEN, &bytesWritten, NULL);
        nBytesWritten = bytesWritten;
        #else  // macOS and other Unix-like systems
        nBytesWritten = (int) write(hComm, request, REQ_LEN);
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

            parseBmsResponse(pResponse);
            break;

        } else {
            printf("Could not read data from port\n");
        }
    }
    return 0;
}

int parseBmsResponse(unsigned char *pResponse) {
	unsigned char pCommand[4]="";
	pCommand[0] = pResponse[5];
	pCommand[1] = pResponse[6];
	pCommand[2] = pResponse[7];
	pCommand[3] = pResponse[8];
	unsigned int pidCode = hex2int(pCommand, 4);

	int value;

 	 switch( pidCode )
 	 { 
   		case 0XF004:   // Relay Status
     		break;

   		case 0XF007:   // Populated Cell Count  IS NOT DISPLAYED
			value = hex2int(&pResponse[9], 2); 
			gPopulatedCellCount = value * 1; 
            // LocalWord[LW_POPULATED_CELL_COUNT] = gPopulatedCellCount;
     		break;

   		case 0XF00A:   // Pack Charge Current Limit
     		break;

   		case 0XF00B:   // Pack Discharge Current Limit
     		break;

   		case 0XF015:   // Signed Pack Current
			value = hex2int(&pResponse[9], 4); //Length is in characters, not bytes
			int current = (value - 32767) * (0.1 * 1);  //Apply Scaling then multiply by 100 for display decimals then subtract 32767 for signed
			int averageCurrent = getAveragedCurrent(current);
            printf("current: %.dA\n", current);

 			//Only display the average current every few times.
			if (gCurrentCounter == NUM_CURRENT_SAMPLES){
                bmsData.current = averageCurrent;
                printf("current: %.2fA\n", bmsData.current);
				gCurrentCounter = 0;
			}
			else {
				gCurrentCounter++;
			}

			if(current > 0 )
			{
                bmsData.chargingDischargingStatus = 1;
			}
			else
			{
				bmsData.chargingDischargingStatus = 2;
			}
            printf("charge_discharge_status: %d\n", bmsData.chargingDischargingStatus);
     		break;

   		case 0XF00D:   // Pack Voltage
			value = hex2int(&pResponse[9], 4); //Length is in characters, not bytes
            bmsData.voltage = (float) value * 0.1;
            printf("cumulative_total_voltage: %.2fV\n", bmsData.voltage);
     		break;

   		case 0XF00E:   // Pack Open Voltage
     		break;

   		case 0XF00F:   // Pack State of Charge
			value = hex2int(&pResponse[9], 2);
            bmsData.stateOfCharge = value * 0.5;
            printf("soc: %.2f%%\n", bmsData.stateOfCharge);
     		break;


   		case 0XF010:   // Pack Amphours
			value = hex2int(&pResponse[9], 4);            
            bmsData.remainingCapacity = value * (0.1 * 100) * bmsData.voltage / 1000;
            printf("remaining_capacity: %.0fAH\n", bmsData.remainingCapacity);
            break;

   		case 0XF011:   // Pack Resistance
     		break;

   		case 0XF012:   // Pack Depth of Discharge
     		break;

   		case 0XF013:   // Pack Health
     		break;

   		case 0XF028:   // Highest Pack Temperature
            printf("pResponse %s\n", pResponse);
			value = hex2int(&pResponse[9], 2);
            printf("value: %d\n", value);
            bmsData.temperatures[1] = value * (0.0001 * 1000);
            printf("highest_temperature: %.2fC\n", bmsData.temperatures[1]);
     		break;

   		case 0XF029:   // Lowest Pack Temperature
			value = hex2int(&pResponse[9], 2);
            bmsData.temperatures[0] = value * (0.0001 * 1000);
            printf("lowest_temperature: %.2fC\n", bmsData.temperatures[0]);
     		break;

   		case 0XF032:   // Low Cell Voltage
			value = hex2int(&pResponse[9], 4);
            bmsData.lowestCellVoltage = value * (0.0001 * 1000);
            printf("lowest_single_voltage: %.2fV\n", bmsData.lowestCellVoltage);
     		break;

   		case 0XF03E:   // Low Cell Voltage ID
     		break;

   		case 0XF033:   // High Cell Voltage
			value = hex2int(&pResponse[9], 4);
            bmsData.highestCellVoltage = value * (0.0001 * 1000);
            printf("highest_single_voltage: %.2fV\n", bmsData.highestCellVoltage);
     		break;

   		case 0XF03D:   // High Cell Voltage ID
     		break;

		case 0XF100:   // Cell Voltages 1-12
			parseBmsCellVolt( pResponse, 48 );  
     		break;

		case 0XF101:   // Cell Voltages 12-24
			parseBmsCellVolt( pResponse, 48 );  
     		break;

   	 	default:
			return 0;
	  }
	return 1;

}

//-----------------------------------------------------------------
// Parse the individual BMS Cell Voltages
// each cell voltage has four chars like "80E8" -> 33000 -> 3.300V
//-----------------------------------------------------------------
void parseBmsCellVolt( unsigned char *pResponse, int len )
{
	unsigned char *pLocalResponse;

	//Get PID of response
	unsigned char pCommand[4]="";
	pCommand[0] = pResponse[5];
	pCommand[1] = pResponse[6];
	pCommand[2] = pResponse[7];
	pCommand[3] = pResponse[8];
	unsigned int pidCode = hex2int(pCommand, 4);

	pLocalResponse = &pResponse[9]; //Strip out the first few non-response characters


	if (pidCode == 0XF100){ //If the response is for cells 1-12
		int i;
		int nCell = 12;
		if (gPopulatedCellCount < 12) {
			nCell = gPopulatedCellCount;
		}

		
  		for (i = 0; i < nCell; i++){
			int value = hex2int(&pLocalResponse[i*4], 4);
            bmsData.cellVoltage[i] = value * (0.0001 * 1000);
            printf("cell_voltages[%d]: %.0fmV\n", i, bmsData.cellVoltage[i]);
		} // for()		
	}
	else if (pidCode == 0XF101){ //If the response is for cells 13-24
		int i;
		int nCell = 12;
		if (gPopulatedCellCount > 12 && gPopulatedCellCount < 24) {
			nCell = gPopulatedCellCount - 12;

  			for (i = 0; i < nCell; i++){  //hardcoded 4
				int value = hex2int(&pLocalResponse[i*4], 4);
                bmsData.cellVoltage[12 + i] = value * (0.0001 * 1000);
                printf("cell_voltages[%d]: %.0fmV\n", i + 12, bmsData.cellVoltage[i + 12]);
			} // for()	
		}

	}



} // parseBmsCellVolt()


int getAveragedCurrent(int newCurrent){
	//Get the average of the three currents in the array
	int i;
	int currentSum = 0;
	for (i = 0; i < NUM_CURRENT_SAMPLES; i++){
		currentSum += currents[i];
	}
	float averageCurrent;

	averageCurrent = (currentSum + newCurrent) / (NUM_CURRENT_SAMPLES + 1);

	//Shift array values one spot to the left
	for (i = 0; i < NUM_CURRENT_SAMPLES - 1; i++){
		currents[i] = currents[i+1];
	}

	//Add the latest current to the end of the array
	currents[NUM_CURRENT_SAMPLES - 1] = newCurrent;

	return averageCurrent;
}
