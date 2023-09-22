#include "Arduino.h"
#ifndef lightingtransition_h
#define lightingtransition_h
#include "lightingsupport.h"
#include "colors.h"

enum TransitionMode : uint8_t  { TRANSITIONMODE_OFF, TRANSITIONMODE_IMMIDIATE, TRANSITIONMODE_BLEND, TRANSITIONMODE_STICKYBLEND, TRANSITIONMODE_ONBOUNCE };

const static lookup transitionmode_conversion [] = {
    {TRANSITIONMODE_OFF, "OFF"},
    {TRANSITIONMODE_IMMIDIATE, "IMMIDIATE"},
    {TRANSITIONMODE_BLEND, "BLEND"},
    {TRANSITIONMODE_STICKYBLEND, "STICKYBLEND"},
    {TRANSITIONMODE_ONBOUNCE, "ONBOUNCE"}
};
TransitionMode string2transitionmode (String str);
String transitionmode2string (TransitionMode mode);

class LightingTransition
{
  public:
    LightingTransition();
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

    String toString();
    uint32_t get_firstcolor();
  private:
    bool _paused;
    TransitionMode _mode;
    TransitionMode _lastmode;
    uint32_t* _colorarray; // Thought: use fixed array instead. Maybe 8 colors, max?
    uint8_t _colorarray_length;
    uint8_t _speed;
};

#endif