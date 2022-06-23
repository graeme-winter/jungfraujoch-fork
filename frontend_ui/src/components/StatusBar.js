
import React, {Component} from 'react';

import AppBar from '@material-ui/core/AppBar';
import Switch from '@material-ui/core/Switch';
import Toolbar from '@material-ui/core/Toolbar';
import Typography from '@material-ui/core/Typography';
import Button from '@material-ui/core/Button';
import {BrokerStatus} from "../protobuf/jfjoch_pb";

function decodeDAQState (value) {
    switch(value) {
        case BrokerStatus.State.NOT_INITIALIZED:
            return "Not initialized";
        case BrokerStatus.State.BUSY:
            return "Busy";
        case BrokerStatus.State.IDLE:
            return "Idle";
        case BrokerStatus.State.DATA_COLLECTION:
            return "Data collection";
        case BrokerStatus.State.PEDESTAL:
            return "Pedestal collection";
        case BrokerStatus.State.ERROR:
            return "Error";
        default:
            return "Unknown state";
    }
}

class StatusBar extends Component {

  render() {
     return <AppBar position="sticky">
       <Toolbar>
       <Typography variant="h6" style={{flexGrow: 0.5}}>
           JUNGFRAU 4M
       </Typography>
       <Typography variant="h6" style={{flexGrow: 2.0}}>
           State: {decodeDAQState(this.props.daqState)}
       </Typography>
           <Typography variant="h6" style={{flexGrow: 2.0}}>
               {((this.props.progress !== undefined) && (this.props.daqState === BrokerStatus.State.DATA_COLLECTION) ) ?
                   "Progress: " + this.props.progress.toPrecision(3) + " %" : ""}
           </Typography>
           <Button color="secondary" onClick={this.props.cancel} variant="contained" disableElevation>Cancel</Button>&nbsp;&nbsp;
           <Button color="secondary" onClick={this.props.initialize} variant="contained" disableElevation>Initialize</Button>
       </Toolbar>
       </AppBar>
  }
}

export default StatusBar;
