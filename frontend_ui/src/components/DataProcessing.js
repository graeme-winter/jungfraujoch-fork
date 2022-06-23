import React, {Component} from 'react';

import Paper from '@material-ui/core/Paper';
import {Slider, Typography, Grid, Switch} from "@material-ui/core";

class DetectorGrid extends Component {

    state = {
        a: 100.0,
        b: 200.0

    }

    set_a = (event) => {
        this.setState({a: event.target.value});
    }

    render() {
        return <Grid container spacing={3}>
            <Grid item xs={4}/>
            <Grid item xs={4}>
                <Paper style={{textAlign: 'left'}}>
                    <strong><Switch onChange={this.props.toggleCheckBox}/>Data processing parameters</strong><br/><br/>
                    <Typography gutterBottom> Count threshold </Typography>
                    <Slider defaultValue={10} min={1} max={25} step={1} onChange={this.props.photonCountSlider} valueLabelDisplay="auto"  />
                    <br/><Typography> Signal-to-noise threshold </Typography>
                    <Slider defaultValue={3} min={1} max={10} step={0.5} onChange={this.props.signalToNoiseSlider} valueLabelDisplay="auto" />
                    <br/><Typography> Minimum pixel / spot </Typography>
                    <Slider defaultValue={2} min={1} max={8} step={1} onChange={this.props.minPixPerSpotSlider} valueLabelDisplay="auto"  />
                    <br/>
                </Paper>
            </Grid>
            <Grid item xs={4}/>
            </Grid>
    }
}

export default DetectorGrid;