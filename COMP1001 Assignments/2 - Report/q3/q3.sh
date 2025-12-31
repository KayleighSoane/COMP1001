#!/bin/bash

set -e

echo "Running q3.sh script..."
echo ""

input=64
threads=4

until [ "$input" -eq 4096 ]; do

	gcc q3.c -fopenmp -o q3exe

	./q3exe "$input" "$threads"

	input=$(( input * 2 ))
done
 
echo "q3exe complete"
echo "q3.sh ended"
