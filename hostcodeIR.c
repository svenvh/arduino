/*
 * This program runs on the host PC and listens for values produced by the
 * Arduino board (which reads the IR receiver).
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Serial input
#define SERIALFILE "/dev/ttyACM0"

// Delay between successive polls
#define DELAY_USEC  200000

// Interprets IR value and executes corresponding command if recognized.
void interpretValue(unsigned int value) {
  switch (value) {
    case 0:
      return;

    // Commands from a JVC RM-C1899S TV remote control.
    case 0x0001C4C8:
      printf("JVC TEXT\n");
      system("xdg-open \"http://www.buienradar.nl/\"");
      break;

    // Commands from a Technics SL-P333 CD Player remote control.
    case 0x004CE663:
      printf("EJECT\n");
      break;
    case 0x004C067F:
      printf("STOP\n");
      system("xmms -n 0 -s");
      break;
    case 0x004CC667:
      printf("PAUSE\n");
      system("xmms -n 0 -u");
      break;
    case 0x004CA66B:
      printf("PLAY\n");
      system("xmms -n 0 -p");
      break;
    case 0x004C866F:
      printf("PREV\n");
      system("xmms -n 0 -r");
      break;
    case 0x004D864F:
      printf("FWD\n");
      system("xmms -n 0 -f");
      break;
    case 0x004D4E56:
      printf("VOLDN\n");
      system("amixer --quiet set Master 1-");
      break;
    case 0x004C4E76:
      printf("VOLUP\n");
      system("amixer --quiet set Master 1+");
      break;

    // Print raw hexadecimal value if we don't know the command.
    default:
      printf("Unknown value 0x%08X\n", value);
  }
}

int main() {
  FILE *serial = fopen(SERIALFILE, "r+");

  if (!serial) {
    fprintf(stderr, "Could not open '%s'\n", SERIALFILE);
    exit(1);
  }

  unsigned int value = 0;

  while (1) {
    int matched = fscanf(serial, "%x", &value);
    if (matched == 1) {
      interpretValue(value);
      value = 0;
    }
    usleep(DELAY_USEC);
  }

  fclose(serial);
  return 0;
}
