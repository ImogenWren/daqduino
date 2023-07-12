/*
 
 Data AQuisition and Processing Functions


*/


#include "globals.h"




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
