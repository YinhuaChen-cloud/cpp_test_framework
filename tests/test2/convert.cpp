#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cassert>
#include <vector>
#include <unordered_map>

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
        dump_cfg(&(top->cfg_arr[i]));
    }
    for(int i = 0; i < top->block_size; i++) {
        std::cout << "BB " << i << std::endl;
        dump_block(&(top->block_arr[i]));
    }
}

void store_top(TopLevel *top);

// 去除字符串首尾的空白字符
std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    size_t last = str.find_last_not_of(" \t\n\r");
    if (first == std::string::npos || last == std::string::npos) {
        return "";
    }
    return str.substr(first, last - first + 1);
}

int main() {

    // 0. 一个顶层，收集完数据后要被 dump 出去
    TopLevel top;
    // funcname -> integer 映射
    std::unordered_map<std::string, int> funcname_int;

    // 1. 读取 zekun_processed_mapping_table.txt，构建整个映射表  ------------------------------ start
    std::ifstream mapping_table_file("processed_mapping_table.txt"); 

    if (mapping_table_file.is_open()) { // 检查文件是否成功打开

        std::string line;           // zekun_processed_mapping_table.txt 的每一行内容
        std::string firstPart;
        std::string secondPart;

        // 第一遍，只要过一遍，直到知道需要给 block_arr 分配多大的内存空间即可 --- checked
        while (std::getline(mapping_table_file, line)) { // 逐行读取文件内容
            std::string input = line;
            size_t pos = input.find(' ');            // 查找第一个空格的位置
            if (pos != std::string::npos) {          // 如果找到了空格
                firstPart = input.substr(0, pos);    // 提取第一个空格之前的部分
                secondPart = input.substr(pos + 1);  // 提取第一个空格之后的部分
                // std::cout << "First part: " << firstPart << ", Second part: " << secondPart << std::endl; // 打印切分后的两部分字符串
            } else {
                std::cout << "未找到空格" << std::endl;
            }
        }
        mapping_table_file.close(); // 关闭文件

        int numBB = std::stoi(firstPart) + 1;

        top.block_size = numBB;
        top.block_arr = (BlockEntry *) malloc(sizeof(BlockEntry) * numBB);

        // 构建一个 function_name -> integer 的映射 (需要读取 function_list.txt，原因后续填满 BasicBlock 需要使用)

        std::ifstream function_list_file("function_list.txt"); 

        if (function_list_file.is_open()) { // 检查文件是否成功打开

            std::string line;           
            std::string firstPart;
            std::string secondPart;

            // 第一遍，只要过一遍，直到知道需要给 cfg_arr 分配多大的内存空间即可 --- checked
            while (std::getline(function_list_file, line)) { // 逐行读取文件内容
                std::string input = line;
                size_t pos = input.find(' ');            // 查找第一个空格的位置
                if (pos != std::string::npos) {          // 如果找到了空格
                    firstPart = input.substr(0, pos);    // 提取第一个空格之前的部分
                    secondPart = input.substr(pos + 1);  // 提取第一个空格之后的部分
                } else {
                    std::cout << "未找到空格" << std::endl;
                }

                // trim 去除首尾空白字符
                // size_t start = line.find_first_not_of(" \t\n\r");
                // size_t end = line.find_last_not_of(" \t\n\r");
                // if (start != std::string::npos && end != std::string::npos) {
                //     firstPart = firstPart.substr(start, end - start + 1);
                //     secondPart = secondPart.substr(start, end - start + 1);
                // }

                firstPart = trim(firstPart);
                secondPart = trim(secondPart);

                // 函数名 -> funcid 映射
                funcname_int[secondPart] = std::stoi(firstPart);

            }

        }
        else {
            std::cout << "无法打开文件" << std::endl;
        }

        // for (const auto& pair : funcname_int) {
        //     std::cout << "Key: " << pair.first << ", Value: " << pair.second << std::endl;
        // }

        // 接下来的任务：填满 BlockEntry 数组 (calls属性) ------------------------ start
        std::ifstream mapping_table_file("processed_mapping_table.txt"); 

        while (std::getline(mapping_table_file, line)) {  // 逐行读取文件内容
            std::string input = line;
            size_t pos = input.find(' ');            // 查找第一个空格的位置
            if (pos != std::string::npos) {          // 如果找到了空格
                firstPart = input.substr(0, pos);    // 提取第一个空格之前的部分
                secondPart = input.substr(pos + 1);  // 提取第一个空格之后的部分
            } else {
                std::cout << "未找到空格" << std::endl;
            }

            size_t found = secondPart.find("Calls");   

            if (found != std::string::npos) {  

                size_t pos = secondPart.find("Calls ");  
                assert(pos != std::string::npos);
                std::string result = secondPart.substr(pos + std::strlen("Calls "));  
                // std::cout << result << std::endl; 
                result = trim(result);

                if (funcname_int.count(result) > 0) {
                    top.block_arr[std::stoi(firstPart)].calls = funcname_int[result];
                } else {
                    top.block_arr[std::stoi(firstPart)].calls = -2; // -2 表示是 C 库函数
                }

                // std::cout << "The string contains the word 'calls'." << std::endl;  

            } else {  
                top.block_arr[std::stoi(firstPart)].calls = -1; 
                // std::cout << "The string does not contain the word 'calls'." << std::endl;  
            }  

        }

        mapping_table_file.close(); // 关闭文件
        // 接下来的任务：填满 BlockEntry 数组 (calls属性) ------------------------ end

    }
    else {
        std::cout << "无法打开文件" << std::endl;
    }
    // 1. 读取 zekun_processed_mapping_table.txt，构建整个映射表  ------------------------------ end

    // 2. 读取 function_list.txt，构建整个映射表 --------------------------------------------------- start
    std::ifstream function_list_file("function_list.txt"); 

    if (function_list_file.is_open()) { // 检查文件是否成功打开

        std::string line;           // zekun_processed_mapping_table.txt 的每一行内容
        std::string firstPart;
        std::string secondPart;

        // 第一遍，只要过一遍，直到知道需要给 cfg_arr 分配多大的内存空间即可 --- checked
        while (std::getline(function_list_file, line)) { // 逐行读取文件内容
            std::string input = line;
            size_t pos = input.find(' ');            // 查找第一个空格的位置
            if (pos != std::string::npos) {          // 如果找到了空格
                firstPart = input.substr(0, pos);    // 提取第一个空格之前的部分
                secondPart = input.substr(pos + 1);  // 提取第一个空格之后的部分
            } else {
                std::cout << "未找到空格" << std::endl;
            }
        }

        int numfunction = std::stoi(firstPart) + 1;

        top.cfg_size = numfunction;
        top.cfg_arr = (CFGEntry *) malloc(sizeof(CFGEntry) * numfunction);

        // 接下来的任务：填满 CFG 数组 -------------------------------------------------------------------- start - checked
        // 第二遍，这一次的目的是填满 cfg_arr (单独读取 cfg.txt 即可) --- doing
        line = "";           // zekun_processed_mapping_table.txt 的每一行内容
        firstPart = "";
        secondPart = "";

        std::ifstream cfg_file("cfg.txt"); 

        int func_id = 0;

        if (cfg_file.is_open()) { // 检查文件是否成功打开

            bool justGetIn = false;   // 一个 flag，表示是否刚刚进入一个函数，用来寻找 entryblock
            bool waitForLast = false; // 一个 flag，标识是否在等待一个函数结束，用来寻找 exitblock
            int exitblockStore = -1;  // 用来储存可能是 exitblock 的整数
            int curBBID = -1;         // 当前基本块的 BBID

            while (std::getline(cfg_file, line)) { // 逐行读取文件内容

                size_t pos = line.find(' ');            // 查找第一个空格的位置
                firstPart = line.substr(0, pos);
                secondPart = line.substr(pos + 1);      // 提取第一个空格之后的部分

                size_t start = line.find_first_not_of(" \t\n\r");
                size_t end = line.find_last_not_of(" \t\n\r");

                if (start != std::string::npos && end != std::string::npos) {
                    firstPart = firstPart.substr(start, end - start + 1);
                    secondPart = secondPart.substr(start, end - start + 1);
                }

                if (firstPart == "Function:") {

                    // printf("waitForLast = %d, exitblockStore = %d\n", waitForLast, exitblockStore);

                    if (waitForLast) {
                        top.cfg_arr[func_id - 1].exit = exitblockStore;
                        // printf("exitBlock = %d\n", top.cfg_arr[func_id].exit);
                        waitForLast = false;
                    }

                    strcpy(top.cfg_arr[func_id].function_name, secondPart.c_str());
                    // printf("funcname = %s\n", top.cfg_arr[func_id].function_name);
                    justGetIn = true;
                    func_id++;
                }
                else if (firstPart == "BasicBlock:") {

                    // std::cout << "secondPart = " << secondPart << std::endl;
                    curBBID = std::stoi(secondPart);

                    // printf("func_id = %d, justGetIn = %d, std::stoi(secondPart) = %d\n", func_id, justGetIn, std::stoi(secondPart));

                    if(justGetIn) {
                        top.cfg_arr[func_id - 1].entry = std::stoi(secondPart);
                        // printf("entryBlock = %d\n", top.cfg_arr[func_id].entry);
                        justGetIn = false;
                    }

                    waitForLast = true;
                    exitblockStore = std::stoi(secondPart);

                }
                else if (firstPart == "Successors:") { 
                    // 填满 BlockEntry 的 successor_size 和 successors_arr

                    std::vector <std::string> words;
                    std::vector <int> integer_words;

                    size_t start = 0, end = 0;
                    while ((end = secondPart.find(' ', start)) != std::string::npos) {
                        words.push_back(secondPart.substr(start, end - start));
                        start = end + 1;
                    }
                    words.push_back(secondPart.substr(start)); // 添加最后一个单词
                    // 把 vector 中的单词字符串转为整数 vector
                    for (const auto& w : words) {
                        if (!w.empty() && w != "Successors:") {
                            int num = std::stoi(w);
                            integer_words.push_back(num);
                        }
                    }

                    top.block_arr[curBBID].successor_size = integer_words.size();
                    top.block_arr[curBBID].successors_arr = (int *) malloc(sizeof(int) * integer_words.size());

                    for(int i = 0; i < integer_words.size(); i++) {
                        top.block_arr[curBBID].successors_arr[i] = integer_words[i];
                    }

                }
                // 不需要 else，因为有可能是 "" 空字符串

            }

            top.cfg_arr[func_id - 1].exit = exitblockStore;
            // top.cfg_arr[func_id].exit = exitblockStore;
            // printf("exitBlock = %d\n", top.cfg_arr[func_id].exit);

        }
        // 接下来的任务：填满 CFG 数组 -------------------------------------------------------------------- end - checked

        function_list_file.close(); // 关闭文件

    }
    else {
        std::cout << "无法打开文件" << std::endl;
    }
    // 2. 读取 function_list.txt，构建整个映射表 --------------------------------------------------- end

    // 3. 这里可以尝试 dump top 的 Readable 形式
    dump_top(&top);
    // 3. 这里可以尝试把 top 里的内容储存到二进制文件里
    store_top(&top);

    return 0;
}

void store_top(TopLevel *top) {
    FILE *file = fopen("top.bin", "wb");
    if (file != NULL) {
        fwrite(top, sizeof(TopLevel), 1, file);
        fwrite(top->cfg_arr, sizeof(CFGEntry) * top->cfg_size, 1, file);
        fwrite(top->block_arr, sizeof(BlockEntry) * top->block_size, 1, file);
        for(int i = 0; i < top->block_size; i++) {
          fwrite(top->block_arr[i].successors_arr, sizeof(int) * top->block_arr[i].successor_size, 1, file);
        }

        fclose(file);
        // printf("Data has been dumped into top.bin\n");
    } else {
        perror("Failed to open file for writing");
    }
}
