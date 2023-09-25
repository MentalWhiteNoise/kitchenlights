#include "managelighting.h"

ManageLighting::ManageLighting(Lighting *lighting, WirelessPush *wirelessPush, Display *display){
    _lighting = lighting;
    _wirelessPush = wirelessPush;
    _display = display;
}
void ManageLighting::ApplyCommand(String command, String parameters){
  if(_wirelessPush){ // Should only be true for primary controller...
    Serial.println("HERE");
    _wirelessPush->PushCommand(command, parameters);
  }

  bool invalidParameter = false;
  
  if (command == F("DEBUG")) {
    Serial.println(_lighting->toString());
  }
  else if (command == F("ON")){ 
    if (parameters != ""){ // #FFFFFF, #FFFFFFFF
      //try{
        uint32_t c = HexToInt32(parameters);
        _lighting->turn_on(c);
      //}
      //catch(...){invalidParameter = true;}
    }
    else {
      _lighting->turn_on();
    }
  }
  
  else if (command == F("OFF")){
    _lighting->turn_off();
  }

  else if (command == F("SET COLOR")){ // #FFFFFF, #FFFFFFFF
    //try{
      _lighting->clear_colorarray();
      uint32_t c = HexToInt32(parameters);
      _lighting->set_color(c);
    //}
    //catch(...){invalidParameter = true;}
  }

  else if (command == F("SET BRIGHTNESS")){ // #FF
    //try{
      uint8_t v = HexToInt8(parameters);
      _lighting->set_brightness(v);
    //}
    //catch(...){invalidParameter = true;}
  }

  else if (command == F("SET WHITE MODE")){ // "ON", "OFF"
    if (parameters == F("ON")){
      _lighting->enable_whitemode();
    }
    else if (parameters == F("OFF")){
      _lighting->disable_whitemode();      
    }
    else{
      invalidParameter = true;
    }
  }
  else if (command == F("SET COLOR MODE")){ // "OFF", "CONSTANT", "WHITEONLY", "RANDOM", "ALTERNATE", "ORDERED"
    //try{
      _lighting->set_colormode(parameters);
    //}
    //catch(...){invalidParameter = true;}
  }
  
  else if (command == F("SET BGCOLOR")){ // #FFFFFF, #FFFFFFFF
    //try{
      uint32_t c = HexToInt32(parameters);
      _lighting->set_bgcolor(c);
    //}
    //catch(...){invalidParameter = true;}
  }
  else if (command == F("SET FADE SPEED")){ // #FF
    //try{
      uint8_t v = HexToInt8(parameters);
      _lighting->set_fadespeed(v);
    //}
    //catch(...){invalidParameter = true;}
  }
  else if (command == F("SET FADE MODE")) { // "OFF", "PULSE", "LINEAR", "HEARTBEAT", "FLASH", "CHASE", "FLICKER"
    //try{
      _lighting->set_fademode(parameters);
    //}
    //catch(...){invalidParameter = true;}
  }
  else if (command == F("DISABLE FADE")) {
    _lighting->disable_fade();
  }
  else if (command == F("ENABLE FADE")) {
    _lighting->enable_fade();
  }
  else if (command == F("PAUSE FADE")) {
    _lighting->pause_fade();
  }
  else if (command == F("RESUME FADE")) {
    _lighting->resume_fade();
  }
  
  else if (command == F("SET COLOR ARRAY")){ // #FFFFFF... , #FFFFFFFF...
    String currentColor = "";
    _lighting->clear_colorarray();
    for(int i = 0; i < parameters.length(); i++){
      if (parameters.charAt(i) == ' '){
        //try{
          uint32_t c = HexToInt32(currentColor);
          _lighting->set_color(c);
          currentColor = "";
        //}
        //catch(...){invalidParameter = true;break;}
      }
      else {
        currentColor += parameters.charAt(i);
      }
    }
    if (currentColor.length() > 0) {
      uint32_t c = HexToInt32(currentColor);
      _lighting->set_color(c);
      currentColor = "";
    }
  }
  
  else if (command == F("ADD TO COLOR ARRAY")){ // #FFFFFF... , #FFFFFFFF...
    String currentColor = "";
    for(int i = 0; i < parameters.length(); i++){
      if (parameters.charAt(i) == ' '){
        //try{
          uint32_t c = HexToInt32(currentColor);
          _lighting->set_color(c);
          currentColor = "";
        //}
        //catch(...){invalidParameter = true;break;}
      }
      else {
        currentColor += parameters.charAt(i);
      }
    }
  }
  else if (command == F("CLEAR COLOR ARRAY")){
    _lighting->clear_colorarray();
  }

  else if (command == F("SET TRANSITION SPEED")){ // #FF
    //try{
      uint8_t v = HexToInt8(parameters);
      _lighting->set_transitionspeed(v);
    //}
    //catch(...){invalidParameter = true;}
  }
  else if (command == F("SET TRANSITION MODE")){ // "OFF", "IMMIDIATE", "BLEND", "STICKYBLEND", "ONBOUNCE", "FLICKER", "CHASE", "PIXELATE"
    //try{
      _lighting->set_transitionmode(parameters);
    //}
    //catch(...){invalidParameter = true;}
  }
  else if (command == F("DISABLE TRANSITION")) {
    _lighting->disable_transition();
  }
  else if (command == F("ENABLE TRANSITION")) {
    _lighting->enable_transition();
  }
  else if (command == F("PAUSE TRANSITION")) {
    _lighting->pause_transition();
  }
  else if (command == F("RESUME TRANSITION")) {
    _lighting->resume_transition();
  }
  else if (command == F("SET FADE CHASE WIDTH")){ // #FF
    //try{
      uint8_t v = HexToInt8(parameters);
      _lighting->set_fadechasewidth(v);
    //}
    //catch(...){invalidParameter = true;}
  }
  else if (command == F("SET FADE CHASE MODE")) { // "OFF", "BOUNCE", "OUTWARD", "INWARD", "CENTERBOUNCE", "CLOCKWISE", "COUNTER"
    //try{
      _lighting->set_fadechasemode(parameters);
    //}
    //catch(...){invalidParameter = true;}
  } 
  else if (command == F("SET TRANSITION WIDTH")){ // #FF
    //try{
      uint8_t v = HexToInt8(parameters);
      _lighting->set_transitionwidth(v);
    //}
    //catch(...){invalidParameter = true;}
  }
  else if (command == F("SET TRANSITION CHASE MODE")) { // "OFF", "BOUNCE", "OUTWARD", "INWARD", "CENTERBOUNCE", "CLOCKWISE", "COUNTER"
    //try{
      _lighting->set_transitionchasemode(parameters);
    //}
    //catch(...){invalidParameter = true;}
  }
  else {
    // Send to _display, when you figure out lines etc...
    Serial.print(F("Invalid method specified: '"));
    Serial.print(command);
    Serial.println("'");
    return;
  }

  if (invalidParameter){
    // Send to _display, when you figure out lines etc...
    Serial.print(F("Invalid parameter passed: '"));
    Serial.print(command);
    Serial.print("' '");
    Serial.print(parameters);
    Serial.println("'");
    return;
  }

}
