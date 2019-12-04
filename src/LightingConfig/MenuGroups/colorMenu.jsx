import React from 'react';
import Divider from '@material-ui/core/Divider';
import ListItem from '@material-ui/core/ListItem';
import SelectMenuItem from '../Controls/selectMenuItem.jsx';
import SliderMenuItem from '../Controls/sliderMenuItem.jsx';
import ColorPicker from '../Controls/colorPicker.jsx';
import { ColorMode} from '../../Lighting/lighting.js'

export default function ColorMenu(props){
    const {Lighting, set_colormode, set_brightness, set_color} = props;
    return (
    <>
    <SelectMenuItem 
      Label={"Color Mode"}
      Value={Lighting.ColorMode}
      OnChange={set_colormode}
      MenuItems={[
        {Value:ColorMode.OFF, Label:"OFF"},
        {Value:ColorMode.WHITEONLY, Label:"WHITEONLY"},
        {Value:ColorMode.CONSTANT, Label:"CONSTANT"},
        {Value:ColorMode.RANDOM, Label:"RANDOM"},
        {Value:ColorMode.ALTERNATE, Label:"ALTERNATE"},
        {Value:ColorMode.ORDERED, Label:"ORDERED"}
      ]}
    />
    <Divider />
    <SliderMenuItem
      Label={"Brightness"}
      OnChange={set_brightness}
      Value={Lighting.Brightness}
      Disabled={Lighting.ColorMode === ColorMode.OFF}
    />
    <ListItem>
      <ColorPicker 
          Color={Lighting.Transition.ColorArray[0]} 
          onChange={set_color}
          Disabled={Lighting.ColorMode !== ColorMode.CONSTANT}
        >
        Color
      </ColorPicker>
    </ListItem>
    </>);
  }