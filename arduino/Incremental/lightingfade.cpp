#include "Arduino.h"
#include "lightingfade.h"
#include "colors.h"

FadeMode string2fademode (String str) { return (FadeMode) lookupFromString (str, fademode_conversion, sizeof(fademode_conversion) / sizeof(fademode_conversion[0])); }
String fademode2string (FadeMode mode) { return String(fademode_conversion[mode].str); }

LightingFade::LightingFade(){      
  _paused = false;
  _mode = FADEMODE_OFF;
  _lastmode = FADEMODE_OFF;
  _bgcolor = 0;
  _speed = 0;
  _tick = millis();
  _pausedCyclePercent = _tick;
  _bottomhalf = false;
  bounced = false;
  _chase = LightingChase();
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
void LightingFade::set_tick_at_cycle_percent(double cyclePercent, uint8_t speed)
{
  unsigned long cycleTime = get_cycle_time(speed);
  _tick = millis() - ((double)cycleTime * cyclePercent);
}
double LightingFade::get_effect(unsigned long tick, uint8_t pixel) {
  if (_mode == FADEMODE_OFF){ return 0; }
  //note: when speed changes, be sure to capture % thru cycle, then cast tick back to preserve position within cycle...
  double cyclePercent = get_cycle_percent(tick);
  bounced = false;
  if (cyclePercent < 0.5)
  {
    if (!_bottomhalf){
      bounced = true;
    }
    _bottomhalf = true;
  }
  else { _bottomhalf = false; }
  double effect = 0;
  if (_mode == FADEMODE_PULSE) // Uses a sine wave
  {
    effect = 1 - unitSineWave(cyclePercent);
  }
  else if(_mode == FADEMODE_LINEAR)
  {
    if (cyclePercent < 0.5)
    { effect = 1 - cyclePercent*2; }
    else 
    { effect = cyclePercent*2 - 1; }
  }
  else if (_mode == FADEMODE_HEARTBEAT)
  {
    effect = 1 - (cyclePercent < 0.25 ? unitSineWave(4 * cyclePercent) : 0) + (cyclePercent > 0.15 && cyclePercent < 0.48333 ? 0.8 * unitSineWave(3 * cyclePercent) : 0);
  }
  else if (_mode == FADEMODE_CHASE)
  {    
    double revisedCyclePercent = _chase.get_effect(cyclePercent, bounced, pixel);
    effect = 1 - unitSineWave(revisedCyclePercent);
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
void LightingFade::pause(){ 
  _pausedCyclePercent = get_cycle_percent(millis());
  _paused = true; 
}
void LightingFade::resume(){ 
  set_tick_at_cycle_percent(_pausedCyclePercent, _speed);
  _paused = false; 
}
void LightingFade::set_mode(String mode){
  FadeMode newMode = string2fademode(mode);
  if (_mode == newMode){ return; }     
  _lastmode = _mode;
  _mode = newMode;
}
void LightingFade::set_speed(uint8_t value){ 
  set_tick_at_cycle_percent(get_cycle_percent(millis()), value);
  _speed = value; 
}
void LightingFade::set_chasemode(String mode){ _chase.set_mode(mode); }
void LightingFade::set_chasewidth(uint8_t value){ _chase.set_width(value); }
void LightingFade::set_bgcolor(uint32_t color){ _bgcolor = color; }
uint32_t LightingFade::get_bgcolor(){ return _bgcolor; }

bool LightingFade::perPixel()
{
  return _mode == FADEMODE_CHASE;
}

String LightingFade::displaySettings(){  
  String strOut = "\tFade Mode: " + fademode2string(_mode);
  strOut += "\n\t\t(Last Fade Mode: " + fademode2string(_lastmode);
  strOut += "\n\tBg Color: #" + ColorAsHex(_bgcolor);
  strOut += "\n\tSpeed: " + String(_speed);
  strOut += "\n\tInternal: ";
  strOut += "\n\t\tStart Tick: " + String(_tick);
  strOut += "\n\t\tBottom Half of Cycle: " + String(_bottomhalf);
  strOut += "\n\t\tPaused: " + String(_paused);
  strOut += "\n\t\tPaused Cycle Percent: " + String(_pausedCyclePercent);
  strOut += "\n\tChase: \n" + _chase.displaySettings();
  return strOut;
}
String LightingFade::toString(){
  String strOut = "";  
  if (_paused){ strOut += F(" - PAUSED - "); }
  strOut += fademode2string(_mode);
  switch (_mode){
    case FADEMODE_OFF:
      return "";
      break;
    case FADEMODE_CHASE:
      String chaseStr = _chase.toString();
      if (chaseStr == ""){
        strOut += F(" - BROKEN CHASE -");
      }
      else {
        strOut += " (" + chaseStr + ")";
      }
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

void LightingFade::serialize(byte* data){
  int start = 2;
  data[start] = (byte)_mode;
  data[start+1] = (byte)WhitePart(_bgcolor);
  data[start+2] = (byte)RedPart(_bgcolor);
  data[start+3] = (byte)GreenPart(_bgcolor);
  data[start+4] = (byte)BluePart(_bgcolor);
  data[start+5] = (byte)_speed;
  //_shift.serialize_fade(data) // 3 bytes
  _chase.serialize_fade(data);
  // bool _paused;
  // double _pausedCyclePercent;
  return;
}
void LightingFade::deserialize(byte* data){
  int start = 2;
  _mode = (FadeMode)data[start];
  _bgcolor = AsColor((uint8_t)data[start+2], (uint8_t)data[start+3], (uint8_t)data[start+4], (uint8_t)data[start+1]);
  _speed = (uint8_t)data[start+5];
  //_shift.deserialize_fade(data) // 3 bytes
  _chase.deserialize_fade(data);
  // bool _paused;
  // double _pausedCyclePercent;
  return;
}