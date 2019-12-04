export const ShiftMode = {
    OFF : "OFF",
    ALTERNATE: "ALTERNATE",
    RANDOM: "RANDOM"
}
export const ShiftTarget = {
    PIXELS : "PIXELS",
    DOORS: "DOORS",
    CABINETS: "CABINETS",
    SIDES: "SIDES",
    STRAIGHTS: "STRAIGHTS"
}

export default class LightingShift{
    constructor(from){
        if (from){
            this.Mode = from.Mode;
            this.LastMode = from.LastMode;
            this.Target = from.Target;
            this.Amount = from.Amount;
        }
        else {
            this.Mode = ShiftMode.OFF;
            this.LastMode = ShiftMode.OFF;
            this.Target = ShiftTarget.PIXELS;
            this.Amount = 0;
        }
    }
}