import React, {Component} from 'react';

import {ThemeProvider, CssBaseline, createTheme, Grid} from "@mui/material";
import {indigo, lime} from "@mui/material/colors";

import DataProcessingSettings from "./components/DataProcessingSettings";
import DetectorSettings from "./components/DetectorSettings";
import Calibration from "./components/Calibration";
import StatusBar from "./components/StatusBar";
import DataProcessingPlots from "./components/DataProcessingPlots";

const jfjoch_theme = createTheme({
    palette: {
        primary: indigo,
        secondary: lime,
    },
});

const addr: string = "http://localhost:5232";

class App extends Component {

    render() {
        return <ThemeProvider theme={jfjoch_theme}>
            <CssBaseline enableColorScheme />
            <StatusBar addr={addr}/> <br/><br/><br/><br/>
            <div style={{display: 'flex',
                alignItems: 'center',
                justifyContent: 'center'}}>
            <Grid container
                  justifyContent="center"
                  alignItems="center"
                  spacing={3}
            sx={{width: "95%"}}>
                <Grid item xs={8}>
                    <DataProcessingPlots addr={addr}/>
                </Grid>
                <Grid item xs={4}>
                    <DataProcessingSettings addr={addr}/>
                </Grid>

                <Grid item xs={4}>
                    <DetectorSettings addr={addr}/>
                </Grid>
                <Grid item xs={8}>
                    <Calibration addr={addr}/>
                </Grid><br/><br/>

            </Grid>
            </div>
        </ThemeProvider>
    }
}

export default App;
