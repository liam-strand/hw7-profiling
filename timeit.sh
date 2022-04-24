#!/bin/sh

cd ~/comp40/hw7-profiling/um-code
make

echo -e "\nmidmark:"
/usr/bin/time ./um ../umbin/midmark.um > /dev/null

echo -e "\nSANDmark:"
/usr/bin/time ./um ../umbin/sandmark.umz > /dev/null

echo -e "\nadvent:"
/usr/bin/time ./um ../umbin/advent.umz < ../umbin/advent_sol.txt > /dev/null

echo -e "\nvalgrind:"
valgrind --tool=callgrind --dump-instr=yes ./um ../umbin/midmark.um > /dev/null

make clean
