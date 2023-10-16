using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WinFormsApp1.Models
{
    public static class Draw
    {
        public struct Point
        {
            public int X;
            public int Y;
            public Point(int x, int y)
            {
                X = x;
                Y = y;
            }
        }

        private static Point MapToBitmapLocation(int pixel)
        {
            if (pixel > 212)
            {
                // flipping second strip
                if (pixel < 255)
                {
                    return new Point(0, 89 + pixel - 212);
                }
                if (pixel < 302)
                {
                    return new Point(pixel - 255, 89 + pixel - 212);
                }
                else
                {
                    return new Point(pixel - 255, 179);
                }
            }

            return new Point(400,pixel);
        }
    
        public static void RenderLights(ulong tick, Lighting lighting, Bitmap target)
        {
            Color color = Color.White;
            var perPixel = lighting.perPixel();
            if (!perPixel)
            {
                var tempColor = lighting.getColor(tick, 0);
                color = Color.FromArgb(Colors.RedPart(tempColor), Colors.GreenPart(tempColor), Colors.BluePart(tempColor));
            }
            for (var i = 0; i < PixelLayout.PixelLocations.First(x => x.locationType == "ALL").end; i++)
            {   
                var pnt = MapToBitmapLocation(i);
                if (perPixel)
                {
                    var tempColor = lighting.getColor(tick, (ushort)i);
                    color = Color.FromArgb(Colors.RedPart(tempColor), Colors.GreenPart(tempColor), Colors.BluePart(tempColor));
                }                
                target.SetPixel(pnt.X * 2, (212 - pnt.Y) * 2, color);
                target.SetPixel(pnt.X * 2+1, (212 - pnt.Y) * 2, color);
                target.SetPixel(pnt.X * 2+1, (212 - pnt.Y) * 2+1, color);
                target.SetPixel(pnt.X * 2, (212 - pnt.Y) * 2+1, color);
            }
            //((Bitmap)pictureBox1.Image).SetPixel(currentX, currentY, Color.FromArgb(r, g, b));
            //pictureBox1.Refresh();
        }
    }
}
