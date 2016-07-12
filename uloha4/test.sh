#!/bin/bash

for name in `ls inputs/*.in`; do
	./run.sh $name
done
