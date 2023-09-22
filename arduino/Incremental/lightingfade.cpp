#include "Arduino.h"
#include "lightingfade.h"

FadeMode string2fademode (String str) { return (FadeMode) lookupFromString (str, fademode_conversion); }
String fademode2string (FadeMode mode) { return String(fademode_conversion[mode].str); }

LightingFade::LightingFade(){      
  _paused = false;
  _mode = FADEMODE_OFF;
  _lastmode = FADEMODE_OFF;
  _bgcolor = 0;
  _speed = 0;
  _tick = millis();
}
unsigned long LightingFade::get_cycle_time(uint8_t speed){
  // -108.11195963 * log(_speed + 1) + 600; // base model
  return 1000*(-76.42549758 * log(10*speed + 1) + 600); // 10x model
}
double LightingFade::get_cycle_percent(unsigned long tick){
  // how does speed relate to time?
  unsigned long cycleTime = get_cycle_time(_speed);
  return ((tick - _tick) % cycleTime)/(double)cycleTime;
}
double LightingFade::get_effect(unsigned long tick) {
  if (_mode == FADEMODE_OFF){ return 0; }
  //note: when speed changes, be sure to capture % thru cycle, then cast tick back to preserve position within cycle...
  double cyclePercent = get_cycle_percent(tick);
  double effect = 0;
  if (_mode == FADEMODE_PULSE) // Uses a sine wave
  {
    effect = unitSineWave(cyclePercent);
  }
  else if(_mode == FADEMODE_LINEAR)
  {
    effect = cyclePercent;
  }
  else if (_mode == FADEMODE_HEARTBEAT)
  {
    effect = (cyclePercent < 0.25 ? unitSineWave(4 * cyclePercent) : 0) + (cyclePercent > 0.15 && cyclePercent < 0.48333 ? 0.8 * unitSineWave(3 * cyclePercent) : 0);
  }
  return effect;
  //int cycleTime = -254.58420964 * power(log(x + 1), 0.5) + 600; // Sqrt model
}
void LightingFade::enable(){
  if (_mode != FADEMODE_OFF){ return; }
  
  if (_lastmode != FADEMODE_OFF){
    _mode = _lastmode;
    _lastmode = FADEMODE_OFF;
  }
  else {
    _mode = FADEMODE_PULSE;
    _lastmode = FADEMODE_OFF;
  }
}
void LightingFade::disable(){
  if (_mode == FADEMODE_OFF){ return; }
  _lastmode = _mode;
  _mode = FADEMODE_OFF;
}
void LightingFade::pause(){ _paused = true; }
void LightingFade::resume(){ _paused = false; }

void LightingFade::set_mode(String mode){
  FadeMode newMode = string2fademode(mode);
  if (_mode == newMode){ return; }     
  _lastmode = _mode;
  _mode = newMode;
}
void LightingFade::set_speed(uint8_t value){ 
  // Get cycle percent at current speed
  unsigned long currentTick = millis();
  double cyclePercent = get_cycle_percent(currentTick);
  // calculate ticks keeping cycle percent fixed
  unsigned long cycleTime = get_cycle_time(value);
  _tick = currentTick - ((double)cycleTime * cyclePercent);
  _speed = value; 
}
void LightingFade::set_bgcolor(uint32_t color){ _bgcolor = color; }
uint32_t LightingFade::get_bgcolor(){ return _bgcolor; }

String LightingFade::toString(){
  String strOut = "";  
  if (_paused){ strOut += F(" - PAUSED - "); }
  strOut += fademode2string(_mode);
  switch (_mode){
    case FADEMODE_OFF:
      return "";
      break;
  }
  
  strOut += F(" background: #");
  strOut += ColorAsHex(_bgcolor); 
  
  if (_speed > 0){
    strOut += F(" at a speed of ");
    strOut += String(_speed);
  }
  return strOut;
}
