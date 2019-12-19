import React, {useEffect} from 'react';
import clsx from 'clsx';
import Timing from '../Lighting/timing.js'
import Lighting from '../Lighting/lighting.js'
import LightingConfig from '../LightingConfig/lightingConfig.jsx'
import { AnimatedDisplay } from '../Display/animatedDisplay.jsx'

import { makeStyles, useTheme } from '@material-ui/core/styles';
import Drawer from '@material-ui/core/Drawer';
import CssBaseline from '@material-ui/core/CssBaseline';
import AppBar from '@material-ui/core/AppBar';
import Toolbar from '@material-ui/core/Toolbar';
import Typography from '@material-ui/core/Typography';
import IconButton from '@material-ui/core/IconButton';
import MenuIcon from '@material-ui/icons/Menu';
import ChevronLeftIcon from '@material-ui/icons/ChevronLeft';
import ChevronRightIcon from '@material-ui/icons/ChevronRight';
import PlayIcon from '@material-ui/icons/PlayArrow';
import StopIcon from '@material-ui/icons/Pause';
import Fab from '@material-ui/core/Fab';

class Pages extends React.Component {
    constructor(props) {
      super(props);
      this.state = {
        Lighting: new Lighting(),
        Timing: new Timing(),
        Display: { height: 70, width: 20}
      };
      this.handleResize = this.handleResize.bind(this);
    }
    componentDidMount() {
      if (this.state.Lighting.Transition.ColorArray_Length === 0){
        this.set_color({Red:255,Green:0,Blue:0,White:0});
      }
    }
    handleResize(w, h){
      // console.log("dimensions: ", h, ", ", w)
      if (h !== this.state.h || w !== this.state.w) {
        this.setState({Display: { height: h, width: w}});
      }
    }
    
    // void turn_on();
    // void turn_on(uint32_t color);
    // turn_off();
    // enable_whitemode();
    // disable_whitemode();
    tick = () => {
      var newTiming = new Timing(this.state.Timing);
      newTiming.tick(this.state.Lighting);
      this.setState ({Timing: newTiming});
    }
    
    // Color Menu Methods
    set_colormode = (value) => {
      var newLighting = new Lighting(this.state.Lighting);
        newLighting.ColorMode = value;
        this.setState ({Lighting: newLighting});
    };
    set_color = (color) => {
      var newLighting = new Lighting(this.state.Lighting);
      newLighting.Transition.Set_Color(color);
      //console.log(newLighting.Transition);
      this.setState ({Lighting: newLighting});
    }
    set_brightness = (value) => {
      var newLighting = new Lighting(this.state.Lighting);
      newLighting.Brightness = value;
      this.setState ({Lighting: newLighting});
    }

    // Fade Menu Methods
    // disable_fade();
    // pause_fade();
    // resume_fade();
    // enable_fade();
    set_fademode = (value) => {
      var newLighting = new Lighting(this.state.Lighting);
        newLighting.Fade.Mode = value;
        this.setState ({Lighting: newLighting});
    };
    set_bgcolor = (value) => {
      var newLighting = new Lighting(this.state.Lighting);
      newLighting.Fade.BgColor = value;
      this.setState ({Lighting: newLighting});
    }
    set_fadespeed = (value) => {
      var newLighting = new Lighting(this.state.Lighting);
      newLighting.Fade.Speed = value;
      this.setState ({Lighting: newLighting});
    }
    set_fadeforwardwidth = (value) => {
      var newLighting = new Lighting(this.state.Lighting);
      newLighting.Fade.ForwardWidth = value;
      this.setState ({Lighting: newLighting});
    }    
    set_fadelagwidth = (value) => {
      var newLighting = new Lighting(this.state.Lighting);
      newLighting.Fade.LagWidth = value;
      this.setState ({Lighting: newLighting});
    }
    set_fadeactivation = (value) => {
      var newLighting = new Lighting(this.state.Lighting);
      newLighting.Fade.Activation = value;
      this.setState ({Lighting: newLighting});
    }
    set_faderandomness = (value) => {
      var newLighting = new Lighting(this.state.Lighting);
      newLighting.Fade.Randomness = value;
      this.setState ({Lighting: newLighting});
    }

