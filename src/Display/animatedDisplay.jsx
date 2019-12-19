import React from 'react';
import Display from './display.jsx'

export class AnimatedDisplay extends React.Component {
    constructor(props) {
      super(props);
      this.updateAnimationState = this.updateAnimationState.bind(this);
      this.animate = this.animate.bind(this);
    }    
    componentDidMount() {
        this.animate();
    }
    componentDidUpdate(prevProps) {
        if (prevProps.play !== this.props.play || this.props.timing !== prevProps.timing)
        {
            this.animate();
        }
    }
    animate(){
        this.rAF = requestAnimationFrame(this.updateAnimationState);

    }
    updateAnimationState(prevState) {
        if (this.props.play){
            this.props.tick();
        }
    }
  
    componentWillUnmount() {
      cancelAnimationFrame(this.rAF);
    }
  
    render() {
      return <Display width={this.props.width} height={this.props.height} timing={this.props.timing} lightingEffect={this.props.lightingEffect} />;
    }
}
