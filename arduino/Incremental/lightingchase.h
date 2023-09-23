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

class LightingChase
{
  public:
    LightingChase();
    double get_effect(double cyclePercent, bool bounced, uint8_t pixel);

    void set_mode(String mode);
    void set_width(uint8_t value);
    //void set_target(String target);
    String toString();
  private: 
    ChaseMode _mode;
    //ChaseTarget _target;
    uint8_t _width;
    double get_width_effect();
};
#endif
