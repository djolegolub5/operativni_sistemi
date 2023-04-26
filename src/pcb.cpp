//
// Created by os on 7/13/22.
//
#include "../h/pcb.hpp"
#include "../h/riscv.hpp"
#include "../h/syscall_c.hpp"
#include "../lib/console.h"
#include "../h/sem.hpp"
#include "../h/cons.hpp"

uint64 PCB::timeSliceCounter = 0;
PCB *PCB::running = nullptr;
List<PCB> PCB::sleepList;


int PCB::thread_exit (){
    if (running->isFinished()) return -1;
    running->setFinished(true);
    thread_dispatch();
    return 0;


}


uint64 PCB::getTimeSlice() {
    return timeSlice;
}



PCB *PCB::createThread(Body* body, void* s)
{
    return new PCB(body, s);



}

void PCB:: wrapperr (void* x){

    if (PCB::running!=Cons::pnit)Riscv::popSppSpie();
    PCB::running->getBody()->f(PCB::running->getBody()->arg);
    thread_exit();
}




int PCB::time_sleep (unsigned long time){


    if (time==0) return 0;
    if (PCB::running->getBody()==nullptr) return -1;
    PCB::running->time=time;
    PCB::sleepList.addLast(PCB::running);
    PCB *old = running;
    PCB::running = Scheduler::get();
    PCB::contextSwitch(old->getContext(), PCB::running->getContext());

    return 0;






}



int PCB::thread_create(PCB** handle,
                       void (*g)(void*),
                       void* arg,
                       void* stack_space){
    Body* bo;
    if (g==nullptr) bo=nullptr;
    else bo=new Body(g,arg);


    *handle= createThread(bo,stack_space);
    if (*handle==nullptr) return -1;
    if (bo != nullptr) { Scheduler::put(*handle); }
    (*handle)->started=true;
    return 0;
}




int PCB::cpp_thread_create_konstruktor(PCB** handle,
                                       void (*g)(void*),
                                       void* arg,
                                       void* stack_space){
    Body* bo;
    if (g==nullptr) bo=nullptr;
    else bo=new Body(g,arg);


    *handle= createThread(bo,stack_space);
    if (*handle==nullptr) return -1;
    return 0;
}

void PCB::yield()
{



    Riscv::pushRegisters();

    PCB::dispatch();

    Riscv::popRegisters();


}

void PCB::dispatch()
{

    PCB *old = running;
    PCB::timeSliceCounter=0;
    if (!old->isFinished()) { Scheduler::put(old); }
    running = Scheduler::get();
    PCB::contextSwitch(&old->context, &running->context);

}


int PCB::startuj(PCB* pcb){
    if (pcb->started==true || pcb->isFinished()) return -1;
    pcb->started=true;
    Scheduler::put(pcb);
    return 0;


}