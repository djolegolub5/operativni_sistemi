//
// Created by os on 6/26/22.
//

#ifndef PROJECT_BASE_V1_1_MEMORYALLOCATOR_HPP
#define PROJECT_BASE_V1_1_MEMORYALLOCATOR_HPP

#include "../lib/hw.h"

struct Elem{

    size_t size;
    Elem* next;


};

class MemoryAllocator {
private:

public:
    static void join(Elem* x);
    static Elem* head;
    static bool flag;
    static void* mem_alloc(size_t size);
    static int mem_free(void* addr);




};


#endif //PROJECT_BASE_V1_1_MEMORYALLOCATOR_HPP
