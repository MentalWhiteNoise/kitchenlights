import React, {useEffect} from 'react';
import clsx from 'clsx';
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
    set_colormode = (value) => {
      var newLighting = new Lighting(this.state.Lighting);
        newLighting.ColorMode = value;
        this.setState ({Lighting: newLighting});
    };
    set_fademode = (value) => {
      var newLighting = new Lighting(this.state.Lighting);
        newLighting.Fade.Mode = value;
        this.setState ({Lighting: newLighting});
    };
    // set_fadechasemode(String mode);
    // set_fadechasetarget(String target);
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
    set_fadechasewidth = (value) => {
      var newLighting = new Lighting(this.state.Lighting);
      newLighting.Fade.Chase.Width = value;
      this.setState ({Lighting: newLighting});
    }
    // disable_fade();
    // pause_fade();
    // resume_fade();
    // enable_fade();
    set_bgcolor = (value) => {
      var newLighting = new Lighting(this.state.Lighting);
      newLighting.Fade.BgColor = value;
      this.setState ({Lighting: newLighting});
    }
    set_brightness = (value) => {
      var newLighting = new Lighting(this.state.Lighting);
      newLighting.Brightness = value;
      this.setState ({Lighting: newLighting});
    }
    set_fadespeed = (value) => {
      var newLighting = new Lighting(this.state.Lighting);
      newLighting.Fade.Speed = value;
      this.setState ({Lighting: newLighting});
    }    
    set_fadeflickeractivation = (value) => {
      var newLighting = new Lighting(this.state.Lighting);
      newLighting.Fade.FlickerActivation = value;
      this.setState ({Lighting: newLighting});
    }
    set_transitionmode = (value) => {
      var newLighting = new Lighting(this.state.Lighting);
        newLighting.Transition.Mode = value;
        this.setState ({Lighting: newLighting});
    };

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
    set_transitionflickeractivation = (value) => {
      var newLighting = new Lighting(this.state.Lighting);
        newLighting.Transition.FlickerActivation = value;
        this.setState ({Lighting: newLighting});
    };
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
    set_transitionchasewidth = (value) => {
      var newLighting = new Lighting(this.state.Lighting);
        newLighting.Transition.Chase.Width = value;
        this.setState ({Lighting: newLighting});
    };
    set_transitioncolorarray = (value) => {
      var newLighting = new Lighting(this.state.Lighting);
      newLighting.Transition.ColorArray = [...value];
      newLighting.Transition.ColorArray_Length = value.length;
      this.setState ({Lighting: newLighting});
    }
    // disable_transition();
    // pause_transition();
    // resume_transition();
    // enable_transition();
    // clear_colorarray();
    set_color = (color) => {
      var newLighting = new Lighting(this.state.Lighting);
      newLighting.Transition.Set_Color(color);
      //console.log(newLighting.Transition);
      this.setState ({Lighting: newLighting});
    }
    append_colorarray = (color) => {
      var newLighting = new Lighting(this.state.Lighting);
      newLighting.Transition.Append_ColorArray(color);
      this.setState ({Lighting: newLighting});
    }
    
    
    render() {
      //console.log(this.state.Lighting);
      const lightingConfig = (
        <LightingConfig 
          Lighting={this.state.Lighting} 
          // turn_on();
          // turn_on(uint32_t color);
          // turn_off();
          // enable_whitemode();
          // disable_whitemode();
          set_colormode={this.set_colormode} 
          set_brightness={this.set_brightness} 
          set_fademode={this.set_fademode} 
          set_transitionmode={this.set_transitionmode}
          set_bgcolor={this.set_bgcolor}
          set_fadespeed={this.set_fadespeed}
          set_fadeflickeractivation={this.set_fadeflickeractivation}
          set_color = {this.set_color}
          set_fadeshiftmode={this.set_fadeshiftmode}
          set_fadeshifttarget={this.set_fadeshifttarget}
          set_fadeshiftamount={this.set_fadeshiftamount}
          set_fadechasemode={this.set_fadechasemode}
          set_fadechasetarget={this.set_fadechasetarget}
          set_fadechasespeed={this.set_fadechasespeed}
          set_fadechasewidth={this.set_fadechasewidth}
          set_transitionspeed={this.set_transitionspeed}
          set_transitionwidth={this.set_transitionwidth}
          set_transitionflickeractivation={this.set_transitionflickeractivation}
          set_transitionshiftmode={this.set_transitionshiftmode}
          set_transitionshifttarget={this.set_transitionshifttarget}
          set_transitionshiftamount={this.set_transitionshiftamount}
          set_transitionchasemode={this.set_transitionchasemode}
          set_transitionchasetarget={this.set_transitionchasetarget}
          set_transitionchasespeed={this.set_transitionchasespeed}
          set_transitionchasewidth={this.set_transitionchasewidth}
          set_transitioncolorarray={this.set_transitioncolorarray}
          // disable_fade();
          // pause_fade();
          // resume_fade();
          // enable_fade();
          // disable_transition();
          // pause_transition();
          // resume_transition();
          // enable_transition();
          // clear_colorarray();
          />);
      return(
        <PersistentDrawerLeft ToolBar={lightingConfig} LightingEffect={this.state.Lighting} OnResize={this.handleResize} Height={this.state.Display.height}  Width={this.state.Display.width} />
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