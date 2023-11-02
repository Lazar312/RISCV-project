//
// Created by os on 2/4/23.
//
#include "../h/Semaphore.h"
#include "../h/ccb.hpp"
int _sem::wait() {
    if(--val < 0){
        blocked->addLast(TCB::running);
        TCB::running->setBlock(true);
        TCB::timeSliceCounter = 0;
        TCB::yield();
    }

    if(TCB::running->semdeleted){
        return -1;
    }
    return 0;
}

int _sem::Signal() {
    if(++val <= 0){
        if(blocked->peekFirst() != nullptr){
            TCB *c = blocked->removeFirst();
            c->setBlock(false);
            Scheduler::put(c);
        //TCB::yield();
        }
    }

    return 0;
}

_sem::~_sem() {
    delete blocked;
}

void _sem::closing() {
    if(blocked->peekFirst() != nullptr){
        TCB *c;
        while( (c = blocked->removeFirst()) != nullptr ){
            c->setSemdeleted(true);
            c->setBlock(false);
            Scheduler::put(c);
            TCB::yield();
        }
    }
}
