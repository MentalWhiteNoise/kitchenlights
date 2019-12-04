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
            set_colormode={props.set_colormode}
            set_brightness={props.set_brightness}
            set_color={props.set_color}
          />
          <Divider />
          <FadeMenu
            Lighting={props.Lighting}
            Expanded={expanded === 'fadePanel'}
            OnExpand={handleChange('fadePanel')}
            set_fademode={props.set_fademode}
            set_bgcolor={props.set_bgcolor}
            set_fadespeed={props.set_fadespeed}
            set_fadeflickeractivation={props.set_fadeflickeractivation}
            set_fadeshiftmode={props.set_fadeshiftmode}
            set_fadeshifttarget={props.set_fadeshifttarget}
            set_fadeshiftamount={props.set_fadeshiftamount}
            set_fadechasemode={props.set_fadechasemode}
            set_fadechasetarget={props.set_fadechasetarget}
            set_fadechasespeed={props.set_fadechasespeed}
            set_fadechasewidth={props.set_fadechasewidth}
          />
          <Divider />
          <TransitionMenu
            Lighting={props.Lighting}
            Expanded={expanded === 'transitionPanel'}
            OnExpand={handleChange('transitionPanel')}
            set_transitionmode={props.set_transitionmode}
            set_transitionspeed={props.set_transitionspeed}
            set_transitionwidth={props.set_transitionwidth}
            set_transitionflickeractivation={props.set_transitionflickeractivation}
            set_transitionshiftmode={props.set_transitionshiftmode}
            set_transitionshifttarget={props.set_transitionshifttarget}
            set_transitionshiftamount={props.set_transitionshiftamount}
            set_transitionchasemode={props.set_transitionchasemode}
            set_transitionchasetarget={props.set_transitionchasetarget}
            set_transitionchasespeed={props.set_transitionchasespeed}
            set_transitionchasewidth={props.set_transitionchasewidth}
          />
        </List>
        </div>
    );
}

