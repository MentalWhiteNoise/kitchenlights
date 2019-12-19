import React, { useState } from 'react';

import List from '@material-ui/core/List';
import Divider from '@material-ui/core/Divider';
import ColorMenu from './MenuGroups/colorMenu.jsx';
import FadeMenu from './MenuGroups/fadeMenu.jsx';
import TransitionMenu from './MenuGroups/transitionMenu.jsx';

export default function ConfigMenu(props){
    const [expanded, setExpanded] = useState(null);
    const handleChange = panel => (event, newExpanded) => {
      setExpanded(newExpanded ? panel : false);
    };

    return (
        <div>
        <List>
          <ColorMenu 
            Lighting={props.Lighting}
            // turn_on={props.}
            // turn_off={props.}
            // enable_whitemode={props.}
            // disable_whitemode={props.}
            set_colormode={props.set_colormode}
            set_color={props.set_color}
            set_brightness={props.set_brightness}
          />
          <Divider />
          <FadeMenu
            Lighting={props.Lighting}
            Expanded={expanded === 'fadePanel'}
            OnExpand={handleChange('fadePanel')}
            
            // Fade Menu Methods
            // disable_fade={this.}
            // pause_fade={this.}
            // resume_fade={this.}
            // enable_fade={this.}
            set_fademode={props.set_fademode}
            set_bgcolor={props.set_bgcolor}
            set_fadespeed={props.set_fadespeed}
            set_fadeforwardwidth={props.set_fadeforwardwidth}
            set_fadelagwidth={props.set_fadelagwidth}
            set_fadeactivation={props.set_fadeactivation}
            set_faderandomness={props.set_faderandomness}

            // Fade Shift Sub Menu Methods
            set_fadeshiftmode ={props.set_fadeshiftmode}
            set_fadeshifttarget={props.set_fadeshifttarget}
            set_fadeshiftamount={props.set_fadeshiftamount}
            set_fadeshiftoffset={props.set_fadeshiftoffset}

            // Fade Chase Sub Menu Methods    
            set_fadechasemode={props.set_fadechasemode}
            set_fadechasetarget={props.set_fadechasetarget}
            set_fadechasespeed={props.set_fadechasespeed}
            set_fadechasecenteroffset={props.set_fadechasecenteroffset}
            set_fadechaseoverlapeffect={props.set_fadechaseoverlapeffect}
          />
          <Divider />
          <TransitionMenu
            Lighting={props.Lighting}
            Expanded={expanded === 'transitionPanel'}
            OnExpand={handleChange('transitionPanel')}
            
            // Transition Menu Methods
            // disable_transition={props.}
            // pause_transition={props.}
            // resume_transition={props.}
            // enable_transition={props.}
            set_transitionmode={props.set_transitionmode}
            set_transitioncolorarray={props.set_transitioncolorarray}
            // clear_colorarray={props.}
            append_colorarray ={props.append_colorarray}
            set_transitionspeed={props.set_transitionspeed}
            set_transitionwidth={props.set_transitionwidth}
            set_transitionforwardwidth={props.set_transitionforwardwidth}
            set_transitionactivation={props.set_transitionactivation}

            // Transition Shift Sub Menu Methods
            set_transitionshiftmode={props.set_transitionshiftmode}
            set_transitionshifttarget={props.set_transitionshifttarget}
            set_transitionshiftamount={props.set_transitionshiftamount}
            set_transitionshiftoffset={props.set_transitionshiftoffset}

            // Transition Chase Sub Menu Methods
            set_transitionchasemode={props.set_transitionchasemode}
            set_transitionchasetarget={props.set_transitionchasetarget}
            set_transitionchasespeed={props.set_transitionchasespeed}
            set_transitionchasecenteroffset ={props.set_transitionchasecenteroffset}
            set_transitionchaseoverlapeffect={props.set_transitionchaseoverlapeffect}
          />
        </List>
        </div>
    );
}

