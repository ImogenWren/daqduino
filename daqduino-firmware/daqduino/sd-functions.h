


#pragma once  // Ensures that header file is only included once


// include the SD library:
#include <SPI.h>
#include <SD.h>

#ifdef USE_SDFAT_LIB
#pragma message "Using SD-FAT Library"
#include <SdFat.h>
SdFat sd;
SdFile myFile;
#else
#pragma message "using Arduino SD Library"
#endif

// set up variables using the SD utility library functions:
Sd2Card card;
SdVolume volume;
SdFile root;

File logFile;
char logName[9] = { "_log.txt" };
char logFileName[12];


// build filename using number passed
void makeLogFileName(uint16_t logNumber) {
  sprintf(logFileName, "%02i%s", logNumber, logName);
  Serial.print("New File Created: ");
  Serial.println(logFileName);
}


void SDcardSetup() {
  pinMode(SD_SELECT_PIN, OUTPUT);
  Serial.print("\nInitializing SD card...");
  int attempt;
  // we'll use the initialization code from the utility libraries
  while (attempt < 10) {
#ifdef USE_SDFAT_LIB
    if (!card.init(SPI_HALF_SPEED, SD_SELECT_PIN))
#else
    if (!SD.begin(SD_SELECT_PIN))
#endif
    {
      Serial.print("Attempt: ");
      Serial.print(attempt);
      Serial.println("\n\ninitialization failed. Is SD card present?");
      delay(400);
      attempt++;
    } else {
      Serial.println("Wiring is correct and a card is present.");
      cardAvailable = true;
    }
  }
  if (!cardAvailable){
    Serial.println("Too Many Attempts - SD Logging disabled");
  }
}

