// All transition functions between zero and 1, x and y

export const WaveForm = {
    FLASH_ON: "FLASH_ON", // sin(x*pi)^0.5
    FLASH_OFF: "FLASH_OFF", // 1 - sin(x*pi)^0.5
    SINUSOIDAL_ON: "SINUSOIDAL_ON", // 0.5 - cos(x*pi)/2   OR   0.5 + sin((x - 0.5)*pi)/2
    SINUSOIDAL_OFF: "SINUSOIDAL_OFF", // 0.5 + cos(x*pi)/2   OR   0.5 + sin((0.5 + x)*pi)/2
    SINUSOIDAL_FULL: "SINUSOIDAL_FULL", //  0.5 - cos(2*x*pi)/2   OR   0.5 + sin((2 * x - 0.5)*pi)/2
    LINEAR: "LINEAR" // x
}

export default function Transition(waveform, time, length /* default = 1 */, start /* default = 0 */){
    const s = (start) ? start : 0; 
    const l = (length) ? length : 1;

    // if time is before transition start:
    if (time < s) {
        // For "Off" wave forms, return on before they start
        if (waveform === WaveForm.FLASH_OFF || waveform === WaveForm.SINUSOIDAL_OFF){
            return 1;
        }
        // All other wave forms, return off before they start
        else {
            return 0;
        }
    }
    // if time is after transition end:    
    else if (time > s + l) {
        // For "On" wave forms, return on after they end
        if (waveform === WaveForm.FLASH_ON || waveform === WaveForm.SINUSOIDAL_ON){
            return 1;
        }
        // All other wave forms, return off after they end
        else {
            return 0;
        }
    }
    var x = (time - s) / l;
    switch(waveform){
        case WaveForm.FLASH_ON:
            return Math.sqrt(Math.sin(x * Math.PI));
        case WaveForm.FLASH_OFF:
            return 1 - Math.sqrt(Math.sin(x * Math.PI));
        case WaveForm.SINUSOIDAL_ON:
            return 0.5 - Math.cos(x * Math.PI)/2;
        case WaveForm.SINUSOIDAL_OFF:
            return 0.5 + Math.cos(x * Math.PI)/2;
        case WaveForm.SINUSOIDAL_FULL:
            return 0.5 - Math.cos(2 * x * Math.PI)/2;
        case WaveForm.LINEAR:
            return x;
        default:
            return x;
    }
}