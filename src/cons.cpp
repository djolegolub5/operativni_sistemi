//
// Created by os on 7/26/22.
//
#include "../h/cons.hpp"
Buff *Cons::gbuffer=nullptr;
Buff* Cons::pbuffer=nullptr;
PCB* Cons::pnit=nullptr;

static void f(void *){
    char volatile status=*((char*)CONSOLE_STATUS);
    while (true) {
        while (status & CONSOLE_TX_STATUS_BIT) {
            char *m = (char *) CONSOLE_TX_DATA;
            *m = Cons::pbuffer->get();
            status = *((char *) CONSOLE_STATUS);
        }
        PCB::yield();
    }
}





char Cons::getc(){


    if (gbuffer) {
        return gbuffer->get();
    }
    else{
        gbuffer=new Buff(1024);

        return gbuffer->get();

    }
}

void Cons::putc(char c) {
    if (!pnit){
        PCB::thread_create(&pnit,f,nullptr,
                           MemoryAllocator::mem_alloc(sizeof(int)*DEFAULT_STACK_SIZE));
    }
    if (pbuffer) {
        pbuffer->put(c);
    } else {
        pbuffer = new Buff(1024);
        pbuffer->put(c);

    }
}


