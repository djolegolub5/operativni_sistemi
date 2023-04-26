//
// Created by os on 7/12/22.
//

#include "../h/syscall_c.hpp"
#include "../h/cons.hpp"

int thread_create (
        thread_t* handle,
        void(*start_routine)(void*),
        void* arg
){

    thread_t* h=handle;
    void(*f)(void*)=start_routine;
    void* a=arg;
    void*x;
    if (f!=nullptr)
        x=mem_alloc(sizeof(uint64)*DEFAULT_STACK_SIZE);
    else x=nullptr;
    if (f && !x) return -1;



    void* r=x;




    __asm__ volatile ("mv a4, %0" :: "r" (r));
    __asm__ volatile ("mv %0, a4" : "=r" (r));

    __asm__ volatile ("mv a0, %0" :: "r" (0x11));
    __asm__ volatile ("mv a1, %0" :: "r" (h));
    __asm__ volatile ("mv a2, %0" :: "r" (f));
    __asm__ volatile ("mv a3, %0" :: "r" (a));
    __asm__ volatile ("mv a7, %0" :: "r" (r));

    __asm__ volatile ("ecall");

    int t;
    __asm__ volatile ("mv %0, a0" : "=r" (t));

    return t;


}

int thread_exit (){
    __asm__ volatile ("mv a0, %0" :: "r" (0x12));
    __asm__ volatile ("ecall");

    int x;
    __asm__ volatile ("mv %0, a0" : "=r" (x));

    return x;



}

void thread_dispatch (){

    __asm__ volatile ("mv a0, %0" :: "r" (0x13));
    __asm__ volatile ("ecall");


}


int time_sleep (time_t x){

    __asm__ volatile ("mv a1, a0");
    __asm__ volatile ("mv a0, %0" :: "r" (0x31));

    __asm__ volatile("ecall");


    int t;

    __asm__ volatile ("mv %0, a0" : "=r" (t));

    return t;


}

char getc (){
    __asm__ volatile ("mv a0, %0" :: "r" (0x41));

    __asm__ volatile ("ecall");



    char x;

    __asm__ volatile ("mv %0, a0" : "=r" (x));



    return x;


}

void putc(char c){

    __asm__ volatile ("mv a1, a0");
    __asm__ volatile ("mv a0, %0" :: "r" (0x42));


    __asm__ volatile("ecall");



}

int thread_create_konstruktor (
        thread_t* handle,
        void(*start_routine)(void*),
        void* arg
){

    thread_t* h=handle;
    void(*f)(void*)=start_routine;
    void* a=arg;
    void*x;
    if (f!=nullptr)
        x=mem_alloc(sizeof(uint64)*DEFAULT_STACK_SIZE);
    else x=nullptr;
    if (f && !x) return -1;

    void* r=x;
    __asm__ volatile ("mv a4, %0" :: "r" (r));
    __asm__ volatile ("mv %0, a4" : "=r" (r));

    __asm__ volatile ("mv a0, %0" :: "r" (0x14));
    __asm__ volatile ("mv a1, %0" :: "r" (h));
    __asm__ volatile ("mv a2, %0" :: "r" (f));
    __asm__ volatile ("mv a3, %0" :: "r" (a));
    __asm__ volatile ("mv a7, %0" :: "r" (r));

    __asm__ volatile ("ecall");

    int t;
    __asm__ volatile ("mv %0, a0" : "=r" (t));

    return t;




}

int startuj(thread_t handle){

    __asm__ volatile ("mv a1, a0");
    __asm__ volatile ("mv a0, %0" :: "r" (0x15));


    int x;

    __asm__ volatile ("ecall");

    __asm__ volatile ("mv %0, a0" : "=r" (x));
    return x;


}

int sem_open (
        sem_t* handle,
        unsigned init
){

    __asm__ volatile ("mv a2, a1");
    __asm__ volatile ("mv a1, a0");
    __asm__ volatile ("mv a0, %0" :: "r" (0x21));


    __asm__ volatile ("ecall");

    int x;

    __asm__ volatile ("mv %0, a0" : "=r" (x));

    return x;


}


int sem_close (sem_t handle){

    __asm__ volatile ("mv a1, a0");
    __asm__ volatile ("mv a0, %0" :: "r" (0x22));

    __asm__ volatile("ecall");


    int x;

    __asm__ volatile ("mv %0, a0" : "=r" (x));

    return x;

}

int sem_wait (sem_t id){

    __asm__ volatile ("mv a1, a0");
    __asm__ volatile ("mv a0, %0" :: "r" (0x23));

    __asm__ volatile("ecall");


    int x;

    __asm__ volatile ("mv %0, a0" : "=r" (x));

    return x;

}

int sem_signal (sem_t id){

    __asm__ volatile ("mv a1, a0");
    __asm__ volatile ("mv a0, %0" :: "r" (0x24));

    __asm__ volatile("ecall");


    int x;

    __asm__ volatile ("mv %0, a0" : "=r" (x));

    return x;


}



void* mem_alloc (size_t size){
    size_t pom=size/MEM_BLOCK_SIZE;
    pom*=MEM_BLOCK_SIZE;
    if (size%MEM_BLOCK_SIZE!=0)pom+=MEM_BLOCK_SIZE;
    size=pom;

    __asm__ volatile ("mv a0, %0" :: "r" (0x01));
    __asm__ volatile ("mv a1, %0" :: "r" (size));

    void* x;



    __asm__ volatile ("ecall");

    __asm__ volatile ("mv %0, a0" : "=r" (x));
    return x;
}

int mem_free (void* addr){

    __asm__ volatile ("mv a1, a0");
    __asm__ volatile ("mv a0, %0" :: "r" (0x02));


    int x;

    __asm__ volatile ("ecall");

    __asm__ volatile ("mv %0, a0" : "=r" (x));
    return x;

}

