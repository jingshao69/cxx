SHELL:=/bin/bash

picalc:	picalc.cpp
	./install_mpfr.sh
	g++ -std=c++11 -o picalc $< -lmpfr -lpthread

test:
	time ./picalc 1000000

clean:
	rm -f picalc
