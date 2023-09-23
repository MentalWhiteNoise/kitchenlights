#include "Arduino.h"
#ifndef pixellayout_h
#define pixellayout_h

struct controller {uint8_t controller; uint8_t start; uint8_t end;};

const static controller Controller [] = {
  { 0, 0, 5 },
  { 1, 6, 11 }
};

struct pixelLocation {const char *locationType; uint8_t instance; uint8_t start; uint8_t end;};

const static pixelLocation PixelLocation [] = {
    {"ALL", 0, 0, 11},
    {"STRIP", 0, 0, 5},
    {"STRIP", 1, 5, 11}
    /*
    {"ALL", 0, 0, 59, 0, 108},
    {"ALL", 0, 60, 107, 1, 108},
    {"DOOR", 0, 0	, 6, 0, 7},
    {"DOOR", 1, 7	, 13, 0, 7},
    {"DOOR", 2, 14, 23, 0, 10},
    {"DOOR", 3, 24, 29, 0, 6},
    {"DOOR", 4, 30, 35, 0, 6},
    {"DOOR", 5, 36, 42, 0, 7},
    {"DOOR", 6, 43, 49, 0, 7},
    {"DOOR", 7, 50, 54, 0, 5},
    {"DOOR", 8, 55, 59, 0, 5},
    {"DOOR", 9, 60, 64, 1, 5},
    {"DOOR", 10, 65, 69, 1, 5},
    {"DOOR", 11, 70, 74, 1, 5},
    {"DOOR", 12, 75, 79, 1, 5},
    {"DOOR", 13, 80, 85, 1, 6},
    {"DOOR", 14, 86, 91, 1, 6},
    {"DOOR", 15, 92, 97, 1, 6},
    {"DOOR", 16, 98, 102, 1, 5},
    {"DOOR", 17, 103, 107, 1, 5},
    {"CABINET", 0, 0, 13, 0, 14},
    {"CABINET", 1, 14, 23, 0, 10},
    {"CABINET", 2, 24, 35, 0, 12},
    {"CABINET", 3, 36, 49, 0, 14},
    {"CABINET", 4, 50, 59, 0, 10},
    {"CABINET", 5, 60, 69, 1, 10},
    {"CABINET", 6, 70, 79, 1, 10},
    {"CABINET", 7, 80, 97, 1, 18},
    {"CABINET", 8, 98, 107, 1, 10},
    {"SIDE", 0, 0, 59, 0, 60},
    {"SIDE", 1, 60, 107, 1, 48},
    {"STRAIGHT", 0, 0, 23, 0, 24},
    {"STRAIGHT", 1, 24, 59, 0, 36},
    {"STRAIGHT", 2, 60,	107, 1, 48}*/
};

double GetPixelDistanceToEnd(String locationType, uint8_t pixel);

#endif