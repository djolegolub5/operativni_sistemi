//
// Created by os on 7/18/22.
//

#ifndef PROJECT_BASE_V1_1_SEM_HPP
#define PROJECT_BASE_V1_1_SEM_HPP

#include "list.hpp"
#include "pcb.hpp"



class Sem{
private:
    int num;
    List<PCB> queue;
    void block();
    void deblock();



public:
    Sem(unsigned init){
        num=init;

    }


    static int sem_open (
            Sem** handle,
            unsigned init
    );


    static int sem_close (Sem* handle);
    static int sem_wait (Sem* id);
    static int sem_signal (Sem* id);


    void * operator new(size_t size){

        size_t pom=size/MEM_BLOCK_SIZE;
        pom*=MEM_BLOCK_SIZE;
        if (size%MEM_BLOCK_SIZE!=0)pom+=MEM_BLOCK_SIZE;
        size=pom;
        void* x= MemoryAllocator::mem_alloc(size);
        return x;

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

};




#endif //PROJECT_BASE_V1_1_SEM_HPP
