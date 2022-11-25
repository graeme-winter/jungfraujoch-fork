# Frame serialization

## Choice of serialization method
To communicate between FPGA-equipped receiver system and writers, Jungfraujoch is using binary CBOR encoding with tinycbor library (Intel).
While using ProtoBuf would suit better the design, as all control structures are serialized with ProtoBuf, performance of such solution is not 
good enough - ProtoBuf needs two memory copies and internally does memory allocation with each serialization. N. Lohmann JSON library used in the code for parsing control files
suffers from the same issue - while it offers CBOR support, it is not suited for tight performance constraints. Therefore, tinycbor was chosen.

Choice of CBOR has additional advantage of increased compatibility with commercial DECTRIS detectors, which used JSON and CBOR for image serialization in the stream interface.

## Description of the protocol

Each communications consist of start frame, certain number of image frames and end frame. Start and end frame carry no additional information.

| Field name   | Type            | Description                                                                                     |
|--------------|-----------------|-------------------------------------------------------------------------------------------------|
| type         | text            | allowed values: start / end / image                                                             |
| file_number  | int64           | Number of data file                                                                             |
| image_number | int64           | Number of image in data file                                                                    |
| data         | byte string     | Compressed image in format directly applicable to HDF5 direct chunk writer                      |
| spots        | array of floats | Spot information: x (spot1), y (spot1), intensity in photons (spot1), x (spot2), y (spot2), ... |
