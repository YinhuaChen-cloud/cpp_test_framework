#ifndef PATH_REDUCER_H
#define PATH_REDUCER_H

#include <stdint.h>

extern "C" {

typedef struct PathReducer PathReducer;
typedef int32_t BlockID;
typedef int32_t FunID;

// Gets a path reducer (k 代表的似乎是保留的循环数)
PathReducer* get_path_reducer(const void* top_level, int32_t k);

// Frees a path reducer
// void path_reducer_free(PathReducer* ptr);
void free_path_reducer(PathReducer* ptr);

// Reduces a path, and gets the hash
const char* reduce_path(const PathReducer* reducer, const BlockID* path, int32_t path_size, FunID entry_fun_id);

}

#endif


