//#include "../h/Threads_C_API_test.hpp" // zadatak 2, niti C API i sinhrona promena konteksta
#include "../h/Threads_CPP_API_test.hpp" // zadatak 2., niti CPP API i sinhrona promena konteksta
//#include "../h/Threads_CPP_API2_test.hpp"
//#include "../h/ConsumerProducer_C_API_test.h" // zadatak 3., kompletan C API sa semaforima, sinhrona promena konteksta
//#include "../h/ConsumerProducer_CPP_Sync_API_test.hpp" // zadatak 3., kompletan CPP API sa semaforima, sinhrona promena konteksta
#include "../h/ThreadSleep_C_API_test.hpp" // thread_sleep test C API
#include "../h/ConsumerProducer_CPP_API_test.hpp" // zadatak 4. CPP API i asinhrona promena konteksta
//#include "../h/printing.hpp"
//#include "../h/test.hpp"
//#include "../h/TestPeriodic.hpp"
void userMain(void *args) {

    //testSleeping();// thread_sleep test C API
    //Threads_CPP_API2_test();
    //Threads_C_API_test(); // zadatak 2., niti C API i sinhrona promena konteksta
    //Threads_CPP_API_test(); // zadatak 2., niti CPP API i sinhrona promena konteksta

    //producerConsumer_C_API(); // zadatak 3., kompletan C API sa semaforima, sinhrona promena konteksta
    //producerConsumer_CPP_Sync_API(); // zadatak 3., kompletan CPP API sa semaforima, sinhrona promena konteksta

    ConsumerProducerCPP::testConsumerProducer(); // zadatak 4. CPP API i asinhrona promena konteksta, kompletan test svega
    //test();
    //testPeriodic();
    //printString("USER MAIN FINISHED\n");
}