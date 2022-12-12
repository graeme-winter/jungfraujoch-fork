
import React, {Component} from 'react';

import Paper from '@mui/material/Paper';
import {DataGrid} from "@mui/x-data-grid";
import {handleErrors} from "./handleErrors";

type MyProps = {
    addr: string;
};

type CalibInfo = {
    module_number: number | string,
    storage_cell_number: number | string,
    pedestal_g0_mean: number,
    pedestal_g1_mean: number,
    pedestal_g2_mean: number,
    pedestal_rms_g0_mean: number,
    pedestal_rms_g1_mean: number,
    pedestal_rms_g2_mean: number,
    masked_pixels: number | string
};

type MyState = {
    calib: CalibInfo[];
    connection_error: boolean;
};

class Calibration extends React.Component<MyProps, MyState> {
    addr: string;
    interval: NodeJS.Timer | undefined;

    constructor(props: MyProps) {
        super(props);
        this.addr = props.addr;
        console.log(this.addr);
    }

    state : MyState = {
        calib: [
            {
                module_number: 0,
                storage_cell_number: 0,
                pedestal_g0_mean: 3500,
                pedestal_g1_mean: 14000,
                pedestal_g2_mean: 15000,
                pedestal_rms_g0_mean: 30,
                pedestal_rms_g1_mean: 11,
                pedestal_rms_g2_mean: 10,
                masked_pixels: 80
            },
            {
                module_number: 1,
                storage_cell_number: 0,
                pedestal_g0_mean: 3500,
                pedestal_g1_mean: 14000,
                pedestal_g2_mean: 15000,
                pedestal_rms_g0_mean: 30,
                pedestal_rms_g1_mean: 11,
                pedestal_rms_g2_mean: 10,
                masked_pixels: 80
            },
            {
                module_number: 2,
                storage_cell_number: 0,
                pedestal_g0_mean: 3500,
                pedestal_g1_mean: 14000,
                pedestal_g2_mean: 15000,
                pedestal_rms_g0_mean: 30,
                pedestal_rms_g1_mean: 11,
                pedestal_rms_g2_mean: 10,
                masked_pixels: 80
            },
            {
                module_number: 3,
                storage_cell_number: 0,
                pedestal_g0_mean: 3500,
                pedestal_g1_mean: 14000,
                pedestal_g2_mean: 15000,
                pedestal_rms_g0_mean: 30,
                pedestal_rms_g1_mean: 11,
                pedestal_rms_g2_mean: 10,
                masked_pixels: 80
            },
            {
                module_number: 4,
                storage_cell_number: 0,
                pedestal_g0_mean: 3500,
                pedestal_g1_mean: 14000,
                pedestal_g2_mean: 15000,
                pedestal_rms_g0_mean: 30,
                pedestal_rms_g1_mean: 11,
                pedestal_rms_g2_mean: 10,
                masked_pixels: 80
            },
            {
                module_number: 5,
                storage_cell_number: 0,
                pedestal_g0_mean: 3500,
                pedestal_g1_mean: 14000,
                pedestal_g2_mean: 15000,
                pedestal_rms_g0_mean: 30,
                pedestal_rms_g1_mean: 11,
                pedestal_rms_g2_mean: 10,
                masked_pixels: 80
            },
            {
                module_number: 6,
                storage_cell_number: 0,
                pedestal_g0_mean: 3500,
                pedestal_g1_mean: 14000,
                pedestal_g2_mean: 15000,
                pedestal_rms_g0_mean: 30,
                pedestal_rms_g1_mean: 11,
                pedestal_rms_g2_mean: 10,
                masked_pixels: 80
            },
            {
                module_number: 7,
                storage_cell_number: 0,
                pedestal_g0_mean: 3500,
                pedestal_g1_mean: 14000,
                pedestal_g2_mean: 15000,
                pedestal_rms_g0_mean: 30,
                pedestal_rms_g1_mean: 11,
                pedestal_rms_g2_mean: 10,
                masked_pixels: 80
            }
        ],
        connection_error : true
    }

    getValues() {
        fetch(this.addr + '/detector/calibration')
            .then(handleErrors)
            .then(res => res.json())
            .then(data => this.setState({calib: data, connection_error: false}))
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

    columns = [
        { field: 'module_number', type: 'number', headerName: 'Module' },
        { field: 'storage_cell_number', type: 'number', headerName: 'SC'},
        { field: 'masked_pixels', headerName: 'Masked pxls', type: 'number'},
        { field: 'pedestal_g0_mean', headerName: 'Pedestal G0', type: 'number'},
        { field: 'pedestal_g1_mean', headerName: 'Pedestal G1', type: 'number'},
        { field: 'pedestal_g2_mean', headerName: 'Pedestal G2', type: 'number'}
    ];

    render() {
        return <Paper style={{textAlign: 'center'}} sx={{ height: 527, width: '100%' }}>
            <DataGrid
                getRowId={(row) => Number(row.module_number) * 64 + Number(row.storage_cell_number)}
                rows={this.state.calib}
                columns={this.columns}
                pageSize={8}
                rowsPerPageOptions={[8]}
            />
        </Paper>

    }
}

export default Calibration;
