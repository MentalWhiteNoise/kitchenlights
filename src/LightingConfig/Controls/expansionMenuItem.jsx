import React from 'react';
import ListItem from '@material-ui/core/ListItem';
import ExpansionPanel from '@material-ui/core/ExpansionPanel';
import ExpansionPanelSummary from '@material-ui/core/ExpansionPanelSummary';
import ExpansionPanelDetails from '@material-ui/core/ExpansionPanelDetails';
import ExpandMoreIcon from '@material-ui/icons/ExpandMore';
import Typography from '@material-ui/core/Typography';

export default function ExpansionMenuItem(props){
    return(
      <ListItem style={{padding:0, margin:0}}>
      <ExpansionPanel
        square 
        expanded={props.Expand}
        onChange={props.OnExpand}
        disabled={props.Disabled}>
      <ExpansionPanelSummary expandIcon={<ExpandMoreIcon />} >
        <Typography >{props.Label}</Typography>
      </ExpansionPanelSummary>
      <ExpansionPanelDetails  style={{padding:0, margin:0}}>
      {props.children}
      </ExpansionPanelDetails>
      </ExpansionPanel>
      </ListItem>
    )
  }