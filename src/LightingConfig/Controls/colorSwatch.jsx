import React, { useState } from 'react';
import { makeStyles } from '@material-ui/core/styles';

const useStyles = makeStyles(theme => ({
    wrapper: {
        width:"100%",
    },
    colorSwatchLabel: {
        color: "rgba(0, 0, 0, 0.54)",
        fontFamily: ["Roboto", "Helvetica", "Arial", "sans-serif"],
        fontSize: "0.75em",
        transition: "color 200ms"
    },
    colorSwatchLabel_Hover: {
        fontFamily: ["Roboto", "Helvetica", "Arial", "sans-serif"],
        fontSize: "0.75em",
        transition: "color 200ms"
    },
    color: {
      width: '36px',
      height: '14px',
      borderRadius: '2px',
    },
    swatch: {
      padding: '5px',
      background: '#fff',
      borderRadius: '1px',
      boxShadow: '0 0 0 1px rgba(0,0,0,.1)',
      display: 'inline-block',
      transition: "box-shadow 200ms, border-width 200ms"
    },
    swatch_Hover: {
      padding: '5px',
      background: '#fff',
      borderRadius: '1px',
      boxShadow: '0 0 0 2px rgba(0,0,0,.25)',
      borderWidth: "3px",
      display: 'inline-block',
      cursor: 'pointer',
      transition: "box-shadow 200ms, border-width 200ms"
    },
    popover: {
      //position: 'fixed',      
      position: 'absolute',
      zIndex: '2',
    },
    cover: {
      position: 'fixed',
      top: '0px',
      right: '0px',
      bottom: '0px',
      left: '0px',
    }
  }));

export default function ColorSwatch(props){
    const classes = useStyles();    
    const [displayColorPicker, setDisplayColorPicker] = useState(false);
    const [hover, setHover] = useState(false);
    const handleClick = props.Disabled ? () => {} : () =>{
        setDisplayColorPicker(!displayColorPicker);
    }
    const handleClose = () =>{
        setDisplayColorPicker(false);
    }
    return (
      <div>

        <div className={ (hover) ? classes.swatch_Hover : classes.swatch } 
            onClick={ handleClick }
            onMouseEnter={() => {setHover(true && props.Disabled === false)}}
            onMouseLeave={() => {setHover(false)}}>
          <div className={ classes.color } style={{backgroundColor:"rgb(" + props.color.r + "," + props.color.g + "," + props.color.b + (props.Disabled ? ", 0.25" : "") + ")"}} />
        </div>
        &nbsp;
        <span className={(hover) ? classes.colorSwatchLabel_Hover : classes.colorSwatchLabel}>
            {props.label}
        </span>
        { displayColorPicker ? <div className={ classes.popover }>
          <div className={ classes.cover } onClick={ handleClose }/>
          {props.children}
        </div> : null }

      </div>
    );
}