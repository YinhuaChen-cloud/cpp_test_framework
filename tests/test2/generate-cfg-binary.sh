# !/bin/bash 
# 运行这个脚本之前记得先切换到 CFG-new_merge_stable 分支

# 运行之前清空之前的文件，防止写到同一个文件里
rm ./cfg.txt ./processed_mapping_table.txt ./function_list.txt ./cfg_text.txt ./convert
# 生成 main.c 和 innerloop.c 中各个函数的 CFG
CONTROL_FLOW_GRAPH="./cfg.txt" AFL_LLVM_CALLER=1 afl-clang-fast -emit-llvm -S -O0 main.c -o main.ll
CONTROL_FLOW_GRAPH="./cfg.txt" AFL_LLVM_CALLER=1 afl-clang-fast -emit-llvm -S -O0 innerloop.c -o innerloop.ll

cat cfg.txt | grep "BasicBlock" | nl -v 0 | awk '{print $1, $3, $4, $5, $6, $7, $8, $9}' > processed_mapping_table.txt
cat cfg.txt | grep "Function" | nl -v 0 | awk '{print $1, $3, $4, $5, $6, $7, $8, $9}' > function_list.txt
mv cfg.txt cfg_text.txt

MAPPING_TABLE=1 CONTROL_FLOW_GRAPH="./cfg.txt" AFL_LLVM_CALLER=1 afl-clang-fast -emit-llvm -S -O0 main.c -o main.ll
MAPPING_TABLE=1 CONTROL_FLOW_GRAPH="./cfg.txt" AFL_LLVM_CALLER=1 afl-clang-fast -emit-llvm -S -O0 innerloop.c -o innerloop.ll

g++ convert.cpp -o convert
./convert
mv top.bin test2_cfg.bin


# $LLVM_DIR/bin/clang -emit-llvm main.c -O0 -c -o main.bc
# $LLVM_DIR/bin/opt --dot-cfg-only main.bc

# # 生成 innerloop.c 中各个函数的 CFG
# $LLVM_DIR/bin/clang -emit-llvm innerloop.c -O0 -c -o innerloop.bc
# $LLVM_DIR/bin/opt --dot-cfg-only innerloop.bc


