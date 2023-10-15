using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WinFormsApp1.Models
{
    public static class LightingSupport
    {
        public static double unitSineWave(double x) => 1 - 0.5 * (Math.Cos(2*Math.PI* x) + 1);
        public static byte HexToInt8(string str)
        {
            string newString = str.Substring(1, str.Length);

            return (byte)int.Parse(str, System.Globalization.NumberStyles.HexNumber);
            //String newString = str.substring(1, str.length());
            /*char cstr[3];
            newString.toCharArray(cstr, 3);
            return (uint8_t)strtol(cstr, 0, 16);*/
        }
        public static ulong HexToInt32(String str)
        {
            string newString = str.Substring(1, str.Length);
            while (newString.Length < 8)
            { newString = "0" + newString; }

            return ulong.Parse(str, System.Globalization.NumberStyles.HexNumber);
            //char cstr[9];
            //newString.toCharArray(cstr, 9);
            //return (uint32_t)strtoul(cstr, 0, 16)
        }
    }
}
