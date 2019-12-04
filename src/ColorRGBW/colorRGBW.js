export default class ColorRGBW{
    Red = 0;
    Green = 0;
    Blue = 0;
    White = 0;
    constructor(r, g, b, w) {
        this.Red = r;
        this.Green = g;
        this.Blue = b;
        this.White = w;
    }
    SetColor (r, g, b, w){
        this.Red = r;
        this.Green = g;
        this.Blue = b;
        this.White = w;
    }
    static Copy (from){
        return {
            Red: from.Red,
            Green: from.Green,
            Blue: from.Blue,
            White: from.White
        }
    }
    static ToInteger(c){
        //console.log(c.Red * 16777216 + c.Blue * 65536 + c.Green * 256 + c.White);
        return ( c.Red << 24 ) | ( c.Green << 16 ) | ( c.Blue << 8 ) | ( c.White << 0 );
    }
    static FromInteger(i){
        return {
            Red: ( i >> 24 ) & 255,
            Green: ( i >> 16 ) & 255,
            Blue: ( i >> 8 ) & 255,
            White: ( i >> 0 ) & 255
        }
    }
}
