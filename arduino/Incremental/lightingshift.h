#include "Arduino.h"
#ifndef lightingshift_h
#define lightingshift_h
#include "lightingsupport.h"
//#include "colors.h"
#include "pixellayout.h"

enum ShiftMode : uint8_t { SHIFTMODE_OFF, SHIFTMODE_ORDERED, SHIFTMODE_ALTERNATE, SHIFTMODE_RANDOM };

const static lookup shiftmode_conversion [] = {
    {SHIFTMODE_OFF, "OFF"},
    {SHIFTMODE_ORDERED, "ORDERED"},
    {SHIFTMODE_ALTERNATE, "ALTERNATE"},
    {SHIFTMODE_RANDOM, "RANDOM"}
};
ShiftMode string2shiftmode (String str);
String shiftmode2string (ShiftMode mode);

enum ShiftTarget : uint8_t { SHIFTTARGET_PIXEL, SHIFTTARGET_DOORS, SHIFTTARGET_CABINETS, SHIFTTARGET_SIDES, SHIFTTARGET_STRAIGHTS };

const static lookup shifttarget_conversion [] = {
    {SHIFTTARGET_PIXEL, "PIXEL"},
    {SHIFTTARGET_DOORS, "DOORS"},
    {SHIFTTARGET_CABINETS, "CABINETS"},
    {SHIFTTARGET_SIDES, "SIDES"}
};
ShiftTarget string2shifttarget (String str);
String shifttarget2string (ShiftTarget target);

class LightingShift
{
  public:
    LightingShift();
    double get_effect(uint16_t pixel, bool switchEffect, uint8_t effectCount);

    void set_mode(String mode);
    void set_amount(uint8_t value);
    void set_target(String target);

    String displaySettings();
    String toString();
    void serialize_fade(byte* data);
    void serialize_transition(byte* data);
    void deserialize_fade(byte* data);
    void deserialize_transition(byte* data);
  private: 
    void serialize(byte* data, int start);
    void deserialize(byte* data, int start);
    ShiftMode _mode;
    ShiftTarget _target;
    uint8_t _amount;
    unsigned int _step;
};
#endif
