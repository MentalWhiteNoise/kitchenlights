import React from 'react';
import Display from './display.jsx'

export class AnimatedDisplay extends React.Component {
    constructor(props) {
      super(props);
      this.state = { tick: 0 };
      this.updateAnimationState = this.updateAnimationState.bind(this);
      this.animate = this.animate.bind(this);
    }    
    componentDidMount() {
        this.animate();
    }
    componentDidUpdate(prevState, prevProps) {
        if (prevProps.play !== this.props.play || this.state.timing !== prevState.timing)
        {
            this.animate();
        }
    }
    animate(){
        this.rAF = requestAnimationFrame(this.updateAnimationState);

    }
    updateAnimationState(prevState) {
        if (this.props.play){
            //console.log("pre:" + this.props.timing);
            //this.setState({ tick: this.state.tick + 1 });
            //this.timing.tick(this.props.lightingEffect)
            this.props.tick();
            //console.log(this.state.tick);
            //console.log(this.timing);
            //this.animate();
            //setTimeout(this.animate, 1000);
        }
    }
  
    componentWillUnmount() {
      cancelAnimationFrame(this.rAF);
    }
  
    render() {
        //console.log("animatedDisplay dimensions: ", this.props.width, ", ", this.props.height)
        if (this.state.play){
            requestAnimationFrame(this.render);
        }
      return <Display width={this.props.width} height={this.props.height} tick={this.state.tick} timing={this.props.timing} lightingEffect={this.props.lightingEffect} />;
    }
}
