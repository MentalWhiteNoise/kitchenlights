using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WinFormsApp1.Models
{
    public enum ChaseMode { CHASEMODE_OFF, CHASEMODE_BOUNCE, CHASEMODE_OUTWARD, CHASEMODE_INWARD, CHASEMODE_CENTERBOUNCE, CHASEMODE_CLOCKWISE, CHASEMODE_COUNTER };
    public enum ChaseTarget { CHASETARGET_ALL, CHASETARGET_DOORS, CHASETARGET_CABINETS, CHASETARGET_SIDES };
    public class LightingChase
    {
        #region Global C++ Objects
        public static Dictionary<ChaseMode, string> chasemode_conversion = new Dictionary<ChaseMode, string>()
        {
            {ChaseMode.CHASEMODE_OFF, "OFF"},
            {ChaseMode.CHASEMODE_BOUNCE, "BOUNCE"},
            {ChaseMode.CHASEMODE_OUTWARD, "OUTWARD"},
            {ChaseMode.CHASEMODE_INWARD, "INWARD"},
            {ChaseMode.CHASEMODE_CENTERBOUNCE, "CENTERBOUNCE"},
            {ChaseMode.CHASEMODE_CLOCKWISE, "CLOCKWISE"},
            {ChaseMode.CHASEMODE_COUNTER, "COUNTER"},
        };
        public static ChaseMode string2chasemode(String str) => chasemode_conversion.First(x => x.Value == str).Key;
        public static String chasemode2string(ChaseMode mode) => chasemode_conversion[mode];

        public static Dictionary<ChaseTarget, string> chasetarget_conversion = new Dictionary<ChaseTarget, string>()
        {
            {ChaseTarget.CHASETARGET_ALL, "ALL"},
            {ChaseTarget.CHASETARGET_DOORS, "DOORS"},
            {ChaseTarget.CHASETARGET_CABINETS, "CABINETS"},
            {ChaseTarget.CHASETARGET_SIDES, "SIDES"},
        };
        public static ChaseTarget string2chasetarget(String str) => chasetarget_conversion.First(x => x.Value == str).Key;
        public static String chasetarget2string(ChaseTarget target) => chasetarget_conversion[target];
        #endregion

        #region Private Properties
        public ChaseMode _mode { get; set; }
        public ChaseTarget _target { get; set; }
        public byte _width { get; set; }

        #endregion

        #region Methods
        public LightingChase()
        {
            _mode = ChaseMode.CHASEMODE_OFF;
            _target = ChaseTarget.CHASETARGET_ALL;
            _width = 0;
        }
        public double get_effect(double cyclePercent, bool bounced, ushort pixel)
        {
            if (_mode == ChaseMode.CHASEMODE_OFF) { return 0; }

            double distancePct = PixelLayout.GetPixelDistanceToEnd(chasetarget2string(_target), pixel);
            double tempValue = 0;
            double activePoint = 0;
            double widthFactor = get_width_effect();

            switch (_mode)
            {
                case ChaseMode.CHASEMODE_BOUNCE:
                    { // chase cycle goes 0 to 1, then 1 to 0. 0 has no effect on 1.

                        bool increasing = cyclePercent < 0.5;
                        if (increasing)
                        {
                            activePoint = cyclePercent * 2;
                            if (distancePct <= activePoint)
                            { // if left of active point
                                tempValue = 1 - (activePoint - distancePct) * widthFactor; // 1 - distance to active point
                            }
                            else
                            { // if right of active point
                                tempValue = 1 - (activePoint + distancePct) * widthFactor; // 1 - distance to 0 + distance of active point to 0
                            }
                        }
                        else
                        {
                            activePoint = 2 - cyclePercent * 2;
                            if (distancePct >= activePoint)
                            { // if right of active point
                                tempValue = 1 - (distancePct - activePoint) * widthFactor; // 1 - distance to active point
                            }
                            else
                            { // if left of active point
                                tempValue = 1 - (2 - distancePct - activePoint) * widthFactor; // 1 - distance to end + distance active point to end
                            }
                        }
                    }
                    break;
                case ChaseMode.CHASEMODE_OUTWARD:
                    { // chase cycle goes 0.5 to 0 and repeats. 0.5 is impacted by the value at 0 (circular). 0.5 to 1.0 mimics state.
                        double effectiveCycle = (distancePct <= 0.5) ? distancePct : 1 - distancePct;
                        if (1 - cyclePercent <= effectiveCycle)
                        {
                            tempValue = 1 - (2 + cyclePercent - effectiveCycle) * widthFactor;
                        }
                        else
                        {
                            tempValue = 1 - (cyclePercent + effectiveCycle) * widthFactor;
                        }
                    }
                    break;
                case ChaseMode.CHASEMODE_INWARD:
                    { // chase cycle goes 0 to 0.5 and repeats. 0 is impacted by the value at 0.5 (circular). 1.0 to 0.5 mimics state.
                        double effectiveCycle = (distancePct <= 0.5) ? distancePct : 1 - distancePct;
                        if (cyclePercent >= effectiveCycle)
                        {
                            tempValue = 1 - (cyclePercent + effectiveCycle) * widthFactor;
                        }
                        else
                        {
                            tempValue = 1 - (effectiveCycle - cyclePercent + 1) * widthFactor;
                        }
                        break;
                    }
                case ChaseMode.CHASEMODE_CENTERBOUNCE:
                    { // chase cycle goes 0 to 0.5, then 0.5 to 0. 0 has no impact on the value at 0.5. 0.5 to 1.0 and 1.0 to 0.5 mimics state
                        double effectiveCycle = (distancePct <= 0.5) ? distancePct : 1 - distancePct;
                        if (cyclePercent <= 0.5)
                        {
                            activePoint = 0.5 - cyclePercent;
                            if (effectiveCycle < activePoint)
                            {
                                //tempValue = effectiveCycle+activePoint;
                                tempValue = 1 - (1 - effectiveCycle - activePoint) * widthFactor;
                            }
                            else
                            {
                                //tempValue = 1-effectiveCycle+activePoint;
                                tempValue = 1 - (effectiveCycle - activePoint) * widthFactor;
                            }
                        }
                        else
                        {
                            activePoint = 2 - cyclePercent;
                            if (effectiveCycle < activePoint)
                            {
                                //tempValue = 1-activePoint+effectiveCycle;
                                tempValue = 1 - (activePoint - effectiveCycle) * widthFactor;
                            }
                            else
                            {
                                //tempValue = 1-effectiveCycle-activePoint;
                                tempValue = 1 - (effectiveCycle + activePoint) * widthFactor;
                            }
                        }
                    }
                    break;
                case ChaseMode.CHASEMODE_CLOCKWISE:
                    { // chase cycle goes 0 to 1 repeating. 0 is impacted by 1 (circular).
                        if (1 - cyclePercent <= distancePct)
                        {
                            //tempValue = 2-cyclePercent-distancePct;
                            tempValue = 1 - (distancePct + cyclePercent - 1) * widthFactor;
                        }
                        else
                        {
                            //tempValue = 1-cyclePercent-distancePct;
                            tempValue = 1 - (cyclePercent + distancePct) * widthFactor;
                        }
                    }
                    break;
                case ChaseMode.CHASEMODE_COUNTER:
                    { // chase cycle goes 1 to 0 repeating. 1 is impacted by 0 (circular)
                        if (cyclePercent >= distancePct)
                        {
                            //tempValue = 1-cyclePercent+distancePct;
                            tempValue = 1 - (cyclePercent - distancePct) * widthFactor;
                        }
                        else
                        {
                            //tempValue = distancePct-cyclePercent;
                            tempValue = 1 - (cyclePercent - distancePct + 1) * widthFactor;
                        }
                    }
                    break;
            }
            return tempValue; // turning off scalign to test
                              // Scale to apply width
                              //return 1 - (1 - tempValue) * 1/get_width_effect();
        }

        public void set_mode(String mode) { _mode = string2chasemode(mode); }
        public void set_width(byte value) { _width = value; }
        public void set_target(String target) { _target = string2chasetarget(target); }

        public String displaySettings()
        {
            String strOut = "Chase Mode: " + chasemode2string(_mode);
            strOut += "\n\t\tWidth: " + /*String*/(_width.ToString());
            strOut += "\n\t\tTarget: " + chasetarget2string(_target);
            return strOut;
        }
        public String toString()
        {
            String strOut = chasemode2string(_mode);
            if (_mode == ChaseMode.CHASEMODE_OFF) { return ""; }
            strOut += (" with a width of ");
            strOut += /*String*/(_width.ToString());
            strOut += (" targeting ");
            strOut += chasetarget2string(_target);
            return strOut;
        }
        public double get_width_effect() {
            //return -5.91764255005309 * pow(log((255 - _width) + 1), 0.5) + 14.034983296193; 
            return -5.91764255005309 * Math.Pow(Math.Log((255 - _width) + 1), 0.5) + 14.034983296193; 
        }
        #endregion
    }
}
