import React, {Component} from 'react';

import StatusBar from './components/StatusBar';
import DataTable from './components/DataTable';
import DetectorGrid from "./components/DetectorGrid";
import FPGAGrid from "./components/FPGAGrid";
import DataProcessing from "./components/DataProcessing";
import SpotFindingPlot from "./components/SpotFindingPlot";

import {BrokerStatus, Empty, DataProcessingSettings} from "./protobuf/jfjoch_pb";
import {gRPC_JFJochBrokerClient} from "./protobuf/jfjoch_grpc_web_pb";

const grpc_web_url = "http://mx-jungfrau-1:8080";

function processGRPCError (err, response) {
    if (err) {
        console.log(`Unexpected error for GetStatus: code = ${err.code}` +
            `, message = "${err.message}"`);
    }
}

class App extends Component {
    state = {
        daqState: "Connecting...",
        indexingRatePlot: [],
        meanSpotsPlot: []
    }

    updateProcessing = false;
    photonCountThr = 10;
    signalToNoiseThr = 4;
    minPixelsPerSpot = 2;
    localBkgSize = 5;
    maxPixelsPerSpot = 50;

    processStatus = (err, response) => {
        if (err) {
            this.setState({ daqState: "Not connected"});
            console.log(`Unexpected error for GetStatus: code = ${err.code}` +
                `, message = "${err.message}"`);
        } else {
            let param_array = [
                {
                    desc: "Efficiency",
                    value: (100.0 * response.getLastMeasurementCollectionEfficiency()).toFixed() + " %"
                },
                {desc: "Compression ratio", value: response.getLastMeasurementCompressionRatio().toPrecision(3) + "x"},
                {desc: "Images collected", value: response.getLastMeasurementImagesCollected()},
                {desc: "File prefix", value: response.getLastMeasurementName()}
            ];

            let state_map = {
                params: param_array,
                modules: response.getModuleStatisticsList()
            };

            if (response.hasIndexerStatus()) {
                param_array.push({
                    desc: "Indexing rate",
                    value: (response.getIndexerStatus().getImagesAnalyzed() > 0) ?
                        ((response.getIndexerStatus().getImagesIndexed() * 1000.0 / response.getIndexerStatus().getImagesAnalyzed()).toFixed() /10.0)
                        + " %" : 0
                });
                param_array.push({
                    desc: "Images indexed",
                    value: response.getIndexerStatus().getImagesIndexed()
                });
                param_array.push({
                    desc: "Images for indexing",
                    value: response.getIndexerStatus().getImagesAnalyzed()
                });

                if (response.getIndexerStatus().getIndexingRateList().length === 0) {
                    state_map.indexingRatePlot = [{x: 0, y: 0}, {x: 1, y: 0}];
                } else {
                    let i = 0;
                    state_map.indexingRatePlot = response.getIndexerStatus().getIndexingRateList()?.map((point) => (
                        {x: i++, y: point}
                    ));
                }
                if (response.getIndexerStatus().getMeanSpotsList().length === 0) {
                    state_map.meanSpotsPlot = [{x: 0, y: 0}, {x: 1, y: 0}];
                } else {
                    let i = 0;
                    state_map.meanSpotsPlot = response.getIndexerStatus().getMeanSpotsList()?.map((point) => (
                        {x: i++, y: point}
                    ));
                }
            }

            state_map.daqState = response.getCurrentState();

            if (response.hasReceiverStatus()) {
                state_map.progress = response.getReceiverStatus().getProgress();
                state_map.fpga_status = response.getReceiverStatus().getFpgaStatusList();
            }
            this.setState(state_map);
        }
    };

    updateGRPC() {
        let broker = new gRPC_JFJochBrokerClient(grpc_web_url, null, null);
        var empty = new Empty();

        if (this.updateProcessing) {
            let data_processing_settings = new DataProcessingSettings();

            data_processing_settings.setPhotonCountThreshold(this.photonCountThr);
            data_processing_settings.setSignalToNoiseThreshold(this.signalToNoiseThr);
            data_processing_settings.setMinPixPerSpot(this.minPixelsPerSpot);
            data_processing_settings.setMaxPixPerSpot(this.maxPixelsPerSpot);
            data_processing_settings.setLocalBkgSize(this.localBkgSize);

            broker.setDataProcessingSettings(data_processing_settings,null, processGRPCError);
        }

        broker.getStatus(empty, {}, this.processStatus);
    }

    componentDidMount() {
        this.updateGRPC();
        this.interval = setInterval(() => this.updateGRPC(), 1000);
    }

    componentWillUnmount() {
        clearInterval(this.interval);
    }

    photonCountSliderMoved = (event, newValue) => {
        if (newValue !== this.photonCountThr) {
            this.photonCountThr = newValue;
        }
    };

    signalToNoiseSliderMoved = (event, newValue) => {
        if (newValue !== this.signalToNoiseThr) {
            this.signalToNoiseThr = newValue;
        }
    };

    minPixelsPerSpotSliderMoved = (event, newValue) => {
        if (newValue !== this.minPixelsPerSpot) {
            this.minPixelsPerSpot = newValue;
        }
    };

    toggleCheckBox = (event) => {
        this.updateProcessing = event.target.checked;
    };

    handleChange = (event) => {
        this.setState({[event.target.name]: event.target.checked });
    };

    initialize() {
        let broker = new gRPC_JFJochBrokerClient(grpc_web_url, null, null);
        let empty = new Empty();
        broker.initialize(empty, {}, processGRPCError);
    }

    cancel() {
        let broker = new gRPC_JFJochBrokerClient(grpc_web_url, null, null);
        let empty = new Empty();
        broker.cancel(empty, {}, processGRPCError);
    }

    render() {
        return <div>
            <StatusBar daqState={this.state.daqState} cancel={this.cancel} initialize={this.initialize} progress={this.state.progress} />
            <br/><DataTable data={this.state.params}/><br/><br/>
            <DetectorGrid modules={this.state.modules}/><br/><br/>
            <FPGAGrid devices={this.state.fpga_status}/><br/><br/>
            <DataProcessing photonCountSlider={this.photonCountSliderMoved} signalToNoiseSlider={this.signalToNoiseSliderMoved}
                            minPixPerSpotSlider={this.minPixelsPerSpotSliderMoved} toggleCheckBox={this.toggleCheckBox} /><br/><br/>
            <SpotFindingPlot indexingRatePlot={this.state.indexingRatePlot} meanSpotsPlot={this.state.meanSpotsPlot}/>
        </div>
    }
}

export default App;
