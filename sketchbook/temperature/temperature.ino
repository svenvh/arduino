/* Temperature sensor (NTC) reader
 *
 * Connect a 10Kohm in series with the NTC.
 *
 * See also: http://www.ladyada.net/learn/sensors/thermistor.html
 */

// Some calibration data
// 572: 20.0
// 578: 19.4
// 595: 17.0

const int portNTC = A0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int value = analogRead(portNTC);
  Serial.println(value);
  delay(1000);
}
