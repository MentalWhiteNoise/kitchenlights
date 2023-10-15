#include "Arduino.h"
#ifndef lightingfade_h
#define lightingfade_h
#include "lightingsupport.h"
#include "lightingchase.h"
#include "lightingshift.h"
#include "colors.h"

enum FadeMode : uint8_t { FADEMODE_OFF, FADEMODE_PULSE, FADEMODE_LINEAR, FADEMODE_HEARTBEAT, FADEMODE_CHASE, FADEMODE_FLICKER };

const static lookup fademode_conversion [] = {
    {FADEMODE_OFF, "OFF"},
    {FADEMODE_PULSE, "PULSE"},
    {FADEMODE_LINEAR, "LINEAR"},
    {FADEMODE_HEARTBEAT, "HEARTBEAT"},
    {FADEMODE_CHASE, "CHASE"},
    {FADEMODE_FLICKER, "FLICKER"}
};
FadeMode string2fademode (String str);
String fademode2string (FadeMode mode);

class LightingFade
{
  public:
    LightingFade();
    double get_effect(unsigned long tick, uint8_t pixel);

    void enable();
    void disable();
    void pause();
    void resume();
    
    void set_mode(String mode);
    void set_speed(uint8_t value);
    void set_bgcolor(uint32_t color);
    uint32_t get_bgcolor();
    
		void set_chasemode(String mode);
		void set_chasewidth(uint8_t value);
    bool perPixel();
    
		void set_chasetarget(String target);
    
		void set_shiftamount(uint8_t value);
		void set_shiftmode(String mode);
		void set_shifttarget(String target);

    void set_flickeractivation(uint8_t value);

    String displaySettings();
    String toString();
    bool bounced;
    void serialize(byte* data);
    void deserialize(byte* data);
  private:
    unsigned long get_cycle_time(uint8_t speed);
    double get_cycle_percent(unsigned long tick);
    void set_tick_at_cycle_percent(double cyclePercent, uint8_t speed);
    bool _bottomhalf; // at the bottom half of the cycle
    bool _paused;
    uint32_t _bgcolor;
    FadeMode _mode;
    FadeMode _lastmode;
    uint8_t _speed;
    uint8_t _flickeractivation;
    unsigned long _tick;
    double _pausedCyclePercent;
		LightingChase _chase;
		LightingShift _shift;
};

#endif
