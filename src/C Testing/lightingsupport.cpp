#include "lightingsupport.h"

uint8_t lookupFromString(String str, const lookup* lookuptable, int lookuptable_size){
   for (int i = 0;  i < lookuptable_size; ++i){
     if (str == lookuptable[i].str){
       return lookuptable[i].val;
     }
   }
   Serial.println("NOT FOUND");
   return -1;
}
uint8_t HexToInt8(String str)
{
  String newString = str.substring(1, str.length());
  char cstr[3];
  newString.toCharArray(cstr, 3);
  return (uint8_t) strtol(cstr, 0, 16);
}
uint32_t HexToInt32(String str)
{
  String newString = str.substring(1, str.length());
  while (newString.length() < 8)
  { newString = "0" + newString; }
  char cstr[9];
  newString.toCharArray(cstr, 9);
  return (uint32_t) strtoul(cstr, 0, 16);
}