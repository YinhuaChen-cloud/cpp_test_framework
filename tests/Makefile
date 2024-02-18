ALL_TESTS=test1 test2 test3

# 将环境变量分割成目录列表
DIR_LIST := $(subst :, ,$(ALL_TESTS))

archive: $(DIR_LIST)
	@echo "In archive"

# 循环调用子目录下的Makefile
.PHONY: $(DIR_LIST)
$(DIR_LIST):
	make -C $@

