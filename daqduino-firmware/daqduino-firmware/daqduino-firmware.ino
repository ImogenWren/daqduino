/*  DAQduino - Firmware - V1.0

          DAQduino is an open-source Data AQuisition (DAQ) Device
            designed to free researchers and students from
            the Tyranny of National Instruments and LabVIEW.

            It can be used as a standalone datalogger, saving data in CSV format to an SD card for later,
            or it can be interfaced with a veriety of software to gather real-time data and 
            share this with other devices 

    This is Firmware Version 1.0(beta).
        - Basic functionality is complete, however may have unknown bugs.
        - More functionality will be realised in later versions.

    Version 1.0
    Imogen Wren H.
    12/07/2023

  # Notes on SD Card Logging Functionality:
      Each time the arduino reboots it will start a new log with a new number. It should be able to detect the number of the last log created so it will always pick the next highest Number.
      there may be edge cases where this does not work!

      Number of logs limited to 64 by a static array in sdFunctions. Increase the size of the array to allow more logs, but given the nature of SD cards, and their limited lifespan, it
      would be best to copy logs into a safe place and delete them from SD card before it is likely to hit this limit to avoid data loss!

      Formatting SD Card:
          - Use FAT32


*/

/*
  ## First Use Instructions:
    - Plug in formatted SD Card.
    - Reboot Unit
    - Ensure that new log file is being created.
    - Power down unit
    - Check SD for useable data
    - Delete unused logs
    - Start Data AQuisition
*/

/* # DAQduino - Maths and Scaling
Initial Scaling - Based on "theory" and "maths"

          Volts per ADC = 4.89 mV
                        = 0.00489 V

          DAQduino internal amplifier is set up for 
          20 mA ~= 4.4 V output

          Therefore:
          scale:
          mA = 20/4.4(Vmeasured)
          mA = 4.54(Vmeasured)


##  First Test

          To account for DC offset in readings, measure difference between multimeter readings and DAQduino reporting.
          To Start: 
|         | ADC   |  Vreported  | Vmeasured  | Vdiff    | % diff   | mA reported | mA measured  | mA diff |
|---      |---    |---          |---         |---       |---       |---          |---           |---      |
| Lowest  | 162   |   0.792     |   0.772    |  0.02    |          | 3.596       | 2.11         | 1.486   |
|MidRange | 400   |   1.956     |   1.888    |  0.068   |          | 8.880       | 20.69        |-11.81   |  
| Highest | 632   |   3.090     |   2.977    |  0.113   |          | 14.031      | 23.47        |-9.439   |
|---      |---    |---          |---         |---       |---       |---          |---           |         |




## Actual Scale - Defintion Based on Measurements

    - This method of calibration should work for any sensor with a linear output, as long as measured voltage is accurate, and the true value used in calibration is known
          - e.g. if a pressor sensor voltage can be tested at a specific known pressure.

Step 1: Record the detected voltage and the sensor true value for minimum and maximum possible datapoints.
Step 2: Calculate range, divisions and offset for each set of data.
Step 3: Place Values into algorithm as indicated
Step 4: ...?? Profit ?

    - Use the following table to assist

|                        | Voltage (V)  |Sensor True Value (measured)|       |
|---                     |---           |---                         |---    |
|Min                     |	 3.295      |	     2.06	                 |  mA	 |
|Max                     |   4.298      |    	22.4	                 |  mA   |	
|Range (max - min)       |	 1.003      |     20.34                  |  mA   |
|ScaleFactor(Voltage/STV)|	 0.049		  |	     1.0                   | V/mA  |
|ScaleFactor(STV/Voltage)|	 1.0		    |     20.3                   | mA/v  |
|Offset                  |   3.295      |	     2.06                  |  mA   |	       	
					
      sensor_value = ((Vin - Voffset) * ScaleFactor(mA/V)) + mAoffset 			

*/

#pragma message "See globals.h for user configuration options"

// Nessissary Include Statements
#include <SPI.h>
#include <SD.h>
#include "globals.h"
#include "timeFunctions.h"
#include "sdFunctions.h"
#include "dataFunctions.h"


