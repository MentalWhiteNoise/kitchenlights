#include "pixellayout.h"

double GetPixelDistanceToEnd(String locationType, uint16_t pixel){
  //uint8_t instanceCount = GetInstanceCount(locationType); // do I care about how many instances?
  for (int i = 0;  i < sizeof(PixelLocation) / sizeof(PixelLocation[0]); ++i){
    if (locationType == PixelLocation[i].locationType && pixel >= PixelLocation[i].start && pixel <= PixelLocation[i].end){
      return (double)(pixel - PixelLocation[i].start) / (double)(PixelLocation[i].end - PixelLocation[i].start);
    }
  }

   return -1; // Not managed by controller
}
uint16_t GetLocationInstance(String locationType, uint16_t pixel){
  if (locationType == "PIXEL"){
    return pixel;
  }
  for (int i = 0;  i < sizeof(PixelLocation) / sizeof(PixelLocation[0]); ++i){
    if (locationType == PixelLocation[i].locationType && pixel >= PixelLocation[i].start && pixel <= PixelLocation[i].end){
      return PixelLocation[i].instance;
    }
  }
  return -1;
}
uint16_t GetInstanceCount(String locationType){
  if (locationType == "PIXEL"){
    for (int i = 0;  i < sizeof(PixelLocation) / sizeof(PixelLocation[0]); ++i){
      if (PixelLocation[i].locationType == "ALL"){
        return PixelLocation[i].end + 1;
      }
    }
  }
  uint16_t instanceCount = 0;
  for (int i = 0;  i < sizeof(PixelLocation) / sizeof(PixelLocation[0]); ++i){
    if (locationType == PixelLocation[i].locationType){
      instanceCount++;
    }
  }
  return instanceCount;
}
uint16_t MapToPhysicalLocation(uint16_t pixel){
  if (pixel > 212){ // flipping second strip
    return 418 - pixel;
  }
  return pixel;
}
