//
// Created by os on 2/7/23.
//
#include "../h/ConsoleIO.hpp"
BufferIO ConsoleIO::bufferInput;
BufferIO ConsoleIO::bufferOutput;
_sem* ConsoleIO::itemAvailvable  ;
_sem* ConsoleIO::spaceAvailable  ;
_sem* ConsoleIO::outputSEM  ;
_sem* ConsoleIO::outputSEMF ;
_sem* ConsoleIO::outputSEMABC;
void ConsoleIO::input(void *a) { //getc
        while(true){
            if(*(char*)CONSOLE_STATUS & CONSOLE_RX_STATUS_BIT) {
                bufferInput.addLast(*(char*)CONSOLE_RX_DATA);
                sem_signal(itemAvailvable);
                sem_wait(spaceAvailable);
            }
            thread_dispatch();
        }
    }
void ConsoleIO::output(void *a) { // putc
    while(true){
        char c;
        if(*(char*)CONSOLE_STATUS & CONSOLE_TX_STATUS_BIT){
            //plic_complete(CONSOLE_IRQ);
            //Riscv::mc_sip(Riscv::SIP_SSIP);
            while(bufferOutput.peakFron() != 0) {

                c = bufferOutput.removeFirst();
                if(c == 0){
                    break;
                }else{
                    sem_wait(ConsoleIO::outputSEMABC);
                    *(char *)CONSOLE_TX_DATA = c;
                }

            }
            //Riscv::ms_sip(Riscv::SIP_SSIP);

        }
        thread_dispatch();
    }
}