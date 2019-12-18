import LightingFade, { FadeMode } from "./lightingFade.js";
import LightingTransition, {TransitionMode} from "./lightingTransition.js";
import ColorRGBW from "../ColorRGBW/colorRGBW.js";
import Random from '../Random/Random.js'

export const ColorMode = {
    OFF: "OFF", 
    CONSTANT: "CONSTANT",
    WHITEONLY: "WHITEONLY",
    RANDOM: "RANDOM",
    ALTERNATE: "ALTERNATE",
    ORDERED: "ORDERED"
}
function GetTargets(){
    return [
        { 
            door: 1,
            pixelCount: 27,
            cabinet: 1,
            straight: 1,
            side: 1
        },
        { 
            door: 2,
            pixelCount: 27,
            cabinet: 1,
            straight: 1,
            side: 1
        },
        { 
            door: 3,
            pixelCount: 24,
            cabinet: 2,
            straight: 1,
            side: 1
        },
        { 
            door: 4,
            pixelCount: 23,
            cabinet: 3,
            straight: 2,
            side: 1
        },
        { 
            door: 5,
            pixelCount: 23,
            cabinet: 3,
            straight: 2,
            side: 1
        },
        { 
            door: 6,
            pixelCount: 21,
            cabinet: 4,
            straight: 2,
            side: 1
        },
        { 
            door: 7,
            pixelCount: 21,
            cabinet: 4,
            straight: 2,
            side: 1
        },
        { 
            door: 8,
            pixelCount: 23,
            cabinet: 5,
            straight: 2,
            side: 1
        },
        { 
            door: 9,
            pixelCount: 23,
            cabinet: 5,
            straight: 2,
            side: 1
        },
        { 
            door: 10,
            pixelCount: 23,
            cabinet: 6,
            straight: 3,
            side: 2
        },
        { 
            door: 11,
            pixelCount: 23,
            cabinet: 6,
            straight: 3,
            side: 2
        },
        { 
            door: 12,
            pixelCount: 26,
            cabinet: 7,
            straight: 3,
            side: 2
        },
        { 
            door: 13,
            pixelCount: 26,
            cabinet: 7,
            straight: 3,
            side: 2
        },
        { 
            door: 14,
            pixelCount: 27,
            cabinet: 8,
            straight: 3,
            side: 2
        },
        { 
            door: 15,
            pixelCount: 27,
            cabinet: 8,
            straight: 3,
            side: 2
        },
        { 
            door: 16,
            pixelCount: 26,
            cabinet: 8,
            straight: 3,
            side: 2
        },
        { 
            door: 17,
            pixelCount: 23,
            cabinet: 9,
            straight: 3,
            side: 2
        },
        { 
            door: 18,
            pixelCount: 23,
            cabinet: 9,
            straight: 3,
            side: 2
        }
    ];
}
export default class Lighting {
    constructor(from){
        if (from){
            this.ColorMode = from.ColorMode;
            this.LastColorMode = from.LastColorMode;
            this.Brightness = from.Brightness;
            this.Fade = new LightingFade(from.Fade);
            this.Transition = new LightingTransition(from.Transition);
            this.Targets = from.Targets;
        }
        else {
            this.ColorMode = ColorMode.OFF;
            this.LastColorMode = ColorMode.OFF;
            this.Brightness = 0;
            this.Fade = new LightingFade();
            this.Transition = new LightingTransition();
            this.Targets = GetTargets();
        }
    }
    GetPixelColor(pixel, timing){
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
    
    GetTransitionColor(transition){
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
    GetNextPosition(transition){
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
    GetCurrentPosition(transition){
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
}