    // Fade Shift Sub Menu Methods
    set_fadeshiftmode = (mode) => {
      var newLighting = new Lighting(this.state.Lighting);
      newLighting.Fade.Shift.Mode = mode;
      this.setState ({Lighting: newLighting});
    }
    set_fadeshifttarget = (target) => {
      var newLighting = new Lighting(this.state.Lighting);
      newLighting.Fade.Shift.Target = target;
      this.setState ({Lighting: newLighting});
    }
    set_fadeshiftamount = (value) => {
      var newLighting = new Lighting(this.state.Lighting);
      newLighting.Fade.Shift.Amount = value;
      this.setState ({Lighting: newLighting});
    }
    set_fadeshiftoffset = (value) => {
      var newLighting = new Lighting(this.state.Lighting);
      newLighting.Fade.Shift.Offset = value;
      this.setState ({Lighting: newLighting});
    }

    // Fade Chase Sub Menu Methods    
    set_fadechasemode  = (mode) => {
      var newLighting = new Lighting(this.state.Lighting);
      newLighting.Fade.Chase.Mode = mode;
      this.setState ({Lighting: newLighting});
    }
    set_fadechasetarget = (target) => {
      var newLighting = new Lighting(this.state.Lighting);
      newLighting.Fade.Chase.Target = target;
      this.setState ({Lighting: newLighting});
    }
    set_fadechasespeed = (value) => {
      var newLighting = new Lighting(this.state.Lighting);
      newLighting.Fade.Chase.Speed = value;
      this.setState ({Lighting: newLighting});
    }
    set_fadechasecenteroffset = (value) => {
      var newLighting = new Lighting(this.state.Lighting);
      newLighting.Fade.Chase.CenterOffset = value;
      this.setState ({Lighting: newLighting});
    }
    set_fadechaseoverlapeffect = (value) => {
      var newLighting = new Lighting(this.state.Lighting);
      newLighting.Fade.Chase.OverlapEffect = value;
      this.setState ({Lighting: newLighting});
    }

    // Transition Menu Methods
    // disable_transition();
    // pause_transition();
    // resume_transition();
    // enable_transition();
    set_transitionmode = (value) => {
      var newLighting = new Lighting(this.state.Lighting);
        newLighting.Transition.Mode = value;
        this.setState ({Lighting: newLighting});
    };
    set_transitioncolorarray = (value) => {
      var newLighting = new Lighting(this.state.Lighting);
      newLighting.Transition.ColorArray = [...value];
      newLighting.Transition.ColorArray_Length = value.length;
      this.setState ({Lighting: newLighting});
    }
    // clear_colorarray();
    append_colorarray = (color) => {
      var newLighting = new Lighting(this.state.Lighting);
      newLighting.Transition.Append_ColorArray(color);
      this.setState ({Lighting: newLighting});
    }
    set_transitionspeed = (value) => {
      var newLighting = new Lighting(this.state.Lighting);
        newLighting.Transition.Speed = value;
        this.setState ({Lighting: newLighting});
    };
    set_transitionwidth = (value) => {
      var newLighting = new Lighting(this.state.Lighting);
        newLighting.Transition.Width = value;
        this.setState ({Lighting: newLighting});
    };
    set_transitionforwardwidth = (value) => {
      var newLighting = new Lighting(this.state.Lighting);
        newLighting.Transition.ForwardWidth = value;
        this.setState ({Lighting: newLighting});
    };
    set_transitionactivation = (value) => {
      var newLighting = new Lighting(this.state.Lighting);
        newLighting.Transition.Activation = value;
        this.setState ({Lighting: newLighting});
    };

    // Transition Shift Sub Menu Methods
    set_transitionshiftmode = (value) => {
      var newLighting = new Lighting(this.state.Lighting);
        newLighting.Transition.Shift.Mode = value;
        this.setState ({Lighting: newLighting});
    };
    set_transitionshifttarget = (value) => {
      var newLighting = new Lighting(this.state.Lighting);
        newLighting.Transition.Shift.Target = value;
        this.setState ({Lighting: newLighting});
    };
    set_transitionshiftamount = (value) => {
      var newLighting = new Lighting(this.state.Lighting);
        newLighting.Transition.Shift.Amount = value;
        this.setState ({Lighting: newLighting});
    };
    set_transitionshiftoffset = (value) => {
      var newLighting = new Lighting(this.state.Lighting);
      newLighting.Transition.Shift.Offset = value;
      this.setState ({Lighting: newLighting});
    }
    
