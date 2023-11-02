//
// Created by os on 2/4/23.
//

#ifndef PROJEKAT2_SEMAPHORE_H
#define PROJEKAT2_SEMAPHORE_H

#include "list.hpp"
#include "ccb.hpp"
class _sem{
public:
    static _sem* createSem(int val){
            return new _sem(val);
        }
    void *operator new(size_t n){
        return __mem_alloc(n);
    }
    void operator delete(void *p){
        __mem_free(p);
    }
    int wait();
    int Signal();
    void closing();
    ~_sem();
private:
    _sem(int val){
            this->val = val;
            blocked = new List<TCB>;
        }
    int val;
    List<TCB> *blocked;


};

#endif //PROJEKAT2_SEMAPHORE_H
