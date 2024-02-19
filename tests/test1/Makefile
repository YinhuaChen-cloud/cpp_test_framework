.PHONY: test1
test1: test1.a

test1.a: test1.cpp
	@echo "In test1, HEADERS = $(HEADERS)"
	g++ -c -I$(HEADERS) test1.cpp
	ar rcs test1.a test1.o

.PHONY: clean
clean: 
	-@rm -f *.o a.out main test *.a


