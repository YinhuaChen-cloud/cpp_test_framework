# ALL_TESTS=test1 test2 test3
ALL_TESTS=test1

# 使用 foreach 函数添加 _clean
CLEAN_ALL := $(foreach mod,$(ALL_TESTS),$(mod)_clean)

archive: $(ALL_TESTS)
	@echo "In archive"
	ar rcs test.a *.o 


# 循环调用子目录下的Makefile
.PHONY: $(ALL_TESTS)
$(ALL_TESTS):
	@echo "HEADERS = $(HEADERS)"
	make -C $@ $@
	cp $@/$@.o .
	

.PHONY: clean
clean: $(CLEAN_ALL)
	-@rm -f *.o a.out main *.a


.PHONY: $(CLEAN_ALL)
$(CLEAN_ALL):
	# echo $@
	# echo $(shell echo $@ | sed "s/......$$//")
	make -C $(shell echo $@ | sed "s/......$$//") clean


