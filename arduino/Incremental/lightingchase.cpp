#include "lightingchase.h"

ChaseMode string2chasemode (String str) { return (ChaseMode) lookupFromString (str, chasemode_conversion, sizeof(chasemode_conversion) / sizeof(chasemode_conversion[0])); }
String chasemode2string (ChaseMode mode) { return String(chasemode_conversion[mode].str); }

ChaseTarget string2chasetarget (String str) { return (ChaseTarget) lookupFromString (str, chasetarget_conversion, sizeof(chasetarget_conversion) / sizeof(chasetarget_conversion[0])); }
String chasetarget2string (ChaseTarget target) { return String(chasetarget_conversion[target].str); }

LightingChase::LightingChase(){
  _mode = CHASEMODE_OFF;
  _target = CHASETARGET_ALL;
  _width = 0;
}

double LightingChase::get_width_effect(){
  return -5.91764255005309*pow(log((255 - _width)+1),0.5) + 14.034983296193;
}
double LightingChase::get_effect(double cyclePercent, bool bounced, uint16_t pixel)
{
  if (_mode == CHASEMODE_OFF) { return 0; }

  double distancePct = GetPixelDistanceToEnd(chasetarget2string(_target), pixel);
  double tempValue = 0;
  double activePoint = 0;
  double widthFactor = get_width_effect();

  switch (_mode){
    case CHASEMODE_BOUNCE: { // chase cycle goes 0 to 1, then 1 to 0. 0 has no effect on 1.
      
      bool increasing = cyclePercent < 0.5;
      if (increasing) {
        activePoint = cyclePercent * 2;
        if (distancePct <= activePoint) { // if left of active point
          tempValue = 1 - (activePoint - distancePct) * widthFactor; // 1 - distance to active point
        }
        else { // if right of active point
          tempValue = 1 - (activePoint + distancePct) * widthFactor; // 1 - distance to 0 + distance of active point to 0
        }
      }
      else {
        activePoint = 2 - cyclePercent * 2;
        if (distancePct >= activePoint){ // if right of active point
          tempValue = 1 - (distancePct - activePoint) * widthFactor; // 1 - distance to active point
        }
        else { // if left of active point
          tempValue = 1 - (2 - distancePct - activePoint) * widthFactor; // 1 - distance to end + distance active point to end
        }
      }
    } break;
    case CHASEMODE_OUTWARD: { // chase cycle goes 0.5 to 0 and repeats. 0.5 is impacted by the value at 0 (circular). 0.5 to 1.0 mimics state.
      double effectiveCycle = (distancePct<=0.5) ? distancePct : 1-distancePct;
      if (1 - cyclePercent <= effectiveCycle){
        tempValue = 1 - (2+cyclePercent-effectiveCycle) * widthFactor;
      }
      else {
        tempValue = 1-(cyclePercent+effectiveCycle) * widthFactor;
      }
    } break;
    case CHASEMODE_INWARD: { // chase cycle goes 0 to 0.5 and repeats. 0 is impacted by the value at 0.5 (circular). 1.0 to 0.5 mimics state.
      double effectiveCycle = (distancePct<=0.5) ? distancePct : 1-distancePct;
      if(cyclePercent>=effectiveCycle){
        tempValue = 1-(cyclePercent+effectiveCycle) * widthFactor;
      }
      else {
        tempValue = 1 - (effectiveCycle-cyclePercent+1)*widthFactor;
      }
      break;
    }
    case CHASEMODE_CENTERBOUNCE: { // chase cycle goes 0 to 0.5, then 0.5 to 0. 0 has no impact on the value at 0.5. 0.5 to 1.0 and 1.0 to 0.5 mimics state
      double effectiveCycle = (distancePct<=0.5) ? distancePct : 1-distancePct;
      if (cyclePercent <= 0.5){
        activePoint = 0.5 - cyclePercent;
        if(effectiveCycle < activePoint){
          //tempValue = effectiveCycle+activePoint;
          tempValue = 1 - (1-effectiveCycle-activePoint)*widthFactor;
        }
        else {
          //tempValue = 1-effectiveCycle+activePoint;
          tempValue = 1-(effectiveCycle-activePoint)*widthFactor;
        }
      }
      else {
        activePoint = 2 - cyclePercent;
        if(effectiveCycle < activePoint){
          //tempValue = 1-activePoint+effectiveCycle;
          tempValue = 1-(activePoint-effectiveCycle)*widthFactor;
        }
        else {
          //tempValue = 1-effectiveCycle-activePoint;
          tempValue = 1-(effectiveCycle+activePoint)*widthFactor;
        }
      }
    } break;
    case CHASEMODE_CLOCKWISE: { // chase cycle goes 0 to 1 repeating. 0 is impacted by 1 (circular).
      if(1-cyclePercent <= distancePct){
        //tempValue = 2-cyclePercent-distancePct;
        tempValue = 1-(distancePct+cyclePercent-1) * widthFactor;
      }
      else {
        //tempValue = 1-cyclePercent-distancePct;
        tempValue = 1-(cyclePercent+distancePct)*widthFactor;
      }
    } break;
    case CHASEMODE_COUNTER: { // chase cycle goes 1 to 0 repeating. 1 is impacted by 0 (circular)
      if(cyclePercent >= distancePct){
        //tempValue = 1-cyclePercent+distancePct;
        tempValue = 1-(cyclePercent-distancePct)*widthFactor;
      }
      else {
        //tempValue = distancePct-cyclePercent;
        tempValue = 1 - (cyclePercent - distancePct + 1) * widthFactor;
      }
    } break;
  }
  return tempValue; // turning off scalign to test
  // Scale to apply width
  //return 1 - (1 - tempValue) * 1/get_width_effect();
}

void LightingChase::set_mode(String mode){ _mode = string2chasemode(mode); }
void LightingChase::set_width(uint8_t value){ _width = value; }
void LightingChase::set_target(String target){ _target = string2chasetarget(target); }

String LightingChase::displaySettings(){  
  String strOut = "Chase Mode: " + chasemode2string(_mode);
  strOut += "\n\t\tWidth: " + String(_width); 
  strOut += "\n\t\tTarget: " + chasetarget2string(_target); 
  return strOut;
}
String LightingChase::toString(){
  String strOut = chasemode2string(_mode);  
  if (_mode == CHASEMODE_OFF){ return ""; }
  strOut += F(" with a width of ");
  strOut += String(_width);
  strOut += F(" targeting ");
  strOut += chasetarget2string(_target);
  return strOut;
}
void LightingChase::serialize_fade(byte* data) { serialize(data, 13); }
void LightingChase::serialize_transition(byte* data) { serialize(data, 89); }
void LightingChase::deserialize_fade(byte* data) { deserialize(data, 13); }
void LightingChase::deserialize_transition(byte* data) { deserialize(data, 89); }
void LightingChase::serialize(byte* data, int start){
  data[start] = (byte)_mode;
  data[start+1] = (byte)_width;
  data[start+2] = (byte)_target;
}
void LightingChase::deserialize(byte* data, int start){
  _mode = (ChaseMode)data[start];
  _width = (uint8_t)data[start+1];
  _target = (ChaseTarget)data[start+2];
}