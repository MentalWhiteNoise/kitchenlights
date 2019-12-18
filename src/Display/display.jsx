import React from 'react';
import drawCabinets from './drawCabinets.js'
import drawLights from './drawLights.js'

export class Display extends React.Component {
    constructor(props) {
      super(props);
      this.canvasRef = React.createRef();
      this.getColor = this.getColor.bind(this);
    }
    getColor(pixel, timing){
        return this.props.lightingEffect.GetPixelColor(pixel, this.props.timing);
    }
    componentDidMount(){
        drawCabinets(this.canvasRef.current);
    }
    componentDidUpdate(prevProps, prevState) {
        if (prevProps)
        {
            if (prevProps.width !== this.props.width || prevProps.height !== this.props.height)
            {
                drawCabinets(this.canvasRef.current);
            }
        }
        drawLights(this.canvasRef.current, this.props.tick, this.getColor, this.props.lightingEffect.Targets);
    }
    render() {
      return(
        <div>
          <canvas ref={this.canvasRef} width={this.props.width} height={this.props.height} />
        </div>
      )
    }
}
export default Display

