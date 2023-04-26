//
// Created by os on 7/13/22.
//

#ifndef PROJECT_BASE_V1_1_PCB_HPP
#define PROJECT_BASE_V1_1_PCB_HPP

#include "../lib/hw.h"
#include "../h/scheduler.hpp"
#include "../h/MemoryAllocator.hpp"
#include "../lib/console.h"


class PCB
{

public:

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



    ~PCB() {


        delete body;
        MemoryAllocator::mem_free(stack);
        MemoryAllocator::mem_free(stackssp);

    }




    bool isFinished() const { return finished; }

    void setFinished(bool value) { finished = value; }


    static PCB *running;


private:
    static List<PCB> sleepList;
    int long time;

public:
    struct Body {
        void (*f)(void*);
        void* arg;
    public:
        Body(void (*g)(void*), void*a){
            f=g;
            arg=a;
        }

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

    static int thread_create (
            PCB** handle,
            void (*g)(void*),
            void* arg,
            void* stack_space
    );



    static int time_sleep (unsigned long time);


    static int cpp_thread_create_konstruktor (
            PCB** handle,
            void (*g)(void*),
            void* arg,
            void* stack_space
    );



    static PCB *createThread(Body* body, void* x);

    static void yield();

    static int thread_exit ();

    static int startuj (PCB* pcb);


    Body* getBody(){
        return this->body;
    }

    uint64 getStacksspk(){
        return stacksspk;
    }

    static PCB* getRunning(){
        return running;
    }

    void setStacksspk(uint64 a){
        stacksspk=a;
    }





private:
    static void dispatch();
    int started=false;
    uint64 *stack;
    uint64 stacksspk;
    static void wrapperr(void* x);

    uint64 *stackssp;
    struct Context
    {
        uint64 ra;
        uint64 sp;
    };

    Body* body;
    Context context;
    bool finished;


    uint64 timeSlice;



    friend class Riscv;
    friend class Cons;

    explicit PCB(Body* body, void* x)

    {

        this->body=body;
        finished=false;

        time=0;

        if (x==nullptr)stack=nullptr;
        else stack=(uint64*)x;
        timeSlice=DEFAULT_TIME_SLICE;
        stackssp=(uint64*)MemoryAllocator::mem_alloc(DEFAULT_STACK_SIZE*sizeof(uint64));

        context.ra=body != nullptr ? (uint64) &wrapperr : 0;


        stacksspk=(uint64) &stackssp[DEFAULT_STACK_SIZE];


        context.sp=stack != nullptr ? (uint64) &stack[DEFAULT_STACK_SIZE] : 0;

}
    public:

    static uint64 timeSliceCounter;

    uint64 getTimeSlice();

    static void contextSwitch(Context *oldContext, Context *runningContext);

    Context* getContext(){
        return &context;
    }

};





#endif //PROJECT_BASE_V1_1_PCB_HPP
