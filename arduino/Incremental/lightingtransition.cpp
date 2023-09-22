#include "lightingtransition.h"
#include "Arduino.h"

TransitionMode string2transitionmode (String str) { return (TransitionMode) lookupFromString (str, transitionmode_conversion); }
String transitionmode2string (TransitionMode mode) { return String(transitionmode_conversion[mode].str); }

LightingTransition::LightingTransition(){
  _paused = false;
  _mode = TRANSITIONMODE_OFF;
  _lastmode = TRANSITIONMODE_OFF;
  _colorarray = 0;
  _colorarray_length = 0;
  _speed = 0;
}
void LightingTransition::enable(){
  if (_mode != TRANSITIONMODE_OFF){ return; }
  if (_lastmode != TRANSITIONMODE_OFF){
    _mode = _lastmode;
    _lastmode = TRANSITIONMODE_OFF;
  }
  else {
    _mode = TRANSITIONMODE_IMMIDIATE;
    _lastmode = TRANSITIONMODE_OFF;
  }
}
void LightingTransition::disable(){
  if (_mode == TRANSITIONMODE_OFF){ return; }
  _lastmode = _mode;
  _mode = TRANSITIONMODE_OFF;
}
void LightingTransition::pause(){ _paused = true; }
void LightingTransition::resume(){ _paused = false; }

void LightingTransition::set_mode(String mode){
  TransitionMode newMode = string2transitionmode(mode);
  if (_mode == newMode){ return; }
  _lastmode = _mode;
  _mode = newMode;
}
void LightingTransition::set_speed(uint8_t value){ _speed = value; }
uint8_t LightingTransition::count_colorarray(){ return _colorarray_length; }
bool LightingTransition::is_paused(){ return _paused; }
bool LightingTransition::is_enabled(){ return (_mode != TRANSITIONMODE_OFF); }
void LightingTransition::clear_colorarray(){
  if (_colorarray_length != 0){ free(_colorarray); }
  _colorarray_length = 0;
}
void LightingTransition::append_colorarray(uint32_t c){
  _colorarray_length++;
  _colorarray = (uint32_t*) realloc(_colorarray, _colorarray_length * sizeof(uint32_t));
  *(_colorarray + (_colorarray_length - 1)) = c;
}
uint32_t LightingTransition::get_firstcolor(){
  if (_colorarray_length == 0){ return 0; }
  else { return _colorarray[0]; }
}

String LightingTransition::toString(){
  String strOut = "";  
  if (_mode == TRANSITIONMODE_OFF){ return ""; }
  if (_paused){ strOut += F(" - PAUSED - "); }
  strOut += transitionmode2string(_mode);
  strOut += F(" ");
  if (_colorarray_length == 0){ return ""; }
  else if (_colorarray_length == 1){ return "Static #" + ColorAsHex(_colorarray[0]); }
  else{
    strOut += F("between:");
    for (int i = 0; i < _colorarray_length; i++){ strOut += "\n\t#" + ColorAsHex(_colorarray[i]); }
    strOut += "\n";
  }
  if (_speed > 0){
    strOut += F(" at a speed of ");
    strOut += String(_speed);
  }  
  return strOut;
}