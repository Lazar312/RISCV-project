//
// Created by marko on 20.4.22..
//

#include "../h/ccb.hpp"
#include "../h/riscv.hpp"

TCB *TCB::running = nullptr;
uint64 TCB::timeSliceCounter = 0;
int TCB::idThread = 0;
//SleepingThread TCB::sleepingThreads;
TCB *TCB::createCoroutine(Body body, void * args, void* s){
    TCB* t = new TCB(body, args, s, TIME_SLICE);
    if(t->body != nullptr)Scheduler::put(t);
    return t;
}
TCB *TCB::createCoroutineThread(Body body, void * args, void* s)
{
    return new TCB(body, args, s, TIME_SLICE);
}

void TCB::yield()
{
    Riscv::pushRegisters();

    TCB::dispatch();

    Riscv::popRegisters();
}

void TCB::dispatch()
{
    TCB *old = running;
    if (!old->isFinished() && !running->blocked) { Scheduler::put(old); }
    running = Scheduler::get();

    TCB::contextSwitch(&old->context, &running->context);
}
void TCB::threadWrapper()
{
    Riscv::popSppSpie();
    running->body(running->args);
    thread_exit();
}

TCB::~TCB() {
    delete[] stack;
}

/*void TCB::sleep(uint64 time) {
    TCB::sleepingThreads.add(TCB::running);
    running->blocked = true;
    running->spleepingTime = (int)time;
    //TCB::yield();
    //running->blocked = false;
}*/

/*void TCB::wakeup() {
    sleepingThreads.onFirst();
    TCB* current;
    //bool doYield = false;
    while( sleepingThreads.hasCurrent()){
        current = sleepingThreads.getCurrent();
        if(--current->spleepingTime <= 0){
            current->blocked = false;
            Scheduler::put(current);// ulaze u minus timeri
            sleepingThreads.removeCurrent();
            //doYield = true;
        }else{
            sleepingThreads.next();
        }
    }
    //if(doYield)yield();
}*/
