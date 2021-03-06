/*
 * idle.cpp
 *
 *  Created on: Aug 24, 2020
 *      Author: OS1
 */

#include "idle.h"
#include "pcb.h"
#include "system.h"
#include <iostream.h>
#include "thread.h"

#define idleRun 1


Idle::Idle():Thread(defaultStackSize,1) {}

Idle::~Idle() {
	waitToComplete();
}

void Idle::run(){
    while(idleRun) {
    	for(int i=0;i<2000;i++);
    }
}

void Idle::start() {
    myPCB->startIdleThread();

}





