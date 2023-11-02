//
// Created by os on 2/3/23.
//

#ifndef PROJECT_BASE_SYSCALL_CPP_HPP
#define PROJECT_BASE_SYSCALL_CPP_HPP
#include "../h/syscall_c.h"



class Thread {
public:
    Thread (void (*body)(void*), void* arg);

    virtual ~Thread ();
    int start ();
    static void dispatch ();
    static int sleep (time_t);

protected:
    static void wrapper(void * arg);
    Thread ();
    virtual void run () {}
private:
    thread_t myHandle;
};
class Semaphore {
public:
    Semaphore (unsigned init = 1);

    virtual ~Semaphore ();
    int wait ();
    int signal ();
private:
    sem_t myHandle;
};
class PeriodicThread : public Thread {
protected:
    PeriodicThread (time_t period);
    virtual void periodicActivation () {}
private:
    static void periodicfunction(void * arg);

};
class Console {
public:
    static char getc ();
    static void putc (char);
};


#endif //PROJECT_BASE_SYSCALL_CPP_HPP
