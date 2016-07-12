#!/bin/bash
for i in $(seq 1 1 100)
do
   cat diamant.txt | ./main2 > /dev/null
done
