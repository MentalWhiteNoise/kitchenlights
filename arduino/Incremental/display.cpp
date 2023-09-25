#include "display.h"

const static uint16_t WakeTime = 10000;

Display::Display(Adafruit_SSD1306 *screen){
  _screen = screen;
  _fixedText[0] = "";
  _fixedText[1] = "";
  _fixedText[2] = "";
  _fixedText[3] = "";
  _tempText[0] = {"",0};
  _tempText[1] = {"",0};
  _tempText[2] = {"",0};
  _tempText[3] = {"",0};
  _tick = millis();
  _sleep = false;
  _hasChanged = false;
}
void Display::ClearDisplay(){
    _screen->clearDisplay();
}
void Display::SetLine(uint8_t line, String text){
  wake();
  _fixedText[line] = text;
}
void Display::StartDisplay(){
    if(!_screen->begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
      Serial.println(F("SSD1306 allocation failed"));
      for(;;); // Don't proceed, loop forever
    }
    _screen->clearDisplay();
    _screen->setTextWrap(false); //?
}
void Display::TemporarilyReplaceLine(uint8_t line, String text, uint16_t duration){
  wake();
  _tempText[line] = {text, _tick + duration};
}
void Display::wake(){
  if (_sleep){
    _screen->ssd1306_command(SSD1306_DISPLAYON);
  }
  _hasChanged = true;
  _tick = millis();
  _sleep = false;
}
void Display::TemporarilyReplaceAll(String text, uint16_t duration){
  wake();
  int line = 0;
  String tempText = "";
  for (int i = 0; i < text.length(); i++){ 
    if (text[i] == '\n' && i < 3){
      _tempText[line] = {tempText, _tick + duration};
      line++;
    }
    else {
      tempText += text[i];
    }
  }
  if (line < 3 && tempText.length() > 0){
    line++;
    _tempText[line] = {tempText, _tick + duration};
  }
  while (line < 4){
    _tempText[line] = {"", _tick + duration};
    line++;
  }
}
void Display::draw(unsigned long tick){
  if (_sleep){
    return;
  }
  if (tick - _tick > WakeTime){
    _sleep = true;
    _hasChanged = false;
    _screen->clearDisplay();
    _screen->ssd1306_command(SSD1306_DISPLAYOFF);
  }
  else if (_hasChanged){
    _screen->clearDisplay();
    _screen->setCursor(0,0);
    _screen->setTextSize(1);
    _screen->setTextColor(SSD1306_WHITE);
    for (int i = 0; i < 4; i++){ 
      if (_tempText[i].expiration > _tick){
        _screen->println(_tempText[i].text);
      }
      else {
        _screen->println(_fixedText[i]);
      }
    }
    _screen->display();
  }
}