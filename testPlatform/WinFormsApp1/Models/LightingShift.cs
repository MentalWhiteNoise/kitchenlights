﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WinFormsApp1.Models
{
    public enum ShiftMode { SHIFTMODE_OFF, SHIFTMODE_ORDERED, SHIFTMODE_ALTERNATE, SHIFTMODE_RANDOM };
    public enum ShiftTarget { SHIFTTARGET_PIXEL, SHIFTTARGET_DOORS, SHIFTTARGET_CABINETS, SHIFTTARGET_SIDES, SHIFTTARGET_STRAIGHTS, SHIFTTARGET_ALTCABINETS };
    public class LightingShift
    {
        #region Global C++ Objects
        public static Dictionary<ShiftMode, string> shiftmode_conversion = new Dictionary<ShiftMode, string>()
        {
            {ShiftMode.SHIFTMODE_OFF, "OFF"},
            {ShiftMode.SHIFTMODE_ORDERED, "ORDERED"},
            {ShiftMode.SHIFTMODE_ALTERNATE, "ALTERNATE"},
            {ShiftMode.SHIFTMODE_RANDOM, "RANDOM"}
        };
        public static ShiftMode string2shiftmode(String str) => shiftmode_conversion.First(x => x.Value == str).Key;
        public static String shiftmode2string(ShiftMode mode) => shiftmode_conversion[mode];

        public static Dictionary<ShiftTarget, string> shifttarget_conversion = new Dictionary<ShiftTarget, string>()
        {
            {ShiftTarget.SHIFTTARGET_PIXEL, "PIXEL"},
            {ShiftTarget.SHIFTTARGET_DOORS, "DOORS"},
            {ShiftTarget.SHIFTTARGET_CABINETS, "CABINETS"},
            {ShiftTarget.SHIFTTARGET_SIDES, "SIDES"},
            {ShiftTarget.SHIFTTARGET_ALTCABINETS, "ALTCABINETS"},
        };
        public static ShiftTarget string2shifttarget(String str) => shifttarget_conversion.First(x => x.Value == str).Key;
        public static String shifttarget2string(ShiftTarget target) => shifttarget_conversion[target];
        #endregion

        #region Private Properties
        public ShiftMode _mode;
        public ShiftTarget _target;
        public byte _amount;
        public byte _width;
        public uint _step;

        public Random random; // for C#
        #endregion

        #region Methods

        public LightingShift()
        {
            _amount = 127;
            _width = 1;
            _mode = ShiftMode.SHIFTMODE_OFF;
            _target = ShiftTarget.SHIFTTARGET_PIXEL;
            _step = 0;

            random = new Random((int)_step); // for C#
        }
        public double get_effect(ushort pixel, bool switchEffect, byte effectCount)
        {
            double stretchAmount = (_amount + 1) / 128.0;
            if (_mode == ShiftMode.SHIFTMODE_OFF) { return 0; }

            String target = shifttarget2string(_target);
            ushort effectivePixel = pixel;
            if (_target == ShiftTarget.SHIFTTARGET_PIXEL)
            {
                effectivePixel = (ushort)(pixel / _width);
            }
            ushort instance = PixelLayout.GetLocationInstance(target, effectivePixel);
            //instances = GetInstanceCount(target);

            if (_mode == ShiftMode.SHIFTMODE_ORDERED)
            {
                /*if (switchEffect)
                {
                    _step++;
                }
                if (_step * stretchAmount > effectCount)
                {
                    _step = 0;
                }*/
                return stretchAmount * ((instance + _step) % (effectCount / stretchAmount));
            }

            if (_mode == ShiftMode.SHIFTMODE_ALTERNATE)
            {
                var totalEffects = effectCount * 2 - 2; // Less start and end, so it doesn't have dupes

                /*if (switchEffect)
                {
                    _step++;
                }
                if (_step * stretchAmount > totalEffects - 1)
                {
                    _step = 0;
                }*/
                var bucket = ((int)(stretchAmount * ((instance + _step))) % (int)(totalEffects / stretchAmount));
                if (bucket < totalEffects / stretchAmount / 2)
                {
                    return bucket;
                }
                else
                {
                    return totalEffects - bucket;
                }
            }
            if (_mode == ShiftMode.SHIFTMODE_RANDOM)
            {
                if (switchEffect)
                {
                    _step++;
                }
                if (_step > 255)
                {
                    _step = 0;
                }
                random = new Random((int)_step);
                //randomSeed(_step);
                //byte select = (byte)random(effectCount);
                byte select = (byte)random.Next(0, (int)(effectCount / stretchAmount));
                for (int i = 0; i < instance; i++)
                {
                    //select = (byte)random(effectCount);
                    select = (byte)random.Next(0, (int)(effectCount / stretchAmount));
                }
                return stretchAmount * select;
            }

            return 0;
        }

        public void set_mode(String mode) { _mode = string2shiftmode(mode); }
        public void set_amount(byte value) { _amount = value; }
        public void set_width(byte value) { _width = value; }
        public void set_target(String target) { _target = string2shifttarget(target); }

        public string displaySettings()
        {
            string strOut = "Shift Mode: " + shiftmode2string(_mode);
            strOut += "\n\t\tAmount: " + _amount.ToString();
            strOut += "\n\t\tTarget: " + shifttarget2string(_target);
            return strOut;
        }
        public string toString() {
            String strOut = shiftmode2string(_mode);
            if (_mode == ShiftMode.SHIFTMODE_OFF) { return ""; }
            //strOut += " " + String(_amount) + " ";
            strOut += " " + _amount.ToString() + " ";
            strOut += shifttarget2string(_target);
            return strOut;
        }

        public bool is_enabled()
        {
            return _mode != ShiftMode.SHIFTMODE_OFF;
        }

        // Ignore these for C# code...
        public void serialize_fade(byte[] data) { }
        public void serialize_transition(byte[] data) { }
        public void deserialize_fade(byte[] data) { }
        public void deserialize_transition(byte[] data) { }
        private void serialize(byte[] data, int start) { }
        private void deserialize(byte[] data, int start) { }
        #endregion
    }
}
