//#include "Arduino.h"
#ifndef managelighting_h
#define managelighting_h

#include "lightingsupport.h"
#include "display.h"
#include "wirelesspush.h"
#include "lighting.h"

struct LightingCommand {String command; String parameters;};

// Apply commands
// Associate controller to pixels (TBD)
class ManageLighting
{
  public:
    ManageLighting(Lighting *lighting, WirelessPush *wirelessPush, Display *display);
    void ApplyCommand(String command, String parameters);
  private:
    Lighting *_lighting;
    WirelessPush *_wirelessPush;
    Display *_display;
};
#endif
