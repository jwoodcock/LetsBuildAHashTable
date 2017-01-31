#    $ make

PyHash: OurHash.c
	g++ -x c -O3 -o $@ $<

benchmark: OurHash.c benchmark.c
	g++ -x c -O3 -o benchmark benchmark.c
