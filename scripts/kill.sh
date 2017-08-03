#!/bin/bash

# EXECUTION: ./automated_som_test.sh

# This part is used to cancel the program once it finishes
ps | grep "./evaluate" > data.txt

data=$(tail -1 data.txt)
pidModule=$(echo $data | awk '{print $1;}')
kill -9 $(echo $pidModule)

data=$(head -1 data.txt)
pidModule=$(echo $data | awk '{print $1;}')
kill -9 $(echo $pidModule)