// All values required for translation of ADC values to actual sensor values
// Values found through mix of "theory" and experimentation
//#define ADC_VOLTS_SCALER 0.0048876  // Value Gained via maths for ADC scale calculation
//#define ADC_VOLTS_SCALER 0.0047341     // Value taking into account 3.14% average difference between top and bottom values
//#define ADC_VOLTS_SCALER 0.0045854     // wwinging it at this point (too low)
//#define ADC_VOLTS_SCALER 0.0046969       // still slightly too low
#define ADC_VOLTS_SCALER 0.0047339

#define mA_PER_VOLT 20.3
#define mA_OFFSET 2.06
#define V_OFFSET 3.295

/* Algorithm for Voltage to mA:

mA = (Vreading - V_OFFSET)* mA_PER_VOLT + mA_OFFSET

*/




// Structure is useful if more than 1 data point is being collected/processed as each data type can be added to structure without changing functions too much based on data types and amounts used!
// Define all data which should be logged/printed in this structure
struct dataStruct {
  float dataA;  // in this case will be voltage
  float dataB;  // In this case will be mA
};

dataStruct globalData;  // Create a global instance to share between functions easily

// Edit this function to change data headers
void logDataHeader() {
  char buffer[32];
  char entryNo[8] = { "Entry" };
  char timeStamp[10] = { "Timestamp" };
  char dataHeadingA[8] = { "Voltage" };
  char dataHeadingB[8] = { "mA" };
  sprintf(buffer, "%s, %s, %s, %s, \n", entryNo, timeStamp, dataHeadingA, dataHeadingB);  // make sure to include "\n for newline"
  Serial.println(buffer);
  log_to_SD(buffer);
}



// Edit this function to change what/how data is aquired and processed
void sampleData() {
  // int adc_value = analogRead(VOLTAGE_SENSE_PIN);  // Take single ADC Reading
  int adc_value = sampleAverage();  // Take 100 or (n) samples & average

  float V_float = float(adc_value) * ADC_VOLTS_SCALER;                // Calculate mV measured by ADC
  float mA_float = ((V_float - V_OFFSET) * mA_PER_VOLT) + mA_OFFSET;  // Calculate mA detected by op-amp

  globalData = { V_float, mA_float };  // This could be returned locally but for simplicity global variable is used
}




// Edit this function to change data logged
// This could just use the globalData variable, but I think there is value in showing how any data could be passed into this function to be logged
void logData(dataStruct localData) {
  // Function here to change floats to char strings if required (it will be!)  (could use function overloading to handle automatically - not tried yet)
  char V_str[16];
  char mA_str[16];
  dtostrf(localData.dataA, 4, 3, V_str);
  dtostrf(localData.dataB, 4, 3, mA_str);
  // Format Data for Logging
  char data_buffer[64];                                                                      // buffer to hold complete char string for printing & writing to SD card
  sprintf(data_buffer, "%04i, %s, %s, %s, \n", entryNumber, timeStamp.time, V_str, mA_str);  // Convert int data to comma delimted string
  Serial.print(data_buffer);                                                                 // Print data to serial monitor
  // Log Data to SD Card
  if (sd_available) {
    log_to_SD(data_buffer);
  }
  entryNumber++;  // increment entry number for next loop
}









// Setup function runs once
void setup() {
  Serial.begin(115200);                                 // Open Serial Communication
  Serial.println("\n\nDAQduino - Data AQuisition System");  // Provide header for Command Line UI
  pinMode(LED_PIN, OUTPUT);                             // Use the LED to flash when no SD card can be found. Also goes HIGH when file is unavailable
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
#ifdef BENCHMARK_LOOP
  uint32_t benchStart = micros();  // Place this line at the start of process to be benchmarked
#endif

  // Find Elapsed Time for data logging
  timeStamp = elapsedTime();  // Get timestamp - benchmarking test: 272 uS -> 0.2 mS
  //Serial.println(timeStamp.time);      // Test line for checking timestamping function is passing correct data

  sampleData();  // Returns data to globalData variable

  logData(globalData);  // Log & print data

#ifdef BENCHMARK_LOOP
#pragma message "Loop Benchmarking Active"
  // Benchmarking printout for testing timing #NOTE - This function call must be placed directly after process being benchmarked
  doBenchmarking(benchStart);  // Benchmarking for total loop (excluding benchmarking function) = 12.31mS
#endif


  delay(400);  // Delay for human readability (replace with timer later)
}
