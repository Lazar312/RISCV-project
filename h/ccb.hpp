//
// Created by marko on 20.4.22..
//

#ifndef OS1_VEZBE07_RISCV_CONTEXT_SWITCH_1_SYNCHRONOUS_CCB_HPP
#define OS1_VEZBE07_RISCV_CONTEXT_SWITCH_1_SYNCHRONOUS_CCB_HPP

#include "../lib/hw.h"
#include "scheduler.hpp"
#include "syscall_c.h"
//#include "SleepingThreads.hpp"
// Coroutine Control Block
class TCB
{
public:
    ~TCB();

    //static SleepingThread sleepingThreads;



    bool isFinished() const { return finished; }

    void setFinished(bool value) { finished = value; }

    using Body = void (*)(void *);

    void setSemdeleted(bool value){semdeleted = value;}

    static TCB *createCoroutine(Body body, void* a, void* s);

    static TCB *createCoroutineThread(Body body, void * args, void* s);

    static void yield();

    static TCB *running;

    void setBlock(bool block){blocked = block;}

    //static void sleep(uint64 time);

    //static void wakeup();

    uint64 getTimeSlice(){return timeSlice;}

    uint64 getSleepingTime(){return spleepingTime;}

    int  getId() const {return id;}

    void setSleepingTime(uint64 time){spleepingTime = time;}

    static uint64 timeSliceCounter;

    void *operator new(size_t n){
        return __mem_alloc(n);
    }
    void operator delete(void *p){
        __mem_free(p);
    }

private:
    explicit TCB(Body body, void* a, void* s, uint64 timeSlice) :
            body(body),
            stack(body != nullptr ? (uint64*) s : nullptr),
            context({(uint64) &threadWrapper,
                     stack != nullptr ? (uint64) &stack[STACK_SIZE] : 0
                    }),
            timeSlice(timeSlice),
            finished(false),
            args(a),
            id(idThread++)
    { }

    struct Context
    {
        uint64 ra;
        uint64 sp;
    };
    static void threadWrapper();
    bool semdeleted = false;
    Body body;
    uint64 *stack;
    Context context;
    uint64 timeSlice;
    bool finished;
    void * args;
    bool blocked = false;
    int id;
    uint64 spleepingTime = 0;
    static void contextSwitch(Context *oldContext, Context *runningContext);

    static void dispatch();
    static int idThread;
    static uint64 constexpr STACK_SIZE = 1024;
    static uint64 constexpr TIME_SLICE = 2;


    friend class Riscv;
    friend class _sem;

};

#endif //OS1_VEZBE07_RISCV_CONTEXT_SWITCH_1_SYNCHRONOUS_CCB_HPP
