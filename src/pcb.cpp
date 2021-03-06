 /*
 * pcb.cpp
 *
 *  Created on: Aug 17, 2020
 *      Author: OS1
 */
#include "pcb.h"
#include "thread.h"
#include "queue.h"
#include "system.h"
#include "SCHEDULE.H"
#include <iostream.h>

int PCB::globalID=0;
Queue* PCB::pcb_id_queue= new Queue();


PCB::PCB(Thread *th, StackSize stSize, Time tmSlice) {
		returnVal=0;
        myThr=th;
        st.stackSize=stSize/sizeof(unsigned); st.bp=st.sp=st.ss=0;

        st.timeSl=tmSlice;
        inicThread();
        pcb_id_queue->put(this);

}

PCB::~PCB() {
	System::setCritSection();
    delete []st.stack;
    delete waitingForMe;
    PCB::pcb_id_queue->removeTh(myThr);
    System::resetCritSection();

}
void PCB::wrapper() {

    System::running->myThr->run();



    System::setCritSection();
        if(System::running->phase!=PCB::IDLE){
            System::running->phase=PCB::FINISHED;
        PCB::findWaitingThread();}
    System::resetCritSection();
    System::dispatch();
}

void PCB::findWaitingThread() {
    PCB* cur=0;
    while(System::running->waitingForMe->head!=0) {
        cur=System::running->waitingForMe->get();
        cur->phase=PCB::READY;
        Scheduler::put(cur);
    }
}



void PCB::inicThread(){

    id=globalID++;

    phase=PCB::NEW;
    waitingForMe= new Queue();


    inicStack();
}

Thread* PCB::getThreadById(ID id) {
   return pcb_id_queue->getThByID(id);
}

void PCB::startThread() {
    System::setCritSection();
        phase=PCB::READY;
        Scheduler::put(this);
    System::resetCritSection();
}

void PCB::startIdleThread() {
    System::setCritSection();
        phase=PCB::IDLE;
    System::resetCritSection();
}

void PCB::inicStack() {

    if( (phase==PCB::NEW || phase== PCB::IDLE)) {
        System::setCritSection();
        unsigned *stack=new unsigned[st.stackSize];
        StackSize sz=st.stackSize;
        #ifndef BCC_BLOCK_IGNORE

		        stack[sz-1]=0x200;
				stack[sz-2]=FP_SEG(&(PCB::wrapper));
				stack[sz-3]=FP_OFF(&(PCB::wrapper));

				st.sp=FP_OFF(stack+sz-12);
                st.bp=st.sp;
				st.ss=FP_SEG(stack+sz-12);
        #endif
		st.stack=stack;
		stack=0;

        System::resetCritSection();
    }
}

void PCB::waitToComplete() {
    System::setCritSection();
        if((this->phase==PCB::IDLE) ||(this->phase==PCB::NEW) || (this->phase==PCB::FINISHED)|| (this->myThr==System::mainT)
        || (this==System::running) ) {

        	System::resetCritSection();
        	System::call_dispatch();
        	return;
        }
    System::running->phase=PCB::BLOCKED;

    this->waitingForMe->put((PCB*)System::running);

    System::resetCritSection();
    System::dispatch();
}



