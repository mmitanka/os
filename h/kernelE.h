
#ifndef KERNELE_H_
#define KERNELE_H_
#include "event.h"
#include "pcb.h"
#include "IVTEntry.h"
#include "system.h"
#include "kernelE.h"
#include "SCHEDULE.H"



class KernelEv {
    public:
        KernelEv(IVTNo n);
        ~KernelEv();

        void wait();
        void signal();

        PCB* owner;
        PCB* blocked;
        IVTNo myIVTEntry;
        int value; 

        friend class Event;
        friend class IVTEntry;


       
};
#endif
