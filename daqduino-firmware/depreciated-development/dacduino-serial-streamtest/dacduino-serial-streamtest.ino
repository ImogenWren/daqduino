/* DACduino Test Firmware

Spits out arbritary data for testing dacduino desktop/CLI apps


*/


void setup() {
  Serial.begin(115200);
  Serial.print("\nDataA,");
  //Serial.print("DataB,");
  Serial.println("");
  delay(300);
}


int dataA = 0;

void loop() {
  Serial.println(dataA);
  dataA++;
  delay(100);
}
