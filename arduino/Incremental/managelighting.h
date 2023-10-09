//#include "Arduino.h"
#ifndef managelighting_h
#define managelighting_h

#include "lightingsupport.h"
#include "display.h"
#include "wirelesspush.h"
#include "lighting.h"
#include "pixellayout.h"
#include <Adafruit_NeoPixel.h>

struct LightingCommand {String command; String parameters;};

// Apply commands
// Associate controller to pixels (TBD)
class ManageLighting
{
  public:
    ManageLighting(Lighting *lighting, WirelessPush *wirelessPush, Display *display, Adafruit_NeoPixel *strip, uint8_t controllerId);
    void ApplyCommand(String command, String parameters);
    void draw(unsigned long tick);
    void turnOff();
    void turnOn();
  private:
    Lighting *_lighting;
    WirelessPush *_wirelessPush;
    Display *_display;
    Adafruit_NeoPixel *_strip;
    uint8_t _controllerId;
    bool _off;
    bool _debug;
};
#endif
