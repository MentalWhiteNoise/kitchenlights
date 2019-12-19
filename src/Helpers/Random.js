
export default class Random {
    constructor(seed){
        this._seed =  seed % 2147483647;
        if (this._seed <= 0){ this._seed += 2147483646;}
    }
}

Random.prototype.getValue = function(){
    return this._seed / 2147483647;
}
Random.prototype.getSeed = function(){
    return this._seed;
}
Random.prototype.next = function(){
    const last = this._seed * 1.0;
    this._seed = this._seed * 16807 % 2147483647;
    if (this._seed === last)
    {
        return this.next();
    }
    return this._seed / 2147483647;
};
Random.prototype.previous = function(){
    const last = this._seed * 1.0;
    this._seed = this._seed * 1407677000 % 2147483647;
    if (this._seed === last)
    {
        return this.previous();
    }
    return this._seed / 2147483647;
};

export function getPreviousSeed(seed){
    return (seed * 1407677000) % 2147483647;
    /*
    const last = seed * 1.0;
    var newSeed = seed * 1407677000 % 2147483647;
    if (seed === last)
    {
        return getNextSeed(newSeed);
    }
    return newSeed;*/
}
export function getNextSeed(seed){
    return (seed * 16807) % 2147483647;
    /*
    const last = seed * 1.0;
    var newSeed = seed * 16807 % 2147483647;
    if (seed === last)
    {
        return getNextSeed(newSeed);
    }
    return newSeed;*/
}

export function valueFromSeed(seed){
    return seed / 2147483647;
}