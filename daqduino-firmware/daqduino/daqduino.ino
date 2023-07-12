/*   DAQduino - Firmware - V0.1

          DAQduino is an open-source Data AQuisition (DAQ) Device
            designed to free researchers and students from
            the Tyranny of National Instruments and LabVIEW.

            It can be used as a standalone datalogger, saving data to an SD card for later,
            or it can be interfaced with a veriety of software to gather real-time data and 
            share this with other devices 

    This is Firmware Version 0.1. It will have reduced functionality while the system is being developed.

    Version 0.1
    Imogen Wren H.
    05/07/2023

*/


/*  Initial Scaling - Based on "theory" and "maths"

          Volts per ADC = 4.89 mV
                        = 0.00489 V

          DAQduino internal amplifier is set up for 

          20 mA ~= 4.4 V output

          Therefore:

          scale:

          mA = 20/4.4(Vmeasured)
          mA = 4.54(Vmeasured)
*/

/*  First Test

          To account for DC offset in readings, measure difference between multimeter readings and DAQduino reporting.
          To Start: 
|         | ADC   |  Vreported  | Vmeasured  | Vdiff    | % diff   | mA reported | mA measured  | mA diff |
|---      |---    |---          |---         |---       |---       |---          |---           |---      |
| Lowest  | 162   |   0.792     |   0.772    |  0.02    |          | 3.596       | 2.11         | 1.486   |
|MidRange | 400   |   1.956     |   1.888    |  0.068   |          | 8.880       | 20.69        |-11.81   |  
| Highest | 632   |   3.090     |   2.977    |  0.113   |          | 14.031      | 23.47        |-9.439   |
|---      |---    |---          |---         |---       |---       |---          |---           |         |


*/

/* Actual Scale - Defintion Based on Measurements

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
|Max                     |  4.298	      |    	22.4	                 |  mA   |	
|Range (max - min)       |	 1.003      |     20.34                  |  mA   |
|ScaleFactor(Voltage/STV)|	 0.049		  |	     1.0                   | V/mA  |
|ScaleFactor(STV/Voltage)|	 1.0		    |     20.3                   | mA/v  |
|Offset                  |  3.295       |	     2.06                  |  mA   |	       	
					
      sensor_value = ((Vin - Voffset) * ScaleFactor(mA/V)) + mAoffset 			

*/

//#define ADC_VOLTS_SCALER 0.0048876  // Value Gained via maths for ADC scale calculation
//#define ADC_VOLTS_SCALER 0.0047341     // Value taking into account 3.14% average difference between top and bottom values
//#define ADC_VOLTS_SCALER 0.0045854     // wwinging it at this point (too low)
//#define ADC_VOLTS_SCALER 0.0046969       // still slightly too low
#define ADC_VOLTS_SCALER 0.0047339

#define mA_PER_VOLT 20.3
#define mA_OFFSET 2.06
#define V_OFFSET 3.295

/* Algorithm:

mA = (Vreading - V_OFFSET)* mA_PER_VOLT + mA_OFFSET

*/
#define VOLTAGE_SENSE_PIN A2
#define SD_SELECT_PIN 10


#define BENCHMARK_SAMPLING  // print benchmark testing output for sampling function
//#define BENCHMARK_LOOP      // print benchmark testing output for entire program loop  // use to check sample rate targets met
//#define PRINT_RAW_DATA     // Print all raw ADC samples before averaging (for debugging purposes)
#define SERIAL_PRINT_TRUE  // Print data output to serial monitor (comment out for faster sampling)?
#define LOG_TO_SD_CARD     // Log data to SD card

//#define USE_SDFAT_LIB  // User option to use SD FAT library instead of standard one (untested)
// If undefined then typical arduino library is used.

#include "sd-functions.h"


bool cardAvailable = false;  // must start true then will be made false if SD Cannot be found


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
  if (!cardAvailable) {
    Serial.println("Too Many Attempts - SD Logging disabled");
  }
}



// Function to take multiple samples of ADC and return average of all samples
int sampleAverage(int no_samples = 100) {
  uint32_t startTime = millis();  // Take start Time for Benchmarking

  // Take samples
  int dataArray[no_samples];
  for (int i = 0; i < no_samples; i++) {
    dataArray[i] = analogRead(VOLTAGE_SENSE_PIN);
  }
  int samplesTaken = sizeof(dataArray) / sizeof(dataArray[0]);  // Divide the size of the whole array by the size of 1 index to = the number of indexes



#ifdef BENCHMARK_SAMPLING
#pragma message "Sampling Benchmarking Active"
  // For Benchmarking
  uint32_t finishTime = millis();
  uint32_t benchTime = finishTime - startTime;
  Serial.print(samplesTaken);
  Serial.print(" Samples Taken in: ");
  Serial.print(benchTime);
  Serial.print(" mS");
  Serial.println();
#endif

#ifdef PRINT_RAW_DATA
#pragma message "Raw Data Output Enabled"
  for (int i = 0; i < no_samples; i++) {
    Serial.print(i);
    Serial.print("  ADC: ");
    Serial.println(dataArray[i]);
  }
#endif

  float average = 0;
  // Do averaging
  for (int i = 0; i < no_samples; i++) {
    average = average + dataArray[i];
  }
  average = average / no_samples;  // change this to use sizeof array
  int32_t int_av = round(average);
  return int_av;
}



void printData(int adc_value, float V_float, float mA_float) {
  char buffer[64];  // Buffer to hold printed values
    // Strings for converting floats for printing
  char V_str[16];
  char mA_str[16];
  // Convert floats to string as arduino does not like floats in sprintf
  dtostrf(V_float, 4, 3, V_str);
  dtostrf(mA_float, 4, 3, mA_str);
  // Place all data into a single buffer & format for printing
  sprintf(buffer, "ADC: %4i  |  V: %s  |  mA: %s ", adc_value, V_str, mA_str);
  // Print the buffer
  Serial.println(buffer);
}


void setup() {
  Serial.begin(115200);
  Serial.println("\n DAQduino - Data AQuisition System");
  //analogReference(INTERNAL); // Only used for analog voltage sampling <1
  SDcardSetup();
}



void loop() {


  // int adc_value = analogRead(VOLTAGE_SENSE_PIN);  // Take single ADC Reading
  int adc_value = sampleAverage();  // Take 100 or (n) samples & average

  float V_float = float(adc_value) * ADC_VOLTS_SCALER;                // Calculate mV measured by ADC
  float mA_float = ((V_float - V_OFFSET) * mA_PER_VOLT) + mA_OFFSET;  // Calculate mA detected by op-amp


  printData(adc_value, V_float, mA_float);  // print data to serial monitor



  // Delay to slow down reporting for human readability
  delay(485);  //TODO: Replace this with a proper timer and sample rate conversion that can be checked via benchmarking
  // Strech function - warn user if true samplerate does not match their selection

#ifdef BENCHMARK_LOOP
#pragma message "Loop Benchmarking Active"
#endif
}
