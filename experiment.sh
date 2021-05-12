#!/bin/bash

threads=(1 2 4 8)
instances="BA CE GOeDF MG MS MT PA PE PI PR RJ RS SC SP TO"

for instance in ${instances[@]}; do
    for num_thread in ${threads[@]}; do
		out_file="results/${instance}_${num_thread}.out"
        echo $out_file
		echo "Instance;FO;Time" >> $out_file
		for i in $(seq 1 10); do
            ./fast-tclp-brkga "instances/${instance}.txt" 20 10 5 $i $num_thread >> $out_file
		done
    done
done
