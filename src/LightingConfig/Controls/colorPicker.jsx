import React, { useState } from 'react';
import { SketchPicker } from 'react-color'
import { FormControl, FormLabel, Grid } from '@material-ui/core';
import { makeStyles } from '@material-ui/core/styles';
import Slider from '@material-ui/core/Slider';

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
      position: 'absolute',
      zIndex: '2',
    },
    cover: {
      position: 'fixed',
      top: '0px',
      right: '0px',
      bottom: '0px',
      left: '0px',
    },

  }));
export default function ColorPicker(props){
    const classes = useStyles();    
    const [value, setValue] = useState({Red: props.Color.Red, Green: props.Color.Green, Blue: props.Color.Blue, White: props.Color.White});
    const handleRgbUpdate = (rgbColor) => {
        const newValue = {
            White: value.White,
            ...rgbColor
        }
        //console.log(newValue)
        setValue(newValue);
        onChange(newValue);
    }
    const onWUpdate = (whiteValue) => {
        //console.log(whiteValue);
        const newValue = {
            ...value,
            White: whiteValue
        }
        //console.log(newValue);
        setValue(newValue);
        onChange(newValue);
    }
    const onChange= (color) => {

        if (props.onChange){
            props.onChange(color);
        }
    }
    return (
        <div>  
        <FormControl style={{width:250}} disabled={props.Disabled}>
            <FormLabel style={{textAlign: "left"}} className="MuiFormLabel-root MuiInputLabel-root MuiInputLabel-formControl MuiInputLabel-animated MuiInputLabel-shrink MuiFormLabel-filled" data-shrink="true">
                {props.children}
            </FormLabel>
            <br/>
            <Grid container justify="center" alignItems="center" spacing={1} className={classes.wrapper}>
            <Grid item xs={6} >
            <MenuSwatch
                color={{r: value.Red, g: value.Green, b: value.Blue}}
                label="RGB"
                Disabled={props.Disabled}>
                <SketchPicker 
                    disableAlpha
                    color={{r:value.Red,g:value.Green,b:value.Blue}}
                    onChange={(c) => {handleRgbUpdate({Red: c.rgb.r, Green: c.rgb.g, Blue: c.rgb.b})}} />
            </MenuSwatch>
            </Grid>
            <Grid item xs={6}>
            <MenuSwatch
                color={{r: value.White, g: value.White, b: value.White}}
                label="W"
                Disabled={props.Disabled}
                >
                <FormControl>
                <Slider
                  style={{width:250}}
                  defaultValue={value.White}
                  aria-labelledby="discrete-slider"
                  valueLabelDisplay="auto"
                  //step={1}
                  //marks
                  min={0}
                  max={255}
                  onChange={(e, v) => {onWUpdate(v)}}
                />
                </FormControl>
            </MenuSwatch>
            </Grid>
            </Grid>
        </FormControl>
        </div>
    );
}

function MenuSwatch(props){
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