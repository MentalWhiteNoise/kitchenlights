//Input parsed into command & parameters

//#include "Arduino.h"
#ifndef parseinput_h
#define parseinput_h

#include "display.h"
#include "wirelesspush.h"
#include "managelighting.h"

const static struct {const char method[35]; const char param[15];} methods []  = { // []PROGMEM = {
    {"DEBUG", ""},
    {"ON", ""},
    {"ON", "#FFFFFF"},
    {"OFF", ""},
    {"SET COLOR", "#FFFFFF"},
    {"SET BRIGHTNESS", "#FF"},
    {"SET WHITE MODE", "ON"},
    {"SET WHITE MODE", "OFF"},
    {"SET COLOR MODE", "OFF"},
    {"SET COLOR MODE", "CONSTANT"},
    {"SET COLOR MODE", "WHITENONLY"},
    
    {"SET BGCOLOR", "#FFFFFF"},
    {"SET FADE SPEED", "#FF"},
    {"SET FADE MODE", "OFF"},
    {"SET FADE MODE", "PULSE"},
    {"SET FADE MODE", "LINEAR"},
    {"SET FADE MODE", "HEARTBEAT"},
    {"SET FADE MODE", "FLASH"},
    {"DISABLE FADE", ""},
    {"ENABLE FADE", ""},
    {"PAUSE FADE", ""},
    {"RESUME FADE", ""},
    
    {"SET COLOR ARRAY", "#FFFFFF..."},
    {"ADD TO COLOR ARRAY", "#FFFFFF..."},
    {"CLEAR COLOR ARRAY", ""},
    {"SET TRANSITION SPEED", "#FF"},
    {"SET COLOR MODE", "RANDOM"},
    {"SET COLOR MODE", "ALTERNATE"},
    {"SET COLOR MODE", "ORDERED"},
    {"SET TRANSITION MODE", "OFF"},
    {"SET TRANSITION MODE", "IMMIDIATE"},
    {"SET TRANSITION MODE", "BLEND"},
    {"SET TRANSITION MODE", "STICKYBLEND"},
    {"SET TRANSITION MODE", "ONBOUNCE"},
    {"DISABLE TRANSITION", ""},
    {"ENABLE TRANSITION", ""},
    {"PAUSE TRANSITION", ""},
    {"RESUME TRANSITION", ""},
    
    {"SET FADE MODE", "CHASE"},
    {"SET FADE CHASE WIDTH", "#FF"},
    {"SET FADE CHASE MODE", "OFF"},
    {"SET FADE CHASE MODE", "BOUNCE"},
    {"SET FADE CHASE MODE", "OUTWARD"},
    {"SET FADE CHASE MODE", "INWARD"},
    {"SET FADE CHASE MODE", "CENTERBOUNCE"},
    {"SET FADE CHASE MODE", "CLOCKWISE"},
    {"SET FADE CHASE MODE", "COUNTER"},
    {"SET TRANSITION MODE", "CHASE"},
    {"SET TRANSITION WIDTH", "#FF"},
    {"SET TRANSITION CHASE MODE", "OFF"},
    {"SET TRANSITION CHASE MODE", "BOUNCE"},
    {"SET TRANSITION CHASE MODE", "OUTWARD"},
    {"SET TRANSITION CHASE MODE", "INWARD"},
    {"SET TRANSITION CHASE MODE", "CENTERBOUNCE"},
    {"SET TRANSITION CHASE MODE", "CLOCKWISE"},
    {"SET TRANSITION CHASE MODE", "COUNTER"}
};

class ParseInput
{
public:
    ParseInput(Display *display, WirelessPush *wirelessPush, ManageLighting *manageLighting);
    String command;
    String parameters;
    void reset_command_stream();
    void apply_command();
    void display();
    String process_request_string(String text);
    bool parse_command_stream();
    String process_request_char(char c);
  private:
    bool is_valid_method (String text);
    Display *_display;
    WirelessPush *_wirelessPush;
    ManageLighting *_manageLighting;
    //Stream *_rs485; // For forwarding commands...
    String commandStream;
    bool invalidStream;
};
#endif
