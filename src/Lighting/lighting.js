import LightingFade, { FadeMode } from "./lightingFade.js";
import LightingTransition, {TransitionMode} from "./lightingTransition.js";
import ColorRGBW from "../ColorRGBW/colorRGBW.js";
import Random from '../Helpers/Random.js'

export const ColorMode = {
    OFF: "OFF", 
    CONSTANT: "CONSTANT", // Single color, as set by first color in transition color array
    WHITEONLY: "WHITEONLY", // quick-option to ignore any color setting; not really needed, as there are other ways to create this effect
    RANDOM: "RANDOM", // Transition to random colors within the color array. 
    ALTERNATE: "ALTERNATE", // Start with the first color in the transition color array, increment through to the last, and then go bak to the first (e.g. 0,1,2,1,0,1,2...)
    ORDERED: "ORDERED" // continuously loop through the transition color array (e.g. 0,1,2,0,1,2,0...)
}

export default class Lighting {
    constructor(from){
        if (from){
            this.ColorMode = from.ColorMode;
            this.LastColorMode = from.LastColorMode; // Allows turn off / turn on, or white only toggle
            this.Brightness = from.Brightness; // An overlay controlling overall brightness (multiplier)
            this.Fade = new LightingFade(from.Fade); // Effects that fade between an active color and a fade background (typically black)
            this.Transition = new LightingTransition(from.Transition); // Effects to transition the active color between a defined set
        }
        else {
            this.ColorMode = ColorMode.OFF;
            this.LastColorMode = ColorMode.OFF;
            this.Brightness = 0;
            this.Fade = new LightingFade();
            this.Transition = new LightingTransition();
        }
    }
}
Lighting.prototype.GetPixelColor = function(pixel, timing){
    var color = new ColorRGBW(0,0,0,0);
    
    if (this.ColorMode === ColorMode.OFF || this.Brightness === 0){
        return color;
    }

    // Get location shift; (uses pixel location!)
    const locationShift = {
        transitionShift: 0,
        fadeShift: 0
    }
    //console.log("timing: ", timing)
    var fadeValue = this.Fade.GetFadePct(timing.fade.percent + locationShift.fadeShift);
    color = ColorRGBW.Copy(this.GetTransitionColor({ ...timing.transition, percent: (timing.transition.percent + locationShift.transitionShift)}));

    //console.log("color, pre fade:", color)
    // Apply Fade
    if (fadeValue !== 0) {
        color.Red = color.Red - fadeValue * (color.Red - this.Fade.BgColor.Red);
        color.Green = color.Green - fadeValue * (color.Green - this.Fade.BgColor.Green);
        color.Blue = color.Blue - fadeValue * (color.Blue - this.Fade.BgColor.Blue);
        color.White = color.White - fadeValue * (color.White - this.Fade.BgColor.White);
    }

    //console.log("color, pre brightness:", color)
    // Apply Brightness
    color.Red *= this.Brightness / 255;
    color.Green *= this.Brightness / 255;
    color.Blue *= this.Brightness / 255;
    color.White *= this.Brightness / 255;

    //console.log("color, prior to return:", color)
    return color;
}
    
Lighting.prototype.GetTransitionColor = function(transition){
    // if transition.percent >= 1, increment position / index
    const currentPosition = this.GetCurrentPosition(transition);
    var currentColor = {...this.Transition.ColorArray[currentPosition]};
    if (transition.percent === 0)
    {
        return currentColor; 
    }
    const nextPosition = this.GetNextPosition(transition);
    const nextColor = this.Transition.ColorArray[nextPosition];
    const transitionPercent = this.Transition.GetTransitionPct(transition.percent)
    currentColor.Red = currentColor.Red + (nextColor.Red - currentColor.Red) * transitionPercent;
    currentColor.Green = currentColor.Green + (nextColor.Green - currentColor.Green) * transitionPercent;
    currentColor.Blue = currentColor.Blue + (nextColor.Blue - currentColor.Blue) * transitionPercent;
    currentColor.White = currentColor.White + (nextColor.White - currentColor.White) * transitionPercent;
    return currentColor;
}

Lighting.prototype.GetNextPosition = function(transition){
    if(this.Transition.ColorArray_Length === 0 || this.Transition.ColorArray_Length === 1)
        return 0;
    if (this.ColorMode === ColorMode.RANDOM) {
        var rnd = new Random(transition.seed);
        var pTemp = Math.round(rnd.getValue() * (this.Transition.ColorArray_Length - 1));
        while (pTemp === transition.index) {
            pTemp = Math.round(rnd.next() * (this.Transition.ColorArray_Length - 1));
        }
        return pTemp;
    }
    return this.GetCurrentPosition({...transition, cycle: transition.cycle + 1 });
}

Lighting.prototype.GetCurrentPosition = function(transition){
    if(this.Transition.ColorArray_Length === 0 || this.Transition.ColorArray_Length === 1)
        return 0;
    
    switch (this.ColorMode){
        //case ColorMode.OFF:return color;
        case ColorMode.CONSTANT:
        case ColorMode.WHITEONLY:
            return 0;
        case ColorMode.RANDOM:
            return transition.index;
        case ColorMode.ALTERNATE:
            const l2 = transition.cycle % ((this.Transition.ColorArray_Length-1) * 2);
            if (l2 > this.Transition.ColorArray_Length-1){
                return ((this.Transition.ColorArray_Length-1) * 2) - l2;
            }
            return l2;
        case ColorMode.ORDERED:
            return transition.cycle % this.Transition.ColorArray_Length;
        default:
            return 0;
    }
}
