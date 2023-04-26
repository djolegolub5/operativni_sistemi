//
// Created by os on 7/12/22.
//
#include "../h/syscall_cpp.hpp"

#include "../h/syscall_c.hpp"



void* operator new (size_t s) {
    return mem_alloc(s);

}

void operator delete (void* addr) noexcept {

    mem_free(addr);


}

void* operator new[](size_t n) {
    return mem_alloc(n);
}
void operator delete[](void* addr) noexcept {
    mem_free(addr);
}

void Thread::dispatch() {

    return thread_dispatch();

}
PeriodicThread::PeriodicThread(time_t period) : Thread(f, this) {
    time = period;




}


int Thread::sleep(time_t x) {
    return time_sleep(x);
}


int Thread::start() {
    return startuj(myHandle);

}

Thread::Thread(void (*body)(void*), void* arg) {

    thread_create(&myHandle, body, arg);

}


Semaphore::Semaphore(unsigned int init) {
    sem_open(&myHandle, init);


}

Semaphore::~Semaphore() {
    sem_close(myHandle);
}

int Semaphore::wait() {
    return sem_wait(myHandle);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}



Thread::~Thread() {
    delete myHandle;
}

Thread::Thread() {
    thread_create_konstruktor(&myHandle, &Thread::f, (void*)this);


}

char Console::getc() {
    return ::getc();
}
void Console::putc(char r) {
    ::putc(r);
}