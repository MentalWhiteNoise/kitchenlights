using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using WinFormsApp1.Models;

namespace WinFormsApp1.Models
{
    public enum ColorMode : byte { COLORMODE_OFF, COLORMODE_CONSTANT, COLORMODE_WHITEONLY, COLORMODE_RANDOM, COLORMODE_ALTERNATE, COLORMODE_ORDERED };

    public class Lighting
    {
        public const ulong defaultconstantcolor = 2147483648; // #80000000 / 128 white, 0 color
        public const byte defaultbrightness = 128;
        public const byte defaultflickeractivation = 64;

        #region Global C++ Objects
        public static Dictionary<ColorMode, string> colormode_conversion = new Dictionary<ColorMode, string>()
        {
            { ColorMode.COLORMODE_OFF, "OFF" },
            { ColorMode.COLORMODE_CONSTANT, "CONSTANT" },
            { ColorMode.COLORMODE_WHITEONLY, "WHITEONLY"},
            { ColorMode.COLORMODE_RANDOM, "RANDOM"},
            { ColorMode.COLORMODE_ALTERNATE, "ALTERNATE"},
            { ColorMode.COLORMODE_ORDERED, "ORDERED"}
        };
        public static ColorMode string2colormode(String str) => colormode_conversion.First(x => x.Value == str).Key;
        public static String colormode2string(ColorMode mode) => colormode_conversion[mode];
        #endregion

        #region Private Properties
        private ColorMode _colormode { get; set; }
        private byte _brightness{ get; set; }
        private ColorMode _lastcolormode { get; set; }
        private LightingFade _fade { get; set; }
        private LightingTransition _transition{ get; set; }
        #endregion

