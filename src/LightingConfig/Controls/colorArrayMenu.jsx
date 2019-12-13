import React, { useState } from 'react';
import { FormControl, FormLabel, Paper, Fab,IconButton, List, ListItem } from '@material-ui/core';
import { makeStyles } from '@material-ui/core/styles';
import ArrowDropUpIcon from '@material-ui/icons/ArrowDropUp';
import ArrowDropDownIcon from '@material-ui/icons/ArrowDropDown';
import DeleteIcon from '@material-ui/icons/Delete';
import AddIcon from '@material-ui/icons/Add';
import ColorPicker from './colorPicker.jsx'

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
      marginTop: '20px',
      //top: '20px',
      borderRadius: '1px',
      display: 'flex',
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

function GetColorArrayImage(props){
  const classes = useStyles();
  const barWidth=230;
  const barHeight=20;
  const barSpacing=10;
  const { colorArray, hover/*, Disabled*/ } = props;
  const xIncrement = barWidth / props.colorArray.length;
  return (
    <div>
    <svg width={barWidth+2} height={barHeight*2+barSpacing+2}>
    <rect x="0" y="0" width={barWidth+1} height={barHeight+1} stroke="black" strokeOpacity="0.8" />
    <rect x="0" y={barHeight + barSpacing} width={barWidth+1} height={barHeight} stroke="black" strokeOpacity="0.8" className={(hover) ? classes.colorSwatchLabel_Hover : classes.colorSwatchLabel}/>
    {colorArray.map((c, i) => {
        return (<rect key={"colorBar_" + i}
            x={xIncrement * i + 1}
            y="1" 
            width={xIncrement} 
            height={barHeight}
            fill={"rgb(" + c.Red + "," + c.Green + "," + c.Blue + ")"}/>)
      })}
      {colorArray.map((c, i) => {
          return (<rect key={"whiteBar_" + i}
              x={xIncrement * i + 1}
              y={barHeight + barSpacing} 
              width={xIncrement} 
              height={barHeight}
              fill={"rgb(" + c.White + "," + c.White + "," + c.White + ")"}/>)
        })}
    </svg>
    </div>
  );
}

export default function ColorArrayMenu(props){
    const classes = useStyles();   
    const [displayEditMenu, setDisplayEditMenu] = useState(false);
    const [hover, setHover] = useState(false);
    const { Disabled,colorArray,onChange } = props;
    const handleAddColor = () =>{
      const newColor = {Red: 255, Green: 255, Blue: 255, White:255};
      var updatedArray = [...colorArray];
      updatedArray.push(newColor);
      updatedArray = updatedArray.map((x, j) => ({ ...x, key: j}));
      onChange(updatedArray);
    }
    const handleRemoveColor = (i) => {
      var updatedArray = [...colorArray];
      updatedArray.splice(i, 1);
      updatedArray = updatedArray.map((x, j) => ({ ...x, key: j}));
      onChange(updatedArray);
    }
    const handleUpdateColor = (c,i) => {
      //var updatedArray = {...colorArray};
      colorArray[i] = c;
      //console.log(updatedArray);
      onChange(colorArray);
    }
    const handleMoveUp = (i) => {
      var updatedArray = [...colorArray]//array_move(colorArray, i, i-1);
      updatedArray.splice(i-1, 0, updatedArray[i]);
      updatedArray.splice(i+1, 1);
      onChange(updatedArray);
    }
    const handleMoveDown = (i) => {
      var updatedArray = [...colorArray]
      updatedArray.splice(i+2, 0, updatedArray[i]);
      updatedArray.splice(i, 1);
      onChange(updatedArray);
    }
    const handleClick = Disabled ? () => {} : () =>{
      setDisplayEditMenu(!displayEditMenu);
    }
    const handleClose = () =>{
      setDisplayEditMenu(false);
    } 
    return (
        <div>  
        <FormControl style={{width:250}} disabled={Disabled}>
            <FormLabel style={{textAlign: "left"}} className="MuiFormLabel-root MuiInputLabel-root MuiInputLabel-formControl MuiInputLabel-animated MuiInputLabel-shrink MuiFormLabel-filled" data-shrink="true">
                {props.label}
            </FormLabel>
            <br/>      
            <div className={ (hover) ? classes.swatch_Hover : classes.swatch } 
                onClick={ handleClick }
                onMouseEnter={() => {setHover(true && props.Disabled !== true)}}
                onMouseLeave={() => {setHover(false)}}>
              <GetColorArrayImage 
                colorArray={colorArray} 
                hover={hover}
                Disabled={props.Disabled}
                />
            </div>
              { displayEditMenu ? (
                <div className={ classes.popover }>
                <div className={ classes.cover } onClick={ handleClose }/>
                <Paper>
                <List>
                {colorArray.map((c, i) => {
                  return (<ListItem key={c.key}>
                    <ColorPicker 
                        Color={c} 
                        onChange={(cl) => { handleUpdateColor(cl, i)}}
                        Disabled={false}
                      > 
                      <div style={{whiteSpace:"nowrap"}}>
                      <IconButton color="primary" aria-label="move up" disabled={i === 0} size="small" onClick={() => {handleMoveUp(i)}}>
                        <ArrowDropUpIcon />
                      </IconButton >
                      <IconButton color="primary" aria-label="move down" disabled={i === colorArray.length - 1} size="small" onClick={() => {handleMoveDown(i)}}>
                        <ArrowDropDownIcon />
                      </IconButton>
                      <IconButton color="primary" aria-label="delete" disabled={colorArray.length === 1} size="small" onClick={() => {handleRemoveColor(i)}}>
                        <DeleteIcon />
                      </IconButton>
                      </div>
                      </ColorPicker>
                    </ListItem>);
                })}
                </List>
                <Fab color="primary" aria-label="delete" disabled={colorArray.length >= 10} size="small" onClick={handleAddColor}>
                  <AddIcon />
                </Fab>
                </Paper>
              </div>) : null }        
        </FormControl>
        </div>
    );
}