//
// Created by marko on 20.4.22..
//

#include "../h/ccb.hpp"
//#include "../h/workers.hpp"
//#include "../h/print.hpp"
#include "../h/syscall_c.h"
#include "../h/usermain.hpp"
#include "../h/riscv.hpp"
#include "../h/ConsoleIO.hpp"
#include "../h/printing.hpp"

int main()
{
    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);
    TCB *t1 = TCB::createCoroutine(nullptr, nullptr , nullptr);
    TCB::running = t1;
    TCB *c2 = TCB::createCoroutine(ConsoleIO::input, nullptr, new uint64[1024]);
    TCB *c3 = TCB::createCoroutine(ConsoleIO::output, nullptr, new uint64[1024]);
    TCB *c1 = TCB::createCoroutine(userMain, nullptr, new uint64[1024]);

    //sem_open(&ConsoleIO::outputSEMF,0);
    sem_open(&ConsoleIO::outputSEMABC,0);
    sem_open(&ConsoleIO::spaceAvailable,0);
    sem_open(&ConsoleIO::itemAvailvable,0);

    /*sem_open(&ConsoleIO::itemAvailvable,0);
    sem_open(&ConsoleIO::spaceAvailable,0);*/
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);
    /*if(t1 == nullptr){
        printString("Nullpointer exception\n");
        printString("Error code: -1\n");
        return -1;
    }*/
    while(!c1->isFinished() ){
        thread_dispatch();
    }



    delete c3;
    delete c1;
    delete c2;
    delete t1;
    printString("FINISHED WITH EXIT CODE 0\n");

    while(ConsoleIO::bufferOutput.peakFron() !=0);

    return 0;

}
