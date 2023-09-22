#include "lightingsupport.h"

uint8_t lookupFromString(String str, const lookup* lookuptable){
   for (int i = 0;  i < sizeof(lookuptable) / sizeof(lookuptable[0]); ++i){
     if (str == lookuptable[i].str){
       return lookuptable[i].val;
     }
   }
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
  { newString += "0"; }
  char cstr[9];
  newString.toCharArray(cstr, 9);
  return (uint32_t) strtoul(cstr, 0, 16);
}