// File: constants.h
// Description: Globally defined constants used in the program. 
// Note: This file is global to all 4 versions of the software (Daly Windows, Daly MacOS, Mewyeah Windows, and Mewyeah MacOS).
// As a result, there are some other defined constants that are BMS specific that are stored in the individual header files like request codes in getData.h

# ifndef CONSTANTS_H
# define CONSTANTS_H

// Used in connection.c
#define DEFAULT_DELAY_TIME_MS 2000

#define MIN_DELAY_TIME 0
#define MAX_COM_PORT_NUMBER 20
#define MIN_COM_PORT_NUMBER 1

// Used in getData.c and outputToCsv.c
#define G_MAX_NUMBER_OF_CELLS       16
#define G_MAX_NUMBER_OF_TEMP_SENSORS 4

// Used in connection.c and readProgramParams.c
#define INVALID_COM_PORT_NUMBER -2
#define INVALID_DELAY_TIME_SUPPLIED -3
#define NO_COM_PORT_NUMBER_SUPPLIED -1
#define NO_DELAY_TIME_SUPPLIED -1
#define INVALID_PORT_VALUE -4

#endif // CONSTANTS_H