    // Transition Chase Sub Menu Methods
    set_transitionchasemode = (value) => {
      var newLighting = new Lighting(this.state.Lighting);
        newLighting.Transition.Chase.Mode = value;
        this.setState ({Lighting: newLighting});
    };
    set_transitionchasetarget = (value) => {
      var newLighting = new Lighting(this.state.Lighting);
        newLighting.Transition.Chase.Target = value;
        this.setState ({Lighting: newLighting});
    };
    set_transitionchasespeed = (value) => {
      var newLighting = new Lighting(this.state.Lighting);
        newLighting.Transition.Chase.Speed = value;
        this.setState ({Lighting: newLighting});
    };
    set_transitionchasecenteroffset = (value) => {
      var newLighting = new Lighting(this.state.Lighting);
      newLighting.Transition.Chase.CenterOffset = value;
      this.setState ({Lighting: newLighting});
    }
    set_transitionchaseoverlapeffect = (value) => {
      var newLighting = new Lighting(this.state.Lighting);
      newLighting.FadTransitione.Chase.OverlapEffect = value;
      this.setState ({Lighting: newLighting});
    }
    
    render() {
      //console.log(this.state.Lighting);
      const lightingConfig = (
        <LightingConfig 
          Lighting={this.state.Lighting} 

          // Color Menu Methods
          // turn_on={this.}
          // turn_off={this.}
          // enable_whitemode={this.}
          // disable_whitemode={this.}
          set_colormode={this.set_colormode}
          set_color={this.set_color}
          set_brightness={this.set_brightness}

          // Fade Menu Methods
          // disable_fade={this.}
          // pause_fade={this.}
          // resume_fade={this.}
          // enable_fade={this.}
          set_fademode={this.set_fademode}
          set_bgcolor={this.set_bgcolor}
          set_fadespeed={this.set_fadespeed}
          set_fadeforwardwidth={this.set_fadeforwardwidth}
          set_fadelagwidth={this.set_fadelagwidth}
          set_fadeactivation={this.set_fadeactivation}
          set_faderandomness={this.set_faderandomness}

          // Fade Shift Sub Menu Methods
          set_fadeshiftmode ={this.set_fadeshiftmode}
          set_fadeshifttarget={this.set_fadeshifttarget}
          set_fadeshiftamount={this.set_fadeshiftamount}
          set_fadeshiftoffset={this.set_fadeshiftoffset}

          // Fade Chase Sub Menu Methods    
          set_fadechasemode={this.set_fadechasemode}
          set_fadechasetarget={this.set_fadechasetarget}
          set_fadechasespeed={this.set_fadechasespeed}
          set_fadechasecenteroffset={this.set_fadechasecenteroffset}
          set_fadechaseoverlapeffect={this.set_fadechaseoverlapeffect}

          // Transition Menu Methods
          // disable_transition={this.}
          // pause_transition={this.}
          // resume_transition={this.}
          // enable_transition={this.}
          set_transitionmode={this.set_transitionmode}
          set_transitioncolorarray={this.set_transitioncolorarray}
          // clear_colorarray={this.}
          append_colorarray ={this.append_colorarray}
          set_transitionspeed={this.set_transitionspeed}
          set_transitionwidth={this.set_transitionwidth}
          set_transitionforwardwidth={this.set_transitionforwardwidth}
          set_transitionactivation={this.set_transitionactivation}

          // Transition Shift Sub Menu Methods
          set_transitionshiftmode={this.set_transitionshiftmode}
          set_transitionshifttarget={this.set_transitionshifttarget}
          set_transitionshiftamount={this.set_transitionshiftamount}
          set_transitionshiftoffset={this.set_transitionshiftoffset}

          // Transition Chase Sub Menu Methods
          set_transitionchasemode={this.set_transitionchasemode}
          set_transitionchasetarget={this.set_transitionchasetarget}
          set_transitionchasespeed={this.set_transitionchasespeed}
          set_transitionchasecenteroffset ={this.set_transitionchasecenteroffset}
          set_transitionchaseoverlapeffect={this.set_transitionchaseoverlapeffect}
          />);
      return(
        <PersistentDrawerLeft ToolBar={lightingConfig} LightingEffect={this.state.Lighting} 
          tick={this.tick} 
          timing={this.state.Timing}
          OnResize={this.handleResize} Height={this.state.Display.height}  Width={this.state.Display.width} />
      )
    }
}
export default Pages

const drawerWidth = 350;

