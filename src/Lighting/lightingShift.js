export const ShiftMode = {
    OFF : "OFF",
    ALTERNATE: "ALTERNATE", // if amount ~= 3, shifts +0, +1, +2, +1, +0, +1
    CYCLE: "CYCLE", // if amount ~= 3, shifts +0, +1, +2, +0, +1, +2
    RANDOM: "RANDOM" // shifts a random amount forward for each "item" - not needed initially
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
            this.LastMode = from.LastMode; // Allows toggling shift on or off
            this.Target = from.Target; // Target item to shift 
            this.Amount = from.Amount; // Number of items evenly participating in the shift. 
                // For fade, a full cycle is distributed evenly across all targets
                // For Transition, each item is incremented forward one cycle, for amount number of items
            this.Offset = from.Offset;
                // Tweeks the offset of shifted cycles. 0 - 1/nth increments, 128 - whole cycles, 255, n cycles
        }
        else {
            this.Mode = ShiftMode.OFF;
            this.LastMode = ShiftMode.OFF;
            this.Target = ShiftTarget.PIXELS;
            this.Amount = 0;
            this.Offset = 128;
        }
    }
}