        #region Methods
        public Lighting(ulong millis)
        {
            _colormode = ColorMode.COLORMODE_OFF;
            _lastcolormode = ColorMode.COLORMODE_OFF;
            _brightness = defaultbrightness;
            _fade = new LightingFade(millis);
            _transition = new LightingTransition(millis);

        }
        public ulong getColor(ulong tick, ushort pixel)
        {
            if (_colormode == ColorMode.COLORMODE_OFF) { return 0; }
            double fadeEffect = _fade.get_effect(tick, pixel);
            ulong color = 0;
            if (_colormode == ColorMode.COLORMODE_WHITEONLY)
            {
                return Colors.AsColor(0, 0, 0, (byte)(_brightness * (1 - fadeEffect)));
            }
            else
            {
                return Colors.TransitionColor(_transition.get_effect(tick, _fade.bounced, pixel), _fade.get_bgcolor(), fadeEffect);
            }
        }
        public void turn_on()
        {
            if (_colormode != ColorMode.COLORMODE_OFF) { return; }
            if (_lastcolormode != ColorMode.COLORMODE_OFF)
            {
                _colormode = _lastcolormode;
                _lastcolormode = ColorMode.COLORMODE_OFF;
            }
            else
            {
                if (_transition.count_colorarray() == 0) { _colormode = ColorMode.COLORMODE_WHITEONLY; }
                else if (_transition.count_colorarray() == 1) { _colormode = ColorMode.COLORMODE_CONSTANT; }
                else
                {
                    _colormode = ColorMode.COLORMODE_ORDERED;
                    //if (_transition.is_paused()) { _transition.resume(millis); }
                    if (!_transition.is_enabled()) { _transition.enable(); }
                }
                _lastcolormode = ColorMode.COLORMODE_OFF;
                if (_brightness == 0) { _brightness = defaultbrightness; }
            }
        }
        public void turn_on(ulong color)
        {
            _transition.clear_colorarray();
            _transition.append_colorarray(color);
            if (_colormode != ColorMode.COLORMODE_CONSTANT) { _lastcolormode = _colormode; }
            _colormode = ColorMode.COLORMODE_CONSTANT;
            if (_brightness == 0) { _brightness = defaultbrightness; }
        }
        public void turn_off()
        {
            if (_colormode == ColorMode.COLORMODE_OFF) { return; }
            else
            {
                _lastcolormode = _colormode;
                _colormode = ColorMode.COLORMODE_OFF;
            }
        }
        public void enable_whitemode()
        {
            if (_colormode == ColorMode.COLORMODE_WHITEONLY) { return; }
            _lastcolormode = _colormode;
            _colormode = ColorMode.COLORMODE_WHITEONLY;
            if (_brightness == 0) { _brightness = defaultbrightness; }
        }
        public void disable_whitemode()
        {
            if (_colormode != ColorMode.COLORMODE_WHITEONLY) { return; }
            if (_lastcolormode != ColorMode.COLORMODE_WHITEONLY) { _colormode = _lastcolormode; }
            else if (_transition.count_colorarray() == 0 && _colormode != ColorMode.COLORMODE_OFF)
            {
                _lastcolormode = _colormode;
                _colormode = ColorMode.COLORMODE_OFF;
            }
            else if (_transition.count_colorarray() == 1)
            {
                _lastcolormode = _colormode;
                _colormode = ColorMode.COLORMODE_CONSTANT;
            }
            else if (_transition.count_colorarray() > 1)
            {
                _lastcolormode = _colormode;
                _colormode = ColorMode.COLORMODE_ORDERED;
                //if (_transition.is_paused()) { _transition.resume(); }
                if (!_transition.is_enabled()) { _transition.enable(); }
            }
        }
        public void set_colormode(String mode)
        {
            //ColorMode { COLORMODE_OFF, COLORMODE_CONSTANT, COLORMODE_WHITEONLY, COLORMODE_RANDOM, COLORMODE_ALTERNATE, COLORMODE_ORDERED };
            ColorMode newmode = string2colormode(mode);
            if (newmode == _colormode) { return; }
            if (newmode == ColorMode.COLORMODE_OFF) { turn_off(); }
            else if (newmode == ColorMode.COLORMODE_WHITEONLY)
            {
                if (_brightness == 0) { _brightness = defaultbrightness; }
            }
            else
            { // full color or constant color
                _lastcolormode = _colormode;
                _colormode = newmode;
                if (_transition.count_colorarray() == 0) { _transition.append_colorarray(defaultconstantcolor); }
                if (_brightness == 0) { _brightness = defaultbrightness; }
            }
        }
        public void set_brightness(byte value) { _brightness = value; }
        public void set_color(ulong color) { _transition.append_colorarray(color); }

        public void set_fademode(String mode) { _fade.set_mode(mode); }
        public void disable_fade() { _fade.disable(); }
        public void pause_fade(ulong millis) { _fade.pause(millis); }
        public void resume_fade(ulong millis) { _fade.resume(millis); }
        public void enable_fade() { _fade.enable(); }
        public void set_bgcolor(ulong color) { _fade.set_bgcolor(color); }
        public void set_fadespeed(byte value, ulong millis) { _fade.set_speed(value, millis); }

        public void set_transitionmode(String mode) { _transition.set_mode(mode); }
        public void disable_transition() { _transition.disable(); }
        public void pause_transition(ulong millis) { _transition.pause(millis); }
        public void resume_transition(ulong millis) { _transition.resume(millis); }
        public void enable_transition() { _transition.enable(); }
        public void clear_colorarray() { _transition.clear_colorarray(); }
        public void append_colorarray(ulong color) { _transition.append_colorarray(color); }
        public void set_transitionspeed(byte value, ulong millis) { _transition.set_speed(value, millis); }

        public void set_fadechasemode(String mode) { _fade.set_chasemode(mode); }
        public void set_fadechasewidth(byte value) { _fade.set_chasewidth(value); }
        public void set_transitionchasemode(String mode) { _transition.set_chasemode(mode); }
        public void set_transitionwidth(byte value) { _transition.set_width(value); }
        public bool perPixel() { return _transition.perPixel() || _fade.perPixel(); }
        public bool isOff() { return _colormode == ColorMode.COLORMODE_OFF; }

