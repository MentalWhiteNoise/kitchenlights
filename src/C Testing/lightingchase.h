#include "Arduino.h"
#ifndef lightingchase_h
#define lightingchase_h
#include "lightingsupport.h"
#include "colors.h"
#include "pixellayout.h"

enum ChaseMode : uint8_t  { CHASEMODE_OFF, CHASEMODE_BOUNCE, CHASEMODE_OUTWARD, CHASEMODE_INWARD, CHASEMODE_CENTERBOUNCE, CHASEMODE_CLOCKWISE, CHASEMODE_COUNTER };

const static lookup chasemode_conversion [] = {
    {CHASEMODE_OFF, "OFF"},
    {CHASEMODE_BOUNCE, "BOUNCE"},
    {CHASEMODE_OUTWARD, "OUTWARD"},
    {CHASEMODE_INWARD, "INWARD"},
    {CHASEMODE_CENTERBOUNCE, "CENTERBOUNCE"},
    {CHASEMODE_CLOCKWISE, "CLOCKWISE"},
    {CHASEMODE_COUNTER, "COUNTER"},
};
ChaseMode string2chasemode (String str);
String chasemode2string (ChaseMode mode);

enum ChaseTarget : uint8_t { CHASETARGET_ALL, CHASETARGET_DOORS, CHASETARGET_CABINETS, CHASETARGET_SIDES };

const static lookup chasetarget_conversion [] = {
    {CHASETARGET_ALL, "ALL"},
    {CHASETARGET_DOORS, "DOORS"},
    {CHASETARGET_CABINETS, "CABINETS"},
    {CHASETARGET_SIDES, "SIDES"},
};
ChaseTarget string2chasetarget (String str);
String chasetarget2string (ChaseTarget target);

class LightingChase
{
  public:
    LightingChase();
    double get_effect(double cyclePercent, bool bounced, uint8_t pixel);

    void set_mode(String mode);
    void set_width(uint8_t value);
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
    ChaseMode _mode;
    ChaseTarget _target;
    uint8_t _width;
    double get_width_effect();
};
#endif
