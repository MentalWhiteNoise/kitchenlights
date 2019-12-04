import React from 'react';
import Divider from '@material-ui/core/Divider';
import List from '@material-ui/core/List';
import ListItem from '@material-ui/core/ListItem';
import SelectMenuItem from '../Controls/selectMenuItem.jsx';
//import SliderMenuItem from '../Controls/sliderMenuItem.jsx';
import ExpansionMenuItem from '../Controls/expansionMenuItem.jsx';
//import ColorPicker from '../Controls/colorPicker.jsx';
import { ColorMode} from '../../Lighting/lighting.js'
import { TransitionMode} from '../../Lighting/lightingTransition.js'
import SliderMenuItem from '../Controls/sliderMenuItem.jsx';
import { ShiftMode, ShiftTarget } from '../../Lighting/lightingShift.js'
import { ChaseMode, ChaseTarget } from '../../Lighting/lightingChase.js'

export default function TransitionMenu(props){
    const {Lighting, set_transitionmode, OnExpand, Expanded} = props;
    return (
    <>
    <SelectMenuItem 
      Label={"Transition Mode"}
      Value={Lighting.Transition.Mode}
      OnChange={set_transitionmode}
      MenuItems={[
        {Value:TransitionMode.OFF, Label:"OFF"},
        {Value:TransitionMode.IMMIDIATE, Label:"IMMIDIATE"},
        {Value:TransitionMode.BLEND, Label:"BLEND"},
        {Value:TransitionMode.STICKYBLEND, Label:"STICKYBLEND"},
        {Value:TransitionMode.ONBOUNCE, Label:"ONBOUNCE"},
        {Value:TransitionMode.FLICKER, Label:"FLICKER"},
        {Value:TransitionMode.CHASE, Label:"CHASE"},
        {Value:TransitionMode.PIXELATE, Label:"PIXELATE"}
      ]}
      Disabled={Lighting.ColorMode === ColorMode.OFF || Lighting.ColorMode === ColorMode.WHITEONLY || Lighting.ColorMode === ColorMode.CONSTANT}
      />
      <ExpansionMenuItem
        Expand={Lighting.Transition.Mode !== TransitionMode.OFF && Expanded}
        OnExpand={OnExpand}
        Disabled={Lighting.Transition.Mode === TransitionMode.OFF}
        Label={"Transition Settings"}
      >
      <List>
        <ListItem>
        Color Array (TBD)
        </ListItem>          
        <Divider />
        <SliderMenuItem
          Label={"Transition Speed"}
          OnChange={props.set_transitionspeed}
          Value={props.Lighting.Transition.Speed}
        />
        <Divider />
        <SliderMenuItem
          Label={"Transition Width"}
          OnChange={props.set_transitionwidth}
          Value={props.Lighting.Transition.Width}
        />
        <Divider />
        <SliderMenuItem
          Label={"Transition Flicker Activation"}
          OnChange={props.set_transitionflickeractivation}
          Value={props.Lighting.Transition.FlickerActivation}
        />        
        <Divider />
        <SelectMenuItem 
          Label={"Shift Mode"}
          Value={props.Lighting.Transition.Shift.Mode}
          OnChange={props.set_transitionshiftmode}
          MenuItems={[
            {Value:ShiftMode.OFF, Label:"OFF"},
            {Value:ShiftMode.ALTERNATE, Label:"ALTERNATE"},
            {Value:ShiftMode.RANDOM, Label:"RANDOM"}
          ]}
          />
        <Divider />
        <SelectMenuItem 
          Label={"Shift Target"}
          Value={props.Lighting.Transition.Shift.Target}
          OnChange={props.set_transitionshifttarget}
          MenuItems={[
            {Value:ShiftTarget.PIXELS, Label:"PIXELS"},
            {Value:ShiftTarget.DOORS, Label:"DOORS"},
            {Value:ShiftTarget.CABINETS, Label:"CABINETS"},
            {Value:ShiftTarget.SIDES, Label:"SIDES"},
            {Value:ShiftTarget.STRAIGHTS, Label:"STRAIGHTS"}
          ]}
          Disabled={props.Lighting.Transition.Shift.Mode === ShiftMode.OFF}
          />
        <Divider />
        <SliderMenuItem
          Label={"Shift Amount"}
          OnChange={props.set_transitionshiftamount}
          Value={props.Lighting.Transition.Shift.Amount}
          Disabled={props.Lighting.Transition.Shift.Mode === ShiftMode.OFF}
        />
        <Divider />
        <SelectMenuItem 
          Label={"Chase Mode"}
          Value={props.Lighting.Transition.Chase.Mode}
          OnChange={props.set_transitionchasemode}
          MenuItems={[
            {Value:ChaseMode.OFF, Label:"OFF"},
            {Value:ChaseMode.BOUNCE, Label:"BOUNCE"},
            {Value:ChaseMode.OUTWARD, Label:"OUTWARD"},
            {Value:ChaseMode.INWARD, Label:"INWARD"},
            {Value:ChaseMode.CENTERBOUNCE, Label:"CENTERBOUNCE"},
            {Value:ChaseMode.CLOCKWISE, Label:"CLOCKWISE"},
            {Value:ChaseMode.COUNTERCLOCKWISE, Label:"COUNTERCLOCKWISE"}
          ]}
          Disabled={props.Lighting.Transition.Mode !== TransitionMode.CHASE}
          />
        <Divider />
        <SelectMenuItem 
          Label={"Chase Target"}
          Value={props.Lighting.Transition.Chase.Target}
          OnChange={props.set_transitionchasetarget}
          MenuItems={[
            {Value:ChaseTarget.ALL, Label:"ALL"},
            {Value:ChaseTarget.DOORS, Label:"DOORS"},
            {Value:ChaseTarget.CABINETS, Label:"CABINETS"},
            {Value:ChaseTarget.SIDES, Label:"SIDES"},
            {Value:ChaseTarget.STRAIGHTS, Label:"STRAIGHTS"}
          ]}
          Disabled={props.Lighting.Transition.Mode !== TransitionMode.CHASE || props.Lighting.Transition.Chase.Mode === ChaseMode.OFF}
          />
        <Divider />
        <SliderMenuItem
          Label={"Chase Speed"}
          OnChange={props.set_transitionchasespeed}
          Value={props.Lighting.Transition.Chase.Speed}
          Disabled={props.Lighting.Transition.Mode !== TransitionMode.CHASE || props.Lighting.Transition.Chase.Mode === ChaseMode.OFF}
        />
        <Divider />
        <SliderMenuItem
          Label={"Chase Width"}
          OnChange={props.set_transitionchasewidth}
          Value={props.Lighting.Transition.Chase.Width}
          Disabled={props.Lighting.Transition.Mode !== TransitionMode.CHASE || props.Lighting.Transition.Chase.Mode === ChaseMode.OFF}
        />
      </List>
      </ExpansionMenuItem>
    </>);
  }