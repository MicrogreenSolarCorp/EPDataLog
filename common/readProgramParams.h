// File: readProgramParams.h
// Description: Headers of functions for reading program parameters.

#ifndef READPROGRAMPARAMS_H
#define READPROGRAMPARAMS_H

#include "../constants.h"

int isInteger(char *str);
int readProgramParams(int argc, char *argv[], int *serialPortNumber, int *delayTimeMs);

#endif // READPROGRAMPARAMS_H
