SHELL:=/bin/bash

picalc:	picalc.cpp
	#sudo apt-get install libmpfr-dev
	g++ -std=c++11 -o picalc $< -lmpfr -lpthread

test:
	time ./picalc 1000000
