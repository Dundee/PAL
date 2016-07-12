#!/bin/bash

file=$1
reffile=`echo $1 | sed "s/\.in$/\.out/"`
comp_in=`echo $1 | sed "s/inputs\//tmp-/"`
comp_out=`echo $reffile | sed "s/inputs\//tmp-/"`
#java -jar ./Minim_ln__kostra.jar < $file | head -2 > $comp_in
./main < $file | head -2 > $comp_in
cat "$reffile" | head -2 > $comp_out
diff -Naur $comp_in $comp_out
rm tmp-*
