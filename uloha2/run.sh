#!/bin/bash
for i in $(seq 1 1 100)
do
   cat input2 | ./main > /dev/null
done
