#include "kernelE.h"
#include "event.h"
#include "pcb.h"
#include "system.h"

#include "IVTEntry.h"

KernelEv::KernelEv(IVTNo n) {
    System::setCritSection();
    blocked= 0;
    owner=(PCB*)System::running;
    value=0;
    //treba i da dodelimo ulaz
    myIVTEntry=n;
    IVTEntry::IVTtab[myIVTEntry]->myKEv=this;

    System::resetCritSection();
}

KernelEv::~KernelEv() {
    blocked=owner=0;
    //oznacimo ulaz u itv da je prazan
    IVTEntry* t=IVTEntry::getIVTEntry(myIVTEntry);
    if(t->myKEv==this) t->myKEv=0;
    IVTEntry::IVTtab[myIVTEntry]=0;
}

void KernelEv::wait() {
    if(System::running!=owner) return;
    if(value==0) {
        owner->phase=PCB::BLOCKED;
        blocked=owner;
        System::dispatch();

    }else {
        value=0;
    }
}


void KernelEv::signal() {

    if(blocked!=0 && blocked->phase==PCB::BLOCKED ){  //neko mi je bloknut 
        blocked->phase=PCB::READY;
        Scheduler::put(blocked);
        blocked=0;

    }else value=1;
    
    System::dispatch();
}