const useStyles = makeStyles(theme => ({
  root: {
    display: 'flex'
  },
  appBar: {
    transition: theme.transitions.create(['margin', 'width'], {
      easing: theme.transitions.easing.sharp,
      duration: theme.transitions.duration.leavingScreen,
    }),
  },
  appBarShift: {
    width: `calc(100% - ${drawerWidth}px)`,
    marginLeft: drawerWidth,
    transition: theme.transitions.create(['margin', 'width'], {
      easing: theme.transitions.easing.easeOut,
      duration: theme.transitions.duration.enteringScreen,
    }),
  },
  menuButton: {
    marginRight: theme.spacing(2),
  },
  hide: {
    display: 'none',
  },
  drawer: {
    width: drawerWidth,
    flexShrink: 0,
  },
  drawerPaper: {
    width: drawerWidth,
  },
  drawerHeader: {
    alignItems: 'center',
    width: drawerWidth,
    padding: theme.spacing(0, 1),
    ...theme.mixins.toolbar,
  },
  content: {
    flexGrow: 1,
    padding: 0,//theme.spacing(3),
    transition: theme.transitions.create('margin', {
      easing: theme.transitions.easing.sharp,
      duration: theme.transitions.duration.leavingScreen,
    }),
    marginLeft: -drawerWidth,
  },
  contentShift: {
    transition: theme.transitions.create('margin', {
      easing: theme.transitions.easing.easeOut,
      duration: theme.transitions.duration.enteringScreen,
    }),
    marginLeft: 0,
  },
}));
function PersistentDrawerLeft(props) {
  const classes = useStyles();
  const theme = useTheme();
  const [open, setOpen] = React.useState(true);
  const [play, setPlay] = React.useState(false);

  useEffect(() => {
    function handleResize() {
      const tempWidth = (open) ? window.innerWidth - drawerWidth - 8 : window.innerWidth - 8;
      const tempHeight = window.innerHeight - /* height of drawer header */ 70;
      
      if (props.Width !== tempWidth || props.Height !== tempHeight){
        props.OnResize(tempWidth, tempHeight);
      }
    }
    window.addEventListener('resize', handleResize)
    handleResize();
    return _ => {
      window.removeEventListener('resize', handleResize)
    }
  });

  const handleDrawerOpen = () => {
    setOpen(true);
  };

  const handleDrawerClose = () => {
    setOpen(false);
  };

  return (
    <div className={classes.root}>
      <CssBaseline />
      <AppBar
        position="fixed"
        className={clsx(classes.appBar, {
          [classes.appBarShift]: open,
        })}
      >
        {<Toolbar>
          <IconButton
            color="inherit"
            aria-label="open drawer"
            onClick={handleDrawerOpen}
            edge="start"
            className={clsx(classes.menuButton, open && classes.hide)}
          >
            <MenuIcon />
          </IconButton>
          <Typography variant="h6" noWrap>
            Upper Display Lights
          </Typography>
        </Toolbar>}
      </AppBar>
      <Drawer
        className={classes.drawer}
        variant="persistent"
        anchor="left"
        open={open}
        classes={{
          paper: classes.drawerPaper,
        }}
      >
        <div className={classes.drawerHeader} >
          <Typography variant="h6" noWrap style={{float: "left", verticalAlign: "center", height: "100%", margin: 15}}>
            Settings
          </Typography>
          <div style={{justifyContent: 'flex-start', float: "right"}}>
          <IconButton onClick={handleDrawerClose}>
            {theme.direction === 'ltr' ? <ChevronLeftIcon /> : <ChevronRightIcon />}
          </IconButton>
        </div>
        </div>
        {props.ToolBar}
      </Drawer>
      <main className={clsx(classes.content, {
          [classes.contentShift]: open,
        })}
      >
        <div className={classes.drawerHeader}/>
            <AnimatedDisplay 
              width={props.Width} 
              height={props.Height} 
              tick={props.tick} 
              timing={props.timing}
              lightingEffect={props.LightingEffect} play={play}/> 
            <div style={{
              position: "absolute", top: (window.innerHeight + 75) / 2 - 25, left: drawerWidth + (window.innerWidth - drawerWidth) / 2 - 25, zIndex:"10"}}>
              <Fab  onClick={()=>{setPlay(!play)}} color="primary" aria-label="add">
                {(play) ? <StopIcon/> : <PlayIcon/>}
              </Fab>
            </div>
      </main>
    </div>
  );
}