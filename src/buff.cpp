#include "../h/buff.hpp"
#include "../h/sem.hpp"
#include "../h/cons.hpp"

Buff::Buff(int _cap) : cap(_cap + 1), head(0), tail(0) {

    buffer = (char*)(MemoryAllocator::mem_alloc(sizeof(char) * cap));
    Sem::sem_open(&itemAvailable, 0);
    Sem::sem_open(&spaceAvailable, _cap);
    Sem::sem_open(&mutexHead, 1);
    Sem::sem_open(&mutexTail, 1);
}

Buff::~Buff() {

    while (getCnt() > 0) {
        char ch = buffer[head];
        Cons::putc(ch);
        head = (head + 1) % cap;
    }


    MemoryAllocator::mem_free(buffer);
    Sem::sem_close(itemAvailable);
    Sem::sem_close(spaceAvailable);
    Sem::sem_close(mutexTail);
    Sem::sem_close(mutexHead);
}

char Buff::get() {
    Sem::sem_wait(itemAvailable);

    Sem::sem_wait(mutexHead);

    char ret = buffer[head];
    head = (head + 1) % cap;
    Sem::sem_signal(mutexHead);

    Sem::sem_signal(spaceAvailable);

    return ret;
}

void Buff::put(char val) {

    Sem::sem_wait(spaceAvailable);

    Sem::sem_wait(mutexTail);
    buffer[tail] = val;
    tail = (tail + 1) % cap;
    Sem::sem_signal(mutexTail);

    Sem::sem_signal(itemAvailable);

}

int Buff::getCnt() {
    int ret;

    Sem::sem_wait(mutexHead);
    Sem::sem_wait(mutexTail);

    if (tail >= head) {
        ret = tail - head;
    } else {
        ret = cap - head + tail;
    }

    Sem::sem_signal(mutexTail);
    Sem::sem_signal(mutexHead);

    return ret;
}
