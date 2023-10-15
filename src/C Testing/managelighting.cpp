#include "managelighting.h"

// Failing with controller id = 1...

ManageLighting::ManageLighting(Lighting *lighting, WirelessPush *wirelessPush, Display *display, Adafruit_NeoPixel *strip, uint8_t controllerId){
  _lighting = lighting;
  _wirelessPush = wirelessPush;
  _display = display;
  _strip = strip;
  _controllerId = controllerId;
  _off = false;
  _debug = false;
}
void ManageLighting::ApplyCommand(String command, String parameters){
  // this is still not completely stable... 
  if(_wirelessPush){ // Should only be true for primary controller...
    _wirelessPush->PushCommand(command, parameters);
  }
  if (_display && _debug){
    String displayText = parameters;
    displayText.replace(' ', '\n');
    displayText = command + "\n" + displayText;
    _display->TemporarilyReplaceAll(displayText, 2000);
  }

  bool invalidParameter = false;
  _changed = millis();
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
    turnOn();
  }
  
  else if (command == F("OFF")){
    _lighting->turn_off();
    turnOff();
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
  else if (command == F("SET FADE CHASE TARGET")) {
    //try{
      _lighting->set_fadechasetarget(parameters);
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
  else if (command == F("SET TRANSITION CHASE TARGET")) {
    //try{
      _lighting->set_transitionchasetarget(parameters);
    //}
    //catch(...){invalidParameter = true;}
  }
  
  else if (command == F("SET TRANSITION SHIFT AMOUNT")) {
    //try{
      uint8_t v = HexToInt8(parameters);
      _lighting->set_transitionshiftamount(v);
    //}
    //catch(...){invalidParameter = true;}
  }
  else if (command == F("SET TRANSITION SHIFT MODE")) { // "OFF", "ALTERNATE", "RANDOM"
    //try{
      _lighting->set_transitionchasemode(parameters);
    //}
    //catch(...){invalidParameter = true;}
  }
  else if (command == F("SET TRANSITION SHIFT TARGET")) { // "PIXEL", "DOORS", "CABINETS", "SIDES"
    //try{
      _lighting->set_transitionshiftmode(parameters);
    //}
    //catch(...){invalidParameter = true;}
  }
  else if (command == F("SET FADE SHIFT AMOUNT")) {
    //try{
      uint8_t v = HexToInt8(parameters);
      _lighting->set_fadeshiftamount(v);
    //}
    //catch(...){invalidParameter = true;}
  }
  else if (command == F("SET FADE SHIFT MODE")) { // "OFF", "ALTERNATE", "RANDOM"
    //try{
      _lighting->set_fadechasemode(parameters);
    //}
    //catch(...){invalidParameter = true;}
  }
  else if (command == F("SET FADE SHIFT TARGET")) { // "PIXEL", "DOORS", "CABINETS", "SIDES"
    //try{
      _lighting->set_fadeshiftmode(parameters);
    //}
    //catch(...){invalidParameter = true;}
  }

  else if (command == F("SET FADE FLICKER ACTIVATION")) {
    //try{
      uint8_t v = HexToInt8(parameters);
      _lighting->set_fadeflickeractivation(v);
    //}
    //catch(...){invalidParameter = true;}
  }
  else if (command == F("SET TRANSITION FLICKER ACTIVATION")) {
    //try{
      uint8_t v = HexToInt8(parameters);
      _lighting->set_transitionflickeractivation(v);
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
void ManageLighting::turnOff(){
  _off = true;
  _strip->clear();
  _strip->show();
}
void ManageLighting::turnOn(){
  _off = false;
}
/* LED Strip Functions:
begin() / show() / clear() / setPin(int16_t p)) / 
setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b))
setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t w))
setPixelColor(uint16_t n, uint32_t c)))
fill(uint32_t c = 0, uint16_t first = 0, uint16_t count = 0)
setBrightness(uint8_t) / uint8_t getBrightness()
uint16_t numPixels()
uint32_t getPixelColor(uint16_t n) 
static uint8_t gamma8(uint8_t x) // An 8-bit gamma-correction function for basic pixel brightness adjustment. Makes color transitions appear more perceptially correct
static uint32_t gamma32(uint32_t x) // A gamma-correction function for 32-bit packed RGB or WRGB colors. Makes color transitions appear more perceptially correct.
static uint32_t Color(uint8_t r, uint8_t g, uint8_t b) 
static uint32_t Color(uint8_t r, uint8_t g, uint8_t b, uint8_t w) 
static uint32_t ColorHSV(uint16_t hue, uint8_t sat = 255, uint8_t val = 255)
*/
void ManageLighting::draw(unsigned long tick){
  if (_changed > 0){
    if (tick - _changed > 1000){
      save();
      _changed = 0;
    }
  }
  if (_off){
    return;
  }
  if (_lighting->isOff()){
    turnOff();
  }
  // _strip->setBrightness(BRIGHTNESS); // move brightness here? Would this be better for the lights?
  if (_lighting->perPixel()){
    // loop through all defined pixels on controller (based on cabinet location)
    /*
    uint32_t color1 = _lighting->getColor(tick,0);
    uint32_t color2 = _lighting->getColor(tick,8);
    uint32_t color3 = _lighting->getColor(tick,17);

    Serial.print("#");
    Serial.print(ColorAsHex(color1));
    Serial.print(" -> ");
    Serial.print(ColorAsHex(color2));
    Serial.print(" -> ");
    Serial.print(ColorAsHex(color3));
    Serial.print("     ");
    Serial.println(tick);

    delay(500);*/
    for (int i = Controller[_controllerId].start; i<=Controller[_controllerId].end;i++)
    {
      uint32_t color = _lighting->getColor(tick,i);
      // map to actual pixles on strip to set color
      _strip->setPixelColor(MapToPhysicalLocation(i), color); 
      /*Serial.print("Set ");
      Serial.print(i);
      Serial.print(" (at ");
      Serial.print(MapToPhysicalLocation(i));
      Serial.print(") to #");
      Serial.print(ColorAsHex(color));
      Serial.println();*/
    }
  }
  else {
    uint32_t color = _lighting->getColor(tick, 0);
    _strip->fill(color);
    /*// For each pixel in strip...
    for(int i=0; i<_strip->numPixels(); i++) { 
      _strip->setPixelColor(i, color);
    }*/
  }
  _strip->show();
  delay(50);
}

void ManageLighting::save(){
  byte data[92];
  _lighting->serialize((byte*)&data);
  //Serial.println(String(data));

  int addressIndex = 0;
  for (int i = 0; i < sizeof(data); i++) 
  {
    //EEPROM.write(addressIndex + i, data[i]);
    EEPROM.write(addressIndex + i, data[i]);
  }
  char hexCar[2];
  Serial.print("save: ");
  for(int i=0; i<sizeof(data); i++){

    sprintf(hexCar, "%02X", (uint8_t)data[i]);
    Serial.print(hexCar);
 }
  Serial.println();
  EEPROM.commit();
}
void ManageLighting::load(){
  byte data[92];

  int addressIndex = 0;

  for (int i = 0; i < sizeof(data); i++)
  { data[i] = EEPROM.read(addressIndex + i); }

  // load from EEPROM
  _lighting->deserialize((byte*)&data);

  char hexCar[2];
  Serial.print("load: ");
  for(int i=0; i<sizeof(data); i++){

    sprintf(hexCar, "%02X", (uint8_t)data[i]);
    Serial.print(hexCar);
 }
  Serial.println();
}