#!/bin/bash

threads="8"
instances="TO MS PE RJ ES"

for instance in ${instances[@]}; do
  for num_thread in ${threads[@]}; do
    out_file="results_igor/${instance}_${num_thread}.out"
    echo $out_file
    echo "pop: 20; pe: 0.3; pm: 0.2; generations: 100; c1: 20; c2: 20; c3: 10" >> $out_file
    echo "Instance;FO;Time" >> $out_file
    for i in $(seq 1 10); do
      ./tclp-brkga "instances/${instance}.txt" 20 20 10 $i $num_thread >> $out_file
    done
  done
done
