#include "lightingchase.h"

ChaseMode string2chasemode (String str) { return (ChaseMode) lookupFromString (str, chasemode_conversion, sizeof(chasemode_conversion) / sizeof(chasemode_conversion[0])); }
String chasemode2string (ChaseMode mode) { return String(chasemode_conversion[mode].str); }


LightingChase::LightingChase(){
  _mode = CHASEMODE_OFF;
  //_target = CHASETARGET_ALL;
  _width = 0;
}

double LightingChase::get_width_effect(){
  return -5.91764255005309*pow(log((255 - _width)+1),0.5) + 14.034983296193;
}
double LightingChase::get_effect(double cyclePercent, bool bounced, uint8_t pixel)
{
  if (_mode == CHASEMODE_OFF) { return 0; }

  double distancePct = GetPixelDistanceToEnd("ALL", pixel); // no target
  double tempValue = 0;
  double activePoint = 0;

  switch (_mode){
    case CHASEMODE_BOUNCE: { // chase cycle goes 0 to 1, then 1 to 0. 0 has no effect on 1.
      bool increasing = cyclePercent < 0.5;
      if (increasing) {
        activePoint = cyclePercent * 2;
        if (distancePct <= activePoint) {
          tempValue = 1 - (activePoint - distancePct);
        }
        else {
          tempValue = 1 - (activePoint + distancePct);
        }
      }
      else {
        activePoint = 2 - cyclePercent * 2;
        if (distancePct >= activePoint){
          tempValue = 1 - (distancePct - activePoint);
        }
        else {
          tempValue = distancePct + activePoint - 1;
        }
      }
    } break;
    case CHASEMODE_OUTWARD: { // chase cycle goes 0.5 to 0 and repeats. 0.5 is impacted by the value at 0 (circular). 0.5 to 1.0 mimics state.
      double effectiveCycle = (distancePct<=0.5) ? distancePct : 1-distancePct;
      if (1 - cyclePercent <= effectiveCycle){
        tempValue = 2-cyclePercent-effectiveCycle;
      }
      else {
        tempValue = 1-cyclePercent-effectiveCycle;
      }
    } break;
    case CHASEMODE_INWARD: { // chase cycle goes 0 to 0.5 and repeats. 0 is impacted by the value at 0.5 (circular). 1.0 to 0.5 mimics state.
      double effectiveCycle = (distancePct<=0.5) ? distancePct : 1-distancePct;
      if(cyclePercent>=effectiveCycle){
        tempValue = 1-cyclePercent+effectiveCycle;
      }
      else {
        tempValue = effectiveCycle-cyclePercent;
      }
      break;
    }
    case CHASEMODE_CENTERBOUNCE: { // chase cycle goes 0 to 0.5, then 0.5 to 0. 0 has no impact on the value at 0.5. 0.5 to 1.0 and 1.0 to 0.5 mimics state
      double effectiveCycle = (distancePct<=0.5) ? distancePct : 1-distancePct;
      if (cyclePercent <= 0.5){
        activePoint = 0.5 - cyclePercent;
        if(effectiveCycle < activePoint){
          tempValue = effectiveCycle+activePoint;
        }
        else {
          tempValue = 1-effectiveCycle+activePoint;
        }
      }
      else {
        activePoint = 2 - cyclePercent;
        if(effectiveCycle < activePoint){
          tempValue = 1-activePoint+effectiveCycle;
        }
        else {
          tempValue = 1-effectiveCycle-activePoint;
        }
      }
    } break;
    case CHASEMODE_CLOCKWISE: { // chase cycle goes 0 to 1 repeating. 0 is impacted by 1 (circular).
      if(1-cyclePercent <= distancePct){
        tempValue = 2-cyclePercent-distancePct;
      }
      else {
        tempValue = 1-cyclePercent-distancePct;
      }
    } break;
    case CHASEMODE_COUNTER: { // chase cycle goes 1 to 0 repeating. 1 is impacted by 0 (circular)
      if(cyclePercent >= distancePct){
        tempValue = 1-cyclePercent+distancePct;
      }
      else {
        tempValue = distancePct-cyclePercent;
      }
    } break;
  }
  // Scale to apply width
  return 1 - (1 - tempValue) * 1/get_width_effect();
}

void LightingChase::set_mode(String mode){ _mode = string2chasemode(mode); }
void LightingChase::set_width(uint8_t value){ _width = value; }
//void LightingChase::set_target(String target){ _target = string2chasetarget(target); }

String LightingChase::displaySettings(){  
  String strOut = "Chase Mode: " + chasemode2string(_mode);
  strOut += "\n\t\tWidth: " + String(_width); 
  return strOut;
}
String LightingChase::toString(){
  String strOut = chasemode2string(_mode);  
  if (_mode == CHASEMODE_OFF){ return ""; }
  strOut += F(" with a width of ");
  strOut += String(_width);
  //strOut += F(" targeting ");
  //strOut += chasetarget2string(_target);
  return strOut;
}