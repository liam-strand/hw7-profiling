#!/bin/sh

cd ~/comp40/hw7-profiling/um-code
make

echo -e "\nmidmark:"
/usr/bin/time ./um ../benchmarks/midmark.um > /dev/null

echo -e "\nSANDmark:"
/usr/bin/time ./um ../benchmarks/sandmark.umz > /dev/null

echo -e "\nadvent:"
/usr/bin/time ./um ../benchmarks/advent.umz < ../benchmarks/advent_sol.txt > /dev/null

echo -e "\nvalgrind:"
valgrind --tool=callgrind --dump-instr=yes ./um ../benchmarks/midmark.um > /dev/null

make clean
