import React from 'react';
import ListItem from '@material-ui/core/ListItem';
import FormControl from '@material-ui/core/FormControl';
import Slider from '@material-ui/core/Slider';
import Typography from '@material-ui/core/Typography';

export default function SliderMenuItem(props){
    return (
      <ListItem
        disabled={props.Disabled}
        
      >
    <FormControl>
    <Typography gutterBottom>
      {props.Label}
    </Typography>
    <Slider
      style={{width:250}}
      defaultValue={props.Value}
      aria-labelledby="discrete-slider"
      valueLabelDisplay="auto"
      //step={1}
      //marks
      min={0}
      max={255}
      onChange={(e, v) => {props.OnChange(v)}}
      disabled={props.Disabled}
    />
    </FormControl>
    </ListItem>
    )
  }