#!/usr/bin/bash
set -eou pipefail

TARGET=$1

sudo apt install gdb gdbserver -y

make

tmux split-window -h -p 40 "gdbserver :12345 ./${TARGET}"
tmux select-pane -t left
clear
gdb -q --tui -x debug.gdb
