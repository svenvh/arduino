/*
  Reads RPI-1031 tilt sensor and lights one of four LEDs based on current tilt.

  Sven van Haastregt, 2012
 */

// LED ports
int LEDpos0 = 10;
int LEDpos1 = 11;
int LEDpos2 = 12;
int LEDpos3 = 13;

// Tilt sensor data ports (S1 and S2)
int TILT_S1 = 2;
int TILT_S2 = 3;

void setup() {                
  pinMode(TILT_S1, INPUT);
  pinMode(TILT_S2, INPUT);
  pinMode(LEDpos0, OUTPUT);     
  pinMode(LEDpos1, OUTPUT);     
  pinMode(LEDpos2, OUTPUT);     
  pinMode(LEDpos3, OUTPUT);     
}

// Flashes all LEDs
void testLEDs() {
  digitalWrite(LEDpos0, HIGH);
  digitalWrite(LEDpos1, HIGH);
  digitalWrite(LEDpos2, HIGH);
  digitalWrite(LEDpos3, HIGH);
  delay(1000);              // wait for a second
  digitalWrite(LEDpos0, LOW);
  digitalWrite(LEDpos1, LOW);
  digitalWrite(LEDpos2, LOW);
  digitalWrite(LEDpos3, LOW);
  delay(1000);              // wait for a second
}

// Lights LED corresponding to position v (0 <= v <= 3)
void showPosition(int v) {
  digitalWrite(LEDpos0, LOW);
  digitalWrite(LEDpos1, LOW);
  digitalWrite(LEDpos2, LOW);
  digitalWrite(LEDpos3, LOW);
  
  if (v == 0) {
    digitalWrite(LEDpos0, HIGH);
  }
  else if (v == 1) {
    digitalWrite(LEDpos1, HIGH);
  }
  else if (v == 2) {
    digitalWrite(LEDpos2, HIGH);
  }
  else {
    digitalWrite(LEDpos3, HIGH);
  }
}

// Returns tilt (0 <= tilt <= 3)
int getTilt() {
  int s1 = digitalRead(TILT_S1);
  int s2 = digitalRead(TILT_S2);
  return (s1<<1) | s2;
}

void loop() {
  showPosition(getTilt());
}
