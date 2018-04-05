#!/bin/bash	
make -f makefile
rm results.txt
./summercamp.x 100000 1 >> results.txt
./summercamp.x 100000 2 >> results.txt
./summercamp.x 100000 4 >> results.txt
./summercamp.x 100000 8 >> results.txt
./summercamp.x 100000 14 >> results.txt
./summercamp.x 100000 20 >> results.txt
./summercamp.x 100000 28 >> results.txt
