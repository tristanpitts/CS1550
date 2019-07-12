#!/bin/bash

for (( i=1; i<=100; i++ ))
do
	java vmsim -n ${i} -a fifo traces/mcf.trace | grep fault | cut -d \: -f 2 >> mcf.txt
done
