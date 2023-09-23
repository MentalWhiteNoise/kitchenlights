#include "Arduino.h"
#ifndef lightingtransition_h
#define lightingtransition_h
#include "lightingsupport.h"
#include "lightingchase.h"
#include "colors.h"

const uint8_t defaultWidth = 8;

enum TransitionMode : uint8_t  { TRANSITIONMODE_OFF, TRANSITIONMODE_IMMIDIATE, TRANSITIONMODE_BLEND, TRANSITIONMODE_STICKYBLEND, TRANSITIONMODE_ONBOUNCE, TRANSITIONMODE_CHASE };

const static lookup transitionmode_conversion [] = {
    {TRANSITIONMODE_OFF, "OFF"},
    {TRANSITIONMODE_IMMIDIATE, "IMMIDIATE"},
    {TRANSITIONMODE_BLEND, "BLEND"},
    {TRANSITIONMODE_STICKYBLEND, "STICKYBLEND"},
    {TRANSITIONMODE_ONBOUNCE, "ONBOUNCE"},
    {TRANSITIONMODE_CHASE, "CHASE"}
};
TransitionMode string2transitionmode (String str);
String transitionmode2string (TransitionMode mode);

class LightingTransition
{
  public:
    LightingTransition();
    //~LightingTransition();
    uint32_t get_effect(unsigned long tick, bool fade_bounced, uint8_t pixel);

    void enable();
    void disable();
    void pause();
    void resume();
    void set_mode(String mode);
    void set_speed(uint8_t value);
    uint8_t count_colorarray();
    bool is_paused();
    bool is_enabled();
    void clear_colorarray();
    void append_colorarray(uint32_t c);
    
		void set_chasemode(String mode);
    void set_width(uint8_t value);

    String toString();
    uint32_t get_firstcolor();
    bool perPixel();
  private:
    unsigned long get_cycle_time(uint8_t speed);
    double get_cycle_percent(unsigned long tick);
    void set_tick_at_cycle_percent(double cyclePercent, uint8_t speed);
    bool _paused;
    TransitionMode _mode;
    TransitionMode _lastmode;
    //uint32_t* _colorarray;
    uint32_t _colorarray[8]; // Keep having problems with dynamic array...
    uint8_t _colorarray_length;
    uint8_t _speed;
    uint8_t _width;
    unsigned long _tick;
    double _pausedCyclePercent;
    uint8_t _lastColorItem;
		LightingChase _chase;
};

#endif