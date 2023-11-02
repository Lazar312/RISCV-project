//
// Created by os on 2/3/23.
//
#include "../h/syscall_c.h"
#include "../h/syscall_cpp.hpp"
#include "../h/ccb.hpp"
struct periodicArgs {
    PeriodicThread* pthread;
    time_t time;
};
void Thread::wrapper(void * arg) {
    Thread *t = (Thread*)arg;
    t->run();
}
void PeriodicThread::periodicfunction(void *arg) {
    periodicArgs *p = (periodicArgs*)arg;
    while(true){
        p->pthread->periodicActivation();
        Thread::sleep(p->time);
    }
}

Thread::Thread(void (*body)(void *), void *arg) {
    thread_t *t = new thread_t ;
    myHandle = *t;
    thread_create(&myHandle,body,arg);
}

void Thread::dispatch() {
    thread_dispatch();
}

int Thread::start() {
    if(myHandle != nullptr){
        Scheduler::put(myHandle);
        return 0;
    }else{
        return -1;
    }
}



Thread::~Thread() {
    delete myHandle;
}

int Thread::sleep(time_t t) {
    return time_sleep(t);
}

Thread::Thread() {
    thread_t *t = new thread_t ;
    myHandle = *t;
    thread_make(&myHandle,wrapper, this);
}

Semaphore::Semaphore(unsigned int init) {
    sem_open(&myHandle,init);
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

char Console::getc() {
    return ::getc();
}

void Console::putc(char c) {
    ::putc(c);
}

PeriodicThread::PeriodicThread(time_t period) :
Thread(periodicfunction, new periodicArgs{this,period}) {  }



