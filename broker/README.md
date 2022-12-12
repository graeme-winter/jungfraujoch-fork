# HTTP Broker

## REST interface

Broker exposes HTTP interface to control Jungfraujoch.
Information of outcome of the procedure is taken from HTTP code, so HTTP client needs to ensure that 
200 code return was provided.


### GET /

Access frontend, same as /frontend

### POST /detector/initialize

Initialize the detector. Needed in two situations: (a) at the beginning of operation, (b) in case of ERROR state. 
Includes pedestal (dark current) measurement.

No input.

No output.

### POST /detector/pedestal

Measure detector dark current.

No input.

No output.


### POST /detector/start

Start measurement

Input JSON with the following fields (given the properties of the JSON parser, absent fields are assigned default value):

| Parameter name       | Allowed values                                        | Default               | Description                                                     |
|----------------------|-------------------------------------------------------|-----------------------|-----------------------------------------------------------------|
| images_per_trigger   | int >= 0                                              | 0                     | Number of images per trigger                                    |
| ntrigger             | int >= 1                                              | 1                     | Number of triggers                                              |
|                      |                                                       |                       |                                                                 |
| only one of the two: |                                                       |                       |                                                                 | 
| summation            | int >= 1                                              | -                     | Frame summation factor;                                         |
| image_time_us        | int >= 500                                            | -                     | Image time in microseconds; must be multiple of base frame rate |
|                      |                                                       |                       |                                                                 |
| beam_x_pxl           | float                                                 | 0                     | Beam center location in pixel                                   |
| beam_y_pxl           | float                                                 | 0                     | Beam center location in pixel                                   |
| detector_distance_mm | float >= 1                                            | 100 mm                | Detector distance to sample in mm                               |
| photon_energy_keV    | float 0.1 - 25                                        | -                     | Photon energy in keV                                            |
| scattering vector    | array of 3 floats                                     | [0, 0, 1]             | Scattering vector for                                           |
|                      |                                                       |                       |                                                                 |
| file_prefix          | string                                                | ""                    | Name of file prefix (no file written if empty string)           |
| data_file_count      | int >= 1                                              | 1                     | Number of data files in round-robin matter                      |
|                      |                                                       |                       |                                                                 |
| compression          | BSHUF_LZ4, BSHUF_ZSTD, BSHUF_ZSTD_RLE, NO_COMPRESSION | BSHUF_LZ4             | Compression algorithm                                           |
|                      |                                                       |                       |                                                                 |
| sample_name          | string                                                | ""                    | Name of sample (for merging, etc.)                              |
| unit_cell            | object with a, b, c, alpha, beta, gamma               | no unit cell provided | Unit cell (optional)                                            |
| space_group_number   | int 0-230                                             | 0                     | Space group number (optional; if 0 => no space group provided)  |

No output

### POST /detector/stop

Block till the last measurement is done.

No input.

No output.

### POST /detector/cancel

Cancel current measurement. It is expected to graciously finish all running procedures.

No input.

No output.

### POST /detector/trigger

Software trigger.

No input.

No output.