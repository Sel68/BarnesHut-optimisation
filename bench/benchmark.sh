#!/bin/bash

SRC_DIR="../"

g++ "$SRC_DIR/random_coordinates.cpp" -o gen
g++ "$SRC_DIR/naive_nbody.cpp" -o s2
g++ "$SRC_DIR/main.cpp" -o s3

echo "Number of points, naive, BarnesHut" > results.csv

points=(10 50 100 200 500 1000 2000 3000 4000 5000 7500 10000)

for p in "${points[@]}"; do
    echo "Testing $p points..."
    
    # Binaries (gen, s2, s3) are created in the CURRENT folder, so ./ works
    echo $p | ./gen
    
    t2=$(./s2 | grep "Execution time:" | awk '{print $3}')
    t3=$(./s3 | grep "Execution time:" | awk '{print $3}')
    
    echo "$p, $t2, $t3" >> results.csv
    echo "Completed for $p points."
done