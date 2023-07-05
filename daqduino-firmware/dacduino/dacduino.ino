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


#define VOLTAGE_SENSE_PIN A2

// Use 1.074 for 1.1v reference
//#define ADC_TO_MV_SCALER 1.074
// Use 4.883 for 5v reference
//#define ADC_TO_MV_SCALER 4.883

#define ADC_VOLTS_SCALER 1.0

#define VOLT_AMP_SCALER  

void setup() {
  Serial.begin(115200);
  //analogReference(INTERNAL);

}

void loop() {

  char buffer[64];                                          // Buffer to hold printed values

  int adc_value = analogRead(VOLTAGE_SENSE_PIN);                // Take Reading

  float mV_float =  float(adc_value) * ADC_VOLTS_SCALER;     // Calculate mV measured by ADC

  float mA_float = mV_float / SHUNT_RESISTOR;                // Calculate mA through shunt resistor

  // Strings for converting floats for printing
  char mV_str[6];

  char mA_str[6];

// Convert floats to string as arduino does not like floats in sprintf
  dtostrf(mV_float, 4, 2, mV_str);
  dtostrf(mA_float, 4, 2, mA_str);

// Place all data into a single buffer & format for printing
  sprintf(buffer, "ADC: %4i, mV: %s, mA: %s ", adc_value, mV_str, mA_str);

// Print the buffer
  Serial.println(buffer);

// Delay to slow down reporting for human readability
  delay(500);
}
