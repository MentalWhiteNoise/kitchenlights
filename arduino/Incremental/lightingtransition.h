#include "Arduino.h"
#ifndef lightingtransition_h
#define lightingtransition_h
#include "lightingsupport.h"
#include "lightingchase.h"
#include "lightingshift.h"
#include "colors.h"

const uint8_t defaultWidth = 8;

enum TransitionMode : uint8_t  { TRANSITIONMODE_OFF, TRANSITIONMODE_IMMIDIATE, TRANSITIONMODE_BLEND, TRANSITIONMODE_STICKYBLEND, TRANSITIONMODE_ONBOUNCE, TRANSITIONMODE_CHASE, TRANSITIONMODE_FLICKER, TRANSITIONMODE_PIXELATE };

const static lookup transitionmode_conversion [] = {
    {TRANSITIONMODE_OFF, "OFF"},
    {TRANSITIONMODE_IMMIDIATE, "IMMIDIATE"},
    {TRANSITIONMODE_BLEND, "BLEND"},
    {TRANSITIONMODE_STICKYBLEND, "STICKYBLEND"},
    {TRANSITIONMODE_ONBOUNCE, "ONBOUNCE"},
    {TRANSITIONMODE_CHASE, "CHASE"},
    {TRANSITIONMODE_FLICKER, "FLICKER"},
    {TRANSITIONMODE_PIXELATE, "PIXELATE"}
};
TransitionMode string2transitionmode (String str);
String transitionmode2string (TransitionMode mode);

class LightingTransition
{
  public:
    LightingTransition();
    //~LightingTransition();
    uint32_t get_effect(unsigned long tick, bool fade_bounced, uint16_t pixel);

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
		void set_chasewidth(uint8_t value);
    
		void set_chasetarget(String target);
    
		void set_shiftamount(uint8_t value);
		void set_shiftwidth(uint8_t value);
		void set_shiftmode(String mode);
		void set_shifttarget(String target);

    void set_flickeractivation(uint8_t value);

    String displaySettings();
    String toString();
    uint32_t get_firstcolor();
    bool perPixel();
    void serialize(byte* data);
    void deserialize(byte* data);
  private:
    void serialize_color(byte* data, int start, uint32_t c);
    uint32_t deserialize_color(byte* data, int start);
    unsigned long get_cycle_time(uint8_t speed);
    double get_cycle_percent(unsigned long tick);
    void set_tick_at_cycle_percent(double cyclePercent, uint8_t speed);
    bool _paused;
    TransitionMode _mode;
    TransitionMode _lastmode;
    //uint32_t* _colorarray;
    uint32_t _colorarray[16]; // Keep having problems with dynamic array...
    uint8_t _colorarray_length;
    uint8_t _speed;
    uint8_t _width;
    uint8_t _flickeractivation;
    unsigned long _tick;
    double _pausedCyclePercent;
    uint8_t _lastColorItem;
		LightingChase _chase;
		LightingShift _shift;
    uint8_t _lastP0;
};

#endif