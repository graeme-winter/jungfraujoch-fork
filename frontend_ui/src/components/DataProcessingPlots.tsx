import React, {Component} from 'react';

import Paper from '@mui/material/Paper';
import PlotWrapper from "./PlotWrapper";
import {Box, Tab, Tabs} from "@mui/material";
import {handleErrors} from "./handleErrors";

type MyProps = {
    addr: string;
};

type Plot = {
    x: number[],
    y: number[]
}

type MyState = {
    plots : {
        indexing_rate: Plot,
        bkg_estimate: Plot,
        spot_count: Plot,
        radial_int_profile: Plot
    },
    tab: number,
    connection_error: boolean
}

class DataProcessingPlots extends Component<MyProps, MyState> {
    addr: string;
    interval: undefined | NodeJS.Timer;

    state: MyState = {
        plots : {
            indexing_rate: {
                x: [0,100,200,300,400,500],
                y: [0.1, 0.3,0.5, 0.2, 0.0, 0.1]
            },
            bkg_estimate: {
                x: [0,100,200,300,400,500],
                y: [50.1, 35.0, 20.4, 11.5, 7.1, 9.6]
            },
            spot_count: {
                x: [0,100,200,300,400,500],
                y: [12,13,45,11,5,47]
            },
            radial_int_profile: {
                x: [0,100,200,300,400,500],
                y: [12,13,45,11,5,47]
            }
        },
        tab: 1,
        connection_error: true
    }

    constructor(props: MyProps) {
        super(props);
        this.addr = props.addr;
    }

    getValues() {
        fetch(this.addr + '/data_processing/plots')
            .then(handleErrors)
            .then(res => res.json())
            .then(data => this.setState({plots: data, connection_error: false}))
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

    tabsOnChange = (event: React.SyntheticEvent, value: number) => {
        this.setState({tab: value});
    }

    selectPlot() {
        switch (this.state.tab) {
            case 1:
                return <PlotWrapper xaxis="Image number" yaxis="Indexing rate"
                                       x={this.state.plots.indexing_rate.x} y={this.state.plots.indexing_rate.y}/>;
            case 2:
                return <PlotWrapper xaxis="Image number" yaxis="Photon count"
                                       x={this.state.plots.bkg_estimate.x} y={this.state.plots.bkg_estimate.y}/>;
            case 3:
                return <PlotWrapper xaxis="Image number" yaxis="Spot count"
                                    x={this.state.plots.spot_count.x} y={this.state.plots.spot_count.y}/>;
            case 4:
                return <PlotWrapper xaxis="Q [A^-1]" yaxis="Photon count"
                                    x={this.state.plots.radial_int_profile.x} y={this.state.plots.radial_int_profile.y}/>;
            default:
                return <div/>
        }
    }
    render() {
        return <Paper style={{textAlign: 'center'}} sx={{ height: 400, width: "100%" }}>
            <Box sx={{ borderBottom: 1, borderColor: 'divider' }}>
                <Tabs value={this.state.tab} onChange={this.tabsOnChange}>
                    <Tab label="Indexing"  value={1}/>
                    <Tab label="Background" value={2} />
                    <Tab label="Spot finding" value={3} />

                    <Tab label="Rad. integration" value={4} />
                </Tabs>
            </Box>

            <Box sx={{width:"95%", height: 350}} >
                {this.selectPlot()}
            </Box>

        </Paper>
    }
}

export default DataProcessingPlots;