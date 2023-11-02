//
// Created by marko on 20.4.22..
//

#ifndef OS1_VEZBE07_RISCV_CONTEXT_SWITCH_1_SYNCHRONOUS_LIST_HPP
#define OS1_VEZBE07_RISCV_CONTEXT_SWITCH_1_SYNCHRONOUS_LIST_HPP
//#include "printing.hpp"
#include "../lib/mem.h"
template<typename T>
class List
{
private:
    struct Elem
    {
        T *data;
        Elem *next;

        Elem(T *data, Elem *next) : data(data), next(next) {}
    };

    Elem *head, *tail;
    mutable Elem *curent, *previous;
public:
    List() : head(0), tail(0),curent(0),previous(0) {}

    List(const List<T> &) = delete;

    List<T> &operator=(const List<T> &) = delete;

    void addFirst(T *data)
    {
        if(!data){ return;}
        Elem *elem = (Elem*) __mem_alloc(sizeof(Elem));
        elem->data = data;
        elem->next = head;
        head = elem;
        if (!tail) { tail = head; }
    }

    void addLast(T *data)
    {
        /*if(!data){ return;}
        Elem *elem = (Elem*) __mem_alloc(sizeof(Elem));
        elem->data = data;
        elem->next = nullptr;
        if (tail)
        {
            tail->next = elem;
            tail = elem;
        } else
        {
            head = tail = elem;
        }*/
        if(!data){ return;}
        Elem *elem = (Elem*) __mem_alloc(sizeof(Elem));
        elem->data = data;
        elem->next = nullptr;
        if (!head)
        {
            head = tail = elem;
        } else
        {
            tail->next = elem;
            tail = tail->next;
        }
    }

    T *removeFirst()
    {
        if (!head) { return 0; }

        Elem *elem = head;
        head = head->next;
        if (!head) { tail = 0, head = 0; }

        T *ret = elem->data;
        __mem_free(elem);
        return ret;
    }

    T *peekFirst()
    {
        if (!head) { return 0; }
        return head->data;
    }
    T *removeLast()
    {
        if (!head) { return 0; }

        Elem *prev = 0;
        for (Elem *curr = head; curr && curr != tail; curr = curr->next)
        {
            prev = curr;
        }

        Elem *elem = tail;
        if (prev) { prev->next = 0; }
        else { head = 0; }
        tail = prev;

        T *ret = elem->data;
        delete elem;
        return ret;
    }

    T *peekLast()
    {
        if (!tail) { return 0; }
        return tail->data;
    }
    T *getCurrent(){
        return curent->data;
    }
    void next(){
        previous = curent;
        curent = curent->next;
    }
    void onFirst(){
        curent = head;
    }
    bool hasCurrent(){
        return curent != nullptr;
    }
    void removeCurrent(){
        if (!head) { return ; }
        if (head == curent){
            if(tail == head){
                delete head;
                head = tail = previous = curent = 0;
            }else{
                Elem *t = head;
                head = head->next;
                previous = curent;
                curent = curent->next;
                delete t;
            }
        }else {
            Elem *t;
            previous->next = curent->next;
            t = curent;
            curent = curent->next;
            if (!curent) { tail = 0; }
            delete t;
        }
    }
};

#endif //OS1_VEZBE07_RISCV_CONTEXT_SWITCH_1_SYNCHRONOUS_LIST_HPP
