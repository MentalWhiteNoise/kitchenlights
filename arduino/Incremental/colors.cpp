#include "colors.h"
#include "Arduino.h"

double unitSineWave(double x){
  // 0 to 1 to 0 along sine curve
  return 1 - 0.5 * (cos(2*PI*x) + 1);
}


uint8_t TransitionColorPart(uint8_t from, uint8_t to, double percent){
  return from + (to - from) * percent;
}
uint32_t TransitionColor(uint32_t from, uint32_t to, double percent){
  uint8_t r = TransitionColorPart(RedPart(from), RedPart(to), percent);
  uint8_t g = TransitionColorPart(GreenPart(from), GreenPart(to), percent);
  uint8_t b = TransitionColorPart(BluePart(from), BluePart(to), percent);
  uint8_t w = TransitionColorPart(WhitePart(from), WhitePart(to), percent);
  return AsColor(r, g, b, w);
}

uint32_t AsColor(uint8_t r, uint8_t g, uint8_t b, uint8_t w){
  return ((uint32_t)w << 24) | ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
}
uint32_t AsColor(uint8_t r, uint8_t g, uint8_t b){
  return ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
}
uint8_t WhitePart(uint32_t color){
  return (uint8_t)(color >> 24);
}
uint8_t RedPart(uint32_t color){
  return (uint8_t)(color >> 16);
}
uint8_t GreenPart(uint32_t color){
  return (uint8_t)(color >> 8);
}
uint8_t BluePart(uint32_t color){
  return (uint8_t)color;
}

uint32_t SetWhitePart(uint32_t color, uint8_t value){
   return ((uint32_t)value << 24) | ((uint32_t)(color >> 16) << 16) | ((uint32_t)(color >> 8) <<  8) | (color >> 16);
}
uint32_t SetRedPart(uint32_t color, uint8_t value){
   return ((uint32_t)(color >> 24) << 24) | ((uint32_t)value << 16) | ((uint32_t)(color >> 8) <<  8) | (color >> 16);
}
uint32_t SetGreenPart(uint32_t color, uint8_t value){
   return ((uint32_t)(color >> 24) << 24) | ((uint32_t)(color >> 16) << 16) | ((uint32_t)value <<  8) | (color >> 16);
}
uint32_t SetBluePart(uint32_t color, uint8_t value){
   return ((uint32_t)(color >> 24) << 24) | ((uint32_t)(color >> 16) << 16) | ((uint32_t)(color >> 8) <<  8) | value;
}
String ColorAsHex(uint32_t color){
  String rtrn = String(color, HEX);
  rtrn.toUpperCase();
  while (rtrn.length() < 8)
  { rtrn = "0" + rtrn; }
  return rtrn;
}