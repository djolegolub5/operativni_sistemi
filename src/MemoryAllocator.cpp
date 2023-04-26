//
// Created by os on 6/26/22.
//

#include "../h/MemoryAllocator.hpp"
#include "../lib/console.h"


Elem* MemoryAllocator::head=nullptr;
bool MemoryAllocator::flag=false;

void* MemoryAllocator::mem_alloc(size_t size) {


    if (!head){
        head=(Elem*)HEAP_START_ADDR;
        head->next=nullptr;
        size_t* kraj=(size_t*)(HEAP_END_ADDR);
        size_t* pocetak=(size_t*)(HEAP_START_ADDR);
        size_t velicina=sizeof(Elem);
        size_t x=((size_t)kraj-(size_t)pocetak);
        x-=velicina;
        head->size=x;

    }

    Elem* prev=nullptr;
    Elem* x=head;
    while (x && x->size<size+sizeof(Elem)){
        prev=x;
        x=x->next;

    }

    if (!x) return nullptr;

    size_t s=x->size;
    x->size=size;
    Elem* novi=(Elem*)((size_t)x+size+sizeof(Elem));
    novi->next=x->next;

    if (prev!=nullptr)prev->next=novi;
    else head=novi;
    novi->size=s-size-sizeof(Elem);
    x->next=nullptr;
    return (void*)((size_t)x+sizeof(Elem));




}
void MemoryAllocator::join(Elem* x){
    if (!x) return;
    if (!x->next)return;
    if ((Elem*)((size_t)x+x->size+sizeof(Elem))==x->next){
        x->size+=x->next->size+sizeof(Elem);
        x->next=x->next->next;
    }

}
int MemoryAllocator::mem_free(void *addr) {
    if (!addr)return -1;
    Elem* curr=head;
    if ((size_t)addr+sizeof(Elem)<(size_t)head)curr=0;

    else {
        while (curr->next != nullptr && (size_t)addr>((size_t)(curr->next)+sizeof(Elem)))curr = curr->next;
    }

    Elem* novi=(Elem*)((size_t)addr-sizeof(Elem));
    if (!curr) {
        novi->next = head;
        head=novi;
    }
    else {
        Elem* x=curr->next;
        curr->next=novi;
        novi->next=x;
    }

    join(novi);
    join(curr);


    return 0;
}



