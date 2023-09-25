#include "Arduino.h"
#ifndef display_h
#define display_h

#include <Adafruit_SSD1306.h>

#define SCREEN_ADDRESS 0x3C

/* Display Notes:
  4 lines at 21 characters a line at 1 point font. Will wrap if it exceeds.
  Scroll is possible but must be built.
*/
/*
 Some functions:
  clearDisplay, inverDisplay, lots of drawing...
  startscrollright, startscrollleft, stop scroll, etc
  setTextWrap
*/
/*
WHAT would I want to display?
Wireless Network & IP Address
Paired Connectivity (status? or when commands are sent?)
Commands success / erred
*/
// if controller is primary
// Manage the visual OLED display
class Display
{
public:
    Display(Adafruit_SSD1306 *screen);
    void ClearDisplay();
    void ReplaceLine(uint8_t line, String text);
    void StartDisplay();
private:
  Adafruit_SSD1306 *_screen;
};
#endif
