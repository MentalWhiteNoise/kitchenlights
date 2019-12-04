import { getDrawArea, drawPoint } from "./drawHelpers";

export default function drawLights(canvas, tick, getColor, targets) {
    const lightSpacing = 0.656;
    var drawArea = getDrawArea(canvas);
    drawArea.context.beginPath();
    drawArea.context.save();
    const c = getColor(0, tick);
    const wallThickness = 0.75;
    const inset = 0.5;

    //console.log(c);
    drawArea.context.fillStyle = "rgb(" + c.Red + ", " + c.Green + "," + c.Blue +")";

    var currentX = 0;
    var currentY = 0;
    targets.forEach(t => {
        var xStart = 0;
        var yStart = 0;
        var xIncrement = 0;
        var yIncrement = 0;
        switch (t.door){
            case 1:
                xStart = 16 - inset;
                yStart = 38+28.5-wallThickness;
                yIncrement = 0-lightSpacing;
                break;
            case 2:
                xStart = 16 - inset;
                yStart = currentY - lightSpacing;
                yIncrement = 0-lightSpacing;
                break;
            case 3:
                xStart = 16 + (wallThickness - inset)*Math.SQRT2/2;
                yStart = 28.5 - (wallThickness + inset)*Math.SQRT2/2;
                yIncrement = 0-lightSpacing*Math.SQRT2/2;
                xIncrement = lightSpacing*Math.SQRT2/2;
                break;
            case 4:
                xStart = 28.5 + wallThickness;
                yStart = 16 - inset;
                xIncrement = lightSpacing;
                break;
            case 5:
            case 7:
            case 9:
                xStart = currentX;
                yStart = 16 - inset;
                xIncrement = lightSpacing;
                break;
            case 6:
                xStart = 28.5 + 32 + wallThickness;
                yStart = 16 - inset;
                xIncrement = lightSpacing;
                break;
            case 8:
                xStart = 28.5 + 32 + 30 + wallThickness;
                yStart = 16 - inset;
                xIncrement = lightSpacing;
                break;
            case 10:
                xStart = 206-30+inset;
                yStart = wallThickness;
                yIncrement = lightSpacing;
                break;
            case 11:
            case 13:
            case 18:
                xStart = 206-30+inset;
                yStart = currentY;
                yIncrement = lightSpacing;
                break;
            case 12:
                xStart = 206-30+inset;
                yStart = 32+wallThickness;
                yIncrement = lightSpacing;
                break;
            case 14:
                xStart = 206-16+inset;
                yStart = 32+36+wallThickness;
                yIncrement = lightSpacing;
                break;
            case 15:
                xStart = 206-16+inset;
                yStart = currentY;
                yIncrement = lightSpacing;
                break;
            case 16:
                xStart = 206-16+inset;
                yStart = 32+36+(56*2/3)+wallThickness;
                yIncrement = lightSpacing;
                break;
            case 17:
                xStart = 206-30+inset;
                yStart = 32+36+56+wallThickness;
                yIncrement = lightSpacing;
                break;
            default:
        }
        currentX = xStart;
        currentY = yStart;
        drawArea.context.beginPath();
        for(var i = 0; i < t.pixelCount; i++){
            currentX += xIncrement;
            currentY += yIncrement;
            drawPoint(drawArea, {x: currentX, y: currentY}, 0.5*drawArea.scale);
        }
    })
    drawArea.context.restore();
}