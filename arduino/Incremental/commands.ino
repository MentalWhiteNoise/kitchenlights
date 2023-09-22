/*
const static struct {const char method[35]; const char param[15];} methods []  = { // []PROGMEM = {
    {"DEBUG", ""},
    {"ON", ""},
    {"ON", "#FFFFFF"},
    {"OFF", ""},
    {"SET COLOR", "#FFFFFF"},
    {"SET BRIGHTNESS", "#FF"},
    {"SET WHITE MODE", "ON"},
    {"SET WHITE MODE", "OFF"},
    {"SET COLOR MODE", "OFF"},
    {"SET COLOR MODE", "CONSTANT"},
    {"SET COLOR MODE", "WHITENONLY"}
};


//bool ValidPartialCommand(String newWord){

bool ValidateWord(String newWord){
  newWord.toUpperCase();
  // 1. Is the parameters not empty?
  if (parameters != ""){
    // only valid for an array!
    if (newWord.charAt(0) == '#' && (newWord.length() == 7 || newWord.length() == 9)){
      for(int i = 0; i < sizeof(methods) / sizeof(methods[0]); ++i){
        String tmpMethod = String(methods[i].method);
        if (tmpMethod == method){
          if (methods[i].param == "#FFFFFF..."){
            parameters += " " + newWord;
          }
          else {
            return false;
          }
        }
      }
    }
  }
  // 2. Is method empty?
  else if (method == ""){
    return isValidMethod(newWord);
  }
  // 3. Is method + word a valid method?
  else if (isValidMethod (method + " " + newWord)){
    return true;
  }  
  // 4. Is word a valid parameter of method?
  else {
    for(int i = 0; i < sizeof(methods) / sizeof(methods[0]); ++i){
      String tmpMethod = String(methods[i].method);
      
      if (tmpMethod.equalsIgnoreCase(method)){
        String tmpParam = String(methods[i].param);
        if (newWord.equalsIgnoreCase(tmpParam)){
          return true;
        }
        else if (tmpParam == F("#FFFFFF...") || tmpParam == F("#FFFFFF")){
          if (newWord.charAt(0) == '#' && (newWord.length() == 7 || newWord.length() == 9)){
            parameters = newWord;
            return true;
          }
        }
        else if (tmpParam == F("#FF")){
          if (newWord.charAt(0) == '#' && newWord.length() == 3 && parameters == ""){
            parameters = newWord;
            return true;
          }
        }      
      }
    }
  }
   
  return false;
};

bool isValidMethod (String potentialMethod){
    for(int i = 0; i < sizeof(methods) / sizeof(methods[0]); ++i){      
      String tmpMethod = String(methods[i].method);
      if (tmpMethod.equalsIgnoreCase(potentialMethod)){
        method = potentialMethod;
        return true;
      }
      bool validMethod = true;
      if (tmpMethod.length() > potentialMethod.length()){
        for (int j = 0; j < potentialMethod.length(); j++){
          if (tmpMethod.charAt(j) != potentialMethod.charAt(j)){
            validMethod = false;
            break;
          }
        }
        if (validMethod){
          return true;
        }
      }
    }
    return false;
}

void ApplyCommand(String command, String parameters){
  bool invalidParameter = false;
  
#ifdef IncludeToString
  if (command == F("STATUS")){
    Serial.println(lighting.toString());
  }
#endif

  if (command == F("ON")){ 
    if (parameters != ""){ // #FFFFFF, #FFFFFFFF
      //try{
        uint32_t c = HexToInt32(parameters);
        lighting.turn_on(c);
      //}
      //catch(...){invalidParameter = true;}
    }
    else {
      lighting.turn_on();
    }
  }
  
  else if (command == F("OFF")){
    lighting.turn_off();
  }

  else if (command == F("SET COLOR")){ // #FFFFFF, #FFFFFFFF
    //try{
      uint32_t c = HexToInt32(parameters);
      lighting.set_color(c);
    //}
    //catch(...){invalidParameter = true;}
  }

  else if (command == F("SET BRIGHTNESS")){ // #FF
    //try{
      uint8_t v = HexToInt8(parameters);
      lighting.set_brightness(v);
    //}
    //catch(...){invalidParameter = true;}
  }

  else if (command == F("SET WHITE MODE")){ // "ON", "OFF"
    if (parameters == F("ON")){
      lighting.enable_whitemode();
    }
    else if (parameters == F("OFF")){
      lighting.disable_whitemode();      
    }
    else{
      invalidParameter = true;
    }
  }
  else if (command == F("SET COLOR MODE")){ // "OFF", "CONSTANT", "WHITEONLY", "RANDOM", "ALTERNATE", "ORDERED"
    //try{
      lighting.set_colormode(parameters);
    //}
    //catch(...){invalidParameter = true;}
  }
  else {
    Serial.print(F("Invalid method specified: '"));
    Serial.print(method);
    Serial.println("'");
    return;
  }

  if (invalidParameter){
    Serial.print(F("Invalid parameter passed: '"));
    Serial.print(method);
    Serial.print("' '");
    Serial.print(parameters);
    Serial.println("'");
    return;
  }
}
*/