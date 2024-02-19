# !/bin/bash 

# 生成 main.c 中各个函数的 CFG
$LLVM_DIR/bin/clang -emit-llvm main.c -O0 -c -o main.bc
$LLVM_DIR/bin/opt --dot-cfg-only main.bc

# 生成 innerloop.c 中各个函数的 CFG
$LLVM_DIR/bin/clang -emit-llvm innerloop.c -O0 -c -o innerloop.bc
$LLVM_DIR/bin/opt --dot-cfg-only innerloop.bc

