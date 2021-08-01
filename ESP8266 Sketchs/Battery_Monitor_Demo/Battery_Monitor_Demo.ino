 // put 120K Ohm in A0 pin
#include <ESP8266WiFi.h>
int adcr = 0;
float cal = 0.00423; // analog to voltage calibration value
void setup() {
  Serial.begin(115200);
}

void loop() {
  adcr = analogRead(A0);
  Serial.print("raw = ");
  Serial.println(adcr); // return raw analog value
  Serial.print("V = ");
  Serial.println(adcr*cal); // return value in Volt
  delay(200);
}
