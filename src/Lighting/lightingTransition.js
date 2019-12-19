import LightingShift from "./lightingShift.js";
import LightingChase from "./lightingChase.js";
import ColorRGBW from "../ColorRGBW/colorRGBW.js";
import Transition, { WaveForm } from '../Helpers/Transitions.js'

export const TransitionMode = {
    OFF: "OFF",
    IMMEDIATE: "IMMEDIATE",
    SMOOTH: "SMOOTH", // Sine wave
    LINEAR: "LINEAR", // Linear transition
    ONBOUNCE: "ONBOUNCE", // Need to know fade pct
    //FLICKER: "FLICKER", // random pixels fade to random percentages
    CHASE: "CHASE",
    PIXELATE: "PIXELATE" // Transition pixels randomly (Q: should this be able to do pixels, doors or cabinets?)
};

const MinCycleTickTime = 10;
//const MaxCycleTickTime = 10000;
const MaxCycleTickTime = 1000;

export default class LightingTransition{
    constructor(from){
        if (from){
            this.Paused = from.Paused; // stops the transition effect where it is, but leaves the settings color in effect
            this.Mode = from.Mode; // method to transition between colors
            this.LastMode = from.LastMode; // Allows to toggle transition effects on or off
            this.ColorArray_Length = from.ColorArray_Length; // Index of array of colors to rotate through (needed for Arduino array)
            this.Speed = from.Speed; // How quickly to transition from one color to the next

            this.Width = from.Width; 
                // For smooth, linear, or pixelate, % of time (with minimum threshold) that pixels are transitioning
                // For pixelate, % of time that pixels transition
                // For chase, % of pixels that are transitioning
            this.ForwardWidth = from.ForwardWidth; 
                // For chase, % of pixels that are fully transitioned to the next color
            this.Activation = from.Activation;                 
                // For chase, what % of pixels are "in transition"...  For Transition, this allows multiple colors to be transitioning at once.
                // For pixelate, controls the slope of the number of pixels transitioning      
            
            this.Shift = new LightingShift(from.Shift);
            this.Chase = new LightingChase(from.Chase);
            this.ColorArray = []; // Array of colors to rotate through
            for(var i = 0; i < this.ColorArray_Length; i++){
                this.ColorArray.push(new ColorRGBW(0,0,0,0));
                this.ColorArray[i] = ColorRGBW.Copy(from.ColorArray[i]);
                this.ColorArray[i].key = i;
            }
        }
        else {
            this.Paused = false;
            this.Mode = TransitionMode.OFF;
            this.LastMode = TransitionMode.OFF;
            this.Speed = 0;
            this.Width = 128;
            this.ForwardWidth = 16;
            this.Activation = 0;
            this.Shift = new LightingShift();
            this.Chase = new LightingChase();
            this.ColorArray = [];
            this.ColorArray.push(new ColorRGBW(0,0,0,0));
            this.ColorArray[0].key = 0;
            this.ColorArray_Length = 1;
        }
        this.CycleRate = (255-this.Speed) / 255 * (MaxCycleTickTime - MinCycleTickTime) + MinCycleTickTime;
    }
}

LightingTransition.prototype.Clear_ColorArray = function(){
    this.ColorArray_Length = 0;
}

LightingTransition.prototype.Append_ColorArray = function(color){
    this.ColorArray.push(new ColorRGBW(0,0,0,0));
    this.ColorArray[this.ColorArray_Length] = ColorRGBW.Copy(color);
    this.ColorArray[this.ColorArray_Length].key = this.ColorArray_Length
    this.ColorArray_Length++;
}

LightingTransition.prototype.Set_Color = function(color){
    //console.log(color);
    this.ColorArray = [];
    this.ColorArray.push(new ColorRGBW(0,0,0,0));
    this.ColorArray[0] = ColorRGBW.Copy(color);
    this.ColorArray[0].key = 0;
    this.ColorArray_Length = 1;
}
LightingTransition.prototype.Get_ColorValue = function(position){
    //console.log(this.ColorArray[position]);
    return this.ColorArray[position];
}

LightingTransition.prototype.GetTransitionDistance = function(pixel, tick){
    return 0;
}
    
// void enableeffect();
// void disableeffect();
// void pauseeffect();
// void resumeeffect();
// void set_mode(String mode);
// void set_speed(uint8_t value);
// void set_width(uint8_t value);
// void set_flickeractivation(uint8_t value);
// void set_chasemode(String mode);
// void set_chasespeed(uint8_t value);
// void set_chasetarget(String target);
// void set_shiftmode(String mode);
// void set_shiftamount(uint8_t value);
// void set_shifttarget(String target);
// uint8_t count_colorarray();
// bool is_paused();
// bool is_enabled();
// void clear_colorarray();
// void append_colorarray(uint32_t c);
// uint32_t get_firstcolor();

LightingTransition.prototype.GetTransitionPct = function(timingPercent){
    const CycleTime = timingPercent % 1; // 0 to 1.0
    //console.log("Speed: ", this.Speed, "CycleRate: ", CycleRate, ", CycleTime: ", CycleTime, ", Mode: ", this.Mode)
    switch(this.Mode){
        case TransitionMode.OFF:
            return 0;
        case TransitionMode.IMMEDIATE:
            return 0;
        case TransitionMode.SMOOTH:
            return Transition(WaveForm.SINUSOIDAL_ON, CycleTime);
        case TransitionMode.LINEAR:
            return Transition(WaveForm.LINEAR, CycleTime);
        case TransitionMode.ONBOUNCE:
            return 0;
        case TransitionMode.CHASE:
            return 0;
        case TransitionMode.PIXELATE:
            return 0;
        default:
            return 0;
    }
}