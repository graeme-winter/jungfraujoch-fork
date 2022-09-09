import React, {Component} from 'react';

import MemoryIcon from '@mui/icons-material/Memory';
import TimelineIcon from '@mui/icons-material/Timeline';
import IsoIcon from '@mui/icons-material/Iso';
import TuneIcon from '@mui/icons-material/Tune';
import TocIcon from '@mui/icons-material/Toc';
import ExpandMoreIcon from '@mui/icons-material/ExpandMore';

import Accordion from '@mui/material/Accordion';
import AccordionSummary from '@mui/material/AccordionSummary';
import AccordionDetails from '@mui/material/AccordionDetails';
import Typography from '@mui/material/Typography';

import StatusBar from './components/StatusBar';
import DataTable from './components/DataTable';
import PedestalGrid from "./components/PedestalGrid";
import FPGAGrid from "./components/FPGAGrid";
import DataProcessing from "./components/DataProcessing";
import ImageAnalysis from "./components/ImageAnalysis";

import {Empty, DataProcessingSettings} from "./protobuf/jfjoch_pb";
import {gRPC_JFJochBrokerClient} from "./protobuf/jfjoch_grpc_web_pb";

const grpc_web_url = "http://mx-jungfrau-1:8080";

function processGRPCError (err) {
    if (err) {
        console.log(`Unexpected error for GetStatus: code = ${err.code}` +
            `, message = "${err.message}"`);
    }
}

function readGrpcPlot(input) {
    if (input.getXList().length === 0)
        return {x: [0 , 1], y: [0, 0]};
    else
        return {x: input.getXList(), y: input.getYList()};
}

class App extends Component {
    state = {
        daqState: "Connecting...",
        indexingRatePlot: [],
        meanSpotsPlot: [],
        bkgEstimatePlot: [],
        radialIntPlot: [],
        currMasterFileName: ""
    }

    updateProcessing = false;
    photonCountThr = 10;
    signalToNoiseThr = 4;
    minPixelsPerSpot = 2;
    localBkgSize = 5;
    maxPixelsPerSpot = 50;
    highResolutionLimit = 1.0;

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
                modules: response.getCalibrationStatistics().getModuleStatisticsList()
            };

            if (response.getReceiverStatus()) {
                state_map.meanSpotsPlot = readGrpcPlot(response.getReceiverStatus().getSpotCount());
                state_map.bkgEstimatePlot = readGrpcPlot(response.getReceiverStatus().getBkgEstimate());
                state_map.radialIntPlot = readGrpcPlot(response.getReceiverStatus().getRadialIntProfile());

                state_map.currMasterFileName = response.getReceiverStatus().getMasterFileName();
            } else
                state_map.currMasterFileName = "";

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

                state_map.indexingRatePlot = readGrpcPlot(response.getIndexerStatus().getIndexingRate());
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
            data_processing_settings.setHighResolutionLimit();
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

    highResolutionLimitSliderMoved = (event, newValue) => {
        if (newValue !== this.highResolutionLimit) {
            this.highResolutionLimit = newValue;
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
            <br/>

            <Accordion>
                <AccordionSummary expandIcon={<ExpandMoreIcon />}>
                    <Typography><TocIcon/>  &nbsp;&nbsp; Last measurement statistics</Typography>
                </AccordionSummary>
                <AccordionDetails>
                    <DataTable data={this.state.params}/>
                </AccordionDetails>
            </Accordion>

            <Accordion>
                <AccordionSummary expandIcon={<ExpandMoreIcon />}>
                    <Typography><TimelineIcon/>  &nbsp;&nbsp; Real-time image analysis {this.state.currMasterFileName !== "" ? "for " + this.state.currMasterFileName :""}</Typography>
                </AccordionSummary>
                <AccordionDetails>
                    <ImageAnalysis indexingRatePlot={this.state.indexingRatePlot} meanSpotsPlot={this.state.meanSpotsPlot}
                                   bkgEstimatePlot={this.state.bkgEstimatePlot} radialIntPlot={this.state.radialIntPlot}/>
                </AccordionDetails>
            </Accordion>
            <Accordion>
                <AccordionSummary expandIcon={<ExpandMoreIcon />}>
                    <Typography><TuneIcon/>  &nbsp;&nbsp; Image analysis settings </Typography>
                </AccordionSummary>
                <AccordionDetails>
                    <DataProcessing photonCountSlider={this.photonCountSliderMoved} signalToNoiseSlider={this.signalToNoiseSliderMoved}
                                    minPixPerSpotSlider={this.minPixelsPerSpotSliderMoved} highResolutionSlider={this.highResolutionLimitSliderMoved}
                                    toggleCheckBox={this.toggleCheckBox} />
                </AccordionDetails>
            </Accordion>
            <Accordion>
                <AccordionSummary expandIcon={<ExpandMoreIcon />}>
                    <Typography><IsoIcon/> &nbsp;&nbsp; Pedestal results </Typography>
                </AccordionSummary>
                <AccordionDetails>
                    <PedestalGrid modules={this.state.modules}/>
                </AccordionDetails>
            </Accordion>
            <Accordion>
                <AccordionSummary expandIcon={<ExpandMoreIcon />}>
                    <Typography><MemoryIcon/> &nbsp;&nbsp;  FPGA statistics </Typography>
                </AccordionSummary>
                <AccordionDetails>
                    <FPGAGrid devices={this.state.fpga_status}/>
                </AccordionDetails>
            </Accordion>

        </div>
    }
}

export default App;
