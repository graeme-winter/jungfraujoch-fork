#!/bin/bash

HLS_IP_DIR=$PWD/action/ip

rm -rf action

# Create directories
mkdir -p action/hw/tcl
mkdir -p action/hw/hdl
mkdir -p action/ip
mkdir -p action/scripts

for i in hls/psi_ch*.zip
do
  x=${i##hls/psi_ch_}
  y=${x%%.zip}
  unzip $i -d action/ip/$y
done


# Create (empty) Makefile
cat <<EOF > action/Makefile
hw:
	echo -n ""
clean:
	echo -n ""
EOF

cat <<EOF > ${SRC_DIR}/oc-accel/snap_env.sh
export ACTION_ROOT=$PWD/action
export TIMING_LABLIMIT="-100"
export OCSE_ROOT=$SRC_DIR/ocse
EOF

cp ${SRC_DIR}/hdl/*.v ${SRC_DIR}/hdl/*.vhd action/hw/hdl

# Update action type and release level based on Definitions.h
ACTION_TYPE=`grep "#define ACTION_TYPE" ${SRC_DIR}/../common/Definitions.h | awk -F"0x" '{print $2}'`
RELEASE_LEVEL=`grep "#define RELEASE_LEVEL" ${SRC_DIR}/../common/Definitions.h | awk -F"0x" '{print $2}'`
MAX_MODULES_FPGA=`grep "\#define MAX_MODULES_FPGA" ${SRC_DIR}/hls/hls_jfjoch.h  |tr -s " " |cut -f3 -d" "`
MODULES_INTERNAL_PACKET_GEN=`grep "\#define MODULES_INTERNAL_PACKET_GEN" ${SRC_DIR}/hls/hls_jfjoch.h  |tr -s " " |cut -f3 -d" "`
GIT_SHA1=`git describe --match=NeVeRmAtCh --always --abbrev=8`

SRC="define ACTION_TYPE 32'h.*"
DST="define ACTION_TYPE 32'h${ACTION_TYPE}"
sed -i "s/$SRC/$DST/" action/hw/hdl/action_config.v

SRC="define RELEASE_LEVEL 32'h.*"
DST="define RELEASE_LEVEL 32'h${RELEASE_LEVEL}"
sed -i "s/$SRC/$DST/" action/hw/hdl/action_config.v

SRC="define GIT_SHA1 32'h.*"
DST="define GIT_SHA1 32'h${GIT_SHA1}"
sed -i "s/$SRC/$DST/" action/hw/hdl/action_config.v

SRC="define MAX_MODULES_FPGA 32'd.*"
DST="define MAX_MODULES_FPGA 32'd${MAX_MODULES_FPGA}"
sed -i "s/$SRC/$DST/" action/hw/hdl/action_config.v

SRC="define MODULES_INTERNAL_PACKET_GEN 32'd.*"
DST="define MODULES_INTERNAL_PACKET_GEN 32'd${MODULES_INTERNAL_PACKET_GEN}"
sed -i "s/$SRC/$DST/" action/hw/hdl/action_config.v

# Run vivado to create project
cp ${SRC_DIR}/scripts/bd_opencapi.tcl action/hw/tcl/bd_opencapi.tcl
sed -i "s,@SRC_DIR@,${SRC_DIR}," action/hw/tcl/bd_opencapi.tcl
sed -i "s,@HLS_IP_DIR@,${HLS_IP_DIR}," action/hw/tcl/bd_opencapi.tcl

cp ${SRC_DIR}/scripts/synth_and_impl.tcl action/scripts/synth_and_impl.tcl
sed -i "s,@GIT_SHA1@,${GIT_SHA1}," action/scripts/synth_and_impl.tcl
sed -i "s,@RELEASE_LEVEL@,${RELEASE_LEVEL}," action/scripts/synth_and_impl.tcl

cd $SRC_DIR/oc-accel || exit 1

if [ -x "$(command -v xrun)" ]; then
	cp $SRC_DIR/scripts/OC-${FPGACARD:=AD9H3}.hls_rx100G.xcelium.defconfig .snap_config
else
  echo "WARNING! No simulator detected"
	cp $SRC_DIR/scripts/OC-${FPGACARD:=AD9H3}.hls_rx100G.nosim.defconfig .snap_config
fi

make oldconfig