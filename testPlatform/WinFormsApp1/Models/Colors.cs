using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WinFormsApp1.Models
{
    public static class Colors
    {
        public static ulong AsColor(byte r, byte g, byte b, byte w) => ((ulong)w << 24) | ((ulong)r << 16) | ((ulong)g << 8) | b;
        public static ulong AsColor(byte r, byte g, byte b) => ((ulong)r << 16) | ((ulong)g << 8) | b;
        public static byte WhitePart(ulong color) => (byte)(color >> 24);
        public static byte RedPart(ulong color) => (byte)(color >> 16);
        public static byte GreenPart(ulong color) => (byte) (color >> 8);
        public static byte BluePart(ulong color) => (byte)color;
        public static ulong SetWhitePart(ulong color, byte value) => ((ulong)value << 24) | ((ulong)(color >> 16) << 16) | ((ulong)(color >> 8) << 8) | (color >> 16);
        public static ulong SetRedPart(ulong color, byte value) => ((ulong)(color >> 24) << 24) | ((ulong)value << 16) | ((ulong)(color >> 8) << 8) | (color >> 16);
        public static ulong SetGreenPart(ulong color, byte value) => ((ulong)(color >> 24) << 24) | ((ulong)(color >> 16) << 16) | ((ulong)value << 8) | (color >> 16);
        public static ulong SetBluePart(ulong color, byte value) => ((ulong)(color >> 24) << 24) | ((ulong)(color >> 16) << 16) | ((ulong)(color >> 8) << 8) | value;
        public static ulong TransitionColor(ulong from, ulong to, double percent)
        {
            byte r = TransitionColorPart(RedPart(from), RedPart(to), percent);
            byte g = TransitionColorPart(GreenPart(from), GreenPart(to), percent);
            byte b = TransitionColorPart(BluePart(from), BluePart(to), percent);
            byte w = TransitionColorPart(WhitePart(from), WhitePart(to), percent);
            return AsColor(r, g, b, w);
        }
        public static string ColorAsHex(ulong color)
        {
            string rtrn = string.Format("{0:X}", color);
            while (rtrn.Length < 8)
            { rtrn = "0" + rtrn; }
            return rtrn;
            /*
            String rtrn = String(color, HEX);
            rtrn.toUpperCase();
            while (rtrn.length() < 8)
            { rtrn = "0" + rtrn; }
            return rtrn;*/
        }
        private static byte TransitionColorPart(byte from, byte to, double percent)
        {
            return (byte)(from + (to - from) * percent);
        }
    }
}
