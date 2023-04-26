//
// Created by os on 7/13/22.
//

#ifndef PROJECT_BASE_V1_1_LIST_HPP
#define PROJECT_BASE_V1_1_LIST_HPP

#include "MemoryAllocator.hpp"


template<typename T>
class List
{
private:
    struct Elem
    {
        T *data;
        Elem *next;

        Elem(T *data, Elem *next) : data(data), next(next) {}
        void * operator new(size_t size){

            size_t pom=size/MEM_BLOCK_SIZE;
            pom*=MEM_BLOCK_SIZE;
            if (size%MEM_BLOCK_SIZE!=0)pom+=MEM_BLOCK_SIZE;
            size=pom;
            return MemoryAllocator::mem_alloc(size);

        }
        void operator delete(void * addr) noexcept{
            MemoryAllocator::mem_free(addr);
        }

        void *operator new[](size_t size){
            size_t pom=size/MEM_BLOCK_SIZE;
            pom*=MEM_BLOCK_SIZE;
            if (size%MEM_BLOCK_SIZE!=0)pom+=MEM_BLOCK_SIZE;
            size=pom;
            return MemoryAllocator::mem_alloc(size);

        }
        void operator delete[](void* addr) noexcept{
            MemoryAllocator::mem_free(addr);


        }


    };

    Elem *head, *tail;

public:

    void * operator new(size_t size){

        size_t pom=size/MEM_BLOCK_SIZE;
        pom*=MEM_BLOCK_SIZE;
        if (size%MEM_BLOCK_SIZE!=0)pom+=MEM_BLOCK_SIZE;
        size=pom;
        return MemoryAllocator::mem_alloc(size);

    }
    void operator delete(void * addr) noexcept{
        MemoryAllocator::mem_free(addr);
    }

    void *operator new[](size_t size){
        size_t pom=size/MEM_BLOCK_SIZE;
        pom*=MEM_BLOCK_SIZE;
        if (size%MEM_BLOCK_SIZE!=0)pom+=MEM_BLOCK_SIZE;
        size=pom;
        return MemoryAllocator::mem_alloc(size);

    }
    void operator delete[](void* addr) noexcept{
        MemoryAllocator::mem_free(addr);


    }




    List() : head(0), tail(0) {}

    List(const List<T> &) = delete;

    List<T> &operator=(const List<T> &) = delete;

    void addFirst(T *data)
    {
        Elem *elem = new Elem(data, head);
        head = elem;
        if (!tail) { tail = head; }
    }

    void addLast(T *data)
    {
        Elem *elem = new Elem(data, 0);
        if (tail)
        {
            tail->next = elem;
            tail = elem;
        } else
        {
            head = tail = elem;
        }
    }

    T *removeFirst()
    {
        if (!head) { return 0; }

        Elem *elem = head;
        head = head->next;
        if (!head) { tail = 0; }

        T *ret = elem->data;
        delete elem;

        return ret;
    }

    T *peekFirst()
    {
        if (!head) { return 0; }
        return head->data;
    }

    T *removeLast()
    {
        if (!head) { return 0; }

        Elem *prev = 0;
        for (Elem *curr = head; curr && curr != tail; curr = curr->next)
        {
            prev = curr;
        }

        Elem *elem = tail;
        if (prev) { prev->next = 0; }
        else { head = 0; }
        tail = prev;

        T *ret = elem->data;
        delete elem;
        return ret;
    }

    T *peekLast()
    {
        if (!tail) { return 0; }
        return tail->data;
    }
};






#endif //PROJECT_BASE_V1_1_LIST_HPP
