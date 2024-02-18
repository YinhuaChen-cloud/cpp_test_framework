
test: main
	./main

	# g++ test.cpp -L. -lpath_reduction -o test
	# RUST_BACKTRACE=1 LD_LIBRARY_PATH=$(shell pwd) ./test
	
main: main.cpp
	g++ main.cpp -o main

.PHONY: clean
clean: 
	-@rm -f *.o a.out test main

