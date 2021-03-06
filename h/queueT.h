#ifndef QUEUET_H_
#define QUEUET_H_

#include "pcb.h"
#include "kernelSe.h"

class QueueTime {

    public:
            struct Elem {
            Elem *next; PCB *pcb;
            Time timeToWait;
            Elem(PCB *p, Time tm) {next=0; pcb=p; timeToWait=tm;}

            };


        
        QueueTime();
        ~QueueTime();

        
        void put(PCB* p, Time tm);

        void unblockWaiting();
        int getSize(); 
        void remove(PCB* p);
        PCB* getHeadPCB();
        Elem *head; 
        KernelSem *mySem;
        friend class Semaphore;
        friend class KernelSem;
       protected:
        int sz;

        int isThere(PCB* p);


};
#endif
