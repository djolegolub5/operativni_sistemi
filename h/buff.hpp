//
// Created by zika on 2/26/22.
//

#ifndef OS1_BUFFER_CPP_H
#define OS1_BUFFER_CPP_H

#include "syscall_c.hpp"
#include "../test/printing.hpp"

class Buff {
private:
    int cap;
    char *buffer;
    int head, tail;

    sem_t spaceAvailable;
    sem_t itemAvailable;
    sem_t mutexHead;
    sem_t mutexTail;

public:

    void * operator new(size_t size){

        size_t pom=size/MEM_BLOCK_SIZE;
        pom*=MEM_BLOCK_SIZE;
        if (size%MEM_BLOCK_SIZE!=0)pom+=MEM_BLOCK_SIZE;
        size=pom;
        return MemoryAllocator::mem_alloc(size);

    }
    void operator delete(void * addr) noexcept{
        MemoryAllocator::mem_free(addr);
    }

    void *operator new[](size_t size){
        size_t pom=size/MEM_BLOCK_SIZE;
        pom*=MEM_BLOCK_SIZE;
        if (size%MEM_BLOCK_SIZE!=0)pom+=MEM_BLOCK_SIZE;
        size=pom;
        return MemoryAllocator::mem_alloc(size);

    }
    void operator delete[](void* addr) noexcept{
        MemoryAllocator::mem_free(addr);


    }


    Buff(int _cap);
    ~Buff();

    void put(char c);
    char get();

    int getCnt();

};


#endif //OS1_BUFFER_CPP_H

