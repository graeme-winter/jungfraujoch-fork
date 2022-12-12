import React, {Component} from 'react';

import {Switch, Grid, InputAdornment, TextField} from "@mui/material";
import Button from "@mui/material/Button";
import Paper from "@mui/material/Paper";
import {handleErrors} from "./handleErrors";

type MyProps = {
    addr: string;
}

type MyState = {
    s: {
        frame_time_us: number | string,
        use_storage_cells: boolean,
        use_internal_packet_generator: boolean,
        collect_raw_data: boolean,
        count_time_us: number | string,
        pedestal_g0_frames: number | string,
        pedestal_g1_frames: number | string,
        pedestal_g2_frames: number | string
    },
    frame_time_error: boolean,
    count_time_error: boolean,
    connection_error: boolean
}

class DetectorSettings extends Component<MyProps, MyState> {
    addr: string;

    constructor(props: MyProps) {
        super(props);
        this.addr = props.addr;
        console.log(this.addr);
    }
    state = {
        s: {
            frame_time_us: 1000,
            use_storage_cells: false,
            use_internal_packet_generator: false,
            collect_raw_data: false,
            count_time_us: 980,
            pedestal_g0_frames: 2000,
            pedestal_g1_frames: 300,
            pedestal_g2_frames: 300
        },
        frame_time_error: false,
        count_time_error: false,
        connection_error: true
    }

    uploadButton = () => { this.putValues(); }
    downloadButton = () => { this.getValues(); }

    updateCollectionTime(frame_time : number | string, count_time : number | string) {
        let frame_err = !Number.isInteger(Number(frame_time));
        let count_err = !Number.isInteger(Number(count_time));

        if (!frame_err && !count_err) {
            if (Number(frame_time) - 20 < Number(count_time))
                count_err = true;
        }

        if (!frame_err) {
            if ((Number(frame_time) < 500) || (Number(frame_time) > 2000))
                frame_err = true;
        }

        if (!count_err) {
            if (Number(count_time) < 0)
                count_err = true;
        }

        this.setState(prevState => (
            {
                frame_time_error: frame_err,
                count_time_error: count_err,
                s : {...prevState.s,
                    frame_time_us: frame_time,
                    count_time_us: count_time
                }
            }
        ));
    }


    updateFrameTime = (event: React.ChangeEvent<HTMLInputElement>) => {
        this.updateCollectionTime(event.target.value, this.state.s.count_time_us);
    }

    updateCountTime = (event: React.ChangeEvent<HTMLInputElement>) => {
        this.updateCollectionTime(this.state.s.frame_time_us, event.target.value);
    }

    storageCellToggle = (event: React.ChangeEvent<HTMLInputElement>) => {
        this.setState(prevState => (
            {
                s : {...prevState.s,
                    use_storage_cells: event.target.checked}
            }
        ));
    }

    rawDataToggle = (event: React.ChangeEvent<HTMLInputElement>) => {
        this.setState(prevState => (
            {
                s : {...prevState.s,
                    collect_raw_data: event.target.checked}
            }
        ));
    }
    deactivate = () => {
        fetch(this.addr + '/detector/deactivate', {
            method: "POST"
        }).then(handleErrors)
            .catch(error => console.log(error) );
    }

    putValues = () => {
        fetch(this.addr + '/detector/settings', {
            method: "PUT",
            body: JSON.stringify(this.state.s)
        }).then(handleErrors)
            .catch(error => console.log(error) );
    }

    getValues = () => {
        fetch(this.addr + '/detector/settings')
            .then(handleErrors)
            .then(res => res.json())
            .then(data => this.setState({
                s: data,
                connection_error: false
            }))
            .catch(error => {
                console.log(error);
                this.setState({connection_error: true});
            });
    }
    componentDidMount() {
        this.getValues();
    }

    render() {
        return <Paper style={{textAlign: 'center'}} sx={{ height: 527, width: '100%' }}>

            <Grid container spacing={0}>

                <Grid item xs={1}/>
                <Grid item xs={10}>
                    <br/><strong>Detector settings (EXPERT) </strong>
                    <br/><br/><br/>
                    <TextField id="frame_time" label="Frame (unsummed) time" variant="outlined"
                               error={this.state.frame_time_error}
                               onChange={this.updateFrameTime}
                               value={this.state.s.frame_time_us}
                               disabled={this.state.connection_error}
                               InputProps = {{
                                   inputProps: {
                                       style: {textAlign: 'right'}
                                   },
                                   endAdornment: <InputAdornment position="end">&micro;s</InputAdornment>
                               }}/>
                    <br/><br/>
                    <TextField id="integration_time" label="Integration time" variant="outlined"
                               error={this.state.count_time_error}
                               onChange={this.updateCountTime}
                               value={this.state.s.count_time_us}
                               disabled={this.state.connection_error}
                               InputProps = {{
                                   inputProps: {
                                       style: {textAlign: 'right'}
                                   },
                                   endAdornment: <InputAdornment position="end">&micro;s</InputAdornment>
                               }}/>

                    <br/><br/>

                    <Switch onChange={this.storageCellToggle} checked={this.state.s.use_storage_cells}
                            disabled={this.state.connection_error}/>
                    Measure with storage cells<br/><br/>

                    <Switch onChange={this.rawDataToggle} checked={this.state.s.collect_raw_data}
                            disabled={this.state.connection_error}/>
                    Collect raw data <br/><br/>

                    <Button color="secondary" onClick={this.downloadButton} variant="contained" disableElevation>
                        Reload</Button>&nbsp;&nbsp;
                    <Button color="secondary" onClick={this.uploadButton} variant="contained" disableElevation
                            disabled={this.state.count_time_error || this.state.frame_time_error}>Upload</Button>
                    <br/><br/>
                    <Button color="secondary" onClick={this.deactivate} variant="contained" disableElevation>
                        Deactivate to turn off</Button>
                    <br/><br/>
                </Grid>
                <Grid item xs={1}/>

            </Grid>
        </Paper>
    }
}

export default DetectorSettings;