#include "Arduino.h"
#ifndef lighting_h
#define lighting_h
//#include "lightingsupport.h"
#include "lightingfade.h"
#include "lightingtransition.h"

const uint32_t defaultconstantcolor = 2147483648; // #80000000 / 128 white, 0 color
const uint8_t defaultbrightness = 128;
const uint8_t defaultflickeractivation = 64;

enum ColorMode : uint8_t { COLORMODE_OFF, COLORMODE_CONSTANT, COLORMODE_WHITEONLY, COLORMODE_RANDOM, COLORMODE_ALTERNATE, COLORMODE_ORDERED };

const static lookup colormode_conversion [] = {
    {COLORMODE_OFF, "OFF"},
    {COLORMODE_CONSTANT, "CONSTANT"},
    {COLORMODE_WHITEONLY, "WHITEONLY"},
    {COLORMODE_RANDOM, "RANDOM"},
    {COLORMODE_ALTERNATE, "ALTERNATE"},
    {COLORMODE_ORDERED, "ORDERED"}
};
ColorMode string2colormode (String str);
String colormode2string (ColorMode mode);

class Lighting
{
  public:
    Lighting();
    
    uint32_t getColor(unsigned long tick, uint16_t pixel);
    void turn_on();
    void turn_on(uint32_t color);
    void turn_off();
    void enable_whitemode();
    void disable_whitemode();
    void set_colormode(String mode);
    void set_brightness(uint8_t value);
    void set_color(uint32_t color);
    
		void set_fademode(String mode);
		void disable_fade();
		void pause_fade();
		void resume_fade();
		void enable_fade();
		void set_bgcolor(uint32_t color);
		void set_fadespeed(uint8_t value);
    
		void set_transitionmode(String mode);
		void disable_transition();
		void pause_transition();
		void resume_transition();
		void enable_transition();
		void clear_colorarray();
		void append_colorarray(uint32_t color);
		void set_transitionspeed(uint8_t value);

		void set_fadechasemode(String mode);
		void set_fadechasewidth(uint8_t value);	
		void set_transitionchasemode(String mode);
		void set_transitionwidth(uint8_t value);
    bool perPixel();
    bool isOff();

		void set_fadechasetarget(String target);
		void set_transitionchasetarget(String target);

		void set_fadeshiftamount(uint8_t value);
		void set_fadeshiftwidth(uint8_t value);
		void set_fadeshiftmode(String mode);
		void set_fadeshifttarget(String target);
		void set_transitionshiftamount(uint8_t value);
		void set_transitionshiftwidth(uint8_t value);
		void set_transitionshiftmode(String mode);
		void set_transitionshifttarget(String target);

    void set_fadeflickeractivation(uint8_t value);
    void set_transitionflickeractivation(uint8_t value);

    //void load(); // loads from EEPROM
    //void save(); // saves to EEPROM
    String displaySettings();
    String toString();
    void serialize(byte* data);
    void deserialize(byte* data);
  private:
    ColorMode _colormode;
    uint8_t _brightness;
    // uint8_t* get_as_bit_stream(); // allows saving
    ColorMode _lastcolormode;
		LightingFade _fade;
		LightingTransition _transition;
};

#endif
