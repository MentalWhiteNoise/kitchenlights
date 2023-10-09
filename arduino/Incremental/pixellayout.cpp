#include "pixellayout.h"

double GetPixelDistanceToEnd(String locationType, uint8_t pixel){
  //uint8_t instanceCount = GetInstanceCount(locationType); // do I care about how many instances?
  for (int i = 0;  i < sizeof(PixelLocation) / sizeof(PixelLocation[0]); ++i){
    if (locationType == PixelLocation[i].locationType && pixel >= PixelLocation[i].start && pixel <= PixelLocation[i].end){
      return (double)(pixel - PixelLocation[i].start) / (double)(PixelLocation[i].end - PixelLocation[i].start);
    }
  }

   return -1; // Not managed by controller
}
uint8_t MapToPhysicalLocation(uint8_t pixel){
  if (pixel > 15){ // flipping second strip
    return 25 - pixel;
  }
  return pixel;
}