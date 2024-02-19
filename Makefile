export HEADERS=$(shell pwd)/headers

test: main
	RUST_BACKTRACE=1 LD_LIBRARY_PATH=$(shell pwd) ./main
	
main: main.cpp test.a
	g++ -I$(HEADERS) main.cpp test.a -L. -lpath_reduction -o main

.PHONY: test.a
test.a: 
	-@rm -f test.a
	make -C tests archive
	ar rcT test.a tests/*.a 

.PHONY: clean
clean: 
	-@rm -f *.o a.out test main *.a
	make -C tests clean


# g++ test.cpp -L. -lpath_reduction -o test
# RUST_BACKTRACE=1 LD_LIBRARY_PATH=$(shell pwd) ./test

