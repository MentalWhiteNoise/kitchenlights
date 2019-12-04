import React from 'react';
import ListItem from '@material-ui/core/ListItem';
import FormControl from '@material-ui/core/FormControl';
import InputLabel from '@material-ui/core/InputLabel';
import Select from '@material-ui/core/Select';
import MenuItem from '@material-ui/core/MenuItem';

export default function SelectMenuItem(props){
    return (
      <ListItem
        disabled={props.Disabled}>
        <FormControl
          disabled={props.Disabled}>
          <InputLabel>{props.Label}</InputLabel>
          <Select
            value={props.Value}
            onChange={(e) => {props.OnChange(e.target.value)}}
            style={{width:250}}
          >
          {
            props.MenuItems.map(mnu => {
              return <MenuItem key={mnu.Value} value={mnu.Value}>{mnu.Label}</MenuItem>
            })
          }
          </Select>
        </FormControl>
      </ListItem>
    )
  }