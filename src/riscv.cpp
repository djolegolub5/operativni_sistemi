//
// Created by os on 7/12/22.
//


#include "../h/riscv.hpp"
#include "../lib/console.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/pcb.hpp"
#include "../h/sem.hpp"
#include "../test/printing.hpp"
#include "../h/cons.hpp"

void Riscv::popSppSpie()
{


    __asm__ volatile("csrw sepc, ra");

    mc_sstatus(SSTATUS_SPP);

    __asm__ volatile("sret");
}

void Riscv::handleSupervisorTrap()
{

    bool volatile flag=false;
    uint64 volatile sp;
    __asm__ volatile ("mv %0, sp" : "=r" (sp));
    uint64 volatile ssp=PCB::getRunning()->getStacksspk();
    __asm__ volatile ("mv sp, %0" :: "r" (ssp));
    uint64 volatile scause = r_scause();

    uint64 volatile sstatus = r_sstatus();


    uint64 volatile sepc = r_sepc() + 4;
    if (scause == 0x0000000000000008UL || scause == 0x0000000000000009UL)
    {
        int volatile x;
        __asm__ volatile ("mv %0, a0" : "=r" (x));


        if (x==0x01){
            size_t s;
            __asm__ volatile ("mv %0, a1" : "=r" (s));
            void* addr=MemoryAllocator::mem_alloc(s);
            __asm__ volatile ("mv a0, %0" :: "r" (addr));
            __asm__ volatile ("sd a0, 80(s0)");


        }
        else if (x==0x02){
            void* addr;
            __asm__ volatile ("mv %0, a1" : "=r" (addr));
            int ret=MemoryAllocator::mem_free(addr);
            __asm__ volatile ("mv a0, %0" :: "r" (ret));
            __asm__ volatile ("sd a0, 80(s0)");



        }
        else if (x==0x11){
            PCB** handle;
            void(*start_routine)(void*);
            void* arg;
            void* s;
            __asm__ volatile ("mv %0, a1" : "=r" (handle));
            __asm__ volatile ("mv %0, a2" : "=r" (start_routine));
            __asm__ volatile ("mv %0, a3" : "=r" (arg));
            __asm__ volatile ("mv %0, a7" : "=r" (s));
            int ret=PCB::thread_create(handle,start_routine,arg,s);

            __asm__ volatile ("mv a0, %0" :: "r" (ret));
            __asm__ volatile ("sd a0, 80(s0)");



        }
        else if (x==0x12){


            int ret=PCB::thread_exit();

            __asm__ volatile ("mv a0, %0" :: "r" (ret));
            __asm__ volatile ("sd a0, 80(s0)");


        }
        else if (x==0x13){

            PCB::yield(); //mozda yield


        }
        else if(x==0x14){


            PCB** handle;
            void(*start_routine)(void*);
            void* arg;
            void* s;
            __asm__ volatile ("mv %0, a1" : "=r" (handle));
            __asm__ volatile ("mv %0, a2" : "=r" (start_routine));
            __asm__ volatile ("mv %0, a3" : "=r" (arg));
            __asm__ volatile ("mv %0, a7" : "=r" (s));
            int ret=PCB::cpp_thread_create_konstruktor(handle,start_routine,arg,s);

            __asm__ volatile ("mv a0, %0" :: "r" (ret));
            __asm__ volatile ("sd a0, 80(s0)");




        }
        else if (x==0x15){

            PCB* pcb;
            __asm__ volatile ("mv %0, a1" : "=r" (pcb));
            int ret=PCB::startuj(pcb);
            __asm__ volatile ("mv a0, %0" :: "r" (ret));
            __asm__ volatile ("sd a0, 80(s0)");


        }
        else if (x==0x21){
            Sem** s;
            unsigned u;
            __asm__ volatile ("mv %0, a1" : "=r" (s));
            __asm__ volatile ("mv %0, a2" : "=r" (u));
            int ret=Sem::sem_open(s,u);
            __asm__ volatile ("mv a0, %0" :: "r" (ret));
            __asm__ volatile ("sd a0, 80(s0)");
        }
        else if (x==0x22){
            Sem* s;
            __asm__ volatile ("mv %0, a1" : "=r" (s));
            int ret=Sem::sem_close(s);
            __asm__ volatile ("mv a0, %0" :: "r" (ret));
            __asm__ volatile ("sd a0, 80(s0)");



        }
        else if (x==0x23){

            Sem* s;
            __asm__ volatile ("mv %0, a1" : "=r" (s));
            int ret=Sem::sem_wait(s);
            __asm__ volatile ("mv a0, %0" :: "r" (ret));
            __asm__ volatile ("sd a0, 80(s0)");

        }

        else if (x==0x24){
            Sem* s;
            __asm__ volatile ("mv %0, a1" : "=r" (s));
            int ret=Sem::sem_signal(s);
            __asm__ volatile ("mv a0, %0" :: "r" (ret));
            __asm__ volatile ("sd a0, 80(s0)");

        }

        else if (x==0x31){


            unsigned long t;
            __asm__ volatile ("mv %0, a1" : "=r" (t));
            int r=PCB::time_sleep(t);

            __asm__ volatile ("mv a0, %0" :: "r" (r));
            __asm__ volatile ("sd a0, 80(s0)");

        }
        else if (x==0x41){

            char t;
            t=Cons::getc();

            __asm__ volatile ("mv a0, %0" :: "r" (t));
            __asm__ volatile ("sd a0, 80(s0)");

        }

        else if (x==0x42){
            char c;
            __asm__ volatile ("mv %0, a1" : "=r" (c));
            Cons::putc(c);


        }
    }

    else if (scause == 0x8000000000000001UL)
    {
        uint64 volatile sepc = r_sepc();
        uint64 volatile sstatus = r_sstatus();

        mc_sip(SIP_SSIP);
        flag=true;


        List<PCB> lista;

        while (true){

            PCB* x=PCB::sleepList.removeFirst();
            if (x==nullptr) break;
            x->time--;
            if (x->time>0) {lista.addLast(x);}
            else {Scheduler::put(x);}


        }
        while (true){

            PCB* x=lista.removeFirst();
            if (x==nullptr) break;
            PCB::sleepList.addLast(x);


        }

        PCB::timeSliceCounter++;
        if (PCB::timeSliceCounter >= PCB::running->getTimeSlice())
        {

            PCB::timeSliceCounter = 0;
            PCB::yield(); //mozda yield

        }

        w_sstatus(sstatus);
        w_sepc(sepc);
    }
    else if (scause == 0x8000000000000009UL)
    {

        flag=true;
        uint64 volatile sepc = r_sepc();
        uint64 volatile sstatus = r_sstatus();
        mc_sip(SIP_SEIP);
        int volatile y=plic_claim();
        if (y==10){

            char* volatile status=(char*)CONSOLE_STATUS;

            while (*status & CONSOLE_RX_STATUS_BIT){


                char* volatile data=(char*)CONSOLE_RX_DATA;

                if (Cons::gbuffer==nullptr) {
                    Cons::gbuffer = new Buff(1024);

                }
                Cons::gbuffer->put(*data);
                status=(char*)CONSOLE_STATUS;



            }

        }

        plic_complete(y);

        w_sstatus(sstatus);
        w_sepc(sepc);

    }
    else
    {
        Cons::putc(scause+'0');
        uint64 volatile sepc = r_sepc();
        uint64 volatile sstatus = r_sstatus();

        w_sstatus(sstatus);
        w_sepc(sepc);

        flag=true;


    }
    if (!flag) {

        w_sepc(sepc);
        w_sstatus(sstatus);
    }
    uint64 s;



    __asm__ volatile ("mv %0, sp" : "=r" (s));
    PCB::getRunning()->setStacksspk(s);
    __asm__ volatile ("mv sp, %0" :: "r" (sp));

}