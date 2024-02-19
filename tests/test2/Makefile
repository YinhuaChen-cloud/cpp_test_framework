.PHONY: test2

test2: test2.a

test2.a: test2.cpp
	@echo "In test2, HEADERS = $(HEADERS)"	
	g++ -c -I$(HEADERS) test2.cpp
	ar rcs test2.a test2.o

run: main.c innerloop.c
	gcc -c main.c
	gcc -c innerloop.c
	gcc main.o innerloop.o -o test
	./test

.PHONY: clean
clean: 
	-@rm -f *.o a.out test *.bc *.dot *.ll .*.dot *.txt PUT 
	-@rm -rf output_dir thepath15

