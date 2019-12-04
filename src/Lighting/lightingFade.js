import ColorRGBW from "../ColorRGBW/colorRGBW.js";
import LightingShift from "./lightingShift.js";
import LightingChase from "./lightingChase.js";

export const FadeMode = {
    OFF : "OFF",
    PULSE: "PULSE", // Sine wave
    LINEAR: "LINEAR", // Linear increase / decrease
    HEARTBEAT: "HEARTBEAT", // Quick on / slow off
    FLASH: "FLASH", // Quick on, quick off, pause
    CHASE: "CHASE", // Fade transition happens pixel by picel in a specified chase pattern
    FLICKER: "FLICKER" // Fade transition happens immidiately on random pixels
}

const MinCycleTickTime = 10;
//const MaxCycleTickTime = 10000;
const MaxCycleTickTime = 1000;

export default class LightingFade{
    constructor(from){
        if (from){
            this.Paused = from.Paused;
            this.BgColor = ColorRGBW.Copy(from.BgColor);
            this.Mode = from.Mode;
            this.LastMode = from.LastMode;
            this.Speed = from.Speed;
            this.FlickerActivation = from.FlickerActivation;
            this.Shift = new LightingShift(from.Shift);
            this.Chase = new LightingChase(from.Chase);
        }
        else {
            this.Paused = false;
            this.BgColor = new ColorRGBW(0,0,0,0);
            this.Mode = FadeMode.OFF;
            this.LastMode = FadeMode.OFF;
            this.Speed = 0;
            this.FlickerActivation = 0;
            this.Shift = new LightingShift();
            this.Chase = new LightingChase();
        }
    }
    GetFadePct(pixel, tick){
        const CycleRate = (255-this.Speed) / 255 * (MaxCycleTickTime - MinCycleTickTime) + MinCycleTickTime;
        const CycleTime = (tick % CycleRate) / CycleRate; // 0 to 1.0
        //console.log("Speed: ", this.Speed, "CycleRate: ", CycleRate, ", CycleTime: ", CycleTime, ", Mode: ", this.Mode)
        switch(this.Mode){
            case FadeMode.OFF:
                return 0;
            case FadeMode.PULSE:
                return 1 - (0.5 - Math.cos(2*CycleTime*Math.PI) / 2);
            case FadeMode.LINEAR:
                if (CycleTime <= 0.5){
                    return 2 * CycleTime;
                }
                return 2 * (1 - CycleTime);
            case FadeMode.FLASH:
                if (CycleTime <= 0.25){
                    return 1 - (0.5 - Math.cos(2 * (CycleTime * 2) * Math.PI) / 2);
                }
                else if (CycleTime <= 0.5){
                    return 1 - (0.5 - Math.cos(2 * ((0.5 - CycleTime) * 2 * Math.PI) / 2)) / 4;
                }
                return 1;
            case FadeMode.HEARTBEAT:
                if (CycleTime <= 0.1) {
                    return 1 - (0.5 - Math.cos(2 * (CycleTime * 5) * Math.PI) / 2);
                    //GetFadePct = UnitSineWave(CycleTime * 5)
                }
                else if (CycleTime <= 0.2){
                    return 1;
                }
                else if (CycleTime <= 0.3) {
                    return 1 - (0.5 - Math.cos(2 * ((0.3 - CycleTime) * 5) * Math.PI) / 2);
                    // GetFadePct = 1 - UnitSineWave((0.3 - CycleTime) * 5)
                }
                return 1;
            case FadeMode.CHASE:
                return 0;
            case FadeMode.FLICKER:
                return 0;
            default:
                return 0;
        }
    }
    
    // enableeffect();
    // disableeffect();
    // pauseeffect();
    // resumeeffect();
    // set_mode(String mode);
    // set_speed(uint8_t value);
    // set_flickeractivation(uint8_t value);
    // set_chasemode(String mode);
    // set_chasespeed(uint8_t value);
    // set_chasewidth(uint8_t value);
    // set_chasetarget(String target);
    // set_shiftmode(String mode);
    // set_shiftamount(uint8_t value);
    // set_shifttarget(String target);
    // set_bgcolor(uint32_t color);
}
