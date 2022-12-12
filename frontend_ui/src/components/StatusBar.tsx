
import React, {Component} from 'react';

import AppBar from '@mui/material/AppBar';
import Toolbar from '@mui/material/Toolbar';
import Typography from '@mui/material/Typography';
import Button from '@mui/material/Button';
import {handleErrors} from "./handleErrors";

function decodeDAQState (val: string) {
    switch(val) {
        case "NOT_INITIALIZED":
            return "Not initialized";
        case "BUSY":
            return "Busy";
        case "IDLE":
            return "Idle";
        case "DATA_COLLECTION":
            return "Data collection";
        case "PEDESTAL":
            return "Pedestal collection";
        case "ERROR":
            return "Error";
        default:
            return "Unknown state";
    }
}

type MyProps = {
    addr: string;
};

type MyState = {
    status: {
        broker_state: string;
        progress?: number | string;
        indexing_rate?: number | string;
    }
    connection_error: boolean;
}

class StatusBar extends Component<MyProps, MyState> {
    addr: string;
    interval: NodeJS.Timer | undefined;

    constructor(props: MyProps) {
        super(props);
        this.addr = props.addr;
        console.log(this.addr);
    }

    state : MyState = {
        status: {
            broker_state: "NOT_CONNECTED"
        },
        connection_error: true
    }

    initialize = () => {
        fetch(this.addr + '/detector/initialize', {
            method: "POST"
        }).then(handleErrors)
            .catch(error => console.log(error) );
    }

    cancel = () => {
        fetch(this.addr + '/detector/cancel', {
            method: "POST"
        }).then(handleErrors)
            .catch(error => console.log(error) );
    }
    getValues() {
        fetch(this.addr + '/detector/status')
            .then(handleErrors)
            .then(res => res.json())
            .then(data => this.setState({status: data, connection_error: false}))
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

    statusDescription() {
        return <div>
            State: {decodeDAQState (this.state.status.broker_state)}
            {"progress" in this.state.status ? " (" + this.state.status.progress + " %)" : ""}
            {"indexing_rate" in this.state.status ? this.state.status.indexing_rate + "Indexing rate  %" : ""}
        </div>
    }
    render() {
        return <AppBar>
            <Toolbar>
                <Typography variant="h6" style={{flexGrow: 0.5}}>
                    PSI Jungfraujoch
                </Typography>

                <Typography variant="h6" style={{flexGrow: 2.0}}>
                    {this.state.connection_error ? "Not connected" : this.statusDescription() }
                </Typography>

                <Typography variant="h6" style={{flexGrow: 2.0}}>
                </Typography>
                <Button color="secondary" onClick={this.cancel} variant="contained" disableElevation>Cancel</Button>&nbsp;&nbsp;
                <Button color="secondary" onClick={this.initialize} variant="contained" disableElevation>Initialize</Button>

            </Toolbar>
        </AppBar>
    }
}

export default StatusBar;
