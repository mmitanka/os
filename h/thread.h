/*
 * thread.h
 *
 *  Created on: Aug 14, 2020
 *      Author: OS1
 */

#ifndef THREAD_H_
#define THREAD_H_

typedef unsigned long StackSize;
const StackSize defaultStackSize =4096;

typedef unsigned int Time;
const Time defaultTimeSlice=2;

typedef int ID;


class PCB;

class  Thread {
	public:

		virtual ~Thread();

		void start();
		void waitToComplete();

		ID getId();
		static ID getRunningId();
		static Thread* getThreadById(ID id);

		PCB* getMyPCB();
	protected:

		Thread(StackSize stackSize=defaultStackSize, Time tSl=defaultTimeSlice);
     	virtual void run() {}

    	friend void interrupt timer();
		friend class System;
		friend class Idle;
		friend class PCB;
		friend class Queue;
		friend class QueueTime;

	private:
		PCB* myPCB;

};

void dispatch();

#endif /* THREAD_H_ */
