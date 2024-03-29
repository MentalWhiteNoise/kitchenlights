import React from 'react';
import Divider from '@material-ui/core/Divider';
import List from '@material-ui/core/List';
import ListItem from '@material-ui/core/ListItem';
import SelectMenuItem from '../Controls/selectMenuItem.jsx';
import SliderMenuItem from '../Controls/sliderMenuItem.jsx';
import ExpansionMenuItem from '../Controls/expansionMenuItem.jsx';
import ColorPicker from '../Controls/colorPicker.jsx';
import { ColorMode} from '../../Lighting/lighting.js'
import { FadeMode} from '../../Lighting/lightingFade.js'
import { ShiftMode, ShiftTarget } from '../../Lighting/lightingShift.js'
import { ChaseMode, ChaseTarget, OverlapEffect } from '../../Lighting/lightingChase.js'

export default function FadeMenu(props){
    return (
    <>
    <SelectMenuItem 
      Label={"Fade Mode"}
      Value={props.Lighting.Fade.Mode}
      OnChange={props.set_fademode}
      MenuItems={[
        {Value:FadeMode.OFF, Label:"OFF"},
        {Value:FadeMode.PULSE, Label:"PULSE"},
        {Value:FadeMode.LINEAR, Label:"LINEAR"},
        {Value:FadeMode.HEARTBEAT, Label:"HEARTBEAT"},
        {Value:FadeMode.FLASH, Label:"FLASH"},
        {Value:FadeMode.CHASE, Label:"CHASE"},
        {Value:FadeMode.FLICKER, Label:"FLICKER"}
      ]}
      Disabled={props.Lighting.ColorMode === ColorMode.OFF}
      />
    <ExpansionMenuItem
      Expand={props.Lighting.Fade.Mode !== FadeMode.OFF && props.Expanded}
      OnExpand={props.OnExpand}
      Disabled={props.Lighting.Fade.Mode === FadeMode.OFF}
      Label={"Fade Settings Test"}
    >
      <List>
        <ListItem>
          <ColorPicker 
              Color={props.Lighting.Fade.BgColor} 
              onChange={props.set_bgcolor}
              label="Background Color"
            />
        </ListItem>
        <Divider />
        <SliderMenuItem
          Label={"Speed"}
          OnChange={props.set_fadespeed}
          Value={props.Lighting.Fade.Speed}
        />
        <Divider />
        <SliderMenuItem
          Label={"Forward Width"}
          OnChange={props.set_fadeforwardwidth}
          Value={props.Lighting.Fade.ForwardWidth}
        />
        <Divider />
        <SliderMenuItem
          Label={"Lag Width"}
          OnChange={props.set_fadelagwidth}
          Value={props.Lighting.Fade.LagWidth}
        />
        <Divider />
        <SliderMenuItem
          Label={"Activation"}
          OnChange={props.set_fadeactivation}
          Value={props.Lighting.Fade.Activation}
          Disabled={props.Lighting.Fade.Mode !== FadeMode.FLASH && props.Lighting.Fade.Mode !== FadeMode.FLICKER}
        />
        <Divider />
        <SliderMenuItem
          Label={"Randomness"}
          OnChange={props.set_faderandomness}
          Value={props.Lighting.Fade.Randomness}
        />
        <Divider />
        <SelectMenuItem 
          Label={"Shift Mode"}
          Value={props.Lighting.Fade.Shift.Mode}
          OnChange={props.set_fadeshiftmode}
          MenuItems={[
            {Value:ShiftMode.OFF, Label:"OFF"},
            {Value:ShiftMode.CYCLE, Label:"CYCLE"},
            {Value:ShiftMode.ALTERNATE, Label:"ALTERNATE"},
            {Value:ShiftMode.RANDOM, Label:"RANDOM"}
            
          ]}
          />
        <Divider />
        <SelectMenuItem 
          Label={"Shift Target"}
          Value={props.Lighting.Fade.Shift.Target}
          OnChange={props.set_fadeshifttarget}
          MenuItems={[
            {Value:ShiftTarget.PIXELS, Label:"PIXELS"},
            {Value:ShiftTarget.DOORS, Label:"DOORS"},
            {Value:ShiftTarget.CABINETS, Label:"CABINETS"},
            {Value:ShiftTarget.SIDES, Label:"SIDES"},
            {Value:ShiftTarget.STRAIGHTS, Label:"STRAIGHTS"}
          ]}
          Disabled={props.Lighting.Fade.Shift.Mode === ShiftMode.OFF}
          />
        <Divider />
        <SliderMenuItem
          Label={"Shift Amount"}
          OnChange={props.set_fadeshiftamount}
          Value={props.Lighting.Fade.Shift.Amount}
          Disabled={props.Lighting.Fade.Shift.Mode === ShiftMode.OFF}
        />
        <Divider />
        <SliderMenuItem
          Label={"Shift Offset"}
          OnChange={props.set_fadeshiftoffset}
          Value={props.Lighting.Fade.Shift.Offset}
          Disabled={props.Lighting.Fade.Shift.Mode === ShiftMode.OFF}
        />
        <Divider />
        <SelectMenuItem 
          Label={"Chase Mode"}
          Value={props.Lighting.Fade.Chase.Mode}
          OnChange={props.set_fadechasemode}
          MenuItems={[
            {Value:ChaseMode.OFF, Label:"OFF"},
            {Value:ChaseMode.BOUNCE, Label:"BOUNCE"},
            {Value:ChaseMode.OUTWARD, Label:"OUTWARD"},
            {Value:ChaseMode.INWARD, Label:"INWARD"},
            {Value:ChaseMode.CENTERBOUNCE, Label:"CENTERBOUNCE"},
            {Value:ChaseMode.CLOCKWISE, Label:"CLOCKWISE"},
            {Value:ChaseMode.COUNTERCLOCKWISE, Label:"COUNTERCLOCKWISE"}
          ]}
          Disabled={props.Lighting.Fade.Mode !== FadeMode.CHASE}
          />
        <Divider />
        <SelectMenuItem 
          Label={"Chase Target"}
          Value={props.Lighting.Fade.Chase.Target}
          OnChange={props.set_fadechasetarget}
          MenuItems={[
            {Value:ChaseTarget.ALL, Label:"ALL"},
            {Value:ChaseTarget.DOORS, Label:"DOORS"},
            {Value:ChaseTarget.CABINETS, Label:"CABINETS"},
            {Value:ChaseTarget.SIDES, Label:"SIDES"},
            {Value:ChaseTarget.STRAIGHTS, Label:"STRAIGHTS"}
          ]}
          Disabled={props.Lighting.Fade.Mode !== FadeMode.CHASE || props.Lighting.Fade.Chase.Mode === ChaseMode.OFF}
          />
        <Divider />
        <SliderMenuItem
          Label={"Chase Speed"}
          OnChange={props.set_fadechasespeed}
          Value={props.Lighting.Fade.Chase.Speed}
          Disabled={props.Lighting.Fade.Mode !== FadeMode.CHASE || props.Lighting.Fade.Chase.Mode === ChaseMode.OFF}
        />
        <Divider />
        <SliderMenuItem
          Label={"Chase Center Offset"}
          OnChange={props.set_fadechasecenteroffset}
          Value={props.Lighting.Fade.Chase.CenterOffset}
          Disabled={props.Lighting.Fade.Mode !== FadeMode.CHASE || props.Lighting.Fade.Chase.Mode === ChaseMode.OFF}
        />
        <SelectMenuItem 
          Label={"Chase Overlap Effect"}
          Value={props.Lighting.Fade.Chase.OverlapEffect}
          OnChange={props.set_fadechaseoverlapeffect}
          MenuItems={[
            {Value:OverlapEffect.AVERAGE, Label:"AVERAGE"},
            {Value:OverlapEffect.MAX, Label:"MAX"},
            {Value:OverlapEffect.MIN, Label:"MIN"}
          ]}
          Disabled={props.Lighting.Fade.Mode !== FadeMode.CHASE || props.Lighting.Fade.Chase.Mode === ChaseMode.OFF}
          />
      </List>
    </ExpansionMenuItem>
    </>);
  }