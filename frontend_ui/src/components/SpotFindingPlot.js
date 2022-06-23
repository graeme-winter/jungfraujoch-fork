import React, {Component} from 'react';
import { ValueScale } from '@devexpress/dx-react-chart';
import {
    ValueAxis,
    Chart,
    LineSeries,
    Title
} from '@devexpress/dx-react-chart-material-ui';

import Paper from '@material-ui/core/Paper';
import Grid from "@material-ui/core/Grid";

const modifyDomain = () => [0.0, 30.0];

class SpotFindingPlot extends Component {
    constructor(props) {
        super(props);
        this.state = {
            data: [{y: props.indexing_plot}],
            layout: {
                xaxis: {title: {text: "Time [s]"}},
                yaxis: {title: {text: "Number of spots"}}
            },
            frames: [],
            config: {responsive: true}
        };
    }

    render() {
        return <Grid container spacing={3}>
            <Grid item xs={1}/><Grid item xs={10}><Paper style={{textAlign: 'center'}}>
                <Chart data={this.props.indexingRatePlot}>
                    <ValueAxis showGrid={false}/>
                    <ValueScale modifyDomain={modifyDomain}/>
                    <Title text="Indexing rate [%]" />
                    <LineSeries valueField="y" argumentField="x" name="Indexing rate"/>
                </Chart>
            </Paper></Grid><Grid item xs={1}/><Grid item xs={1}/>
            <Grid item xs={10}><Paper style={{textAlign: 'center'}}>
                <Chart data={this.props.meanSpotsPlot}>
                    <ValueAxis showGrid={false}/>
                    <ValueScale/>
                    <Title text="Mean spot count" />
                    <LineSeries valueField="y" argumentField="x" name="Mean spots"/>
                </Chart>
            </Paper></Grid><Grid item xs={1}/>
        </Grid>
    }
}

export default SpotFindingPlot;
