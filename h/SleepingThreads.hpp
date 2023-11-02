//
// Created by os on 2/9/23.
//

#ifndef PROJEKAT2_SLEEPINGTHREADS_HPP
#define PROJEKAT2_SLEEPINGTHREADS_HPP

#include "ccb.hpp"

class SleepingThread{
private:
    struct Elem
    {   TCB *data;
        Elem *next;

        Elem(TCB* data, Elem *next) : data(data), next(next) {}
    };
    Elem * head;
public:
    SleepingThread(): head(0){}

    void add(TCB *tcb){
        Elem *current = head, *previous = nullptr;
        uint64 time = tcb->getSleepingTime();
        while(current && (current->data->getSleepingTime() <= time)){
            time -= (int)current->data->getSleepingTime();
            previous = current;
            current = current->next;
        }
        tcb->setBlock(true);
        if(!previous){
            Elem *e = (Elem*)__mem_alloc(sizeof(Elem));
            e->data = tcb;
            e->next = current;
            if(head)head->data->setSleepingTime(head->data->getSleepingTime() - time);
            head = e;
        }else{
            tcb->setSleepingTime(time);
            if(current)current->data->setSleepingTime(current->data->getSleepingTime() - time);
            Elem *e = (Elem*)__mem_alloc(sizeof(Elem));
            e->data = tcb;
            e->next = current;
            previous->next = e;
        }
    }
    void remove(){
        if(!head)return;
        head->data->setSleepingTime(head->data->getSleepingTime() - 1);
        Elem *current = head;
        while(current && current->data->getSleepingTime() == 0){
            current->data->setBlock(false);
            Scheduler::put(current->data);
            current = current->next;
            head = current;
        }
    }
};
#endif //PROJEKAT2_SLEEPINGTHREADS_HPP
