# !/bin/bash 
# 运行这个脚本之前记得先切换到 path_fuzzer 分支

# 使用 main.c 和 innerloop.c 生成 PUT，随后使用 path_fuzzer 产生路径
AFL_LLVM_CALLER=1 afl-clang-fast -O0 main.c -c -o main.o
AFL_LLVM_CALLER=1 afl-clang-fast -O0 innerloop.c -c -o innerloop.o
AFL_LLVM_CALLER=1 afl-clang-fast -O0 main.o innerloop.o -o PUT

# AFL_LLVM_CALLER=1 afl-clang-fast main.c -O0 -emit-llvm -S -o main.ll
# AFL_LLVM_CALLER=1 afl-clang-fast innerloop.c -O0 -emit-llvm -S -o innerloop.ll

