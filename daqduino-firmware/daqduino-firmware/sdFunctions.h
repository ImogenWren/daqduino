/* SD card - Functions

 - If using classes these would be public methods

*/

#include "globals.h"
#include "sdUtil.h"

#pragma once


// Sets up the SD card - loops untill SD card is found.
// Flashes LED while looking for card to let user know no card is present


void SD_setup() {
  Serial.print("Initializing SD card...");
  int attempts = 0;
  while (!SD.begin(SD_SELECT_PIN)) {  // If the SD card does not begin loop untill SD card is found
    digitalWrite(LED_PIN, ledState);
    Serial.print("\nAttempt: ");
    Serial.print(attempts);
    Serial.println("  SD Card initialization failed!");
    attempts++;
    delay(500);
    if (ledState == HIGH) {
      ledState = LOW;
    } else {
      ledState = HIGH;
    }
    if (attempts > NUMBER_SD_ATTEMPTS) {  // If tried too many times then exit loop and start running program anyway
      sd_available = false;
      digitalWrite(LED_PIN, HIGH);  // remains high as sd is unavailable
      break;
    }
  }
  Serial.println("SD Card initialization done.");
  digitalWrite(LED_BUILTIN, LOW);  // turn the LED off
}


// function to scan SD card and find next unused number
// Future revisions could use RTC to label files with date
void logFile_setup() {
  root = SD.open("/");      // Open the root directory
  printDirectory(root, 0);  // Print the root directory
  Serial.print("No. Files Found: ");
  Serial.println(no_indexes);
  int fileExist = 0;  // this will hold the last file number found at the end of the loop, we can then add 1 to it for our new filename!
  if (no_indexes > 0) {
    // Serial.println(sizeof(fileNoArray));  // Debugging - ensure that size of array matches our expectations
    // NOTE: line above does not work to debug any longer as we have a fixed array size of 64! that means that after 64 logs everything will break! Not ideal, but pretty workable
    for (int i = 0; i < no_indexes; i++) {
      if (fileNoArray[i] > fileExist) {  // Only log the number if its higher than the last one found - this way it doesnt matter which order files are scanned in (BUGFIX)
        fileExist = fileNoArray[i];
      }
      // Serial.print(fileExist);             // Used for debugging not required now
      // Serial.println(" File Exists");
    }
  } else {
    fileExist = 0;  // no previous files found
  }
  makeLogFileName(fileExist + 1);  // Use last found number + 1 as new file name - saved as global variable
}


// Log passed string to SD card
void log_to_SD(char *dataBuffer) {  // must be *deferenced
  // Log Data to SD Card
  File logFile = SD.open(logFileName, FILE_WRITE);  // Current Benchmark test ~20 mS
  if (logFile) {
    logFile.write(dataBuffer);              // Must not be *deferenced
    size_t sizeoflog = sizeof(dataBuffer);  // Honestly dont know the status of referencing, nothing this returned seems to be correct any longer but the data is being logged!
    logFile.close();
    // UI message should be disableable
#ifdef SD_LOGGING_CONFIRM
    Serial.print(sizeoflog);
    Serial.println(" Bytes Logged to SD Card");
#endif
  } else {
    Serial.print(logFileName);
    Serial.println(" File Not Available");
    digitalWrite(LED_PIN, HIGH);
  }
}
