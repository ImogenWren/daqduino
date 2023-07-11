/*  DAQduino - SD Card Test

    Quickly test SD card functions to ensure correct formatting and wiring.



        The SD card can have 2 functions

        1. To log the data for later viewing
        2. To store config options (not required untill the MCU is able to control hardware options)

  Formatting SD Card:
      - Use FAT32


  ### User Options ###
  _Potential Options to Include_
  - no samples to log
  - logging time
  - time between data
  - logging trigger  


*/

/*
 Current Use:

    - Plug in formatted SD Card.
    - Reboot Unit
    - Ensure that new log file is being created.
    - Power down unit
    - Check SD for useable data

  Each time the arduino reboots it will start a new log with a new number. It should be able to detect the number of the last log created so it will always pick the next highest Number.
  there may be edge cases where this does not work!

  Number of logs limited to 64 by a static array in sdFunctions. Increase the size of the array to allow more logs, but given the nature of SD cards, and their limited lifespan, it
  would be best to copy logs into a safe place and delete them from SD card before it is likely to hit this limit.

*/

#include <SPI.h>
#include <SD.h>
#include "timeFunctions.h"


#define SD_SELECT_PIN 10
#include "sdFunctions.h"


File logFile;  // SD file specific to logging




uint32_t loop_startTime;

void setup() {
  Serial.begin(115200);                                 // Open Serial Communication
  Serial.println("\n\nDAQduino - SD Card - Test Log");  // Provide header for Command Line UI
  randomSeed(A0);                                       // Random Number Generator for random data
  SD_setup();                                           // Open SD Card Communications
  root = SD.open("/");
  printDirectory(root, 0);  // Print the root directory
  pinMode(LED_BUILTIN, OUTPUT);
  // function to scan SD card and find next unused number
  // Future revisions could use RTC to label files with date
  Serial.print("No. Files Found: ");
  Serial.println(no_indexes);
     int fileExist = 0;  // this will hold the last file number found at the end of the loop, we can then add 1 to it for our new filename!
  if (no_indexes > 0) {
    // Serial.println(sizeof(fileNoArray));  // Debugging - ensure that size of array matches our expectations
    // NOTE: line above does not work to debug any longer as we have a fixed array size of 64! that means that after 64 logs everything will break! Not ideal, but pretty workable
     for (int i = 0; i < no_indexes; i++) {
      fileExist = fileNoArray[i];
      // Serial.print(fileExist);             // Used for debugging not required now
      // Serial.println(" File Exists");
    }
  } else {
    fileExist = 0;                                     // no previous files found
  }
  makeLogFileName(fileExist + 1);  // Use last found number + 1 as new file name
  delay(2000);
  // Function to print data header here, both to serial and log file if required
  loop_startTime = millis();
  Serial.print("Loop Start mS: ");
  Serial.println(loop_startTime);
}








int data = 0;  // variable used to stand in for real data

int datapoint;  //track and record the number of datapoints recorded into each log

void loop() {

  // Find Elapsed Time for data logging
  timeStruct timeStamp = elapsedTime();  // Get timestamp - benchmarking test: 272 uS -> 0.2 mS


  //Serial.println(timeStamp.time);      // Test line for checking timestamping function is passing correct data

  data = random();  // Do data collection

  // Format Data for Logging
  char data_buffer[64];                                                       // buffer to hold complete char string for printing & writing to SD card
  sprintf(data_buffer, "%04i, %s, %6i,\n", datapoint, timeStamp.time, data);  // Convert int data to comma delimted string
  Serial.print(data_buffer);                                                  // Print data to serial monitor

  // uint32_t benchStart = micros();     // Place this line at the start of process to be benchmarked

  // Log Data to SD Card
  File logFile = SD.open(logFileName, FILE_WRITE);  // Current Benchmark test ~20 mS
  if (logFile) {
    logFile.write(data_buffer);
    size_t sizeoflog = sizeof(data_buffer);
    logFile.close();
    // UI message should be disableable
    Serial.print(sizeoflog);
    Serial.println(" Bytes Logged to SD Card\n");

  } else {
    Serial.println("File Not Available");
    digitalWrite(LED_BUILTIN, HIGH);
  }

  // Benchmarking printout for testing timing
  /* 
  uint32_t benchFinish = micros();                       // Place this line at the end of process to be benchmarked
  benchFinish = (benchFinish - benchStart);
  Serial.print("Benchmark time uS: ");
  Serial.print(benchFinish);
  Serial.print("  Time mS: ");
  Serial.println(benchFinish * 0.001);
*/
  datapoint++;
  delay(400);  // Delay for human readability (replace with timer later)
}
