
import React, {Component} from 'react';

import TableContainer from '@mui/material/TableContainer';
import Table from '@mui/material/Table';
import Paper from '@mui/material/Paper';
import TableBody from '@mui/material/TableBody';
import TableRow from '@mui/material/TableRow';
import TableCell from '@mui/material/TableCell';

class DataTable extends Component {
  render() {
     return <TableContainer component={Paper} style={{width: 650, marginLeft: "auto", marginRight: "auto"}}>
      <Table size="small" aria-label="simple table">
          <TableBody>
            {this.props.data?.map((p) => (
            <TableRow>
              <TableCell component="th" scope="row"> {p.desc} </TableCell>
              <TableCell align="right">{p.value}</TableCell>
            </TableRow>
            ))}
          </TableBody>
      </Table>
    </TableContainer>
  }
}

export default DataTable;
