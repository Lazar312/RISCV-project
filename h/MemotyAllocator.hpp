//
// Created by os on 2/8/23.
//

#ifndef PROJEKAT2_MEMOTYALLOCATOR_HPP
#define PROJEKAT2_MEMOTYALLOCATOR_HPP
#include "../lib/hw.h"
class MemotyAllocator{
public:
    static void* mem_alloc(size_t t);
    static int mem_free(void* ptr);

private:

};
#endif //PROJEKAT2_MEMOTYALLOCATOR_HPP
