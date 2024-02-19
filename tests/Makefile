ALL_TESTS=test2

# 使用 foreach 函数添加 _clean
CLEAN_ALL := $(foreach mod,$(ALL_TESTS),$(mod)_clean)

.PHONY: archive
archive: $(ALL_TESTS)
	@echo "In archive"

# 循环调用子目录下的Makefile
.PHONY: $(ALL_TESTS)
$(ALL_TESTS):
	@echo "HEADERS = $(HEADERS)"
	make -C $@ $@
	cp $@/$@.a .
	

.PHONY: clean
clean: $(CLEAN_ALL)
	-@rm -f *.o a.out main *.a


.PHONY: $(CLEAN_ALL)
$(CLEAN_ALL):
	# echo $@
	# echo $(shell echo $@ | sed "s/......$$//")
	make -C $(shell echo $@ | sed "s/......$$//") clean


