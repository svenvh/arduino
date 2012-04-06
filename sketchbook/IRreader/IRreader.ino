/* InfraRed reader
 * 
 * Reads digital output of an IR receiver, transforms the pulses into
 * a 32-bit word and writes this word in hex to the serial output.
 *
 * See also: http://www.ladyada.net/learn/sensors/ir.html
 */

#define IRpin_PIN      PIND
#define IRpin          2

#define MAXPULSE 10000

// what our timing resolution should be, larger is better
// as its more 'precise' - but too large and you wont get
// accurate timing
#define RESOLUTION 20 

// Pulse length (usec) which we consider as start of a new command
#define THRESHOLD_START 10000

// Pulse length (usec) which distinguishes short signals from long
#define THRESHOLD_SHORT_LONG 1200

// we will store up to 100 pulse pairs (this is -a lot-)
uint16_t pulses[100][2];  // pair is high and low pulse 
uint8_t currentpulse = 0; // index for pulses we're storing


void setup() {
  Serial.begin(9600);
}

void loop() {
  uint16_t highpulse, lowpulse;  // temporary storage timing
  highpulse = lowpulse = 0; // start out with no pulse length

//  while (digitalRead(IRpin)) { // this is too slow!
    while (IRpin_PIN & (1 << IRpin)) {
     // pin is still HIGH

     // count off another few microseconds
     highpulse++;
     delayMicroseconds(RESOLUTION);

     // If the pulse is too long, we 'timed out' - either nothing
     // was received or the code is finished, so print what
     // we've grabbed so far, and then reset
     if ((highpulse >= MAXPULSE) && (currentpulse != 0)) {
       printpulses();
       currentpulse=0;
       return;
     }
  }
  // we didn't time out so lets stash the reading
  pulses[currentpulse][0] = highpulse;

  // same as above
  while (! (IRpin_PIN & _BV(IRpin))) {
     // pin is still LOW
     lowpulse++;
     delayMicroseconds(RESOLUTION);
     if ((lowpulse >= MAXPULSE)  && (currentpulse != 0)) {
       printpulses();
       currentpulse=0;
       return;
     }
  }
  pulses[currentpulse][1] = lowpulse;

  // we read one high-low pulse successfully, continue!
  currentpulse++;
}

void printpulses(void) {
  /*Serial.println("\n\r\n\rReceived: \n\rOFF \tON");
  for (uint8_t i = 0; i < currentpulse; i++) {
    Serial.print(pulses[i][0] * RESOLUTION, DEC);
    Serial.print(" usec, ");
    Serial.print(pulses[i][1] * RESOLUTION, DEC);
    Serial.println(" usec");
  }*/
  Serial.print(decodePulses(), HEX);
  Serial.println("");
}

// Produces a 32-bit word from the durations of the ON pulses.
// In the resulting word, 0 means a short pulse and 1 means a long pulse
// (where "short" and "long" are defined by THRESHOLD_SHORT_LONG).
uint32_t decodePulses() {
  uint32_t bits = 0;
  for (uint8_t i = 1; i < currentpulse; i++) {
    if (pulses[i][0] > THRESHOLD_START/RESOLUTION) {
      // Done if we found the start of another command
      return bits;
    }
    bits = bits << 1;
    if (pulses[i][0] > THRESHOLD_SHORT_LONG/RESOLUTION) {
      bits = bits | 1;
    }
  }
  return bits;
}
