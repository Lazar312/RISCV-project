//
// Created by os on 1/30/23.
//
#ifndef PROJECT_BASE_SYSCALL_C_HPP
#define PROJECT_BASE_SYSCALL_C_HPP

#include "../lib/hw.h"
#include "../lib/mem.h"
//#include "printing.hpp"
#include "../lib/console.h"
//#include "Semaphore.h"

extern const size_t DEFAULT_STACK_SIZE;
extern const time_t DEFAULT_TIME_SLICE;
typedef struct args{
    void * arg3;
    void * arg4;
    void * arg5;
}args;

void translate(int arg1, void* arg2, void *args);



void* mem_alloc (size_t size);
using size_t = decltype(sizeof(0));
int mem_free (void*);

class TCB;
typedef TCB* thread_t;

int thread_create (thread_t* handle, void(*start_routine)(void*), void* arg);
int thread_make(thread_t* handle, void(*start_routine)(void*), void* arg);
int thread_exit ();

void thread_dispatch ();

class _sem;
typedef _sem* sem_t;

int sem_open (sem_t* handle, unsigned init);

int sem_close (sem_t handle);

int sem_wait (sem_t id);

int sem_signal (sem_t id);

typedef unsigned long time_t;

int time_sleep (time_t);

char getc ();

const int EOF = -1;

void putc(char);

extern const void* HEAP_START_ADDR;
extern const void* HEAP_END_ADDR;
extern const size_t MEM_BLOCK_SIZE;



#endif //PROJECT_BASE_SYSCALL_C_HPP


