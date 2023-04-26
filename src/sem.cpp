//
// Created by os on 7/18/22.
//

#include "../h/sem.hpp"

int Sem::sem_open (
        Sem** handle,
        unsigned init
){
    *handle=new Sem(init);
    if (*handle==nullptr) return -1;
    return 0;

}



int Sem::sem_close (Sem* handle){
    if (handle==nullptr) return -1;
    while (true){
        PCB* curr=handle->queue.removeFirst();
        if (!curr) break;
        Scheduler::put(curr);
    }
    delete handle;
    handle=nullptr;
    return 0;
}

void Sem::block(){
    queue.addLast(PCB::running);
    PCB* old=PCB::running;
    PCB::running=Scheduler::get();
    PCB::contextSwitch(old->getContext(), PCB::running->getContext());
}

void Sem::deblock(){
    Scheduler::put(queue.removeFirst());

}



int Sem::sem_wait(Sem* id){
    if (id==nullptr) return -1;
    if (--id->num<0) id->block();
    return 0;

}


int Sem::sem_signal(Sem* id){
    if (id==nullptr) return -1;
    if (++id->num<=0) id->deblock();
    return 0;

}


