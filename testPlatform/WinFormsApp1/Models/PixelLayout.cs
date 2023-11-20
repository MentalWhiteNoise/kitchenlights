using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WinFormsApp1.Models
{
    public struct PixelLocation
    {
        public PixelLocation(string _locationType, byte _instance, ushort _start, ushort _end)
        {
            locationType = _locationType;
            instance = _instance;
            start = _start;
            end = _end;
        }
        public string locationType; 
        public byte instance; 
        public ushort start; 
        public ushort end;
    }
    public static class PixelLayout
    {
        public static readonly PixelLocation[] PixelLocations = new PixelLocation[]
        {
          new PixelLocation("ALL", 0, 0, 418),
          new PixelLocation("SIDES", 0, 0, 212),
          new PixelLocation( "SIDES", 1, 213, 418),
          new PixelLocation( "CABINETS", 0, 0, 43),
          new PixelLocation("CABINETS", 1, 44, 118),
          new PixelLocation("CABINETS", 2, 119, 168),
          new PixelLocation("CABINETS", 3, 169, 212),
          new PixelLocation("CABINETS", 4, 213, 254),
          new PixelLocation("CABINETS", 5, 255, 301),
          new PixelLocation("CABINETS", 6, 302, 344),
          new PixelLocation("CABINETS", 7, 345, 366),
          new PixelLocation("CABINETS", 8, 367, 418),

          new PixelLocation("ALTCABINETS", 0, 0, 43),
          new PixelLocation("ALTCABINETS", 1, 44, 67),
          new PixelLocation("ALTCABINETS", 2, 68, 118),
          new PixelLocation("ALTCABINETS", 3, 119, 168),
          new PixelLocation("ALTCABINETS", 4, 169, 212),
          new PixelLocation("ALTCABINETS", 5, 213, 254),
          new PixelLocation("ALTCABINETS", 6, 255, 301),
          new PixelLocation("ALTCABINETS", 7, 302, 344),
          new PixelLocation("ALTCABINETS", 8, 345, 366),
          new PixelLocation("ALTCABINETS", 9, 367, 418),

          new PixelLocation("DOORS", 0, 0, 21),
          new PixelLocation("DOORS", 1, 22, 43),
          new PixelLocation("DOORS", 2, 44, 67),
          new PixelLocation("DOORS", 3, 68, 92),
          new PixelLocation("DOORS", 4, 93, 118),
          new PixelLocation("DOORS", 5, 119, 143),
          new PixelLocation("DOORS", 6, 144, 168),
          new PixelLocation("DOORS", 7, 169, 190),
          new PixelLocation("DOORS", 8, 191, 212),
          new PixelLocation("DOORS", 9, 213, 233),
          new PixelLocation("DOORS", 10, 234, 254),
          new PixelLocation("DOORS", 11, 255, 277),
          new PixelLocation("DOORS", 12, 278, 301),
          new PixelLocation("DOORS", 13, 302, 322),
          new PixelLocation("DOORS", 14, 323, 344),
          new PixelLocation("DOORS", 15, 345, 366),
          new PixelLocation("DOORS", 16, 367, 392),
          new PixelLocation("DOORS", 17, 393, 418)
        };
        public static double GetPixelDistanceToEnd(String locationType, ushort pixel)
        {
            //uint8_t instanceCount = GetInstanceCount(locationType); // do I care about how many instances?
            //for (int i = 0; i < sizeof(PixelLocation) / sizeof(PixelLocation[0]); ++i)
            for (int i = 0; i < PixelLocations.Length; ++i)
            {
                if (locationType == PixelLocations[i].locationType && pixel >= PixelLocations[i].start && pixel <= PixelLocations[i].end)
                {
                    return (double)(pixel - PixelLocations[i].start) / (double)(PixelLocations[i].end - PixelLocations[i].start);
                }
            }

            return -1; // Not managed by controller
        }
        public static ushort GetLocationInstance(String locationType, ushort pixel)
        {
            if (locationType == "PIXEL")
            {
                return pixel;
            }
            //for (int i = 0; i < sizeof(PixelLocation) / sizeof(PixelLocation[0]); ++i)
            for (int i = 0; i < PixelLocations.Length; ++i)
            {
                if (locationType == PixelLocations[i].locationType && pixel >= PixelLocations[i].start && pixel <= PixelLocations[i].end)
                {
                    return PixelLocations[i].instance;
                }
            }
            return 0;
        }
        public static ushort MapToPhysicalLocation(ushort pixel)
        {
            if (pixel > 212)
            { // flipping second strip
                return (ushort)(418 - pixel);
            }
            return pixel;
        }
    }
}
