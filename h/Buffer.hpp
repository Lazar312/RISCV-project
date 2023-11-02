//
// Created by os on 2/7/23.
//

#ifndef PROJEKAT2_BUFFER_HPP
#define PROJEKAT2_BUFFER_HPP
#include "../h/syscall_c.h"
class BufferIO{
private:
    struct Elem
    {   char data;
        Elem *next;

        Elem(char data, Elem *next) : data(data), next(next) {}
    };
    Elem * head, *tail;
public:
    BufferIO() : head(0), tail(0){}

    BufferIO(const BufferIO &) = delete;

    BufferIO &operator=(const BufferIO &) = delete;

    char peakFron(){
        if(!head){return 0;}
        return head->data;
    }
    void addLast(char data)
    {
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
    char removeFirst(){
        if (!head) { return 0; }

        Elem *elem = head;
        head = head->next;
        if(tail == elem)tail = nullptr;

        char ret = elem->data;
        //__mem_free(elem);
        return ret;
    }

    char removeLast(){
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

        char ret = elem->data;
        __mem_free (elem);
        return ret;
    }
};
#endif //PROJEKAT2_BUFFER_HPP
