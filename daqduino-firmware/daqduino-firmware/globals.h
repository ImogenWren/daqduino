/* globals.h - All Global Variables

      - Declaired here to avoid not being available for specific sub scripts

*/


#pragma once    // Ensure that this file is only included once


// Hardware pin Definitions
#define VOLTAGE_SENSE_PIN A2 
#define SD_SELECT_PIN 10
#define LED_PIN 5


// User Defined options
#define BENCHMARK_SAMPLING  // print benchmark testing output for sampling function
#define BENCHMARK_LOOP      // print benchmark testing output for entire program loop  // use to check sample rate targets met
//#define PRINT_RAW_DATA     // Print all raw ADC samples before averaging (for debugging purposes)
#define SD_LOGGING_CONFIRM   // prints confirmation that data is logged to SD card
#define NUMBER_SD_ATTEMPTS 10   //number of attempts to establish connection to SD card

// Not Currently Implemented
//#define SERIAL_PRINT_TRUE  // Print data output to serial monitor (comment out for faster sampling)?
//#define LOG_TO_SD_CARD     // Log data to SD card




// Log File Variables
//File logFile;             // SD file specific to logging this is now local variable - not needed here
char logName[9] = { "_log.txt" };  // File must use INT_xxxx.xxx filename structure for delimiting later
char logFileName[12];              // holds completed logfile name

// Root Directory Variables - used in setup to create new unique log_file name
File root;     // SD card root, used to look for existing files                               
int fileNoArray[64];  // Avoid the need for dynamic array INSTEAD:
int no_indexes = 0;    // Instead save the number of useful indexes seperately. Feels exceptionally clunky but it should work


// Other Global Variables (private)
int ledState = HIGH;    // Tracks the state of the LED (only used in setup)
uint32_t loop_startTime;  //used to track time elapsed from first sample
bool sd_available = true;   // Track availability of SD card through program, could be used for ensuring that serial output is enabled and data can be gathered even if SD card is broken/missing
                            // Must start true - is turned false if max attempts is reached
int entryNumber;  //track and record the number of datapoints recorded into each log


// Structure to allow passing of time char string in and out of functions
struct timeStruct{
  char time[13];
};

timeStruct timeStamp;  //global variable to track timestamp between functions