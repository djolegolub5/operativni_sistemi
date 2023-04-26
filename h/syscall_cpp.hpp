//
// Created by os on 7/12/22.
//

#ifndef PROJECT_BASE_V1_1_SYSCALL_CPP_HPP
#define PROJECT_BASE_V1_1_SYSCALL_CPP_HPP

#include "../h/syscall_c.hpp"
#include "../test/printing.hpp"
void* operator new(size_t);
void operator delete(void*) noexcept;

void* operator new[](size_t n);
void operator delete[](void* n) noexcept;

class Thread {
public:
    Thread(void (*body)(void*), void* arg);
    virtual ~Thread();
    int start();
    static void dispatch();
    static int sleep(time_t);
protected:
    Thread();
    virtual void run() {}
private:
    thread_t myHandle;
    static void f(void* x) {
        Thread* t = (Thread*)x;
        t->run();
    }

};
class Semaphore {
public:
    Semaphore(unsigned init = 1);
    virtual ~Semaphore();
    int wait();
    int signal();
private:
    sem_t myHandle;
};
class PeriodicThread : public Thread {
protected:
    PeriodicThread(time_t period);
    virtual void periodicActivation() {}
private:
    static void f(void* x) {
        PeriodicThread* p = (PeriodicThread*)x;
        time_t spavaj = p->time;
        while (true) {
            p->periodicActivation();
            sleep(spavaj);

        }
    }

    time_t time;




};
class Console {
public:
    static char getc();
    static void putc(char);
};


#endif //PROJECT_BASE_V1_1_SYSCALL_CPP_HPP
