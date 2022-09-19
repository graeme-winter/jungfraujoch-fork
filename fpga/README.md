# FPGA Interface

## Content of directories

CPU Part:

* `receiver` Application that controls receiving process, applies image analysis and compression and sends data via ZeroMQ
* `host` Host side API to access the FPGA board, implements high level operations
* `pcie_driver` Linux kernel driver for PCIe version of the FPGA board (under development)

FPGA part:

* `scripts` Scripts for FPGA synthesis
* `xdc` Constraints for FPGA
* `hdl` FPGA design parts developed in Verilog
* `hls` FPGA design parts developed in C++ with high-level synthesis

Dependencies:

* `include` External (Xilinx) headers for high-level synthesis code
* `oc-accel` OC-Accel from IBM/OpenCAPI Consortium
* `ocse` Simulation environment for OpenCAPI


## HLS compilation
Make HLS routines:
```
mkdir build
cd build
cmake3 ..
make hls
```

## Synthesis
Create OpenCAPI bitstream:
```
mkdir build
cd build
cmake3 ..
make action_opencapi
```

Create PCIe bitstream:
```
mkdir build
cd build
cmake3 ..
make action_pcie
```

## Simulation

To build model and run simulation with OCSE:
 ```
source <VIVADO_PATH>/settings64.sh
export CDS_INST_DIR=<path to Xcelium installation directory>
export IES_LIBS=<path to Vivado libraries compiled for Xcelium>
git submodule update --init
mkdir -p build
cmake3 ..
make action_model jfjoch_action_test
cd ../fpga/oc-accel/hardware/sim
time ./run_sim -quiet -app ../../../build/fpga/host/jfjoch_action_test <git repo dir> <number of images def=2> <number of modules def=1> > sim1.out
 ```

To visualize results of simulation:
```
cd fpga
simvision -wave -input scripts/waveforms.svwf oc-accel/hardware/sim/xcelium/latest/capiWave.shm
```

## Hardware verification

To test that FPGA board is working properly without access to a JUNGFRAU detector, you can use `jfjoch_action_test` tool.

## FPGA reference
### Register map OpenCAPI
FPGA setup can be done via 32-bit registers - register map is consistent between PCIe and OpenCAPI designs:

| Address           | Bits | Meaning                                                               | Mode | Notes                             |
|-------------------|------|-----------------------------------------------------------------------|:-----|-----------------------------------|
| 0x0000            | 32   | Action Control Register                                               |      |                                   |
|                   |      | Bit 0 - Action start                                                  | R/W  |                                   |
|                   |      | Bit 1 - Action idle                                                   | R    |                                   |
|                   |      | Bit 2 - Action cancel                                                 | R/W  | cleared on reset or action start  |
|                   |      | Bit 8 - 100 GbE RX aligned                                            | R    |                                   |
|                   |      | Bit 9 - 100 GbE RX status OK                                          | R    |                                   |
|                   |      | Bit 10 - Datamover MM2S error (OpenCAPI only)                         | R    | cleared on reset or action start  | 
|                   |      | Bit 11 - Datamover S2MM error (OpenCAPI only)                         | R    | cleared on reset or action start  |
|                   |      | Bit 12 - HBM catastrophic temp. error                                 | R    |                                   |
|                   |      | Bit 13 - HBM catastrophic temp. error                                 | R    | cleared on reset or action start  |
|                   |      | Bit 14 - HBM setup complete                                           | R    |                                   |
|                   |      | Bit 15 - 100 GbE RX packet bad FCS                                    | R    | cleared on reset or action start  |
|                   |      | Bit 16 - AXI Mailbox interrupt 0                                      | R    |                                   |
|                   |      | Bit 17 - AXI Mailbox interrupt 1                                      | R    |                                   |
|                   |      | Bit 18 - QSFP-DD module present                                       | R    |                                   |
|                   |      | Bits 24-27 - Various errors in host memory writer                     | R    | cleared on reset or action start  |
| 0x0004            | 32   | Reserved                                                              | -    |                                   |
| 0x0008            | 32   | Set user LED G0 (yellow or green), 1 is on, 0 is off                  | R/W  |                                   |
| 0x000C            | 32   | Action GIT SHA1                                                       | R    |                                   |
| 0x0010            | 32   | Action Type                                                           | R    |                                   |
| 0x0014            | 32   | Action Release Level                                                  | R    |                                   |
| 0x0018            | 32   | HBM current temperature                                               | R    |                                   |
| 0x001C            | 32   | HBM max. temperature                                                  | R    | reset on action start             |
| 0x0020            | 32   | Max. number supported detector modules                                | R    | constant                          |
| 0x0024            | 32   | Number of modules in internal packet generator memory                 | R    | constant                          |
| 0x0028            | 64   | Pipeline stalls before writing to host memory                         | R    | reset on action start             |
| 0x0030            | 64   | Pipeline stalls before accessing HBM                                  | R    | reset on action start             |
| 0x0038            | 32   | FIFO status (see action_config.v for details)                         | R/W  |                                   |
| 0x003C            | 32   | Reserved                                                              | -    |                                   |
| 0x0040            | 64   | Packets processed by the action                                       | R/W  | cleared on reset or action start  |
| 0x0048            | 64   | Valid ethernet packets                                                | R/W  | cleared on reset                  |
| 0x0050            | 64   | Valid ICMP packets                                                    | R/W  | cleared on reset                  |
| 0x0058            | 64   | Valid UDP packets                                                     | R/W  | cleared on reset                  |
| 0x0060            | 64   | MAC address of FPGA card                                              | R/W  | network byte order                |
| 0x0068            | 32   | IPv4 address of FPGA card                                             | R/W  | network byte order                |
| 0x006C            | 32   | Number of detector modules                                            | R/W  |                                   |
| 0x0070            | 32   | Data collection mode                                                  | R/W  |                                   |
| 0x0074            | 32   | One over energy in keV (in fixed-point:12 int. + 24 frac. bit format) | R/W  |                                   |
| 0x0078            | 32   | Number of frames to be generated by internal packet generator         | R/W  |                                   |
| 0x007C            | 32   | Number of storage cells                                               | R/W  |                                   |
|                   |      |                                                                       |      |                                   |
| 0x10000 - 0x1FFFF |      | Alpha-Data Board Control Interface (requires license)                 |      | See Alpha Data documentation      |
| 0x30000 - 0x3FFFF |      | AXI Mailbox for Work Request / Work Completion                        |      | See Xilinx PG114 for register map |
| 0x40000 - 0x4FFFF |      | CMAC 100G                                                             |      | See Xilinx PG203 for register map |
| 0x60000 - 0x60FFF | 64   | Input calibration memory addresses block RAM                          |      |                                   |

