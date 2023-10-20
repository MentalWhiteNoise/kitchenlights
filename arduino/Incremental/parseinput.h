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
    {"SET FADE WIDTH", "#FF"},
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
    {"SET TRANSITION CHASE WIDTH", "#FF"},
    {"SET TRANSITION CHASE MODE", "OFF"},
    {"SET TRANSITION CHASE MODE", "BOUNCE"},
    {"SET TRANSITION CHASE MODE", "OUTWARD"},
    {"SET TRANSITION CHASE MODE", "INWARD"},
    {"SET TRANSITION CHASE MODE", "CENTERBOUNCE"},
    {"SET TRANSITION CHASE MODE", "CLOCKWISE"},
    {"SET TRANSITION CHASE MODE", "COUNTER"},
    
    {"SET FADE CHASE TARGET", "ALL"},
    {"SET FADE CHASE TARGET", "DOORS"},
    {"SET FADE CHASE TARGET", "CABINETS"},
    {"SET FADE CHASE TARGET", "SIDES"},
    {"SET TRANSITION CHASE TARGET", "ALL"},
    {"SET TRANSITION CHASE TARGET", "DOORS"},
    {"SET TRANSITION CHASE TARGET", "CABINETS"},
    {"SET TRANSITION CHASE TARGET", "SIDES"},
    
    {"SET TRANSITION SHIFT AMOUNT", "#FF"},
    {"SET TRANSITION SHIFT WIDTH", "#FF"},
    {"SET FADE SHIFT MODE", "OFF"},
    {"SET FADE SHIFT MODE", "ORDERED"},
    {"SET FADE SHIFT MODE", "ALTERNATE"},
    {"SET FADE SHIFT MODE", "RANDOM"},
    {"SET TRANSITION SHIFT MODE", "OFF"},
    {"SET TRANSITION SHIFT MODE", "ORDERED"},
    {"SET TRANSITION SHIFT MODE", "ALTERNATE"},
    {"SET TRANSITION SHIFT MODE", "RANDOM"},

    {"SET FADE SHIFT AMOUNT", "#FF"},
    {"SET FADE SHIFT WIDTH", "#FF"},
    {"SET FADE SHIFT TARGET", "PIXEL"},
    {"SET FADE SHIFT TARGET", "DOORS"},
    {"SET FADE SHIFT TARGET", "CABINETS"},
    {"SET FADE SHIFT TARGET", "SIDES"},
    {"SET TRANSITION SHIFT TARGET", "PIXEL"},
    {"SET TRANSITION SHIFT TARGET", "DOORS"},
    {"SET TRANSITION SHIFT TARGET", "CABINETS"},
    {"SET TRANSITION SHIFT TARGET", "SIDES"},
    
    {"SET FADE FLICKER ACTIVATION", "#FF"},
    {"SET TRANSITION FLICKER ACTIVATION", "#FF"},
    {"SET FADE MODE", "FLICKER"},
    {"SET TRANSITION MODE", "FLICKER"},
    {"SET TRANSITION MODE", "PIXELATE"}
};

class ParseInput
{
public:
    ParseInput(ManageLighting *manageLighting);
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
    ManageLighting *_manageLighting;
    //Stream *_rs485; // For forwarding commands...
    String commandStream;
    bool invalidStream;
};
#endif
