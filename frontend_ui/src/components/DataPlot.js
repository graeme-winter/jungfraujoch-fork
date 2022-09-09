import React, {Component} from 'react';
import Plot from "react-plotly.js";
import Paper from '@mui/material/Paper';
import Grid from "@mui/material/Grid";

class DataPlot extends Component {
    render() {
        return <Grid container spacing={3}>
            <Grid item xs/>
            <Grid item >
                <Paper style={{textAlign: 'center'}}>
                    <Plot
                        data={[
                            {
                                x: this.props.data.x,
                                y: this.props.data.y,
                                type: 'scatter',
                                mode: 'lines',
                                marker: {color: 'red'},
                            },
                        ]}
                        layout={ {width: 1600, height: 400, title: this.props.title, xaxis: {title: this.props.xaxis},
                            yaxis: {title: this.props.yaxis}} }
                    />
                </Paper><br/>
            </Grid>
            <Grid item xs/>
        </Grid>
    }
}

export default DataPlot;