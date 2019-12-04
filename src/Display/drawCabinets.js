import { getDrawArea, drawShape } from "./drawHelpers";

export default function drawCabinets(canvas){
    var drawArea = getDrawArea(canvas);

    drawArea.context.save();
    drawArea.context.moveTo(0,0);
    drawArea.context.fillStyle = "black";    
    drawArea.context.fillRect(0, 0, canvas.width, canvas.height);
    drawArea.context.fill();

    // corner lowers, outline
    drawArea.context.beginPath();
    drawArea.context.lineWidth = "2";
    drawArea.context.strokeStyle = "gray";
    drawShape(drawArea, [
        {x: 0, y: 38+28.5},
        {x: 24, y: 38+28.5},
        {x: 24, y: 24},
        {x: 28.5+32+30+32, y: 24},
        {x: 28.5+32+30+32, y: 0},
        {x: 0, y: 0},
        {x: 0, y: 38+28.5},
    ]);
    
    // den side lowers, outline
    drawArea.context.beginPath();
    drawShape(drawArea, [
        {x: 206-30, y: 0},
        {x: 206, y: 0},
        {x: 206, y: 32+36+56+32},
        {x: 206-30, y: 32+36+56+32},
        {x: 206-30, y: 0}
    ]);

    // corner uppers, outline
    drawArea.context.beginPath();
    drawArea.context.lineWidth = "2";
    drawArea.context.strokeStyle = "white";
    drawShape(drawArea, [
        {x: 0, y: 38+28.5},
        {x: 16, y: 38+28.5},
        {x: 16, y: 28.5},
        {x: 28.5, y: 16},
        {x: 28.5+32+30+32, y: 16},
        {x: 28.5+32+30+32, y: 0},
        {x: 0, y: 0},
        {x: 0, y: 38+28.5},
    ]);
    
    // den side uppers, outline
    drawArea.context.beginPath();
    drawShape(drawArea, [
        {x: 206-30, y: 0},
        {x: 206, y: 0},
        {x: 206, y: 32+36+56+32},
        {x: 206-30, y: 32+36+56+32},
        {x: 206-30, y: 32+36+56},
        {x: 206-16, y: 32+36+56},
        {x: 206-16, y: 32+36},
        {x: 206-30, y: 32+36},
        {x: 206-30, y: 0}
    ]);

    // corner side cabinet dividers
    drawArea.context.beginPath();
    drawArea.context.lineWidth = "2";
    drawArea.context.strokeStyle = "white";
    drawShape(drawArea, [{x: 0, y: 28.5},{x: 16, y: 28.5}]);
    drawShape(drawArea, [{x: 28.5, y: 0},{x: 28.5, y: 16}]);
    drawShape(drawArea, [{x: 28.5+32, y: 0},{x: 28.5+32, y: 16}]);
    drawShape(drawArea, [{x: 28.5+32+30, y: 0},{x: 28.5+32+30, y: 16}]);

    // den side cabinet dividers
    drawArea.context.beginPath();
    drawArea.context.lineWidth = "2";
    drawArea.context.strokeStyle = "white";
    drawShape(drawArea, [{x: 206-30, y: 32},{x: 206, y: 32}]);
    drawShape(drawArea, [{x: 206-16, y: 32+36},{x: 206, y: 32+36}]);
    drawShape(drawArea, [{x: 206-16, y: 32+36+56},{x: 206, y: 32+36+56}]);
    
    drawArea.context.beginPath();
    drawArea.context.setLineDash([2, 5]);
    drawShape(drawArea, [{x: 0, y: 28.5+38/2},{x: 16, y: 28.5+38/2}]);
    drawShape(drawArea, [{x: 28.5+32/2, y: 0},{x: 28.5+32/2, y: 16}]);
    drawShape(drawArea, [{x: 28.5+32+30/2, y: 0},{x: 28.5+32+30/2, y: 16}]);
    drawShape(drawArea, [{x: 28.5+32+30+32/2, y: 0},{x: 28.5+32+30+32/2, y: 16}]);

    drawShape(drawArea, [{x: 206-30, y: 32/2},{x: 206, y: 32/2}]);
    drawShape(drawArea, [{x: 206-30, y: 32+36/2},{x: 206, y: 32+36/2}]);
    drawShape(drawArea, [{x: 206-16, y: 32+36+56/3},{x: 206, y: 32+36+56/3}]);
    drawShape(drawArea, [{x: 206-16, y: 32+36+56*2/3},{x: 206, y: 32+36+56*2/3}]);
    drawShape(drawArea, [{x: 206-30, y: 32+36+56+32/2},{x: 206, y: 32+36+56+32/2}]);
    drawArea.context.restore();
}
