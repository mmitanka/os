/*
 * pcb.h
 *
 *  Created on: Aug 14, 2020
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_
#include "thread.h";

class Queue;
class Thread;

class PCB {
public :
	PCB(Thread* th, StackSize ssz, Time tmsl);
	~PCB();
	 static int globalID;
	 static Queue* pcb_id_queue;

	 static void wrapper();
	 static Thread* getThreadById(ID id);

	 ID id;

	 enum Phase {NEW, READY, FINISHED, BLOCKED, IDLE, MAIN};
	 volatile Phase phase;

	 Thread *myThr;


	 struct Stack {
	    unsigned *stack;
	    unsigned sp, bp, ss;
	    StackSize stackSize;
		Time timeSl;
	 };
	 volatile Stack st;
	 int returnVal;
	 Queue* waitingForMe;

	 void startThread();
     void startIdleThread();
	 void waitToComplete();

	 void inicThread();
	 void inicStack();

	 static void findWaitingThread();

	 friend class Thread;
	 friend class System;
	 friend class Queue;

};



#endif /* PCB_H_ */
