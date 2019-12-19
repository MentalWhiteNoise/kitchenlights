export const ChaseMode = {
    OFF : "OFF",
    BOUNCE: "BOUNCE", // start from the left boundry, proceed to right, then move back left
    OUTWARD: "OUTWARD", // start from the center and proceed in both directions
    INWARD: "INWARD", // start from edges and proceed towards center
    CENTERBOUNCE: "CENTERBOUNCE", // start from center, proceed to edges, then proceed to center
    CLOCKWISE: "CLOCKWISE", // start from left, proceed right
    COUNTERCLOCKWISE: "COUNTERCLOCKWISE" // start from right, proceed left
}
export const ChaseTarget = {
    ALL : "ALL",
    DOORS: "DOORS",
    CABINETS: "CABINETS",
    SIDES: "SIDES",
    STRAIGHTS: "STRAIGHTS"
}
export const OverlapEffect = {
    MAX : "MAX", // Take the maximum effect. For fade, this means the maximum fade effect is applied. For color transition, this is the max of each color value
    MIN: "MIN", // Take the minimum effect. For fade, this means the minimum fade effect is applied. For color transition, this is the min of each color value
    AVERAGE: "AVERAGE" // take the average of the effect. For fade, this is applying the average fade value. For color transition, this is finding a color in between the two.
}

export default class LightingChase{
    constructor(from){
        if (from){
            this.Mode = from.Mode;
            this.LastMode = from.LastMode; // Allows toggling on/off
            this.Target = from.Target; // boundaries of chase effect
            this.Speed = from.Speed; // how quickly the "chase" moves
            this.CenterOffset = from.CenterOffset // Only used in center bounce, outward, or inward. 128 = centered, 0 = fully left (clockwise), 255 = fully right (counter clockwise)
            this.OverlapEffect = from.OverlapEffect // only on bounce or center bounce. If Activation > 0, there may be multiple transition spots at one time. How should these transitions overlap / resolve?
        }
        else {
            this.Mode = ChaseMode.OFF;
            this.LastMode = ChaseMode.OFF;
            this.Target = ChaseTarget.ALL;
            this.Speed = 0;
            this.CenterOffset = 128;
            this.OverlapEffect = OverlapEffect.AVERAGE;
        }
    }
}