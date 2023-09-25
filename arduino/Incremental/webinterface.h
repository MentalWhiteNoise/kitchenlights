//If primary controller
//	Handles Web Input
//	Parse Input 
//	Apply Commands
//#include "Arduino.h"
#ifndef webinterface_h
#define webinterface_h

#include "parseinput.h"
#include "display.h"
#include "WiFi.h"
#include "ESPAsyncWebSrv.h"
#include "lighting.h"

class WebInterface
{
public:
    WebInterface(ParseInput *parseInput, Display *display, AsyncWebServer *server, Lighting *lighting);
    void ConfigureInterface(char*ssid, char*password);
    ParseInput *_parseInput;
    Lighting *_lighting;
private:
    Display *_display;
    AsyncWebServer *_server;
};
#endif
