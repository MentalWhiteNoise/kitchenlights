/*
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
*/
/*
uint8_t HexToInt8(String str)
{
  String newString = str.substring(1, str.length());
  char cstr[2];
  newString.toCharArray(cstr, 2);
  return (uint8_t) strtol(cstr, 0, 16);
}
uint32_t HexToInt32(String str)
{
  String newString = str.substring(1, str.length());
  while (newString.length() < 8)
  { newString += "0"; }
  char cstr[8];
  newString.toCharArray(cstr, 8);
  return (uint32_t) strtol(cstr, 0, 16);
}
*/
/*
void SetColor(uint32_t color){
  //analogWrite(redPin, 255 - RedPart(color));
  //analogWrite(greenPin, 255 - GreenPart(color));
  //analogWrite(bluePin, 255 - BluePart(color));
}
*/