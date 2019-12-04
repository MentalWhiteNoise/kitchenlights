export const ChaseMode = {
    OFF : "OFF",
    BOUNCE: "BOUNCE",
    OUTWARD: "OUTWARD",
    INWARD: "INWARD",
    CENTERBOUNCE: "CENTERBOUNCE",
    CLOCKWISE: "CLOCKWISE",
    COUNTERCLOCKWISE: "COUNTERCLOCKWISE"
}
export const ChaseTarget = {
    ALL : "ALL",
    DOORS: "DOORS",
    CABINETS: "CABINETS",
    SIDES: "SIDES",
    STRAIGHTS: "STRAIGHTS"
}

export default class LightingChase{
    constructor(from){
        if (from){
            this.Mode = from.Mode;
            this.LastMode = from.LastMode;
            this.Target = from.Target;
            this.Speed = from.Speed;
            this.Width = from.Width;
        }
        else {
            this.Mode = ChaseMode.OFF;
            this.LastMode = ChaseMode.OFF;
            this.Target = ChaseTarget.ALL;
            this.Speed = 0;
            this.Width = 0;
        }
    }
}