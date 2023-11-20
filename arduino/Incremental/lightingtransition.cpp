#include "lightingtransition.h"
#include "Arduino.h"
#include "colors.h"

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
  _flickeractivation = 127;
  _tick = millis();
  _pausedCyclePercent = _tick;
  _lastColorItem = 0;
  _chase = LightingChase();
  _shift = LightingShift();
  _lastP0 = 0;
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
uint32_t LightingTransition::get_effect(unsigned long tick, bool fade_bounced, uint16_t pixel) {
  if (_colorarray_length == 0){ return 0; }
  if (_mode == TRANSITIONMODE_OFF){ return _colorarray[0]; }
  //note: when speed changes, be sure to capture % thru cycle, then cast tick back to preserve position within cycle...
  double cyclePercent = get_cycle_percent(tick);
  
  //bool shiftBounced = (pixel == 0 && fade_bounced || _lastP0 > 0 && (int)(cyclePercent * _colorarray_length) == 0);
  uint8_t p0Bucket = (uint8_t)(cyclePercent * _colorarray_length) % _colorarray_length;
  bool shiftBounced = (pixel == 0 && fade_bounced || pixel == 0 && _lastP0 != p0Bucket);
  double shift = _shift.get_effect(pixel, shiftBounced, _colorarray_length);

  if (pixel == 0 /*&& (int)(cyclePercent * _colorarray_length) == 0*/){
    _lastP0 = (uint8_t)p0Bucket;
  }
  cyclePercent = cyclePercent - ((int)cyclePercent);

  int colorBucket = (int)(cyclePercent * _colorarray_length + shift) % _colorarray_length;
  double colorPercent = (cyclePercent * _colorarray_length + shift) - (double)colorBucket;
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
    //Serial.print(_lastColorItem);
    //Serial.print(" ");
    
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
    //altPercent = unitSineWave(altPercent/2); // 0 to 1 along sine curve - is this needed for chase?
    return TransitionColor(_colorarray[colorBucket], _colorarray[altBucket], altPercent);
  }
  if (_mode == TRANSITIONMODE_FLICKER)
  {
    throw "Not sure what to do here!";
  }
  if (_mode == TRANSITIONMODE_PIXELATE)
  {
    throw "Not sure what to do here!";
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
      _width = 0;
      break;
  }
}
void LightingTransition::set_speed(uint8_t value){
  set_tick_at_cycle_percent(get_cycle_percent(millis()), value);
  _speed = value; 
}
void LightingTransition::set_width(uint8_t value){ _width = value; }
void LightingTransition::set_chasewidth(uint8_t value) { _chase.set_width(value); }
void LightingTransition::set_chasemode(String mode){ _chase.set_mode(mode); }
uint8_t LightingTransition::count_colorarray(){ return _colorarray_length; }
bool LightingTransition::is_paused(){ return _paused; }
bool LightingTransition::is_enabled(){ return (_mode != TRANSITIONMODE_OFF); }
void LightingTransition::clear_colorarray(){
  //if (_colorarray_length != 0){ free(_colorarray); } // this was breaking stuff...
  _colorarray_length = 0;
}
void LightingTransition::append_colorarray(uint32_t c){
  if (_colorarray_length < 16)
  {
    _colorarray_length++;
    _colorarray[_colorarray_length - 1] = c;
  }
  else
  {
    Serial.print("Currently limitted to 16 colors.");
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
  return _mode == TRANSITIONMODE_CHASE || _mode == TRANSITIONMODE_FLICKER || _mode == TRANSITIONMODE_PIXELATE || _shift.is_enabled();
}

void LightingTransition::set_chasetarget(String target){_chase.set_target(target);}

void LightingTransition::set_shiftamount(uint8_t value){_shift.set_amount(value);}
void LightingTransition::set_shiftwidth(uint8_t value){_shift.set_width(value);}
void LightingTransition::set_shiftmode(String mode){_shift.set_mode(mode);}
void LightingTransition::set_shifttarget(String target){_shift.set_target(target);}

void LightingTransition::set_flickeractivation(uint8_t value){_flickeractivation = value;}

String LightingTransition::displaySettings(){  
  String strOut = "\tTransition Mode: " + transitionmode2string(_mode);
  strOut += "\n\t\t(Last Transition Mode: " + transitionmode2string(_lastmode);
  strOut += "\n\tSpeed: " + String(_speed);
  strOut += "\n\tWidth: " + String(_width);
  strOut += "\n\tFlicker Activation: " + String(_flickeractivation);
  strOut += "\n\tColor Array: ";
  for (int i = 0; i < _colorarray_length; i++){ 
    strOut += "\n\t\t#" + ColorAsHex(_colorarray[i]); 
  }
  strOut += "\n\tInternal: ";
  strOut += "\n\t\tStart Tick: " + String(_tick);
  strOut += "\n\t\tPaused: " + String(_paused);
  strOut += "\n\t\tPaused Cycle Percent: " + String(_pausedCyclePercent);
  strOut += "\n\tChase: \n" + _chase.displaySettings();
  strOut += "\n\tShift: \n" + _shift.displaySettings();
  return strOut;
}
String LightingTransition::toString(){  
  String strOut = "";  
  if (_mode == TRANSITIONMODE_OFF){ return ""; }
  if (_paused){ strOut += F(" - PAUSED - "); }
  strOut += transitionmode2string(_mode);
  strOut += F(" ");
  switch (_mode){
    case TRANSITIONMODE_PIXELATE:
    case TRANSITIONMODE_FLICKER:
      if (_flickeractivation > 0){ strOut += "(" + String(_flickeractivation) + ") "; }
      break;
    case TRANSITIONMODE_CHASE: {
      String chaseStr = _chase.toString();
      if (chaseStr == ""){ strOut += F("- BROKEN CHASE -"); }
      else { strOut += "(" + chaseStr + ") "; }
    } break;
  }
  String shiftStr = _shift.toString();
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
  if(shiftStr != ""){
    strOut += "\nShifted: " + shiftStr;
  }
  return strOut;
}
void LightingTransition::serialize(byte* data){
  int start = 18; // -> 94
  data[start] = (byte)_mode;
  data[start+1] = (byte)_colorarray_length;
  for(int i = 0; i < 16; i++){
    if (i < _colorarray_length){
      serialize_color(data, start + 2 + i*4, _colorarray[i]);
    }
    else {
      serialize_color(data, start + 2 + i*4, 0);
    }
  }
  data[start+66] = (byte)_speed;
  data[start+67] = (byte)_width;
  data[start+68] = (byte)_flickeractivation;
  data[start+69] = (_paused) ? 1 + (byte)uint(_pausedCyclePercent*100) : 0;
  _shift.serialize_transition(data);
  _chase.serialize_transition(data);
  return;
}
void LightingTransition::deserialize(byte* data){
  int start = 18;
  _mode = (TransitionMode)data[start];
  _colorarray_length = (uint8_t)data[start+1];
  for(int i = 0; i < 16; i++){
    _colorarray[i] = deserialize_color(data, start + 2 + i*4);
  }
  _speed = (uint8_t)data[start+66];
  _width = (uint8_t)data[start+67];
  _flickeractivation = (uint8_t)data[start+68];
  uint8_t tempPercent = (uint8_t)data[start+69];
  if (tempPercent > 0){
    _pausedCyclePercent = double(tempPercent - 1)/100;
    _paused = false;
  }
  else {
    _pausedCyclePercent = 0;
    _paused = false;
  }
  _shift.deserialize_fade(data);
  _chase.deserialize_transition(data);
  return;
}
void LightingTransition::serialize_color(byte* data, int start, uint32_t c){
  data[start+0] = (byte)WhitePart(c);
  data[start+1] = (byte)RedPart(c);
  data[start+2] = (byte)GreenPart(c);
  data[start+3] = (byte)BluePart(c);
}
uint32_t LightingTransition::deserialize_color(byte* data, int start){
  return AsColor((uint8_t)data[start+1], (uint8_t)data[start+2], (uint8_t)data[start+3], (uint8_t)data[start+0]);
}
