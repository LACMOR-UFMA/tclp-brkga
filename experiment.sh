#!/bin/bash

threads="8 16 24 32"
instances="AC AL AM AP BA CE ES GOeDF MA MG MS MT PA PB PE PI PR RJ RN RO RR RS SC SE SP TO"

for instance in ${instances[@]}; do
  for num_thread in ${threads[@]}; do
    out_file="results/${instance}_${num_thread}.out"
    echo $out_file
    echo "pop: 100; pe: 0.3; pm: 0.2; generations: 200; c1: 1; c2: 50; c3: 1" >> $out_file
    echo "Instance;FO;Time" >> $out_file
    for i in $(seq 1 10); do
      ./tclp-brkga "instances/${instance}.txt" 1 50 1 $i $num_thread >> $out_file
    done
  done
done
