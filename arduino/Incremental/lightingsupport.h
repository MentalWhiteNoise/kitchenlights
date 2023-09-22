#include "Arduino.h"
#ifndef lightingsupport_h
#define lightingsupport_h

struct lookup {uint8_t val; const char *str;};

uint8_t lookupFromString(String str, const lookup* lookuptable);

double unitSineWave(double x);

uint8_t HexToInt8(String str);
uint32_t HexToInt32(String str);

#endif