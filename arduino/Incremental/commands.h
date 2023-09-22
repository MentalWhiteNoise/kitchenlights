#include "Arduino.h"
#include "lighting.h"
#ifndef commands_h
#define commands_h

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
    {"RESUME TRANSITION", ""}
};

class Commands
{
  public:
    Commands(Lighting *lighting, Stream *serial);
    String command;
    String parameters;
    void reset_command_stream();
    void apply_command();
    void display();
    void process_request_stream();
    bool parse_command_stream();
  private:
    bool is_valid_method (String text);
    Lighting *_lighting;
    Stream *_serial;
    //Stream *_rs485; // For forwarding commands...
    String commandStream;
    bool invalidStream;
};
#endif