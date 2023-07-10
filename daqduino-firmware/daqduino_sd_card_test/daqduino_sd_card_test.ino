/*  DAQduino - SD Card Test

    Quickly test SD card functions to ensure correct formatting and wiring.



        The SD card can have 2 functions

        1. To log the data for later viewing
        2. To store config options (not required untill the MCU is able to control hardware options)

  Formatting SD Card:


  

  ### User Options ###
  _Potential Options to Include_
  - no samples to log
  - logging time
  - time between data
  - logging trigger  


*/

#include <SPI.h>
#include <SD.h>

#define SD_SELECT_PIN 5

File logFile;

bool sd_available;

char logName[9] = { "_log.txt" };
char logFileName[12];


void SD_setup() {
  Serial.print("Initializing SD card...");
  int attempts;
 while (!SD.begin(SD_SELECT_PIN)) {
   Serial.print("Attempt: ");
   Serial.print(attempts);
    Serial.println(" SD Card initialization failed!");
    attempts++;
    delay(500);
  }
  Serial.println("SD Card initialization done.");
}


// build filename using number passed
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

int i;
void loop() {
 File logFile = SD.open(logFileName, FILE_WRITE);
  if (logFile) {
    logFile.write(i);
    size_t sizeoflog = sizeof(i);
    logFile.close();
    Serial.print(sizeoflog);
    Serial.println("Bytes Logged to SD Card");
  }
  delay(1000);
}
