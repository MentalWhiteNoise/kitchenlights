#include "Arduino.h"
#ifndef lightingfade_h
#define lightingfade_h
#include "lightingsupport.h"
#include "colors.h"

enum FadeMode : uint8_t { FADEMODE_OFF, FADEMODE_PULSE, FADEMODE_LINEAR, FADEMODE_HEARTBEAT };

const static lookup fademode_conversion [] = {
    {FADEMODE_OFF, "OFF"},
    {FADEMODE_PULSE, "PULSE"},
    {FADEMODE_LINEAR, "LINEAR"},
    {FADEMODE_HEARTBEAT, "HEARTBEAT"}
};
FadeMode string2fademode (String str);
String fademode2string (FadeMode mode);

class LightingFade
{
  public:
    LightingFade();
    double get_effect(unsigned long tick);

    void enable();
    void disable();
    void pause();
    void resume();
    
    void set_mode(String mode);
    void set_speed(uint8_t value);
    void set_bgcolor(uint32_t color);
    uint32_t get_bgcolor();
    String toString();
  private:
    unsigned long get_cycle_time(uint8_t speed);
    double get_cycle_percent(unsigned long tick);
    bool _paused;
    uint32_t _bgcolor;
    FadeMode _mode;
    FadeMode _lastmode;
    uint8_t _speed;
    unsigned long _tick;
};

#endif
