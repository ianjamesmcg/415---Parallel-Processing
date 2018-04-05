#!/bin/bash	
make -f makefile
rm resultsChunk.txt
./summercamp.x 100000 20 >> resultsChunk.txt
