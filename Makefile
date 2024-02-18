
test: main
	./main

	# g++ test.cpp -L. -lpath_reduction -o test
	# RUST_BACKTRACE=1 LD_LIBRARY_PATH=$(shell pwd) ./test
	
main: main.cpp test.a
	g++ -Itests main.cpp test.a -o main

test.a:
	make -C tests archive
	mv tests/test.a .

.PHONY: clean
clean: 
	-@rm -f *.o a.out test main *.a
	make -C tests clean


