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
#include "globals.h"
#include "timeFunctions.h"
#include "sdFunctions.h"









// Edit this function to change data headers
void logDataHeader() {
  char buffer[32];
  char entryNo[8] = { "Entry" };
  char timeStamp[10] = { "Timestamp" };
  char dataHeading[8] = { "Data" };
  sprintf(buffer, "%s, %s, %s, \n", entryNo, timeStamp, dataHeading);  // make sure to include "\n for newline"
  Serial.println(buffer);
  log_to_SD(buffer);
}

// Edit this function to change data logged
void logData(uint32_t data) {
  // Function here to change floats to char strings if required (it will be!)  (could use function overloading to handle automatically - not tried yet)

  // Format Data for Logging
  char data_buffer[64];                                                         // buffer to hold complete char string for printing & writing to SD card
  sprintf(data_buffer, "%04i, %s, %6i,\n", entryNumber, timeStamp.time, data);  // Convert int data to comma delimted string
  Serial.print(data_buffer);                                                    // Print data to serial monitor
  entryNumber++;                                                                // increment entry number for next loop
  // Log Data to SD Card
  log_to_SD(data_buffer);
}


// Function to calculate local time elapsed for benchmarking purposes. Pass the start time of benchmarking in micros()
void doBenchmarking(uint32_t benchStart) {
  uint32_t benchFinish = micros();  // Place this line at the end of process to be benchmarked
  benchFinish = (benchFinish - benchStart);
  Serial.print("Benchmark | Time uS: ");
  Serial.print(benchFinish);
  Serial.print("  Time mS: ");
  Serial.println(benchFinish * 0.001);
  Serial.println("");  // make space at output
}


// Setup function runs once
void setup() {
  Serial.begin(115200);                                 // Open Serial Communication
  Serial.println("\n\nDAQduino - SD Card - Test Log");  // Provide header for Command Line UI
  pinMode(LED_PIN, OUTPUT);                             // Use the LED to flash when no SD card can be found. Also goes HIGH when file is unavailable
  randomSeed(A0);                                       // Random Number Generator for random data
  SD_setup();                                           // Open SD Card Communications
  logFile_setup();                                      // Sets up file system with new unique log number in the form XX_LOG.TXT

  delay(2000);
  // Function to print data header here, both to serial and log file if required
  logDataHeader();
  loop_startTime = millis();
  Serial.print("Loop Start mS: ");
  Serial.println(loop_startTime);
}






void loop() {
  uint32_t benchStart = micros();  // Place this line at the start of process to be benchmarked

  // Find Elapsed Time for data logging
  timeStamp = elapsedTime();  // Get timestamp - benchmarking test: 272 uS -> 0.2 mS
  //Serial.println(timeStamp.time);      // Test line for checking timestamping function is passing correct data


  data = random();  // Do data collection

  logData(data);  // Log & print data

  // Benchmarking printout for testing timing #NOTE - This function call must be placed directly after process being benchmarked
  doBenchmarking(benchStart);

  delay(400);  // Delay for human readability (replace with timer later)
}
