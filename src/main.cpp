//#include "../test/Threads_C_API_test.hpp" // zadatak 2, niti C API i sinhrona promena konteksta
//#include "../test/Threads_CPP_API_test.hpp" // zadatak 2., niti CPP API i sinhrona promena konteksta
#include "../h/riscv.hpp"
#include "../h/sem.hpp"
//#include "../test/ConsumerProducer_C_API_test.h" // zadatak 3., kompletan C API sa semaforima, sinhrona promena konteksta
//#include "../test/ConsumerProducer_CPP_Sync_API_test.hpp" // zadatak 3., kompletan CPP API sa semaforima, sinhrona promena konteksta

#include "../test/ThreadSleep_C_API_test.hpp" // thread_sleep test C API
#include "../test/ConsumerProducer_CPP_API_test.hpp" // zadatak 4. CPP API i asinhrona promena konteksta


Sem* glavni;

void idle(void* x){
    while (true){
        thread_dispatch();

    }
}

void userMain(){




    //Threads_C_API_test(); // zadatak 2., niti C API i sinhrona promena konteksta
    //Threads_CPP_API_test(); // zadatak 2., niti CPP API i sinhrona promena konteksta

    //producerConsumer_C_API(); // zadatak 3., kompletan C API sa semaforima, sinhrona promena konteksta
    //producerConsumer_CPP_Sync_API(); // zadatak 3., kompletan CPP API sa semaforima, sinhrona promena konteksta

    //testSleeping(); // thread_sleep test C API
    ConsumerProducerCPP::testConsumerProducer(); // zadatak 4. CPP API i asinhrona promena konteksta, kompletan test svega

}


static void user(void* x){
    userMain();
    sem_signal(glavni);
}



void main() {
    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);

    PCB* r=nullptr;
    PCB* p=nullptr;
    PCB* u=nullptr;

    PCB::thread_create(&r, nullptr, nullptr, nullptr);

    PCB::running=r;

    PCB::thread_create(&p,idle, nullptr,
                       MemoryAllocator::mem_alloc(sizeof(int)*DEFAULT_STACK_SIZE));
    PCB::thread_create(&u,user, nullptr,
                       MemoryAllocator::mem_alloc(sizeof(int)*DEFAULT_STACK_SIZE));
    Sem::sem_open(&glavni,0);
    Sem::sem_wait(glavni);


    PCB::yield();






}


