#!/usr/bin/bash
set -eou pipefail

TARGET=$1
LOCAL_HOST="1234"


make

tmux split-window -h -p 40 "gdbserver :${LOCAL_HOST} ./${TARGET}"
tmux select-pane -t left
clear

gdb -ex "target remote :${LOCAL_HOST}" -q --tui
