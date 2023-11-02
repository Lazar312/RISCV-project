#include "../h/syscall_c.h"
#include "../h/ccb.hpp"

void translate(int arg1, void* arg2, void* args){
    //__asm__ volatile ("mv a3, %0" : : "r"(arg3));

    __asm__ volatile ("ecall");


}

void* mem_alloc (size_t size){
    void* ret;
    translate(0x01, &size, nullptr);
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return ret;
}
using size_t = decltype(sizeof(0));
int mem_free (void* p){
    int ret;
    translate(0x01, p, nullptr);
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return ret;
}
int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg){
    int i;
    args *t = new args ;
    t->arg3 = &start_routine;
    t->arg4 = arg;
    t->arg5 = new uint64[DEFAULT_STACK_SIZE];
    translate(0x11,handle,t);
    delete t;
    __asm__ volatile("mv %0, a0" : "=r"(i));
    return i;
}

int thread_exit (){
    int ret;
    translate(0x12, nullptr, nullptr);
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return ret;
}
int thread_make(thread_t* handle, void(*start_routine)(void*), void* arg){
    void *temp;
    args *t = new args ;
    t->arg3 = &start_routine;
    t->arg4 = arg;
    t->arg5 = new uint64[DEFAULT_STACK_SIZE];
    translate(0x50,handle,t);
    __asm__ volatile("mv %0, a0" : "=r"(temp));
    delete t;
    if(temp == nullptr){
        return -1;
    }
    return 0;
}
void thread_dispatch (){
    translate(0x13, nullptr, nullptr);
}
int sem_open (sem_t* handle, unsigned init
){
    void *temp;
    translate(0x21,handle,&init);
    __asm__ volatile ("mv %0, a0" : "=r"(temp));
    if(temp == nullptr){
        return -1;
    }
    return 0;
}
int sem_close (sem_t handle){
    int ret;
    translate(0x22,handle, nullptr);
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return ret;
}
int sem_wait (sem_t id){
    int ret;
    translate(0x23,id, nullptr);
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return ret;
}
int sem_signal (sem_t id){
    int ret;
    translate(0x24,id, nullptr);
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return ret;
}
void putc(char c){
    translate(0x41, &c, nullptr);
    //__putc(c);
}
char getc(){
    char ret;
    translate(0x42, nullptr, nullptr);
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return ret;
}
int time_sleep (time_t t){
    int ret;
    translate(0x31,&t, nullptr);
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return ret;
}