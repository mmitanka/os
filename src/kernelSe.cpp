#include "kernelSe.h"
#include "listSem.h"
#include "queue.h"
#include "queueT.h"
#include "system.h"
#include "SCHEDULE.H"

ListSem* KernelSem::allSemaphores=new ListSem();

KernelSem::KernelSem(Semaphore *sem, int v) {
	asm{
		pushf
		cli
	}
    mySem=sem; value=v;

    KernelSem::allSemaphores->put(this);
    blocked= new Queue();
    timeBlock=new QueueTime();


    asm {
    	popf
    }
}

KernelSem::~KernelSem() {
    System::setCritSection();



    delete blocked; 
    delete timeBlock;
    mySem=0;

    System::resetCritSection();

}

int KernelSem::signal(int n){
    if(n<0) return n;
    System::setCritSection();

    if(n==0) {
        if(value++<0 && (blocked->getSize()>0 ||timeBlock->getSize()>0)) {
                PCB* p=blocked->getHeadPCB();
                PCB* p2;
                p2=p;
                if(p!=0){
                    blocked->removeTh(p->myThr); //treba da stavim pcb u ready 
                }else {
                     p=timeBlock->getHeadPCB();
                     timeBlock->remove(p); 
                     p2=p;
                }
                if(p2) {
                    p2->phase=PCB::READY;
                    p2->returnVal=1; //one cuvaju ovo kao polje u pcb

                    Scheduler::put(p2);
                    System::resetCritSection();
                }

        }
        System::resetCritSection();
         return 0;

    }else {
        int i=0;
        while(n>0 &&(blocked->getSize()>0 ||timeBlock->getSize()>0)) {
            value++;
            if(value<=0) {
                PCB* p=blocked->getHeadPCB();
                PCB* p2;
                p2=p;
                if(p!=0){
                    blocked->removeTh(p->myThr); //treba da stavim pcb u ready 
                }else {
                     p=timeBlock->getHeadPCB();
                     timeBlock->remove(p); 
                     p2=p;
                }
                if(p2) {
                    i++;

                    p2->phase=PCB::READY;
                    p2->returnVal=1; 

                    Scheduler::put(p2);
                }
            }
            n--;

        }
        System::resetCritSection();
        return i;
    }
}

int KernelSem::wait(Time t) {
    System::running->returnVal=1;
    System::setCritSection();
    if(--value<0) {

        if(!t) {
            blocked->put((PCB*)System::running);

        }else {
            timeBlock->put((PCB*)System::running, t);

        }
        System::running->phase=PCB::BLOCKED;
        System::resetCritSection();
        System::dispatch();
    }else {
    System::resetCritSection();

    }
    return System::running->returnVal;
     
}

int KernelSem::val() const {
    return value;
}

void KernelSem::releaseSem() {
    while(timeBlock->getSize()>0) {
        PCB* p=timeBlock->getHeadPCB();
        PCB* p2;
        p2=p;
        timeBlock->remove(p);
        if(p2) {
                p2->phase=PCB::READY;
                p2->returnVal=1; 

                Scheduler::put(p2);
        }

    }
    while(blocked->getSize()>0) {
        PCB* p=blocked->getHeadPCB();
        PCB* p2=p;
        blocked->removeTh(p->myThr);
                if(p2) {
                p2->phase=PCB::READY;
                p2->returnVal=1; 

                Scheduler::put(p2);
        }
    }
}
