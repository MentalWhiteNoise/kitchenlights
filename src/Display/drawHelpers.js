export function getDrawArea(canvas) {
    console.log("Canvas height: ", canvas.height, ", width: ", canvas.width)
    const kitchenWidth = 206;
    const kitchenHeight = 156;
    const border = 5;

    const scale = Math.min((canvas.height - border) / kitchenHeight, (canvas.width - border) / kitchenWidth);

    var area = {
        context: canvas.getContext("2d"),
        scale: scale,
        offset: {
            x: (canvas.width - border - kitchenWidth * scale)/2 + border/2,
            y: (canvas.height - border - kitchenHeight * scale)/2 + border/2
        }
    };
    return area;
}

export function drawPoint(drawArea, position, size){  
    drawArea.context.arc(drawArea.offset.x + position.x * drawArea.scale, drawArea.offset.y + position.y * drawArea.scale, size, 0, 2 * Math.PI, false);
    drawArea.context.fill();
}

export function drawShape(drawArea, pointArray){  
    drawArea.context.moveTo(drawArea.offset.x + pointArray[0].x * drawArea.scale, drawArea.offset.y + pointArray[0].y * drawArea.scale);
    pointArray.forEach(function(point) {
        drawArea.context.lineTo(drawArea.offset.x + point.x * drawArea.scale, drawArea.offset.y + point.y * drawArea.scale);
    });
    drawArea.context.stroke(); // Draw it
}