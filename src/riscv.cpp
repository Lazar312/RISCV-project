//
// Created by marko on 20.4.22..
//
#include "../h/usermain.hpp"
#include "../h/riscv.hpp"
//#include "../h/printing.hpp"
#include "../h/ccb.hpp"
#include "../h/Semaphore.h"
#include "../h/ConsoleIO.hpp"
#include "../h/SleepingThreads.hpp"
SleepingThread sleeping = SleepingThread();
int i = 0;
uint64 geta1(){
    uint64 temp;
    __asm__ volatile ("mv %0, a1" : "=r"(temp));
    return temp;
}
uint64 geta2(){
    uint64 temp;
    __asm__ volatile ("mv %0, a2" : "=r"(temp));
    return temp;
}
void Riscv::popSppSpie()
{
    __asm__ volatile("csrw sepc, ra");
    if ( i++ >= 0) mc_sstatus(Riscv::SSTATUS_SPP);
    __asm__ volatile("sret");
}

void Riscv::handleSupervisorTrap()
{
    uint64 volatile sepc = r_sepc();
    uint64 volatile sstatus = r_sstatus();
    uint64 opcode ;
    //printInteger(arg1);

    __asm__ volatile ("mv %0, a0" : "=r"(opcode));
    uint64 scause = r_scause();
    if (scause == 0x0000000000000009UL ||  scause == 0x0000000000000008UL) {
        sepc += 4;
        if(opcode == 0x01){
            size_t *size = (size_t*)geta1();
            void* ret = __mem_alloc(*size);
            __asm__ volatile ("mv a0, %0" : : "r"(ret));
        }else if(opcode == 0x02){
            void *p = (void*)geta1();
            int ret = __mem_free(p);
            __asm__ volatile ("mv a0, %0" : : "r"(ret));
        }
        else if(opcode == 0x11) {// thread_create
            args *a = (args*)geta2();
            void(**start_routine)(void*) = (void (**)(void *))a->arg3;
            TCB **handle = (TCB**)geta1();
            *handle = TCB::createCoroutine(*start_routine, a->arg4, a->arg5);
            __asm__ volatile ("mv a0, %0" : : "r"(-1));


        }else if(opcode == 0x12){ //thread_exit
            TCB::running->finished = true;
            TCB::yield();
            __asm__ volatile ("mv a0, %0" : : "r"(-1));
        }
        else if(opcode == 0x13){// thread__dispatch()
            TCB::yield();

        }else if(opcode == 0x21){// sem_open

            int * init = (int *)geta2();
            _sem **handle = (_sem**)geta1();
            if(scause == 9){
                (*handle) = _sem::createSem(*init);
            }else if (scause == 8){
                (*handle) = _sem::createSem(*init);
            }
            __asm__ volatile ("mv a0, %0" : : "r"(*handle));

        }else if(opcode == 0x22){// sem_close
            _sem *handle = (_sem*)geta1();
            handle->closing();
            delete handle;
            __asm__ volatile ("mv a0, %0" : : "r"(0));

        }else if(opcode == 0x23){// sem_wait
            _sem *handle = (_sem*)geta1();
            int ret = handle->wait();
            __asm__ volatile ("mv a0, %0" : : "r"(ret));

        }else if(opcode == 0x24){// sem_signal
            _sem *handle = (_sem*)geta1();
            int ret = handle->Signal();
            __asm__ volatile ("mv a0, %0" : : "r"(ret));

        }else if(opcode == 0x31){//time_sleep
            auto *time = (uint64*)geta1();
            TCB::running->setSleepingTime(*time);
            if((*time) > 0){
                sleeping.add(TCB::running);
                TCB::yield();
            }
        }else if(opcode == 0x41){//putc
            char *c = (char*)geta1();
            ConsoleIO::bufferOutput.addLast(*c);
            ConsoleIO::outputSEMABC->Signal();
            //TCB::yield();
            //ConsoleIO::outputSEM->wait();
            //moze imati gresku

        }else if(opcode == 0x42) {//getc
            ConsoleIO::itemAvailvable->wait();
            char c = ConsoleIO::bufferInput.removeFirst();
            ConsoleIO::spaceAvailable->Signal();
            __asm__ volatile ("mv a0, %0" : : "r"(c));

        }else if(opcode == 0x50){
            args *a = (args*)geta2();
            void(**start_routine)(void*) = (void (**)(void *))a->arg3;
            TCB **handle = (TCB**)geta1();
            (*handle) = TCB::createCoroutineThread(*start_routine, a->arg4, a->arg5);
            __asm__ volatile ("mv a0, %0" : : "r"(*handle));
        }
        else{
            TCB::timeSliceCounter = 0;
            TCB::yield();

        }
        w_sstatus(sstatus);
        w_sepc(sepc);
    }
    else if (scause == 0x8000000000000001UL){
        mc_sip(SIP_SSIP);
        TCB::timeSliceCounter++;
        sleeping.remove();
        if(TCB::timeSliceCounter >= TCB::running->getTimeSlice()){
            uint64 volatile sepc = r_sepc() ;
            uint64 volatile sstatus = r_sstatus();
            TCB::timeSliceCounter = 0;
            TCB::yield();
            w_sstatus(sstatus);
            w_sepc(sepc);
        }
    }
    else if (scause == 0x8000000000000009UL)
    {
        int i = plic_claim();

        plic_complete(i);
        //console_handler();
        // interrupt: yes; cause code: supervisor external interrupt (PLIC; could be keyboard)

    }
    else
    {

        // unexpected trap cause
    }
}