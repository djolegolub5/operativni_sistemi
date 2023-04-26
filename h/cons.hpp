//
// Created by os on 7/26/22.
//

#ifndef PROJECT_BASE_V1_1_CONS_HPP
#define PROJECT_BASE_V1_1_CONS_HPP
#include "buff.hpp"


class Cons{
public:
    friend class PCB;
    static void putc(char c);
    static char getc();

    static PCB* pnit;
    static Buff* gbuffer;
    static Buff* pbuffer;
};



#endif //PROJECT_BASE_V1_1_CONS_HPP
