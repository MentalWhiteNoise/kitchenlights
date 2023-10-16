using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WinFormsApp1.Models
{
    public enum TransitionMode { TRANSITIONMODE_OFF, TRANSITIONMODE_IMMIDIATE, TRANSITIONMODE_BLEND, TRANSITIONMODE_STICKYBLEND, TRANSITIONMODE_ONBOUNCE, TRANSITIONMODE_CHASE, TRANSITIONMODE_FLICKER, TRANSITIONMODE_PIXELATE };
    public class LightingTransition
    {
        public const byte defaultWidth = 8;
        #region Global C++ Objects
        public static Dictionary<TransitionMode, string> transitionmode_conversion = new Dictionary<TransitionMode, string>()
        {
            {TransitionMode.TRANSITIONMODE_OFF, "OFF"},
            {TransitionMode.TRANSITIONMODE_IMMIDIATE, "IMMIDIATE"},
            {TransitionMode.TRANSITIONMODE_BLEND, "BLEND"},
            {TransitionMode.TRANSITIONMODE_STICKYBLEND, "STICKYBLEND"},
            {TransitionMode.TRANSITIONMODE_ONBOUNCE, "ONBOUNCE"},
            {TransitionMode.TRANSITIONMODE_CHASE, "CHASE"},
            {TransitionMode.TRANSITIONMODE_FLICKER, "FLICKER"},
            {TransitionMode.TRANSITIONMODE_PIXELATE, "PIXELATE"}
        };
        public static TransitionMode string2transitionmode(String str) => transitionmode_conversion.First(x => x.Value == str).Key;
        public static String transitionmode2string(TransitionMode mode) => transitionmode_conversion[mode];
        #endregion

        #region Private Properties
        public bool _paused { get; set; }
        public TransitionMode _mode { get; set; }
        public TransitionMode _lastmode { get; set; }
        //uint32_t* _colorarray;
        public ulong[] _colorarray { get; set; } // Keep having problems with dynamic array...
        public byte _colorarray_length { get; set; }
        public byte _speed { get; set; }
        public byte _width { get; set; }
        public byte _flickeractivation { get; set; }
        public ulong _tick { get; set; }
        public double _pausedCyclePercent { get; set; }
        public byte _lastColorItem { get; set; }
        public LightingChase _chase { get; set; }
        public LightingShift _shift { get; set; }
        public byte _lastP0 { get; set; }
        #endregion

        #region Methods
        public LightingTransition(ulong millis)
        {
            _paused = false;
            _mode = TransitionMode.TRANSITIONMODE_OFF;
            _lastmode = TransitionMode.TRANSITIONMODE_OFF;
            //_colorarray = {(uint32_t)0,(uint32_t)0,(uint32_t)0,(uint32_t)0,(uint32_t)0,(uint32_t)0,(uint32_t)0,(uint32_t)0};
            _colorarray = new ulong[16];
            _colorarray_length = 0;
            _speed = 0;
            _width = 0;
            _flickeractivation = 127;
            _tick = millis;
            _pausedCyclePercent = _tick;
            _lastColorItem = 0;
            _chase = new LightingChase();
            _shift = new LightingShift();
            _lastP0 = 0;
        }
        //~LightingTransition();
        public ulong get_effect(ulong tick, bool fade_bounced, ushort pixel)
        {
            if (_colorarray_length == 0) { return 0; }
            if (_mode == TransitionMode.TRANSITIONMODE_OFF) { return _colorarray[0]; }
            //note: when speed changes, be sure to capture % thru cycle, then cast tick back to preserve position within cycle...
            double cyclePercent = get_cycle_percent(tick);

            bool shiftBounced = (pixel == 0 && fade_bounced || _lastP0 > 0 && (int)(cyclePercent * _colorarray_length) == 0);
            double shift = _shift.get_effect(pixel, shiftBounced, _colorarray_length);

            if (pixel == 0 && (int)(cyclePercent * _colorarray_length) == 0)
            {
                _lastP0 = (byte)((int)(cyclePercent * _colorarray_length));
            }
            cyclePercent = cyclePercent + shift;
            cyclePercent = cyclePercent - ((int)cyclePercent);

            int colorBucket = (int)(cyclePercent * _colorarray_length);
            double colorPercent = (cyclePercent * _colorarray_length) - (double)colorBucket;
            double effect = 0;
            if (_mode == TransitionMode.TRANSITIONMODE_IMMIDIATE)
            {
                return _colorarray[colorBucket];
            }
            else if (_mode == TransitionMode.TRANSITIONMODE_BLEND)
            {
                if (colorPercent == 0.5)
                { return _colorarray[colorBucket]; }
                int altBucket = colorBucket - 1;
                double altPercent = 0.5 - colorPercent;
                if (colorPercent < 0.5)
                {
                    if (altBucket == -1)
                    { altBucket = _colorarray_length - 1; }
                }
                else if (colorBucket == _colorarray_length - 1)
                {
                    altBucket = 0;
                    altPercent = colorPercent - 0.5;
                }
                else
                {
                    altBucket = colorBucket + 1;
                    altPercent = colorPercent - 0.5;
                }
                altPercent = LightingSupport.unitSineWave(altPercent / 2); // 0 to 1 along sine curve
                return Colors.TransitionColor(_colorarray[colorBucket], _colorarray[altBucket], altPercent);
            }
            else if (_mode == TransitionMode.TRANSITIONMODE_STICKYBLEND)
            {
                if (colorPercent >= 0.25 && colorPercent <= 0.75)
                { return _colorarray[colorBucket]; }
                int altBucket = colorBucket - 1;
                double altPercent = 0.5 - 2 * colorPercent; // 0.5 at 0 to 1.0 at 0.25
                if (colorPercent < 0.25)
                {
                    if (altBucket == -1)
                    { altBucket = _colorarray_length - 1; }
                }
                else if (colorBucket == _colorarray_length - 1)
                {
                    altBucket = 0;
                    altPercent = 2 * colorPercent - 1.5; // 0 at 0.75 amd 0.5 at 1
                }
                else
                {
                    altBucket = colorBucket + 1;
                    altPercent = 2 * colorPercent - 1.5; // 0 at 0.75 amd 0.5 at 1
                }

                altPercent = LightingSupport.unitSineWave(altPercent / 2); // 0 to 1 along sine curve
                return Colors.TransitionColor(_colorarray[colorBucket], _colorarray[altBucket], altPercent);

            }
            if (_mode == TransitionMode.TRANSITIONMODE_ONBOUNCE)
            {
                //Serial.print(_lastColorItem);
                //Serial.print(" ");

                if (fade_bounced)
                {
                    _lastColorItem++;
                    if (_lastColorItem >= _colorarray_length) { _lastColorItem = 0; }
                }
                return _colorarray[_lastColorItem];
            }
            if (_mode == TransitionMode.TRANSITIONMODE_CHASE)
            {
                double revisedCyclePercent = _chase.get_effect(cyclePercent, fade_bounced, pixel);
                colorBucket = (int)(revisedCyclePercent * _colorarray_length);
                colorPercent = (revisedCyclePercent * _colorarray_length) - (double)colorBucket;

                if (colorPercent == 0.5)
                { return _colorarray[colorBucket]; }
                int altBucket = colorBucket - 1;
                double altPercent = 0.5 - colorPercent;
                if (colorPercent < 0.5)
                {
                    if (altBucket == -1)
                    { altBucket = _colorarray_length - 1; }
                }
                else if (colorBucket == _colorarray_length - 1)
                {
                    altBucket = 0;
                    altPercent = colorPercent - 0.5;
                }
                else
                {
                    altBucket = colorBucket + 1;
                    altPercent = colorPercent - 0.5;
                }
                //altPercent = unitSineWave(altPercent/2); // 0 to 1 along sine curve - is this needed for chase?
                return Colors.TransitionColor(_colorarray[colorBucket], _colorarray[altBucket], altPercent);
            }
            if (_mode == TransitionMode.TRANSITIONMODE_FLICKER)
            {
                throw new Exception("Not sure what to do here!");
            }
            if (_mode == TransitionMode.TRANSITIONMODE_PIXELATE)
            {
                throw new Exception("Not sure what to do here!");
            }
            return (ulong)effect;
        }

        public void enable()
        {
            if (_mode != TransitionMode.TRANSITIONMODE_OFF) { return; }
            if (_lastmode != TransitionMode.TRANSITIONMODE_OFF)
            {
                _mode = _lastmode;
                _lastmode = TransitionMode.TRANSITIONMODE_OFF;
            }
            else
            {
                _mode = TransitionMode.TRANSITIONMODE_IMMIDIATE;
                _lastmode = TransitionMode.TRANSITIONMODE_OFF;
            }
        }
        public void disable()
        {
            if (_mode == TransitionMode.TRANSITIONMODE_OFF) { return; }
            _lastmode = _mode;
            _mode = TransitionMode.TRANSITIONMODE_OFF;
        }
        public void pause(ulong millis)
        {
            _pausedCyclePercent = get_cycle_percent(millis);
            _paused = true;
        }
        public void resume(ulong millis)
        {
            set_tick_at_cycle_percent(_pausedCyclePercent, _speed, millis);
            _paused = false;
        }
        public void set_mode(String mode)
        {
            TransitionMode newMode = string2transitionmode(mode);
            if (_mode == newMode) { return; }
            _lastmode = _mode;
            _mode = newMode;
            switch (_mode)
            {
                /*case TRANSITIONMODE_FLICKER:
                  if (_flickeractivation == 0){ _flickeractivation = defaultflickeractivation; }
                  if (_width == 0){ _width = defaultWidth; }
                  break;*/
                case TransitionMode.TRANSITIONMODE_ONBOUNCE:
                case TransitionMode.TRANSITIONMODE_CHASE: // What about a defalt chase setting?
                                           //case TRANSITIONMODE_PIXELATE:
                    if (_width == 0) { _width = defaultWidth; }
                    break;
                default: // keeps others from having width... until implemented.
                    _width = 0;
                    break;
            }
        }
        public void set_speed(byte value, ulong millis)
        {
            set_tick_at_cycle_percent(get_cycle_percent(millis), value, millis);
            _speed = value;
        }
        public byte count_colorarray() => _colorarray_length;
        public bool is_paused() => _paused;
        public bool is_enabled() => _mode != TransitionMode.TRANSITIONMODE_OFF;
        public void clear_colorarray() { _colorarray_length = 0; }
        public void append_colorarray(ulong c)
        {
            if (_colorarray_length < 16)
            {
                _colorarray_length++;
                _colorarray[_colorarray_length - 1] = c;
            }
            else
            {
                Console.WriteLine("Currently limitted to 16 colors.");
            }
        }

        public void set_chasemode(String mode) { _chase.set_mode(mode); }
        public void set_width(byte value)
        {
            _width = value;
            _chase.set_width(value);
        }

        public void set_chasetarget(String target) { _chase.set_target(target); }

        public void set_shiftamount(byte value) { _shift.set_amount(value); }
        public void set_shiftmode(String mode) { _shift.set_mode(mode); }
        public void set_shifttarget(String target) { _shift.set_target(target); }

        public void set_flickeractivation(byte value) { _flickeractivation = value; }

        public String displaySettings()
        {
            String strOut = "\tTransition Mode: " + transitionmode2string(_mode);
            strOut += "\n\t\t(Last Transition Mode: " + transitionmode2string(_lastmode);
            strOut += "\n\tSpeed: " + /*String*/(_speed.ToString());
            strOut += "\n\tWidth: " + /*String*/(_width.ToString());
            strOut += "\n\tFlicker Activation: " + /*String*/(_flickeractivation.ToString());
            strOut += "\n\tColor Array: ";
            for (int i = 0; i < _colorarray_length; i++)
            {
                strOut += "\n\t\t#" + Colors.ColorAsHex(_colorarray[i]);
            }
            strOut += "\n\tInternal: ";
            strOut += "\n\t\tStart Tick: " + /*String*/(_tick.ToString());
            strOut += "\n\t\tPaused: " + /*String*/(_paused.ToString());
            strOut += "\n\t\tPaused Cycle Percent: " + /*String*/(_pausedCyclePercent.ToString());
            strOut += "\n\tChase: \n" + _chase.displaySettings();
            return strOut;
        }
        public String toString()
        {
            String strOut = "";
            if (_mode == TransitionMode.TRANSITIONMODE_OFF) { return ""; }
            if (_paused) { strOut += (" - PAUSED - "); }
            strOut += transitionmode2string(_mode);
            strOut += (" ");
            switch (_mode)
            {
                case TransitionMode.TRANSITIONMODE_PIXELATE:
                case TransitionMode.TRANSITIONMODE_FLICKER:
                    if (_flickeractivation > 0) { strOut += "(" + /*String*/(_flickeractivation.ToString()) + ") "; }
                    break;
                case TransitionMode.TRANSITIONMODE_CHASE:
                    {
                        String chaseStr = _chase.toString();
                        if (chaseStr == "") { strOut += ("- BROKEN CHASE -"); }
                        else { strOut += "(" + chaseStr + ") "; }
                    }
                    break;
            }
            if (_colorarray_length == 0) { return ""; }
            else if (_colorarray_length == 1) { return "Static #" + Colors.ColorAsHex(_colorarray[0]); }
            else
            {
                strOut += ("between:");
                for (int i = 0; i < _colorarray_length; i++) { strOut += "\n\t#" + Colors.ColorAsHex(_colorarray[i]); }
                strOut += "\n";
            }
            if (_speed > 0)
            {
                strOut += (" at a speed of ");
                strOut += /*String*/(_speed.ToString());
            }
            if (_width > 0)
            { // what modes should have a width? 
                strOut += (" and a width of ");
                strOut += /*String*/(_width.ToString());
            }
            return strOut;
        }
        public ulong get_firstcolor()
        {
            if (_colorarray_length == 0) { return 0; }
            else { return _colorarray[0]; }
        }
        public bool perPixel()
        {
            return _mode == TransitionMode.TRANSITIONMODE_CHASE || _mode == TransitionMode.TRANSITIONMODE_FLICKER || _mode == TransitionMode.TRANSITIONMODE_PIXELATE || _shift.is_enabled();
        }


        public ulong get_cycle_time(byte speed)
        {
            // -108.11195963 * log(_speed + 1) + 600; // base model
            return (ulong)(1000 * (-76.42549758 * Math.Log(10 * speed + 1) + 600) * _colorarray_length); // 10x model
        }
        public double get_cycle_percent(ulong tick)
        {
            // how does speed relate to time?
            ulong cycleTime = get_cycle_time(_speed);
            return ((tick - _tick) % cycleTime) / (double)cycleTime;
        }
        public void set_tick_at_cycle_percent(double cyclePercent, byte speed, ulong millis)
        {
            ulong cycleTime = get_cycle_time(speed);
            _tick = (ulong)(millis - ((double)cycleTime * cyclePercent));
        }
        #endregion
    }
}
