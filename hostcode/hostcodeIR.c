/*
 * This program runs on the host PC and listens for values produced by the
 * Arduino board (which reads the IR receiver).
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/keysym.h>

// Serial input
#define SERIALFILE "/dev/ttyACM0"

// Delay between successive polls
#define DELAY_USEC  200000

// Sends a key press and a key release event to the currently focused window.
// See /usr/include/X11/keysymdef.h for available keys.
void sendKey(int keysym) {
  Display *display = XOpenDisplay(0);
  Window rootWindow = XDefaultRootWindow(display);

  // Get window with focus
  Window focusWindow;
  int revert;
  XGetInputFocus(display, &focusWindow, &revert);

  XKeyEvent event;
  event.display = display;
  event.root = rootWindow;
  event.window = focusWindow;
  event.same_screen = True;
  event.state = 0;
  event.keycode = XKeysymToKeycode(display, keysym);

  // Send KeyPress event
  event.type = KeyPress;
  XSendEvent(event.display, event.window, True, KeyPressMask, (XEvent*)&event);

  // Send KeyRelease event
  event.type = KeyRelease;
  XSendEvent(event.display, event.window, True, KeyPressMask, (XEvent*)&event);

  XCloseDisplay(display);
}

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

    // Number keys 1 to 9
    case 0x004C167D:
      printf("NUM_1\n");
      sendKey(XK_1);
      break;
    case 0x004D165D:
      printf("NUM_2\n");
      sendKey(XK_2);
      break;
    case 0x004C966D:
      printf("NUM_3\n");
      sendKey(XK_3);
      break;
    case 0x004D964D:
      printf("NUM_4\n");
      sendKey(XK_4);
      break;
    case 0x004C5675:
      printf("NUM_5\n");
      sendKey(XK_5);
      break;
    case 0x004D5655:
      printf("NUM_6\n");
      sendKey(XK_6);
      break;
    case 0x004CD665:
      printf("NUM_7\n");
      sendKey(XK_7);
      break;
    case 0x004DD645:
      printf("NUM_8\n");
      sendKey(XK_8);
      break;
    case 0x004C3679:
      printf("NUM_9\n");
      sendKey(XK_9);
      break;
    // Number key 10
    case 0x004C1E7C:
      break;

    // Codes from a JVC RM-SMXJ100E remote control
    // short pulse = 450 us, long pulse = 1100 us, spacing = 450 us
    case 0x80200460:
      sendKey(XK_Up);
      break;
    case 0x80204410:
      sendKey(XK_Right);
      break;
    case 0x80200CE0:
      sendKey(XK_Down);
      break;
    case 0x80204C90:
      sendKey(XK_Left);
      break;
    case 0x80208C10:
      sendKey(XK_Return);
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
