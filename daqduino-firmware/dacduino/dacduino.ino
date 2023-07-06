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


/*  Scaling

          Volts per ADC = 4.89 mV
                        = 0.00489 V

          DAQduino internal amplifier is set up for 

          20 mA ~= 4.4 V output

          Therefore:

          scale:

          mA = 20/4.4 V
          mA = 4.54 V
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

//#define ADC_VOLTS_SCALER 0.0048876  // Value Gained via maths for ADC scale calculation
//#define ADC_VOLTS_SCALER 0.0047341     // Value taking into account 3.14% average difference between top and bottom values
//#define ADC_VOLTS_SCALER 0.0045854     // wwinging it at this point (too low)
//#define ADC_VOLTS_SCALER 0.0046969       // still slightly too low
#define ADC_VOLTS_SCALER   0.0047339 

#define VOLT_AMP_SCALER 4.5

#define VOLTAGE_SENSE_PIN A2

// Use 1.074 for 1.1v reference
//#define ADC_TO_MV_SCALER 1.074
// Use 4.883 for 5v reference
//#define ADC_TO_MV_SCALER 4.883



void setup() {
  Serial.begin(115200);
  Serial.println("\n DAQduino - Data AQuisition System");
  //analogReference(INTERNAL);
}

void loop() {

  char buffer[64];  // Buffer to hold printed values

  int adc_value = analogRead(VOLTAGE_SENSE_PIN);  // Take Reading

  float V_float = float(adc_value) * ADC_VOLTS_SCALER;  // Calculate mV measured by ADC


  float mA_float = (V_float * VOLT_AMP_SCALER );  // Calculate mA through shunt resistor

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

  // Delay to slow down reporting for human readability
  delay(500);
}
