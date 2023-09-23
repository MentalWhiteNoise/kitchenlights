#include "lightingtransition.h"
#include "Arduino.h"

TransitionMode string2transitionmode (String str) { return (TransitionMode) lookupFromString (str, transitionmode_conversion, sizeof(transitionmode_conversion) / sizeof(transitionmode_conversion[0])); }
String transitionmode2string (TransitionMode mode) { return String(transitionmode_conversion[mode].str); }

LightingTransition::LightingTransition(){
  _paused = false;
  _mode = TRANSITIONMODE_OFF;
  _lastmode = TRANSITIONMODE_OFF;
  //_colorarray = {(uint32_t)0,(uint32_t)0,(uint32_t)0,(uint32_t)0,(uint32_t)0,(uint32_t)0,(uint32_t)0,(uint32_t)0};
  _colorarray_length = 0;
  _speed = 0;
  _width = 0;
  _tick = millis();
  _pausedCyclePercent = _tick;
  _lastColorItem = 0;
  _chase = LightingChase();
}
unsigned long LightingTransition::get_cycle_time(uint8_t speed){
  // -108.11195963 * log(_speed + 1) + 600; // base model
  return 1000*(-76.42549758 * log(10*speed + 1) + 600) * _colorarray_length; // 10x model
}
double LightingTransition::get_cycle_percent(unsigned long tick){
  // how does speed relate to time?
  unsigned long cycleTime = get_cycle_time(_speed);
  return ((tick - _tick) % cycleTime)/(double)cycleTime;
}
void LightingTransition::set_tick_at_cycle_percent(double cyclePercent, uint8_t speed)
{
  unsigned long cycleTime = get_cycle_time(speed);
  _tick = millis() - ((double)cycleTime * cyclePercent);
}
uint32_t LightingTransition::get_effect(unsigned long tick, bool fade_bounced, uint8_t pixel) {
  if (_colorarray_length == 0){ return 0; }
  if (_mode == TRANSITIONMODE_OFF){ return _colorarray[0]; }
  //note: when speed changes, be sure to capture % thru cycle, then cast tick back to preserve position within cycle...
  double cyclePercent = get_cycle_percent(tick);
  
  int colorBucket = (int)(cyclePercent * _colorarray_length);
  double colorPercent = (cyclePercent * _colorarray_length) - (double)colorBucket;
  double effect = 0;
  if (_mode == TRANSITIONMODE_IMMIDIATE)
  {
    return _colorarray[colorBucket];
  }
  else if (_mode == TRANSITIONMODE_BLEND)
  {
    if (colorPercent == 0.5)
    { return _colorarray[colorBucket]; }
    int altBucket = colorBucket - 1;
    double altPercent = 0.5 - colorPercent;
    if (colorPercent < 0.5)
    { 
      if (altBucket == -1)
      { altBucket = _colorarray_length - 1; }
    }
    else if (colorBucket == _colorarray_length - 1)
    { 
      altBucket = 0; 
      altPercent = colorPercent - 0.5;
    }
    else
    { 
      altBucket = colorBucket + 1; 
      altPercent = colorPercent - 0.5;
    }
    altPercent = unitSineWave(altPercent/2); // 0 to 1 along sine curve
    return TransitionColor(_colorarray[colorBucket], _colorarray[altBucket], altPercent);
  }
  else if (_mode == TRANSITIONMODE_STICKYBLEND)
  {
    if (colorPercent >= 0.25 && colorPercent <= 0.75)
    { return _colorarray[colorBucket]; }
    int altBucket = colorBucket - 1;
    double altPercent = 0.5 - 2*colorPercent; // 0.5 at 0 to 1.0 at 0.25
    if (colorPercent < 0.25)
    { 
      if (altBucket == -1)
      { altBucket = _colorarray_length - 1; }
    }
    else if (colorBucket == _colorarray_length - 1)
    { 
      altBucket = 0; 
      altPercent = 2 * colorPercent - 1.5; // 0 at 0.75 amd 0.5 at 1
    }
    else
    { 
      altBucket = colorBucket + 1; 
      altPercent = 2 * colorPercent - 1.5; // 0 at 0.75 amd 0.5 at 1
    }
    
    altPercent = unitSineWave(altPercent/2); // 0 to 1 along sine curve
    return TransitionColor(_colorarray[colorBucket], _colorarray[altBucket], altPercent);

  }
  if (_mode == TRANSITIONMODE_ONBOUNCE)
  {
    Serial.print(_lastColorItem);
    Serial.print(" ");
    
    if (fade_bounced){
      _lastColorItem++;
      if (_lastColorItem >= _colorarray_length) { _lastColorItem = 0; }
    }
    return _colorarray[_lastColorItem];
  }
  if (_mode == TRANSITIONMODE_CHASE)
  {
    double revisedCyclePercent = _chase.get_effect(cyclePercent, fade_bounced, pixel);
    colorBucket = (int)(revisedCyclePercent * _colorarray_length);
    colorPercent = (revisedCyclePercent * _colorarray_length) - (double)colorBucket;

    if (colorPercent == 0.5)
    { return _colorarray[colorBucket]; }
    int altBucket = colorBucket - 1;
    double altPercent = 0.5 - colorPercent;
    if (colorPercent < 0.5)
    { 
      if (altBucket == -1)
      { altBucket = _colorarray_length - 1; }
    }
    else if (colorBucket == _colorarray_length - 1)
    { 
      altBucket = 0; 
      altPercent = colorPercent - 0.5;
    }
    else
    { 
      altBucket = colorBucket + 1; 
      altPercent = colorPercent - 0.5;
    }
    altPercent = unitSineWave(altPercent/2); // 0 to 1 along sine curve - is this needed for chase?
    return TransitionColor(_colorarray[colorBucket], _colorarray[altBucket], altPercent);
  }
  return effect;
  //int cycleTime = -254.58420964 * power(log(x + 1), 0.5) + 600; // Sqrt model
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
void LightingTransition::pause(){ 
  _pausedCyclePercent = get_cycle_percent(millis());
  _paused = true; 
}
void LightingTransition::resume(){ 
  set_tick_at_cycle_percent(_pausedCyclePercent, _speed);
  _paused = false; 
}

void LightingTransition::set_mode(String mode){
  TransitionMode newMode = string2transitionmode(mode);
  if (_mode == newMode){ return; }
  _lastmode = _mode;
  _mode = newMode;
  switch (_mode){
    /*case TRANSITIONMODE_FLICKER:
      if (_flickeractivation == 0){ _flickeractivation = defaultflickeractivation; }
      if (_width == 0){ _width = defaultWidth; }
      break;*/
    case TRANSITIONMODE_ONBOUNCE:
    case TRANSITIONMODE_CHASE: // What about a defalt chase setting?
    //case TRANSITIONMODE_PIXELATE:
      if (_width == 0){ _width = defaultWidth; }
      break; 
    default: // keeps others from having width... until implemented.
      _width == 0;
      break;
  }
}
void LightingTransition::set_speed(uint8_t value){
  set_tick_at_cycle_percent(get_cycle_percent(millis()), value);
  _speed = value; 
}
void LightingTransition::set_width(uint8_t value){
  _width = value;
  _chase.set_width(value);
}
void LightingTransition::set_chasemode(String mode){ _chase.set_mode(mode); }
uint8_t LightingTransition::count_colorarray(){ return _colorarray_length; }
bool LightingTransition::is_paused(){ return _paused; }
bool LightingTransition::is_enabled(){ return (_mode != TRANSITIONMODE_OFF); }
void LightingTransition::clear_colorarray(){
  //if (_colorarray_length != 0){ free(_colorarray); } // this was breaking stuff...
  _colorarray_length = 0;
}
void LightingTransition::append_colorarray(uint32_t c){
  if (_colorarray_length < 8)
  {
    _colorarray_length++;
    _colorarray[_colorarray_length - 1] = c;
  }
  else
  {
    Serial.print("Currently limitted to 8 colors.");
  }
  //_colorarray = (uint32_t*) realloc(_colorarray, _colorarray_length * sizeof(uint32_t));
  //*(_colorarray + (_colorarray_length - 1)) = c;
}
uint32_t LightingTransition::get_firstcolor(){
  if (_colorarray_length == 0){ return 0; }
  else { return _colorarray[0]; }
}
bool LightingTransition::perPixel()
{
  return _mode == TRANSITIONMODE_CHASE;
}

String LightingTransition::toString(){  
  String strOut = "";  
  if (_mode == TRANSITIONMODE_OFF){ return ""; }
  if (_paused){ strOut += F(" - PAUSED - "); }
  strOut += transitionmode2string(_mode);
  strOut += F(" ");
  switch (_mode){
    /*case TRANSITIONMODE_FLICKER:
      if (_flickeractivation > 0){ strOut += "(" + String(_flickeractivation) + ") "; }
      break;*/
    case TRANSITIONMODE_CHASE: {
      String chaseStr = _chase.toString();
      if (chaseStr == ""){ strOut += F("- BROKEN CHASE -"); }
      else { strOut += "(" + chaseStr + ") "; }
    } break;
  }
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
  if (_width > 0){ // what modes should have a width? 
    strOut += F(" and a width of ");
    strOut += String(_width);    
  }
  return strOut;
}