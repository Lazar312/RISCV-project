//
// Created by os on 2/7/23.
//

#ifndef PROJEKAT2_CONSOLEIO_HPP
#define PROJEKAT2_CONSOLEIO_HPP
#include "Buffer.hpp"
#include "../lib/hw.h"
#include "syscall_cpp.hpp"
#include "../h/riscv.hpp"
class ConsoleIO {
public:
    static void input(void *a);
    static void output(void *a);

    static BufferIO bufferInput;
    static BufferIO bufferOutput;

    static _sem *itemAvailvable;
    static _sem *spaceAvailable;
    static _sem *outputSEM;
    static _sem *outputSEMF;
    static _sem *outputSEMABC;

    friend Riscv;
};
#endif //PROJEKAT2_CONSOLEIO_HPP