### Register map PCI Express
FPGA setup can be done via 32-bit registers - register map is consistent between PCIe and OpenCAPI designs:

| Address           | Bits | Meaning                                                               | Mode | Notes                             |
|-------------------|------|-----------------------------------------------------------------------|:-----|-----------------------------------|
| 0x00000 - 0x0FFFF |      | Reserved for internal memory of MicroBlaze                            |      |                                   |
| 0x10000           | 32   | Action Control Register                                               |      |                                   |
|                   |      | Bit 0 - Action start                                                  | R/W  |                                   |
|                   |      | Bit 1 - Action idle                                                   | R    |                                   |
|                   |      | Bit 2 - Action cancel                                                 | R/W  | cleared on reset or action start  |
|                   |      | Bit 8 - 100 GbE RX aligned                                            | R    |                                   |
|                   |      | Bit 9 - 100 GbE RX status OK                                          | R    |                                   |
|                   |      | Bit 10 - Datamover MM2S error (OpenCAPI only)                         | R    | cleared on reset or action start  | 
|                   |      | Bit 11 - Datamover S2MM error (OpenCAPI only)                         | R    | cleared on reset or action start  |
|                   |      | Bit 12 - HBM catastrophic temp. error                                 | R    |                                   |
|                   |      | Bit 13 - HBM catastrophic temp. error                                 | R    | cleared on reset or action start  |
|                   |      | Bit 14 - HBM setup complete                                           | R    |                                   |
|                   |      | Bit 15 - 100 GbE RX packet bad FCS                                    | R    | cleared on reset or action start  |
|                   |      | Bit 16 - AXI Mailbox interrupt 0                                      | R    |                                   |
|                   |      | Bit 17 - AXI Mailbox interrupt 1                                      | R    |                                   |
|                   |      | Bit 18 - QSFP-DD module present                                       | R    |                                   |
|                   |      | Bits 24-27 - Various errors in host memory writer                     | R    | cleared on reset or action start  |
| 0x10004           | 32   | Reserved                                                              | -    |                                   |
| 0x10008           | 32   | Set user LED G0 (yellow or green), 1 is on, 0 is off                  | R/W  |                                   |
| 0x1000C           | 32   | Action GIT SHA1                                                       | R    |                                   |
| 0x10010           | 32   | Action Type                                                           | R    |                                   |
| 0x10014           | 32   | Action Release Level                                                  | R    |                                   |
| 0x10018           | 32   | HBM current temperature                                               | R    |                                   |
| 0x1001C           | 32   | HBM max. temperature                                                  | R    | reset on action start             |
| 0x10020           | 32   | Max. number supported detector modules                                | R    | constant                          |
| 0x10024           | 32   | Number of modules in internal packet generator memory                 | R    | constant                          |
| 0x10028           | 64   | Pipeline stalls before writing to host memory                         | R    | reset on action start             |
| 0x10030           | 64   | Pipeline stalls before accessing HBM                                  | R    | reset on action start             |
| 0x10038           | 32   | FIFO status (see action_config.v for details)                         | R/W  |                                   |
| 0x1003C           | 32   | Reserved                                                              | -    |                                   |
| 0x10040           | 64   | Packets processed by the action                                       | R/W  | cleared on reset or action start  |
| 0x10048           | 64   | Valid ethernet packets                                                | R/W  | cleared on reset                  |
| 0x10050           | 64   | Valid ICMP packets                                                    | R/W  | cleared on reset                  |
| 0x10058           | 64   | Valid UDP packets                                                     | R/W  | cleared on reset                  |
| 0x10060           | 64   | MAC address of FPGA card                                              | R/W  | network byte order                |
| 0x10068           | 32   | IPv4 address of FPGA card                                             | R/W  | network byte order                |
| 0x1006C           | 32   | Number of detector modules                                            | R/W  |                                   |
| 0x10070           | 32   | Data collection mode                                                  | R/W  |                                   |
| 0x10074           | 32   | One over energy in keV (in fixed-point:12 int. + 24 frac. bit format) | R/W  |                                   |
| 0x10078           | 32   | Number of frames to be generated by internal packet generator         | R/W  |                                   |
| 0x1007C           | 32   | Number of storage cells                                               | R/W  |                                   |
|                   |      |                                                                       |      |                                   |
| 0x20000 - 0x2FFFF |      | QuadSPI flash                                                         |      | See Xilinx PG153 for register map |
| 0x30000 - 0x3FFFF |      | AXI Mailbox for Work Request / Work Completion                        |      | See Xilinx PG114 for register map |
| 0x40000 - 0x4FFFF |      | CMAC 100G                                                             |      | See Xilinx PG203 for register map |
| 0x60000 - 0x60FFF | 64   | Input calibration memory addresses block RAM                          |      |                                   |
| 0x70000 - 0x7FFFF |      | AXI Firewall                                                          |      | See Xilinx PG293 for register map |
| 0x80000 - 0x8FFFF |      | Interrupt controller                                                  |      | See Xilinx PG099 for register map |
| 0x70000 - 0x7FFFF |      | PCIe DMA control                                                      |      | See Xilinx PG195 for register map |
| 0xC0000 - 0xFFFFF |      | Xilinx Card Management Solution Subsystem management subsystem        |      | See Xilinx PG348 for register map |

