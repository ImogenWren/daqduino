/* globals.h - All Global Variables

      - Declaired here to avoid not being available for specific sub scripts

*/


#pragma once    // Ensure that this file is only included once


// User Defined Variables/Consts
#define SD_SELECT_PIN 10
#define LED_PIN 5


// Log File Variables
//File logFile;             // SD file specific to logging this is now local variable - not needed here
char logName[9] = { "_log.txt" };  // File must use INT_xxxx.xxx filename structure for delimiting later
char logFileName[12];              // holds completed logfile name

// Root Directory Variables - used in setup to create new unique log_file name
File root;     // SD card root, used to look for existing files                               
int fileNoArray[64];  // Avoid the need for dynamic array INSTEAD:
int no_indexes = 0;    // Instead save the number of useful indexes seperately. Feels exceptionally clunky but it should work


int ledState = HIGH;    // Tracks the state of the LED (only used in setup)
uint32_t loop_startTime;  //used to track time elapsed from first sample
bool sd_available;   // Track availability of SD card through program, could be used for ensuring that serial output is enabled and data can be gathered even if SD card is broken/missing


int data = 0;  // variable used to stand in for real data
int entryNumber;  //track and record the number of datapoints recorded into each log


// Structure to allow passing of time char string in and out of functions
struct timeStruct{
  char time[13];
};

timeStruct timeStamp;  //global variable to track timestamp between functions