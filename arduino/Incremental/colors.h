#include "Arduino.h"
#ifndef colors_h
#define colors_h

double unitSineWave(double x);

uint32_t AsColor(uint8_t r, uint8_t g, uint8_t b, uint8_t w);
uint32_t AsColor(uint8_t r, uint8_t g, uint8_t b);
uint8_t WhitePart(uint32_t color);
uint8_t RedPart(uint32_t color);
uint8_t GreenPart(uint32_t color);
uint8_t BluePart(uint32_t color);
uint32_t SetWhitePart(uint32_t color, uint8_t value);
uint32_t SetRedPart(uint32_t color, uint8_t value);
uint32_t SetGreenPart(uint32_t color, uint8_t value);
uint32_t SetBluePart(uint32_t color, uint8_t value);
uint32_t TransitionColor(uint32_t from, uint32_t to, double percent);
String ColorAsHex(uint32_t color);

#endif