#include "Arduino.h"
#include "lighting.h"
#include "commands.h"

Commands::Commands(Lighting *lighting, Stream* serial){
  command = "";
  parameters = "";
  _lighting = lighting;
  _serial = serial;
  commandStream = "";
  invalidStream = false;
}

void Commands::process_request_stream()
{  
  // FUTURE: Add ability to "age out" if they did not send an end character.
  // If invalid, still should read to end of stream and clear out the bad request.
  while (_serial->available()) { // how will this impact the timing?  Should I just check if serial is available, and then read it if it is? How about syncing the command?
    char c = _serial->read();  //gets one byte from serial buffer
    if (c == '\n' || c == '\r' || c == '\0' || c == '|') { // This is the only thing that submits a process...
      
      // check for unprocessed command text
      if (commandStream.length() > 0 && !invalidStream){
          if (!parse_command_stream()){
            invalidStream = true;
            _serial->println(F("Failed after submission"));
            break;
          }
      }
      if (!invalidStream && command.length() > 0)
      {
        display();
        apply_command();
      }
      if (invalidStream){
        _serial->print(F("Invalid parameters specified: unexpected value of '"));
        
        _serial->print(commandStream);
        _serial->print("' at '");
        _serial->print(command);
        _serial->print("' '");
        _serial->print(parameters);
        _serial->println("'");
      }
      reset_command_stream();
    }
    else if (!invalidStream && c == ' ' && commandStream.length() > 0){
      commandStream.toUpperCase();
      if (!parse_command_stream()){
        _serial->println(F("Failed at loop"));
        invalidStream = true;
        break;
      }
      commandStream = "";
    }
    else if(!invalidStream) {
      commandStream += c;
    }
  }

}

bool Commands::parse_command_stream(){
  commandStream.toUpperCase();
  // 1. Is the parameters not empty?
  if (parameters != ""){
    // only valid for an array!
    if (commandStream.charAt(0) == '#' && (commandStream.length() == 7 || commandStream.length() == 9)){
      bool found = false;
      for(int i = 0; i < sizeof(methods) / sizeof(methods[0]); ++i){
        String tmpMethod = String(methods[i].method);
        if (tmpMethod.equalsIgnoreCase(command)){
          if (String(methods[i].param) == "#FFFFFF..."){
            parameters += " " + commandStream;
            return true;
          }
        }
      }
      if (!found) {
        return false;
      }

    }
  }
  // 2. Is method empty?
  else if (command == ""){
    return is_valid_method(commandStream);
  }
  // 3. Is method + word a valid method?
  else if (is_valid_method(command + " " + commandStream)){
    return true;
  }  
  // 4. Is word a valid parameter of method?
  else {
    for(int i = 0; i < sizeof(methods) / sizeof(methods[0]); ++i){
      String tmpMethod = String(methods[i].method);
      
      if (tmpMethod.equalsIgnoreCase(command)){
        String tmpParam = String(methods[i].param);
        if (commandStream.equalsIgnoreCase(tmpParam)){
          parameters = commandStream;
          return true;
        }
        else if (tmpParam == F("#FFFFFF...") || tmpParam == F("#FFFFFF")){
          if (commandStream.charAt(0) == '#' && (commandStream.length() == 7 || commandStream.length() == 9)){
            parameters = commandStream;
            return true;
          }
        }
        else if (tmpParam == F("#FF")){
          if (commandStream.charAt(0) == '#' && commandStream.length() == 3 && parameters == ""){
            parameters = commandStream;
            return true;
          }
        }      
      }
    }
  }
   
  return false;
}

bool Commands::is_valid_method(String text){
  for(int i = 0; i < sizeof(methods) / sizeof(methods[0]); ++i){      
    String tmpMethod = String(methods[i].method);
    if (tmpMethod.equalsIgnoreCase(text)){
      command = text;
      return true;
    }
    bool validMethod = true;
    if (tmpMethod.length() > text.length()){
      for (int j = 0; j < text.length(); j++){
        if (tmpMethod.charAt(j) != text.charAt(j)){
          validMethod = false;
          break;
        }
      }
      if (validMethod){
        command = text;
        return true;
      }
    }
  }
  return false;
}

void Commands::apply_command(){
  bool invalidParameter = false;
  
  if (command == F("DEBUG")) {
    _serial->println(_lighting->toString());
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
  else {
    _serial->print(F("Invalid method specified: '"));
    _serial->print(command);
    _serial->println("'");
    return;
  }

  if (invalidParameter){
    _serial->print(F("Invalid parameter passed: '"));
    _serial->print(command);
    _serial->print("' '");
    _serial->print(parameters);
    _serial->println("'");
    return;
  }
}

void Commands::reset_command_stream(){
  command = "";
  parameters = "";
  commandStream = "";
  invalidStream = false;
}

void Commands::display(){
    if (command.length() > 0)
    { _serial->print(command); }
    else 
    { _serial->print("{no method}"); }
    if (parameters.length() > 0)
    {
      _serial->print(": ");
      _serial->print(parameters);
    }
    _serial->println();
}