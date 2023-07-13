
#pragma once

// returns a structure containing a string of the complete elapsed time
timeStruct elapsedTime(uint16_t loop_startTime = 0) {
  char timeChar[13];  // buffer to hold time from start of program
  uint32_t elapsed_millis = millis() - loop_startTime;
  uint32_t elapsed_seconds = elapsed_millis * 0.001;      //  Benchmarking with divisions ~ 124 uS // With multiplication ~68 uS
  uint32_t elapsed_minutes = elapsed_seconds * 0.0167;
  uint32_t elapsed_hours = elapsed_minutes * 0.0167;
  // Subtract each "bin" from the one above to clean the timestamp
  elapsed_millis = elapsed_millis - elapsed_seconds * 1000;
  elapsed_seconds = elapsed_seconds - elapsed_minutes * 60;
  elapsed_minutes = elapsed_minutes - elapsed_hours * 60;
  sprintf(timeChar, "%02lu:%02lu:%02lu.%03lu", elapsed_hours, elapsed_minutes, elapsed_seconds, elapsed_millis);
  timeStruct elapsed;
  strcpy(elapsed.time, timeChar);   // Hacky way of doing this? Now operating at ~ 272 uS for fetching timestamp process strcpy(copied, copyinto)
  return elapsed;
}



// Function to calculate local time elapsed for benchmarking purposes. Pass the start time of benchmarking in micros()
void doBenchmarking(uint32_t benchStart) {
  uint32_t benchFinish = micros();  // Place this line at the end of process to be benchmarked
  benchFinish = (benchFinish - benchStart);
  Serial.print("Benchmark | Time uS: ");
  Serial.print(benchFinish);
  Serial.print("  Time mS: ");
  Serial.println(benchFinish * 0.001);
  Serial.println("");  // make space at output
}


