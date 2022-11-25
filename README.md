# Jungfraujoch

Application to receive data from the JUNGFRAU detector at Swiss Light Source macromolecular crystallography beamlines.

Citation: F. Leonarski, M. Bruckner, C. Lopez-Cuenca, A. Mozzanica, H.-C. Stadler, Z. Matej, A. Castellane, B. Mesnet, J. Wojdyla, B. Schmitt and M. Wang "Jungfraujoch: hardware-accelerated data-acquisition system for kilohertz pixel-array X-ray detectors" (2023), J. Synchrotron Rad., 30, [doi:10.1107/S1600577522010268](https://doi.org/10.1107/S1600577522010268) (in press).

## License

Software components are licensed with GNU Public License version 3. 

Hardware components are licensed with Strongly-reciprocal CERN Open Hardware Licence version 2.

## Hardware requirements
1. JUNGFRAU detector (optimally 4M with 2 kHz enabled read-out boards)
2. IC 922 server equipped with AD9H3 FPGA boards and Tesla T4 GPUs
3. 100G fiber-optic switch between JUNGFRAU and IC 922

## FPGA bitstream
### Dependencies
To build FPGA image you need:
1. Xilinx Vivado 2022.1
2. License for Ultrascale+ 100G core - license is available at no cost from Xilinx, but needs to be seperatly requested, see [Xilinx website](https://www.xilinx.com/products/intellectual-property/cmac_usplus.html).
3. License for Alpha Data 9H3 Reference Design - necessary to include Board Control Interface in the FPGA design, to allow on-line reading of board temperature, current and voltage
4. Cadence Xcelium 20.09 for simulation

Instructions see [here](fpga/README.md)

## Software

### Dependencies
1. C++17 compiler and C++17 standard library (NOT provided by default RHEL 7 installation, need to install Developer Tools, tested with `devtools-11`)
2. CMake version 3.18 or newer + GNU make tool 
3. HDF5 library version 1.10 or newer
4. ZeroMQ library
5. Google Remote Procedure Call (gRPC) - see notes below
6. CUDA compiler version 11 or newer for GPU code (spot finding, indexing, pedestal calculation)

Additional dependencies: SLS Detector Package, OC-Accel, tinycbor (Intel) and Zstandard (Facebook) are provided as GIT submodules. 

Directly included in the repository:
* JSON parser/writer from N. Lohmann - see [github.com/nlohmann/json](https://github.com/nlohmann/json)
* Catch testing library - see [github.com/catchorg/Catch2](https://github.com/catchorg/Catch2)
* Xilinx arbitrary precision arithmetic headers - see [github.com/Xilinx/HLS_arbitrary_Precision_Types](https://github.com/Xilinx/HLS_arbitrary_Precision_Types)
* Bitshuffle filter from K. Masui - see [github.com/kiyo-masui/bitshuffle](https://github.com/kiyo-masui/bitshuffle)
* LZ4 compression by Y.Collet - see [github.com/lz4/lz4](https://github.com/lz4/lz4)
* Xgandalf indexer - see [stash.desy.de/users/gevorkov/repos/xgandalf](https://stash.desy.de/users/gevorkov/repos/xgandalf/browse)
* GEMMI library to handle space group naming / numbering - see [github.com/project-gemmi/gemmi](https://github.com/project-gemmi/gemmi)
* Spdlog logging library - see [github.com/gabime/spdlog](https://github.com/gabime/spdlog)
For license check LICENSE file in respective directory

### Software components

* `jfjoch_receiver` in `fpga/host` - the main component of Jungfraujoch, used to receive data with FPGA smart network cards, compress images and send them over ZeroMQ
* `jfjoch_broker` in `broker` - gRPC based broker, controlling all Jungfraujoch components
* `jfjoch_writer` in `writer` - HDF5 writer
* `jfjoch_xgandalf` in `indexing` - wrapper of Xgandalf indexing algorithm compatible with Jungfraujoch
* `jfjoch_detector` in `detector` - wrapper over detector control

Configuration for the modules is given in configuration files present in `etc` directory.

### Installing dependencies on RHEL8 / POWER9

It is highly recommended to use RHEL 8.4, which provides recent `cmake` version.
Code is routinely tested for compilation with GCC 10.0 and Clang 11.0, both compilers can be used. 

For RHEL8 on POWER9 system it is best to install `HDF5` from source, as there is no proper rpm package at the moment:
```
wget https://support.hdfgroup.org/ftp/HDF5/releases/hdf5-1.10/hdf5-1.10.7/src/hdf5-1.10.7.tar.gz
tar xvzf hdf5-1.10.7.tar.gz
cd hdf5-1.10.7
./configure --prefix=/opt/hdf5-1.10.7
make -j40
sudo make install
```

### Installation of gRPC
By default, gRPC will be cloned and compiled automatically, when compiling Jungfraujoch with `cmake`, however this significantly increases installation time.
Alternative is to compile gRPC beforehand. This is very useful especially, when one expects to compile Jungfraujoch multiple times on the same machine.
It is recommended to install gRPC in own directory (not in system path), as there is no easy way to uninstall it.
To compile gRPC:
```
git clone https://github.com/grpc/grpc
cd grpc
git checkout v1.41.1
git submodule update --init
mkdir build
cd build
cmake -DgRPC_ZLIB_PROVIDER="package" -DCMAKE_INSTALL_PREFIX=/opt/grpc ..
make
sudo make install
```
Currently, handling paths for gRPC and its dependencies gives sometimes troubles and few environmental variables need to be setup before compilation (assuming gRPC installed in /opt/grpc):
```
export CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH:/opt/grpc/lib/cmake:/opt/grpc/lib64/cmake
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/grpc/lib:/opt/grpc/lib64
export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/opt/grpc/lib/pkgconfig:/opt/grpc/lib64/pkgconfig
```

### Compilation
Use the following commands (use `cmake` instead of `cmake3` in non-RHEL systems):

```
git submodule update --init
mkdir build
cd build
cmake3 ..
make jfjoch
```

## Tests

To enable compiling test routines use parameter `-DJFJOCH_COMPILE_TESTS=ON` for `cmake`. 
Automated test routine is then accessible as `tests/CatchTest`. There are also benchmark routines:

* `CompressionBenchmark` to measure compression bandwidth (single threaded)
* `HDF5DatasetWriteTest` to measure HDF5 dataset writing speed (single threaded)
* `DataAnalysisPerfTest` to measure data analysis performance (single threaded)
* `PedestalPerfTest` to measure pedestal calculation performance

In addition, tests are executed to verify that datasets written by Jungfraujoch are readable with XDS Durin plugin and CrystFEL. Input files for these programs are placed in `xds_durin` and `crystfel` folders. See `.gitlab-ci.yml` for details.

## Building web user interface
### Dependencies
For web user interface:
1. Node.js
2. Web server, e.g. Apache httpd 
3. Web grpc

### Building
To build web interface:
```
cd frontend_ui
npm build
```

To install on RHEL 7 and Apache:
```
cd build
sudo cp -r * /var/www/html/
```
