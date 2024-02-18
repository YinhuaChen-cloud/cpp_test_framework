#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cassert>
#include <vector>
#include <unordered_map>
#include <stdio.h>
#include "path_reduction.h"

// C++ 接口 ------------------------------------------------------------------- start
#define FUNC_ID_LEN 256

// CFG 结构体只需要在 cfg.txt 中解析获取
// CYHNO_TE: 这里需要一个二进制文件
typedef struct CFGEntry {
  // int funcID;  funcID 就是 CFG 在 cfg_arr 中的下标索引
  char function_name[FUNC_ID_LEN];  // 函数的字符串形式，整数形式就是 cfg_arr 的下标
  int entry;                        // 整数，表示函数入点 block
  int exit;                         // 整数，表示函数出点 block
} CFGEntry;

// calls 需要 processed_mapping_table.txt 中获取
// successor_size 从 cfg.txt 中获取
// successors_arr 从 cfg.txt 中获取
// CYHNO_TE: 这里需要一个二进制文件
typedef struct BlockEntry {
  // int bbID;    基本块ID 就是在 block_arr 中的下标索引
  int calls;                   // -1 时，表示没有调用任何函数；非负数时，表示调用的函数的 funcID，也就是在 cfg_arr 中的下标索引
  int successor_size;          // 表示这个基本块拥有的 successors 数量
  int *successors_arr;         // 这个基本块的 successors_arr 数组
} BlockEntry;

// cfg_size 只需要过一遍 function_list.txt 即可，内存也那样分配就好
// block_size 只需要过一遍 processed_mapping_table.txt 即可，内存也那样分配就好
// CYHNO_TE: 这是顶层结构体
// CYHNO_TE: 这里需要一个二进制文件
typedef struct TopLevel {
  int cfg_size;           // cfg 的数量
  CFGEntry *cfg_arr;           // cfg 数组
  int block_size;         // blocks 总数
  BlockEntry *block_arr;  // block 数组
} TopLevel;
// C++ 接口 ------------------------------------------------------------------- end

void dump_block(BlockEntry *block) {
    std::cout << "block->calls = " << block->calls << std::endl;
    std::cout << "block->successor_size = " << block->successor_size << std::endl;
    for (int i = 0; i < block->successor_size; i++) {
        printf("block->successor_arr[%d] = %d\n", i, block->successors_arr[i]);
    }
}

void dump_cfg(CFGEntry *cfg) {
    std::string funcname(cfg->function_name);
    std::cout << "cfg->function_name = " << funcname << std::endl;
    std::cout << "cfg->entry = " << cfg->entry << std::endl;
    std::cout << "cfg->exit = " << cfg->exit << std::endl;
}

void dump_top(TopLevel *top) {
    std::cout << "top->cfg_size = " << top->cfg_size << std::endl;
    std::cout << "top->block_size = " << top->block_size << std::endl;
    for(int i = 0; i < top->cfg_size; i++) {
        std::cout << "cfg-id = " << i << std::endl;
        dump_cfg(&(top->cfg_arr[i]));
    }
    for(int i = 0; i < top->block_size; i++) {
        std::cout << "BB " << i << std::endl;
        dump_block(&(top->block_arr[i]));
    }
}

void load_top(TopLevel *top, const char *path) {

    FILE *file = fopen(path, "rb");
    if (file != NULL) {

        fread(top, sizeof(TopLevel), 1, file);

        // std::cout << "top->cfg_size = " << top->cfg_size << std::endl;
        // std::cout << "top->block_size = " << top->block_size << std::endl;

        top->cfg_arr = (CFGEntry *) malloc(sizeof(CFGEntry) * top->cfg_size);

        fread(top->cfg_arr, sizeof(CFGEntry) * top->cfg_size, 1, file);

        top->block_arr = (BlockEntry *) malloc(sizeof(BlockEntry) * top->block_size);

        fread(top->block_arr, sizeof(BlockEntry) * top->block_size, 1, file);

        for(int i = 0; i < top->block_size; i++) {
          top->block_arr[i].successors_arr = (int *) malloc(sizeof(int) * top->block_arr[i].successor_size);
          fread(top->block_arr[i].successors_arr, sizeof(int) * top->block_arr[i].successor_size, 1, file);
        }

        fclose(file);
        // printf("Read data from file\n");
    } else {
        perror("Failed to open file for reading");
    }
}

void test1() {

    TopLevel top;

    load_top(&top, "base64_cfg.bin");
    // dump_top(&top);

    // 测试能否加载 base64 CFG
    PathReducer* pathreducer = get_path_reducer(&top, 10);

    // printf("path_size = %d\n", sizeof(path)/sizeof(path[0]));
    // printf("before running function 'path_reducer_free'\n");
    // free_path_reducer(pathreducer);
    // printf("after running function 'path_reducer_free'\n");

}





