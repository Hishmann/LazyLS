#!/bin/bash

TARGET_PID=${1:-$$}
SESSION_NAME="monitor_session"
CPP_FILE="temp2.cpp"
EXE_FILE="temp"

# 1. Compile the C++ renderer
g++ $CPP_FILE -o $EXE_FILE

tmux new-session -d -s "simple_monitor"

PID=$(tmux display-message -p "#{pane_pid}")

tmux split-window -v "./$EXE_FILE $PID"

tmux attach-session -t "simple_monitor"

# tmux kill-session | Kills all the windows