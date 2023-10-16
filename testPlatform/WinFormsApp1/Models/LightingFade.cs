using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using WinFormsApp1.Models;


namespace WinFormsApp1.Models
{
    public enum FadeMode : byte { FADEMODE_OFF, FADEMODE_PULSE, FADEMODE_LINEAR, FADEMODE_HEARTBEAT, FADEMODE_CHASE, FADEMODE_FLICKER };
    public class LightingFade
    {
        #region Global C++ Objects
        public static Dictionary<FadeMode, string> fademode_conversion = new Dictionary<FadeMode, string>()
        {
            { FadeMode.FADEMODE_OFF, "OFF" },
            { FadeMode.FADEMODE_PULSE, "PULSE" },
            { FadeMode.FADEMODE_LINEAR, "LINEAR"},
            { FadeMode.FADEMODE_HEARTBEAT, "HEARTBEAT"},
            { FadeMode.FADEMODE_CHASE, "CHASE"},
            {FadeMode.FADEMODE_FLICKER, "FLICKER"}
        };
        public static FadeMode string2fademode(String str) => fademode_conversion.First(x => x.Value == str).Key;
        public static String fademode2string(FadeMode mode) => fademode_conversion[mode];
        #endregion

        #region Private Properties
        public bool _bottomhalf { get; set; } // at the bottom half of the cycle
        public bool _paused { get; set; }
        public ulong _bgcolor { get; set; }
        public FadeMode _mode { get; set; }
        public FadeMode _lastmode { get; set; }
        public byte _speed { get; set; }
        public byte _flickeractivation { get; set; }
        public ulong _tick { get; set; }
        public double _pausedCyclePercent { get; set; }
        public LightingChase _chase { get; set; }
        public LightingShift _shift { get; set; }
        #endregion

        #region Public Properties
        public bool bounced { get; set; }
        #endregion

        #region Methods
        public LightingFade(ulong millis) { 
            _paused = false;
            _mode = FadeMode.FADEMODE_OFF;
            _lastmode = FadeMode.FADEMODE_OFF;
            _bgcolor = 0;
            _speed = 0;
            _flickeractivation = 127;
            _tick = millis;
            _pausedCyclePercent = _tick;
            _bottomhalf = false;
            bounced = false;
            _chase = new LightingChase();
            _shift = new LightingShift();
        }
        public double get_effect(ulong tick, ushort pixel)
        {

            if (_mode == FadeMode.FADEMODE_OFF) { return 0; }
            //note: when speed changes, be sure to capture % thru cycle, then cast tick back to preserve position within cycle...
            double cyclePercent = get_cycle_percent(tick);

            double shift = _shift.get_effect(pixel, (pixel == 0 && cyclePercent < 0.5 && !_bottomhalf), 2);
            cyclePercent = cyclePercent + shift;

            bounced = false;
            if (cyclePercent < 0.5)
            {
                if (!_bottomhalf)
                {
                    bounced = true;
                }
                _bottomhalf = true;
            }
            else { _bottomhalf = false; }

            double effect = 0;
            if (_mode == FadeMode.FADEMODE_PULSE) // Uses a sine wave
            {
                effect = 1 - LightingSupport.unitSineWave(cyclePercent);
            }
            else if (_mode == FadeMode.FADEMODE_LINEAR)
            {
                if (cyclePercent < 0.5)
                { effect = 1 - cyclePercent * 2; }
                else
                { effect = cyclePercent * 2 - 1; }
            }
            else if (_mode == FadeMode.FADEMODE_HEARTBEAT)
            {
                effect = 1 - (cyclePercent < 0.25 ? LightingSupport.unitSineWave(4 * cyclePercent) : 0) + (cyclePercent > 0.15 && cyclePercent < 0.48333 ? 0.8 * LightingSupport.unitSineWave(3 * cyclePercent) : 0);
            }
            else if (_mode == FadeMode.FADEMODE_CHASE)
            {
                double revisedCyclePercent = _chase.get_effect(cyclePercent, bounced, pixel);
                effect = 1 - LightingSupport.unitSineWave(revisedCyclePercent);
            }
            else if (_mode == FadeMode.FADEMODE_FLICKER)
            {
                throw new Exception("Not sure what to do here!");
            }
            return effect;
            //int cycleTime = -254.58420964 * power(log(x + 1), 0.5) + 600; // Sqrt model
        }

