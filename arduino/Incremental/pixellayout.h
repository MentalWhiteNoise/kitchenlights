#include "Arduino.h"
#ifndef pixellayout_h
#define pixellayout_h

struct controller {uint8_t controller; uint16_t start; uint16_t end;};

const static controller Controller [] = {
  { 0, 0, 212 }, 
  { 1, 213, 418 } 
};

struct pixelLocation {const char *locationType; uint8_t instance; uint16_t start; uint16_t end;};

const static pixelLocation PixelLocation [] = {
  {"ALL", 0, 0, 418},
  {"SIDES", 0, 0, 212},
  {"SIDES", 1, 213, 418},
  {"CABINETS", 0, 0, 43},
  {"CABINETS", 1, 44, 118},
  {"CABINETS", 2, 119, 168},
  {"CABINETS", 3, 169, 212},
  {"CABINETS", 4, 213, 254},
  {"CABINETS", 5, 255, 301},
  {"CABINETS", 6, 302, 344},
  {"CABINETS", 7, 345, 366},
  {"CABINETS", 8, 367, 418},
  
  {"ALTCABINETS", 0, 0, 43},
  {"ALTCABINETS", 1, 44, 67},
  {"ALTCABINETS", 2, 68, 118},
  {"ALTCABINETS", 3, 119, 168},
  {"ALTCABINETS", 4, 169, 212},
  {"ALTCABINETS", 5, 213, 254},
  {"ALTCABINETS", 6, 255, 301},
  {"ALTCABINETS", 7, 302, 344},
  {"ALTCABINETS", 8, 345, 366},
  {"ALTCABINETS", 9, 367, 418},

  {"DOORS", 0, 0, 21},
  {"DOORS", 1, 22, 43},
  {"DOORS", 2, 44, 67},
  {"DOORS", 3, 68, 92},
  {"DOORS", 4, 93, 118},
  {"DOORS", 5, 119, 143},
  {"DOORS", 6, 144, 168},
  {"DOORS", 7, 169, 190},
  {"DOORS", 8, 191, 212},
  {"DOORS", 9, 213, 233},
  {"DOORS", 10, 234, 254},
  {"DOORS", 11, 255, 277},
  {"DOORS", 12, 278, 301},
  {"DOORS", 13, 302, 322},
  {"DOORS", 14, 323, 344},
  {"DOORS", 15, 345, 366},
  {"DOORS", 16, 367, 392},
  {"DOORS", 17, 393, 418}
};

double GetPixelDistanceToEnd(String locationType, uint16_t pixel);
uint16_t GetLocationInstance(String locationType, uint16_t pixel);
uint16_t MapToPhysicalLocation(uint16_t pixel);

#endif