### AXI Mailbox

AXI mailbox is used to send work request from host to action, and receive work completions. Messages are always multiple of 128-bit. See Xilinx PG114 on how to operate AXI Mailbox.

Work request has the following structure:

| Bit start | Bit end | Meaning                                            |
|-----------|---------|----------------------------------------------------|
| 0         | 31      | Work request ID (handle)                           |
| 32        | 95      | Address (Virt: OpenCAPI, DMA: PCIe)                |
| 96        | 127     | Includes parity bit, so bits 0-127 are even parity |

Work completion has the following structure:

| Bit start | Bit end | Meaning                                                                                                      |
|-----------|---------|--------------------------------------------------------------------------------------------------------------|
| 0         | 31      | Work request ID (handle)                                                                                     |
| 32        | 39      | Module number                                                                                                |
| 40        | 40      | All packets for the module arrived OK                                                                        |
| 41        | 41      | Trigger signal high                                                                                          |
| 42        | 62      | Reserved                                                                                                     |
| 63        | 63      | Parity bit - bits 0-127 are even parity                                                                      |
| 64        | 127     | Frame number                                                                                                 |
| 128       | 159     | JF debug                                                                                                     |
| 160       | 191     | JF Timestamp (low 32-bit)                                                                                    |
| 192       | 255     | Bunch ID                                                                                                     |
| 256       | 383     | Optional packet mask (1 bit per packet: 0 packet missing, 1 packet arrived); transmitted only if bit 40 is 0 |
