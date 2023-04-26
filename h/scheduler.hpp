//
// Created by os on 7/13/22.
//

#ifndef PROJECT_BASE_V1_1_SCHEDULER_HPP
#define PROJECT_BASE_V1_1_SCHEDULER_HPP

#include "list.hpp"
#include "MemoryAllocator.hpp"
class PCB;

class Scheduler
{
private:
    static List<PCB> readyCoroutineQueue;

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


    static PCB *get();

    static void put(PCB *ccb);

};



#endif //PROJECT_BASE_V1_1_SCHEDULER_HPP
