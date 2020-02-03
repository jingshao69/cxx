SRCS= csum_compare.c checksum5.s checksum6.s checksum14.s

all: csum_compare_o csum_compare

csum_compare_o: $(SRCS)
	gcc -march=haswell -O3 -o $@ $^

csum_compare: $(SRCS)
	gcc -march=haswell -o $@ $^

clean:
	rm -f csum_compare csum_compare_o
	

