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
struct TemporaryText {String text; unsigned long expiration;};

class Display
{
public:
  Display(Adafruit_SSD1306 *screen);
  void ClearDisplay();
  void StartDisplay();
  void SetLine(uint8_t line, String text);
  void TemporarilyReplaceLine(uint8_t line, String text, uint16_t duration);
  void TemporarilyReplaceAll(String text, uint16_t duration);
  void draw(unsigned long tick);
  void wake();
private:
  Adafruit_SSD1306 *_screen;
  String _fixedText[4];
  TemporaryText _tempText[4];
  unsigned long _tick;
  bool _sleep;
  bool _hasChanged;
  unsigned long _nextExpiration;
};
#endif
