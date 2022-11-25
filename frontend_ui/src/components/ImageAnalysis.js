import React, {Component} from 'react';
import DataPlot from './DataPlot';

class ImageAnalysis extends Component {
    render() {
        return <div>
            <DataPlot data={this.props.indexingRatePlot} title="Indexing rate [%]" yaxis="Indexing rate [%]" xaxis="Image number"/>
            <DataPlot data={this.props.meanSpotsPlot} title="Mean spot count" yaxis="GetSpots" xaxis="Image number"/>
            <DataPlot data={this.props.bkgEstimatePlot} title="Background estimate" yaxis="Counts" xaxis="Image number"/>
            <DataPlot data={this.props.radialIntPlot} title="Radial integration plot" xaxis="Q [A^-1]" yaxis="Counts"/>
        </div>
    }
}

export default ImageAnalysis;
