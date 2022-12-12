import React, {Component} from 'react';

import Paper from '@mui/material/Paper';
import {Grid, Slider, Typography} from "@mui/material";
import {handleErrors} from "./handleErrors";

type MyProps = {
    addr: string;
};

type MyState = {
    s: {
        photon_count_threshold: number | string,
        signal_to_noise_threshold: number | string,
        min_pix_per_spot: number | string,
        max_pix_per_spot: number | string,
        local_bkg_size: number | string,
        high_resolution_limit?: number | string,
        low_resolution_limit?: number | string,
        bkg_estimate_low_q: number | string,
        bkg_estimate_high_q: number | string
    },
    connection_error: boolean;
}

class DataProcessingSettings extends Component<MyProps, MyState> {
    addr : string;
    interval : NodeJS.Timer | undefined;
    state : MyState = {
        s: {
            photon_count_threshold: 8,
            signal_to_noise_threshold: 3.0,
            min_pix_per_spot: 2,
            max_pix_per_spot: 50,
            local_bkg_size: 5,
            high_resolution_limit: 2.5,
            bkg_estimate_low_q: 3,
            bkg_estimate_high_q: 5
        },
        connection_error: true
    }

    constructor(props: MyProps) {
        super(props);
        this.addr = props.addr;
    }
    putValues() {
        fetch(this.addr + '/data_processing/settings', {
            method: "PUT",
            body: JSON.stringify(this.state.s)
        }).then(handleErrors)
            .catch(error => console.log(error) );
    }

    getValues() {
        fetch(this.addr + '/data_processing/settings')
            .then(handleErrors)
            .then(res => res.json())
            .then(data => this.setState({s: data, connection_error: false}))
            .catch(error => {
                console.log(error);
                this.setState({connection_error: true});
            });
    }
    componentDidMount() {
        this.getValues();
        this.interval = setInterval(() => this.getValues(), 1000);
    }

    componentWillUnmount() {
        clearInterval(this.interval);
    }

    setPhotonCountThreshold = (event: Event, newValue: number | number[]) => {
        this.setState(prevState => (
            {
                s : {
                    ...prevState.s,
                    photon_count_threshold: newValue as number
                }
            }
        ));
        this.putValues();
    }

    setSignalToNoiseThreshold = (event: Event, newValue: number | number[]) => {
        this.setState(prevState => (
            {
                s : {
                    ...prevState.s,
                    signal_to_noise_threshold: newValue as number
                }
            }
        ));
        this.putValues();
    }

    setMinPixPerSpot = (event: Event, newValue: number | number[]) => {
        this.setState(prevState => (
            {
                s : {
                    ...prevState.s,
                    min_pix_per_spot: newValue as number
                }
            }
        ));
        this.putValues();
    }

    setHighResolutionLimit = (event: Event, newValue: number | number[]) => {
        this.setState(prevState => (
            {
                s : {
                    ...prevState.s,
                    high_resolution_limit: newValue as number
                }
            }
        ));
        this.putValues();
    }

    render() {
        return <Paper style={{textAlign: 'center'}} sx={{ height: 400, width: '100%' }}>
                    <Grid container spacing={0}>

                        <Grid item xs={1}/>
                        <Grid item xs={10}>
                            <br/><strong>Data processing parameters</strong><br/><br/>
                            <Typography gutterBottom> Count threshold </Typography>
                            <Slider disabled={this.state.connection_error}
                                    value={Number(this.state.s.photon_count_threshold)}
                                    onChange={this.setPhotonCountThreshold}
                                    min={1} max={50} step={1} valueLabelDisplay="auto" />

                            <br/><Typography> Signal-to-noise threshold </Typography>
                            <Slider disabled={this.state.connection_error}
                                    value={Number(this.state.s.signal_to_noise_threshold)}
                                    onChange={this.setSignalToNoiseThreshold}
                                    min={2} max={10} step={0.5} valueLabelDisplay="auto" />

                            <br/><Typography> Minimum pixel / spot </Typography>
                            <Slider disabled={this.state.connection_error}
                                    value={Number(this.state.s.min_pix_per_spot)}
                                    onChange={this.setMinPixPerSpot}
                                    min={1} max={8} step={1} valueLabelDisplay="auto"  />
                            <Typography> High resolution limit [&#8491;] </Typography>
                            <Slider disabled={this.state.connection_error}
                                    value={Number(this.state.s.high_resolution_limit)}
                                    onChange={this.setHighResolutionLimit}
                                    min={1} max={5} step={0.2} valueLabelDisplay="auto"  />
                            <br/>
                        </Grid>
                        <Grid item xs={1}/>

                    </Grid>
                </Paper>
    }
}

export default DataProcessingSettings;