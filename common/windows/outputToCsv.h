// File: outputToCsv.h
// Description: Headers of functions for outputting BMS data to CSV.
#ifndef OUTPUTTOCSV_H
#define OUTPUTTOCSV_H

#include <time.h>
#include <stdio.h>

// Function Prototypes
FILE *openCsvFile();
int printCsvHeader(FILE *fp);
int outputBMSDataToCsv(FILE *fp);

#endif // OUTPUTTOCSV_H
