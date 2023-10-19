#include "lightingshift.h"

ShiftMode string2shiftmode (String str) { return (ShiftMode) lookupFromString (str, shiftmode_conversion, sizeof(shiftmode_conversion) / sizeof(shiftmode_conversion[0])); }
String shiftmode2string (ShiftMode mode) { return String(shiftmode_conversion[mode].str); }

ShiftTarget string2shifttarget (String str) { return (ShiftTarget) lookupFromString (str, shifttarget_conversion, sizeof(shifttarget_conversion) / sizeof(shifttarget_conversion[0])); }
String shifttarget2string (ShiftTarget target) { return String(shifttarget_conversion[target].str); }

LightingShift::LightingShift(){
  _amount = 127;
  _width = 1;
  _mode = SHIFTMODE_OFF;
  _target = SHIFTTARGET_PIXEL;
  _step = 0;
}
void LightingShift::set_mode(String mode) { _mode = string2shiftmode(mode); }
void LightingShift::set_amount(uint8_t value) { _amount = value; }
void LightingShift::set_width(uint8_t value) { _width = value; }
void LightingShift::set_target(String target){ _target = string2shifttarget(target); }

double LightingShift::get_effect(uint16_t pixel, bool switchEffect, uint8_t effectCount){
  double stretchAmount = (_amount + 1) / 128.0;
  if (_mode == SHIFTMODE_OFF) { return 0; }

  String target = shifttarget2string(_target);
  uint16_t effectivePixel = pixel;
  if (_target == SHIFTTARGET_PIXEL)
  {
      effectivePixel = (uint16_t)(pixel / _width);
  }
  uint8_t instance = GetLocationInstance(target, effectivePixel);
  //instances = GetInstanceCount(target);

  if (_mode == SHIFTMODE_ORDERED) { 
    /*if (switchEffect){
      _step++;
    }
    if (_step > effectCount){
      _step = 0;
    }*/
    return stretchAmount * ((instance + _step) % effectCount); 
  }

  if (_mode == SHIFTMODE_ALTERNATE) { 
    uint8_t totalEffects = effectCount * 2 - 2; // Less start and end, so it doesn't have dupes
    /*if (switchEffect){
      _step++;
    }
    if (_step * stretchAmount > totalEffects - 1){
      _step = 0;
    }*/
    uint8_t bucket = (stretchAmount * ((instance + _step)) % (totalEffects / stretchAmount));
    if (bucket < totalEffects / stretchAmount / 2){
      return bucket;
    }
    else {
      return totalEffects - bucket;
    }
  }
  if (_mode == SHIFTMODE_RANDOM){
    if (switchEffect){
      _step++;
    }
    randomSeed(_step);
    uint8_t select = (uint8_t)random.Next(0, (uint8_t)(effectCount / stretchAmount));
    for(int i = 0; i < instance; i++){
      select = (uint8_t)random.Next(0, (uint8_t)(effectCount / stretchAmount));
    }
    return stretchAmount * select;
  }

  return 0;
}
bool LightingShift::is_enabled(){
  return _mode != SHIFTMODE_OFF;
}

String LightingShift::displaySettings(){
  String strOut = "Shift Mode: " + shiftmode2string(_mode);
  strOut += "\n\t\tAmount: " + String(_amount); 
  if (_target == SHIFTTARGET_PIXEL)
  {
    strOut += "\n\t\tWidth: " + String(_width); 
  }
  strOut += "\n\t\tTarget: " + shifttarget2string(_target); 
  return strOut;
}
String LightingShift::toString(){
  String strOut = shiftmode2string(_mode);
  if (_mode == SHIFTMODE_OFF){ return ""; }
  strOut += " " + String(_amount) + " ";  
  if (_target == SHIFTTARGET_PIXEL)
  { strOut += " (" + String(_width) + " pixels) "; }
  strOut += shifttarget2string(_target);
  return strOut;
}

void LightingShift::serialize_fade(byte* data) { serialize(data, 11); }
void LightingShift::serialize_transition(byte* data) { serialize(data, 88); }
void LightingShift::deserialize_fade(byte* data) { deserialize(data, 11); }
void LightingShift::deserialize_transition(byte* data) { deserialize(data, 88); }
void LightingShift::serialize(byte* data, int start){
  data[start] = (byte)_mode;
  data[start+1] = (byte)_amount;
  data[start+2] = (byte)_width;
  data[start+3] = (byte)_target;
}
void LightingShift::deserialize(byte* data, int start){
  _mode = (ShiftMode)data[start];
  _amount = (uint8_t)data[start+1];
  _width = (uint8_t)data[start+1];
  _target = (ShiftTarget)data[start+2];
}
