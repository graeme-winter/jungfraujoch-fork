import React, {Component} from 'react';

import Plot from "react-plotly.js";

// Not using TypeScript, as plotly is not TypeScript :(

class PlotWrapper extends Component {
    render() {
        return <Plot
            style={{width: "100%", height: "100%"}}
            data={[
                {
                    x: this.props.x,
                    y: this.props.y,
                    type: 'scatter',
                    mode: 'lines',
                    marker: {color: "303f9f"},
                },
            ]}
            layout={ {
               xaxis: {title: this.props.xaxis},
               yaxis: {title: this.props.yaxis},
                uirevision: true
        } }
            config = {{responsive: true}}
        />
    }
}

export default PlotWrapper;