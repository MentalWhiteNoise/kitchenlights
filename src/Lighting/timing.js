import Lighting, { ColorMode } from "./lighting.js";
import LightingFade, { FadeMode } from "./lightingFade.js";
import LightingTransition, {TransitionMode} from "./lightingTransition.js";
import ColorRGBW from "../ColorRGBW/colorRGBW.js";
import Random, {getNextSeed, valueFromSeed} from "../Random/Random.js"

export default class Timing {
    constructor(from){
        if (from){
            this.transition = {...from.transition}
            this.fade = {...from.fade}
            this.tickCount = from.tickCount;
        }
        else {
            this.transition = {
                cycle: 0,
                seed: 123456789,
                index: 0,
                percent: 0
            }
            this.fade = {
                cycle: 0,
                seed: 987532,
                percent: 0
            }
            this.tickCount = 0;
        }
    }
}

Timing.prototype.tick = function(lighting){
    this.tickCount++;
    //console.log(this.tickCount);
    const fadeCycle = (lighting.Fade.Mode === FadeMode.OFF) ? 0 : this.tickCount / lighting.Fade.CycleRate;
    const transitionCycle = (lighting.Transition.Mode === TransitionMode.OFF || lighting.ColorMode === ColorMode.OFF || lighting.ColorMode === ColorMode.WHITEONLY || lighting.ColorMode === ColorMode.CONSTANT ) ? 0 : 
        (lighting.Transition.Mode === TransitionMode.ONBOUNCE ? this.tickCount / lighting.Fade.CycleRate : this.tickCount / lighting.Transition.CycleRate);
    this.transition.percent = transitionCycle % 1;
    this.fade.percent = fadeCycle % 1;
    
    // restart to prevent overflow 
    if (lighting.ColorArray_Length < this.transition.index){
        this.transition.index = 0;
    }
    
    if (Math.trunc(fadeCycle) !== this.fade.cycle){
        this.fade.cycle = Math.trunc(fadeCycle);
        this.fade.seed = getNextSeed(this.fade.seed);
    }
    if (Math.trunc(transitionCycle) !== this.transition.cycle){
        this.transition.cycle = Math.trunc(transitionCycle);
        this.transition.seed = getNextSeed(this.transition.seed);

        // continue seed until a new color is pulled, if random
        if (lighting.ColorMode === ColorMode.RANDOM && lighting.Transition.ColorArray_Length >= 2) {
            var pTemp = Math.round(valueFromSeed(this.transition.seed) * (lighting.Transition.ColorArray_Length - 1));
            while (pTemp === this.transition.index) {
                this.transition.seed = getNextSeed(this.transition.seed);
                pTemp = Math.round(valueFromSeed(this.transition.seed) * (lighting.Transition.ColorArray_Length - 1));
            }
            this.transition.index = pTemp;
        }
    }
}
Timing.prototype.GetState = function(){
    return {
        transition: {...this.transition},
        fade: {...this.fade}
    };
}