        public void set_fadechasetarget(String target) { _fade.set_chasetarget(target); }
        public void set_transitionchasetarget(String target) { _transition.set_chasetarget(target); }

        public void set_fadeshiftamount(byte value) { _fade.set_shiftamount(value); }
        public void set_fadeshiftmode(String mode) { _fade.set_shiftmode(mode); }
        public void set_fadeshifttarget(String target) {_fade.set_shifttarget(target); }
        public void set_transitionshiftamount(byte value) { _transition.set_shiftamount(value); }
        public void set_transitionshiftmode(String mode) { _transition.set_shiftmode(mode); }
        public void set_transitionshifttarget(String target) { _transition.set_shifttarget(target); }

        public void set_fadeflickeractivation(byte value) { _fade.set_flickeractivation(value); }
        public void set_transitionflickeractivation(byte value) { _transition.set_flickeractivation(value); }

        //void load(); // loads from EEPROM
        //void save(); // saves to EEPROM
        public String displaySettings()
        {
            String strOut = "Color Mode: " + colormode2string(_colormode);
            strOut += "\n\t(Last Color Mode: " + colormode2string(_lastcolormode);
            strOut += "\nBrightness: " + /*String*/(_brightness.ToString());
            strOut += "\nTransition: \n" + _transition.displaySettings();
            strOut += "\nFade: \n" + _fade.displaySettings();
            return strOut;
        }
        public String toString()
        {
            String strOut = colormode2string(_colormode);
            String fadeStr = _fade.toString();
            String transitionStr;
            ulong constantColor = 0;
            switch (_colormode)
            {
                case ColorMode.COLORMODE_OFF:
                    return strOut;
                    break;
                case ColorMode.COLORMODE_CONSTANT:
                    /*if (_color == 0 || _brightness == 0){
                      strOut += F(" - BROKEN - ");
                      return strOut;
                    }*/
                    strOut += " #";
                    strOut += Colors.ColorAsHex(_transition.get_firstcolor());
                    if (_brightness < 255)
                    {
                        strOut += (" at a brightness of ");
                        strOut += /*String*/(_brightness.ToString());
                    }
                    break;
                case ColorMode.COLORMODE_WHITEONLY:
                    if (_brightness == 0)
                    {
                        strOut += (" - BROKEN - ");
                        return strOut;
                    }
                    strOut += (" at a brightness of ");
                    strOut += /*String*/(_brightness.ToString());
                    break;
                case ColorMode.COLORMODE_RANDOM:
                    if (_brightness == 0)
                    {
                        strOut += (" - BROKEN - ");
                        return strOut;
                    }
                    transitionStr = _transition.toString();
                    if (transitionStr == "")
                    {
                        strOut += F(" - BROKEN - ");
                        return strOut;
                    }
                    strOut += (" at a brightness of ");
                    strOut += /*String*/(_brightness.ToString());
                    strOut += "Transition: " + transitionStr;
                    break;
                case ColorMode.COLORMODE_ALTERNATE:
                    if (_brightness == 0)
                    {
                        strOut += (" - BROKEN - ");
                        return strOut;
                    }
                    transitionStr = _transition.toString();
                    if (transitionStr == "")
                    {
                        strOut += (" - BROKEN - ");
                        return strOut;
                    }
                    strOut += (" at a brightness of ");
                    strOut += /*String*/(_brightness.ToString());
                    strOut += ("Transition: ");
                    strOut += transitionStr;
                    break;
                case ColorMode.COLORMODE_ORDERED:
                    if (_brightness == 0)
                    {
                        strOut += (" - BROKEN - ");
                        return strOut;
                    }
                    transitionStr = _transition.toString();
                    if (transitionStr == "")
                    {
                        strOut += (" - BROKEN - ");
                        return strOut;
                    }
                    strOut += (" at a brightness of ");
                    strOut += /*String*/(_brightness.ToString());
                    strOut += "\nTransition: " + transitionStr;
                    break;
            }
            if (fadeStr != "") { strOut += "\nFade: " + fadeStr; }
            return strOut;
        }

        #endregion

    }
}
