#!/bin/bash

echo -e '\n\033[0;36m   Utilization \033[0m\n'

cat $HLS_DIR/syn/report/$1_csynth.rpt |grep -A23 "Utilization Estimates" |tail -n21

echo -e '\n\033[0;36m   Timing \033[0m\n'

cat $HLS_DIR/syn/report/$1_csynth.rpt |grep -A8 "Performance Estimates" |tail -n5

if [ -e $CURR_DIR/vivado_hls.log ]; then
  LOG_FILE=$CURR_DIR/vivado_hls.log
else
  LOG_FILE=$CURR_DIR/vitis_hls.log
fi
if grep "\[SCHED 204-68\]" $LOG_FILE; then
  echo -e '\n\033[0;31m   II Violations: \033[0m'`cat $LOG_FILE | grep "\[SCHED 204-68\]" | wc -l` '\n'
  cat $LOG_FILE | grep "\[SCHED 204-68\]"
  exit 1
else
  echo -e '\n\033[0;36m   No II violations \033[0m\n'
  exit 0
fi
