import ColorRGBW from "../ColorRGBW/colorRGBW.js";
import LightingShift from "./lightingShift.js";
import LightingChase from "./lightingChase.js";
import Transition, { WaveForm } from '../Helpers/Transitions.js'

export const FadeMode = {
    OFF : "OFF",
    SMOOTH: "SMOOTH", // Sine wave
    PULSE: "PULSE", // Quick on / slow off
    LINEAR: "LINEAR", // Linear increase / decrease
    HEARTBEAT: "HEARTBEAT", // A full pulse, followed by a smaller pulse
    FLASH: "FLASH", // Quick on, quick off, pause
    CHASE: "CHASE", // Fade transition happens pixel by pixel in a specified chase pattern
    FLICKER: "FLICKER" // random pixels fade to random percentages
}

const MinCycleTickTime = 10;
//const MaxCycleTickTime = 10000;
const MaxCycleTickTime = 1000;

export default class LightingFade{
    constructor(from){
        if (from){
            this.Paused = from.Paused; // stops the fade effect where it is, but leaves the fade value in effect
            this.BgColor = ColorRGBW.Copy(from.BgColor); // Color to fade to... typically just black.
            this.Mode = from.Mode; // method to use in applying fade
            this.LastMode = from.LastMode; // allows toggling fade on or off
            this.Speed = from.Speed; // how quickly the fade transitions between cycles. Not used for chase mode
            this.ForwardWidth = from.ForwardWidth; 
                // For smooth, pulse, or linear, % of time that pixels are fully faded. 
                // For flash, % of time faded.
                // For flicker, % of pixels fully faded
                // For chase, % of pixels that are fully forward (faded or next color)
            this.LagWidth = from.LagWidth; 
                // For smooth, pulse, or linear, % of time that pixels are fully unfaded. 
                // For flash, % of time unfaded.
                // For flicker, % of pixels fully unfaded
                // For chase, % of pixels in initial state (fully unfaded or current color)
            this.Activation = from.Activation; 
                // For flicker, how quickly do they fade in and out?
                // For chase, what % of pixels are "in transition"...  For Fade, this is the number of dark spots moving.
                // For flash, how many times in a cycle foes it flash?
            this.Randomness = from.Randomness 
                // For flicker, how how random is the strength of the flicker effect? 
                // For flash, how random are the intervals between flashes within a cycle?
                // For Heartbeat, increasing randomness should cause skips and double beats, etc.         
            this.Shift = new LightingShift(from.Shift); // Allows different areas to transition at different rates
            this.Chase = new LightingChase(from.Chase); // For chase mode, defines properties specific to the chase effect
        }
        else {
            this.Paused = false;
            this.BgColor = new ColorRGBW(0,0,0,0);
            this.Mode = FadeMode.OFF;
            this.LastMode = FadeMode.OFF;
            this.Speed = 0;
            this.ForwardWidth = 0;
            this.LagWidth = 0;
            this.Activation = 0;
            this.Randomness = 0;
            this.Shift = new LightingShift();
            this.Chase = new LightingChase();
        }
        this.CycleRate = (255-this.Speed) / 255 * (MaxCycleTickTime - MinCycleTickTime) + MinCycleTickTime;
    }
}

// Apply the mode to the percent of time into a cycle
// Note that this includes a cycle unique random seed
LightingFade.prototype.GetFadePct = function(timingPercent){        
    // if timingPercent >= 1, increment position / index ?
    const CycleTime = timingPercent % 1; // 0 to 1.0
    //console.log("Speed: ", this.Speed, "CycleRate: ", CycleRate, ", CycleTime: ", CycleTime, ", Mode: ", this.Mode)
    switch(this.Mode){
        case FadeMode.OFF:
            return 0;
        case FadeMode.PULSE:
            console.log("Pulse: { t: ", CycleTime, ", s: 0, l: 0, x: ", Transition(WaveForm.SINUSOIDAL_FULL, CycleTime), " }");
            return 1 - Transition(WaveForm.SINUSOIDAL_FULL, CycleTime);
        case FadeMode.LINEAR:
            if (CycleTime <= 0.5){
                return 1 - Transition(WaveForm.LINEAR, CycleTime, 0.5);
            }
            return Transition(WaveForm.LINEAR, CycleTime, 0.5, 0.5);
        case FadeMode.FLASH:
            return 1 - Transition(WaveForm.PULSE, CycleTime, 0.25, 0.75);
        case FadeMode.HEARTBEAT:
            const mainpulse = Transition(WaveForm.SINUSOIDAL_FULL, CycleTime, 0.25, 0);
            const secondPulse = 0.5 * Transition(WaveForm.SINUSOIDAL_FULL, CycleTime, 0.25, 0.2);
            console.log("Heartbeat: { main: ", mainpulse, ", secondary: ", secondPulse, " effect: ", (mainpulse + secondPulse), " }");
            //console.log(mainpulse + secondPulse);
            return 1 - (mainpulse + secondPulse);
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
