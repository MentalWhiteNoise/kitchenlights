import React, { useState } from 'react';
import { SketchPicker } from 'react-color'
import { FormControl, FormLabel, Grid } from '@material-ui/core';
import { makeStyles } from '@material-ui/core/styles';
import Slider from '@material-ui/core/Slider';
import ColorSwatch from './colorSwatch.jsx'

const useStyles = makeStyles(theme => ({
    wrapper: {
        width:"100%",
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
    const popUpSliderStyle = {
      padding: '5px',
      backgroundColor: 'white',
      boxShadow: '0 0 0 2px rgba(0,0,0,.25)', // NOTE: this makes the color selector look strange, but corrects for terrible look on the white selector. Maybe differentiate the two somehow?
      borderRadius: '1px',
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
                {props.label}
            </FormLabel>
            <br/>
            <Grid container justify="center" alignItems="center" spacing={1} className={classes.wrapper}>
            <Grid item xs={props.children ? 4 : 6} >
            <ColorSwatch
                color={{r: value.Red, g: value.Green, b: value.Blue}}
                label="RGB"
                Disabled={props.Disabled}>
                <SketchPicker 
                    disableAlpha
                    color={{r:value.Red,g:value.Green,b:value.Blue}}
                    onChange={(c) => {handleRgbUpdate({Red: c.rgb.r, Green: c.rgb.g, Blue: c.rgb.b})}} />
            </ColorSwatch>
            </Grid>
            <Grid item xs={ props.children ? 4 : 6 }>
            <ColorSwatch
                color={{r: value.White, g: value.White, b: value.White}}
                label="W"
                Disabled={props.Disabled}
                >
                <FormControl style={popUpSliderStyle}>
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
            </ColorSwatch>
            </Grid>
            {
              props.children ? (
                <Grid item xs={4}>
                {props.children}
                </Grid>
              ) : <></>

            }
            </Grid>
        </FormControl>
        </div>
    );
}
