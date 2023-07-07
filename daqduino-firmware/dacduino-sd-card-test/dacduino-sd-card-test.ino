/*  DAQduino - SD Card Test

    Quickly test SD card functions to ensure correct formatting and wiring.



        The SD card can have 2 functions

        1. To log the data for later viewing
        2. To store config options (not required untill the MCU is able to control hardware options)


*/

#include <SPI.h>
#include <SD.h>

#define SD_SELECT_PIN 10

File logFile;

void SD_setup(){
 Serial.print("Initializing SD card...");
  if (!SD.begin(sdSelectpin)) {

    Serial.println("initialization failed!");

    noSDcard = true;
    display.setTextColor(1, 0);
    display.setCursor(0, 8);
    display.print("No SD Card");
    display.display();
    delay(500);
    // while (1);
    // break;
  }
  Serial.println("initialization done.");


}



void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
