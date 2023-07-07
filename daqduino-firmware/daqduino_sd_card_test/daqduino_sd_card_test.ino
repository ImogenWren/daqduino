/*  DAQduino - SD Card Test

    Quickly test SD card functions to ensure correct formatting and wiring.



        The SD card can have 2 functions

        1. To log the data for later viewing
        2. To store config options (not required untill the MCU is able to control hardware options)

  Formatting SD Card:

  


*/

#include <SPI.h>
#include <SD.h>

#define SD_SELECT_PIN 10

File logFile;

bool sd_available;

char logName[9] = { "_log.txt" };
char logFileName[12];


void SD_setup() {
  Serial.print("Initializing SD card...");
  if (!SD.begin(SD_SELECT_PIN)) {
    Serial.println("SD Card initialization failed!");
    sd_available = false;
    delay(500);
    // while (1);
    // break;
  }
  Serial.println("SD Card initialization done.");
}



void makeLogFileName(uint16_t logNumber) {
  sprintf(logFileName, "%02i%s", logNumber, logName);
  Serial.print("New File Created: ");
  Serial.println(logFileName);
}


void setup() {
  Serial.begin(115200);
  SD_setup();
  makeLogFileName(1);
}

void loop() {

}
