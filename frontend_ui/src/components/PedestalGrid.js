
import React, {Component} from 'react';

import Grid from '@mui/material/Grid';
import Paper from '@mui/material/Paper';

function formatPedestal(val) {
    var x = Number(val);
    if (x === 0) return "0";
    return Math.round(x);
}

class PedestalGrid extends Component {
    render() {
        return <Grid container spacing={3}>
            {this.props.modules?.map((module) => (
                <Grid item xs={3}>
                    <Paper style={{textAlign: 'center'}}>
                        <strong>Detector module {module.getModuleNumber()} / SC {module.getStorageCellNumber()}</strong><br/>
                        Bad pixels: {module.getMaskedPixels()}<br/>
                        Pedestal G0: {formatPedestal(module.getPedestalG0Mean())} (&#177; {formatPedestal(module.getPedestalRmsG0Mean())})<br/>
                        Pedestal G1: {formatPedestal(module.getPedestalG1Mean())} (&#177; {formatPedestal(module.getPedestalRmsG1Mean())})<br/>
                        Pedestal G2: {formatPedestal(module.getPedestalG2Mean())} (&#177; {formatPedestal(module.getPedestalRmsG2Mean())})<br/>
                    </Paper>
                </Grid>))}
        </Grid>
    }
}

export default PedestalGrid;
