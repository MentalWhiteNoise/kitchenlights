#include "Arduino.h"
#include "lighting.h"
#include "colors.h"

ColorMode string2colormode (String str) { return (ColorMode) lookupFromString (str, colormode_conversion, sizeof(colormode_conversion) / sizeof(colormode_conversion[0])); }
String colormode2string (ColorMode mode) { return String(colormode_conversion[mode].str); }

Lighting::Lighting(){
  _colormode = COLORMODE_OFF;
  _lastcolormode = COLORMODE_OFF;
  _brightness = defaultbrightness;
  _fade = LightingFade();
  _transition = LightingTransition();
}

uint32_t Lighting::getColor(unsigned long tick, uint8_t pixel){
  if (_colormode == COLORMODE_OFF){ return 0; }
  double fadeEffect = _fade.get_effect(tick, pixel);
  uint32_t color = 0;
  if (_colormode == COLORMODE_WHITEONLY)
  { 
    return AsColor(0,0,0,_brightness * (1-fadeEffect));
  }
  else 
  {
    return TransitionColor(_transition.get_effect(tick, _fade.bounced, pixel), _fade.get_bgcolor(), fadeEffect);
  }
}
bool Lighting::perPixel(){ return _transition.perPixel() || _fade.perPixel(); }
bool Lighting::isOff(){ return _colormode == COLORMODE_OFF; }
void Lighting::turn_on(){
  if (_colormode != COLORMODE_OFF){ return; }
  if (_lastcolormode != COLORMODE_OFF){
    _colormode = _lastcolormode;
    _lastcolormode = COLORMODE_OFF;
  }
  else {
    if (_transition.count_colorarray() == 0){ _colormode = COLORMODE_WHITEONLY; }
    else if (_transition.count_colorarray() == 1){ _colormode = COLORMODE_CONSTANT; }
    else{
      _colormode = COLORMODE_ORDERED;
      if (_transition.is_paused()){ _transition.resume(); }
      if (!_transition.is_enabled()){ _transition.enable(); }
    }
    _lastcolormode = COLORMODE_OFF;
    if (_brightness == 0){ _brightness = defaultbrightness; }
  }
}

void Lighting::turn_on(uint32_t color){
  _transition.clear_colorarray();
  _transition.append_colorarray(color);
  if (_colormode != COLORMODE_CONSTANT){ _lastcolormode = _colormode; }
  _colormode = COLORMODE_CONSTANT;
  if (_brightness == 0){ _brightness = defaultbrightness; }
}

void Lighting::turn_off(){
  if (_colormode == COLORMODE_OFF){ return; }
  else {
    _lastcolormode = _colormode;
    _colormode = COLORMODE_OFF;
  }
}

void Lighting::enable_whitemode(){
  if (_colormode == COLORMODE_WHITEONLY){ return; }
  _lastcolormode == _colormode;
  _colormode = COLORMODE_WHITEONLY;
  if (_brightness == 0){ _brightness = defaultbrightness; }
}

void Lighting::disable_whitemode(){
  if (_colormode != COLORMODE_WHITEONLY){ return; }
  if (_lastcolormode != COLORMODE_WHITEONLY){ _colormode = _lastcolormode; }
  else if (_transition.count_colorarray() == 0 && _colormode != COLORMODE_OFF){
    _lastcolormode = _colormode;
    _colormode = COLORMODE_OFF;
  }
  else if (_transition.count_colorarray() == 1){
    _lastcolormode = _colormode;
    _colormode = COLORMODE_CONSTANT;
  }
  else if (_transition.count_colorarray() > 1){
    _lastcolormode = _colormode;
    _colormode = COLORMODE_ORDERED;
    if (_transition.is_paused()){ _transition.resume(); }
    if (!_transition.is_enabled()){ _transition.enable(); }
  }
}

void Lighting::set_colormode(String mode){
  //ColorMode { COLORMODE_OFF, COLORMODE_CONSTANT, COLORMODE_WHITEONLY, COLORMODE_RANDOM, COLORMODE_ALTERNATE, COLORMODE_ORDERED };
  ColorMode newmode = string2colormode(mode);
  if (newmode == _colormode) { return; }
  if (newmode == COLORMODE_OFF){ turn_off(); }
  else if (newmode == COLORMODE_WHITEONLY){
    if (_brightness == 0){ _brightness = defaultbrightness; }
  }
  else { // full color or constant color
    _lastcolormode = _colormode;
    _colormode = newmode;
    if (_transition.count_colorarray() == 0){ _transition.append_colorarray(defaultconstantcolor); }
    if (_brightness == 0){ _brightness = defaultbrightness; }
  }
}
void Lighting::set_color(uint32_t color){_transition.append_colorarray(color);}
void Lighting::set_brightness(uint8_t value){_brightness = value;}
void Lighting::set_fademode(String mode){_fade.set_mode(mode);}
void Lighting::disable_fade(){_fade.disable();}
void Lighting::pause_fade(){_fade.pause();}
void Lighting::resume_fade(){_fade.resume();}
void Lighting::enable_fade(){_fade.enable();}
void Lighting::set_bgcolor(uint32_t color){_fade.set_bgcolor(color);}
void Lighting::set_fadespeed(uint8_t value){_fade.set_speed(value);}