        public void enable()
        {
            if (_mode != FadeMode.FADEMODE_OFF) { return; }

            if (_lastmode != FadeMode.FADEMODE_OFF)
            {
                _mode = _lastmode;
                _lastmode = FadeMode.FADEMODE_OFF;
            }
            else
            {
                _mode = FadeMode.FADEMODE_PULSE;
                _lastmode = FadeMode.FADEMODE_OFF;
            }
        }
        public void disable()
        {
            if (_mode == FadeMode.FADEMODE_OFF) { return; }
            _lastmode = _mode;
            _mode = FadeMode.FADEMODE_OFF;
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
            FadeMode newMode = string2fademode(mode);
            if (_mode == newMode) { return; }
            _lastmode = _mode;
            _mode = newMode;
        }
        public void set_speed(byte value, ulong millis)
        {
            set_tick_at_cycle_percent(get_cycle_percent(millis), value, millis);
            _speed = value;
        }
        public void set_bgcolor(ulong color) { _bgcolor = color; }
        public ulong get_bgcolor() => _bgcolor;

        public void set_chasemode(String mode) { _chase.set_mode(mode); }
        public void set_chasewidth(byte value) { _chase.set_width(value); }
        public bool perPixel() { return _mode == FadeMode.FADEMODE_CHASE || _mode == FadeMode.FADEMODE_FLICKER || _shift.is_enabled(); }

        public void set_chasetarget(String target) { _chase.set_target(target); }

        public void set_shiftamount(byte value) { _shift.set_amount(value); }
        public void set_shiftmode(String mode) { _shift.set_mode(mode); }
        public void set_shifttarget(String target) { _shift.set_target(target); }

        public void set_flickeractivation(byte value) { _flickeractivation = value; }

        public String displaySettings()
        {
            String strOut = "\tFade Mode: " + fademode2string(_mode);
            strOut += "\n\t\t(Last Fade Mode: " + fademode2string(_lastmode);
            strOut += "\n\tBg Color: #" + Colors.ColorAsHex(_bgcolor);
            strOut += "\n\tSpeed: " + /*String*/(_speed.ToString());
            strOut += "\n\tFlicker Activation: " + /*String*/(_flickeractivation.ToString());
            strOut += "\n\tInternal: ";
            strOut += "\n\t\tStart Tick: " + /*String*/(_tick.ToString());
            strOut += "\n\t\tBottom Half of Cycle: " + /*String*/(_bottomhalf.ToString());
            strOut += "\n\t\tPaused: " + /*String*/(_paused.ToString());
            strOut += "\n\t\tPaused Cycle Percent: " + /*String*/(_pausedCyclePercent.ToString());
            strOut += "\n\tChase: \n" + _chase.displaySettings();
            return strOut;
        }
        public String toString()
        {
            String strOut = "";
            if (_paused) { strOut += (" - PAUSED - "); }
            strOut += fademode2string(_mode);
            String chaseStr = _chase.toString();
            switch (_mode)
            {
                case FadeMode.FADEMODE_OFF:
                    return "";
                    break;
                case FadeMode.FADEMODE_CHASE:
                    if (chaseStr == "")
                    {
                        strOut += (" - BROKEN CHASE -");
                    }
                    else
                    {
                        strOut += " (" + chaseStr + ")";
                    }
                    break;
                case FadeMode.FADEMODE_FLICKER:
                    if (_flickeractivation > 0) { strOut += "(" + /*String*/(_flickeractivation.ToString()) + ") "; }
                    break;
            }

            strOut += (" background: #");
            strOut += Colors.ColorAsHex(_bgcolor);

            if (_speed > 0)
            {
                strOut += (" at a speed of ");
                strOut += /*String*/(_speed.ToString());
            }
            return strOut;
        }
        public ulong get_cycle_time(byte speed)
        {
            // -108.11195963 * log(_speed + 1) + 600; // base model
            return (ulong)(1000 * (-76.42549758 * Math.Log(10 * speed + 1) + 600)); // 10x model
        }
        public double get_cycle_percent(ulong tick)
        {
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
