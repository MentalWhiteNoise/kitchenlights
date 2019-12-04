import LightingShift from "./lightingShift.js";
import LightingChase from "./lightingChase.js";
import ColorRGBW from "../ColorRGBW/colorRGBW.js";

export const TransitionMode = {
    OFF: "OFF",
    IMMIDIATE: "IMMIDIATE",
    BLEND: "BLEND",
    STICKYBLEND: "STICKYBLEND",
    ONBOUNCE: "ONBOUNCE", // Need to know fade pct
    FLICKER: "FLICKER", // FlickerActivation
    CHASE: "CHASE",
    PIXELATE: "PIXELATE"
};

export default class LightingTransition{
    constructor(from){
        if (from){
            this.Paused = from.Paused;
            this.Mode = from.Mode;
            this.LastMode = from.LastMode;
            this.ColorArray_Length = from.ColorArray_Length;
            this.Speed = from.Speed;
            this.Width = from.Width;
            this.FlickerActivation = from.FlickerActivation;
            this.Shift = new LightingShift(from.Shift);
            this.Chase = new LightingChase(from.Chase);
            this.ColorArray = [];
            for(var i = 0; i < this.ColorArray_Length; i++){
                this.ColorArray[i] = new ColorRGBW(0,0,0,0);
                this.ColorArray[i] = ColorRGBW.Copy(from.ColorArray[i]);
            }
        }
        else {
            this.Paused = false;
            this.Mode = TransitionMode.OFF;
            this.LastMode = TransitionMode.OFF;
            this.Speed = 0;
            this.Width = 0;
            this.FlickerActivation = 0;
            this.Shift = new LightingShift();
            this.Chase = new LightingChase();
            this.ColorArray = [];
            this.ColorArray[0] = new ColorRGBW(0,0,0,0);
            this.ColorArray_Length = 1;
        }
    }
    
    Clear_ColorArray(){
        this.ColorArray_Length = 0;
    }

    Append_ColorArray(color){
        this.ColorArray[this.ColorArray_Length] = new ColorRGBW(0,0,0,0);
        this.ColorArray[this.ColorArray_Length] = ColorRGBW.Copy(color);
        this.ColorArray_Length++;
    }

    Set_Color(color){
        //console.log(color);
        this.ColorArray[0] = new ColorRGBW(0,0,0,0);
        this.ColorArray[0] = ColorRGBW.Copy(color);
        this.ColorArray_Length = 1;
    }

    Get_ColorValue(position){
        //console.log(this.ColorArray[position]);
        return this.ColorArray[position];
    }

    GetTransitionPct(pixel, tick){
        /*switch (this.Mode){
            case TransitionMode.OFF
        }*/
        return 0;
    }

    GetTransitionDistance(pixel, tick){
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


}