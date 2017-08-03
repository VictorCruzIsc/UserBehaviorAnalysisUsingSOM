#!/bin/bash

# EXECUTION: ./execute_som.sh 50 4 5 9

execution_parameters="$1 200 500 150 3 R/r_trained_som/trained_lattice_6_$1_25.txt 6 R/r_trained_som/trained_lattice_$2_$1_25.txt $2 R/r_trained_som/trained_lattice_$3_$1_25.txt $3 R/r_trained_som/trained_lattice_$4_$1_25.txt $4"
echo $execution_parameters

./evaluate_module_automated $execution_parameters