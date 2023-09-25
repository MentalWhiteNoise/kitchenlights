#include "display.h"

Display::Display(Adafruit_SSD1306 *screen){
  _screen = screen;
}
void Display::ClearDisplay(){
    _screen->clearDisplay();
    _screen->setCursor(0,0);
}
void Display::ReplaceLine(uint8_t line, String text){
    // Do I need ot keep an array of what's on the screen and redraw?
    _screen->setTextSize(1);             // Normal 1:1 pixel scale
    _screen->setTextColor(SSD1306_WHITE);        // Draw white text
    _screen->println(text);
    _screen->display();
}
void Display::StartDisplay(){
    if(!_screen->begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
      Serial.println(F("SSD1306 allocation failed"));
      for(;;); // Don't proceed, loop forever
    }
    _screen->clearDisplay();
}