
import React, {Component} from 'react';

import Grid from '@material-ui/core/Grid';
import Paper from '@material-ui/core/Paper';

class FPGAGrid extends Component {
    render() {
        let i = 0;
        return <Grid container spacing={3}>
            {this.props.devices?.map((dev) => (
                <Grid item xs={6}>
                    <Paper style={{textAlign: 'center'}}>
                        <strong>FPGA device {i++}</strong><br/>
                        Packets Ethernet (total): {dev.getPacketsEther()}<br/>
                        Packets UDP (total):      {dev.getPacketsUdp()}<br/>
                        Packets from detector:    {dev.getPacketsJfjoch()}<br/>
                        Transceiver present:      {dev.getQsfpModulePresent() ? "OK" : <span style={{color:"red"}}> No </span>}<br/>
                        Ethernet aligned:         {dev.getEthernetRxAligned() ? "OK" : <span style={{color:"red"}}> Error </span>}<br/>
                        Temperature FPGA:         {dev.getFpgaTempDegc().toFixed(1)} &#8451;<br/>
                        Temperature HBM:          {dev.getHbmTemp()} &#8451;<br/>
                        Power usage FPGA:         {(dev.getCurrentEdge3p3vA() * dev.getVoltageEdge3p3vV()
                        + dev.getCurrentEdge12vA() * dev.getVoltageEdge12vV()).toFixed(1)} W <br/>
                    </Paper>
                </Grid>))}
        </Grid>
    }
}

export default FPGAGrid;
