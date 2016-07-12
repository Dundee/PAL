#!/bin/bash
for i in $(seq 1 1 100)
do
   cat diamant.txt | ./main > /dev/null
done