void Lighting::set_transitionmode(String mode){_transition.set_mode(mode);}
void Lighting::disable_transition(){_transition.disable();}
void Lighting::pause_transition(){_transition.pause();}
void Lighting::resume_transition(){_transition.resume();}
void Lighting::enable_transition(){_transition.enable();}
void Lighting::clear_colorarray(){_transition.clear_colorarray();}
void Lighting::append_colorarray(uint32_t color){_transition.append_colorarray(color);}
void Lighting::set_transitionspeed(uint8_t value){_transition.set_speed(value);}

void Lighting::set_fadechasemode(String mode){_fade.set_chasemode(mode);}
void Lighting::set_fadechasewidth(uint8_t value){_fade.set_chasewidth(value);}
void Lighting::set_transitionchasemode(String mode){_transition.set_chasemode(mode);}
void Lighting::set_transitionwidth(uint8_t value){_transition.set_width(value);}

String Lighting::displaySettings(){  
  String strOut = "Color Mode: " + colormode2string(_colormode);
  strOut += "\n\t(Last Color Mode: " + colormode2string(_lastcolormode);
  strOut += "\nBrightness: " + String(_brightness); 
  strOut += "\nTransition: \n" + _transition.displaySettings();
  strOut += "\nFade: \n" + _fade.displaySettings();
  return strOut;
}
String Lighting::toString(){
  String strOut = colormode2string(_colormode);
  String fadeStr = _fade.toString();
  String transitionStr;
  uint32_t constantColor = 0;
  switch (_colormode){
    case COLORMODE_OFF:
      return strOut;
      break;
    case COLORMODE_CONSTANT:
      /*if (_color == 0 || _brightness == 0){
        strOut += F(" - BROKEN - ");
        return strOut;
      }*/
      strOut += " #";
      strOut += ColorAsHex(_transition.get_firstcolor()); 
      if (_brightness < 255) {
        strOut += F(" at a brightness of ");
        strOut += String(_brightness); 
      }
      break;
    case COLORMODE_WHITEONLY:
      if (_brightness == 0){
        strOut += F(" - BROKEN - ");
        return strOut;
      }
      strOut += F(" at a brightness of ");
      strOut += String(_brightness); 
      break;
    case COLORMODE_RANDOM:
      if (_brightness == 0){
        strOut += F(" - BROKEN - ");
        return strOut;
      }
      transitionStr = _transition.toString();
      if (transitionStr == ""){
        strOut += F(" - BROKEN - ");
        return strOut;
      }
      strOut += F(" at a brightness of ");
      strOut += String(_brightness); 
      strOut += "Transition: " + transitionStr;
      break;
    case COLORMODE_ALTERNATE:
      if (_brightness == 0){
        strOut += F(" - BROKEN - ");
        return strOut;
      }
      transitionStr = _transition.toString();
      if (transitionStr == ""){
        strOut += F(" - BROKEN - ");
        return strOut;
      }
      strOut += F(" at a brightness of ");
      strOut += String(_brightness); 
      strOut += F("Transition: ");
      strOut += transitionStr;
      break;
    case COLORMODE_ORDERED:
      if (_brightness == 0){
        strOut += F(" - BROKEN - ");
        return strOut;
      }
      transitionStr = _transition.toString();
      if (transitionStr == ""){
        strOut += F(" - BROKEN - ");
        return strOut;
      }
      strOut += F(" at a brightness of ");
      strOut += String(_brightness); 
      strOut += "\nTransition: " + transitionStr;
      break;
  }
  if (fadeStr != ""){ strOut += "\nFade: " + fadeStr; }
  return strOut;
}

void Lighting::serialize(byte* data){
  data[0] = (byte)_colormode;
  data[1] = (byte)_brightness;
  _fade.serialize(data);
  _transition.serialize(data);
  return;
}
void Lighting::deserialize(byte* data)
{
  _colormode = (ColorMode)data[0];
  _brightness = (uint8_t)data[1];
  _fade.deserialize(data);
  _transition.deserialize(data);
  return;
}