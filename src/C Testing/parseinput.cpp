#include "parseinput.h"

ParseInput::ParseInput(ManageLighting *manageLighting){
    _manageLighting = manageLighting;
    
    command = "";
    parameters = "";
    commandStream = "";
    invalidStream = false;
}
void ParseInput::reset_command_stream(){
  command = "";
  parameters = "";
  commandStream = "";
  invalidStream = false;
}
void ParseInput::apply_command(){
    _manageLighting->ApplyCommand(command, parameters);
}
void ParseInput::display(){
    if (command.length() > 0)
    { Serial.print(command); }
    else 
    { Serial.print("{no method}"); }
    if (parameters.length() > 0)
    {
        Serial.print(": ");
        Serial.print(parameters);
    }
    Serial.println();
}
String ParseInput::process_request_string(String text){
  String rtrn = "";
  for(int i = 0; i < text.length(); ++i){
    rtrn += process_request_char(text[i]);
  }
  rtrn += process_request_char('|'); // Send end character to clear any extra commands...
  return rtrn;
}
bool ParseInput::parse_command_stream(){
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
String ParseInput::process_request_char(char c){
  String rtrn = "";
  if (c == '\n' || c == '\r' || c == '\0' || c == '|') { // This is the only thing that submits a process...
    
    // check for unprocessed command text
    if (commandStream.length() > 0 && !invalidStream){
        if (!parse_command_stream()){
          invalidStream = true;
          rtrn += "\nFailed after submission";
          return rtrn;
        }
    }
    if (!invalidStream && command.length() > 0)
    {
      display();
      apply_command();
    }
    if (invalidStream){
      rtrn += "\nInvalid parameters specified: unexpected value of '";
      
      rtrn += commandStream;
      rtrn += "' at '";
      rtrn += command;
      rtrn += "' '";
      rtrn += parameters;
      rtrn += "'";
    }
    reset_command_stream();
  }
  else if (!invalidStream && c == ' ' && commandStream.length() > 0){
    commandStream.toUpperCase();
    if (!parse_command_stream()){
      rtrn += "\nFailed at loop";
      invalidStream = true;
      return rtrn;
    }
    commandStream = "";
  }
  else if(!invalidStream) {
    commandStream += c;
  }
  return rtrn;
}
bool ParseInput::is_valid_method (String text){
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
