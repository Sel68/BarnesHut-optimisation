#!/bin/bash

g++ random_coordinates.cpp -o gen
g++ naive_nbody.cpp -o s2
g++ main.cpp -o s3

echo "Number of points, naive, BarnesHut" > results.csv

points=(10 50 100 200 500 1000 2000 3000 4000 5000 7500 10000)

for p in "${points[@]}"; do
    echo "Testing $p points..."
    echo $p | ./gen
    
    # awk '{print $3}' selects the 3rd word in the output line
    t2=$(./s2 | grep "Execution time:" | awk '{print $3}')
    t3=$(./s3 | grep "Execution time:" | awk '{print $3}')
    
    echo "$p, $t2, $t3" >> results.csv
    echo "Completed for $p points."
done