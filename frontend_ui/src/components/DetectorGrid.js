
import React, {Component} from 'react';

import Grid from '@material-ui/core/Grid';
import Paper from '@material-ui/core/Paper';

function formatPedestal(val) {
    var x = Number(val);
    if (x === 0) return "0";
    return x.toPrecision(5);
}

class DetectorGrid extends Component {
    render() {
        let i = 0;
        return <Grid container spacing={3}>
            {this.props.modules?.map((module) => (
                <Grid item xs={3}>
                    <Paper style={{textAlign: 'center'}}>
                        <strong>Detector module {i++}</strong><br/>
                        Bad pixels: {module.getMaskedPixels()}<br/>
                        Mean pedestal G0: {formatPedestal(module.getPedestalG0Mean())}<br/>
                        Mean pedestal G1: {formatPedestal(module.getPedestalG1Mean())}<br/>
                        Mean pedestal G2: {formatPedestal(module.getPedestalG2Mean())}<br/>
                    </Paper>
                </Grid>))}
        </Grid>
    }
}

export default DetectorGrid;
