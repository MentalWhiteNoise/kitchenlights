import LightingFade from "./lightingFade.js";
import LightingTransition from "./lightingTransition.js";
import ColorRGBW from "../ColorRGBW/colorRGBW.js";

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
    GetPixelColor(pixel, tick){
        var color = new ColorRGBW(0,0,0,0);
        
        if (this.ColorMode === ColorMode.OFF || this.Brightness === 0){
            return color;
        }
        var fadeValue = this.Fade.GetFadePct(pixel, tick);
        
        switch (this.ColorMode){
            //case ColorMode.OFF:return color;
            case ColorMode.CONSTANT:
                if (this.Transition.ColorArray_Length > 0){
                    color = ColorRGBW.Copy(this.Transition.Get_ColorValue(0));
                }
                break;
            case ColorMode.WHITEONLY:
                if (this.Transition.ColorArray_Length > 0){
                    const c = this.Transition.Get_ColorValue(0);        
                    color = new ColorRGBW(0,0,0,(c.White === 0) ? this.Brightness : c.White);            
                    console.log(color);
                }
                break;
            case ColorMode.RANDOM:
            case ColorMode.ALTERNATE:
            case ColorMode.ORDERED:
                return color;
            default:
                return color;
        }

        //console.log("color, pre fade:", color)
        // Apply Fade
        color.Red = color.Red - fadeValue * (color.Red - this.Fade.BgColor.Red);
        color.Green = color.Green - fadeValue * (color.Green - this.Fade.BgColor.Green);
        color.Blue = color.Blue - fadeValue * (color.Blue - this.Fade.BgColor.Blue);
        color.White = color.White - fadeValue * (color.White - this.Fade.BgColor.White);

        //console.log("color, pre brightness:", color)
        // Apply Brightness
        color.Red *= this.Brightness / 255;
        color.Green *= this.Brightness / 255;
        color.Blue *= this.Brightness / 255;
        color.White *= this.Brightness / 255;

        //console.log("color, prior to return:", color)
        return color;
    }
    GetRandomColor(){

    }
}