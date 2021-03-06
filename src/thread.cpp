/*
 * thread.cpp
 *
 *  Created on: Aug 14, 2020
 *      Author: OS1
 */
#include "thread.h"
#include "pcb.h"
#include "system.h"


#define MAXSTACK 65536

Thread::Thread(StackSize ssz, Time tmsl) {
    if(tmsl<0) tmsl=defaultTimeSlice;
    if(ssz>MAXSTACK) ssz=MAXSTACK;

    System::setCritSection();
        myPCB= new PCB(this, ssz,tmsl);
    System::resetCritSection();
    System::call_dispatch();
}

Thread::~Thread() {
	waitToComplete();
	System::setCritSection();
	delete myPCB;
	System::resetCritSection();
	System::call_dispatch();
}


PCB* Thread::getMyPCB() {
    return myPCB;
}

ID Thread::getId(){
	return myPCB->id;
}

Thread* Thread::getThreadById(ID id){

	return PCB::getThreadById(id);

}

void dispatch() {
	System::dispatch();
}
void Thread::start() {
    myPCB->startThread();
}

void Thread::waitToComplete(){
    myPCB->waitToComplete();
}
ID Thread::getRunningId() {
    System::setCritSection();
    ID i= System::getRunningId();
    System::resetCritSection();
    System::call_dispatch();
    return i;
}



