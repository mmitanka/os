/*
 * queue.h
 *
 *  Created on: Aug 14, 2020
 *      Author: OS1
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#include "thread.h"
class PCB;



class Queue{
public:
        struct Elem {
            Elem *next; PCB *pcb;
            Elem(PCB *p) {next=0; pcb=p;}

        };


        Queue();
        ~Queue();

        PCB* get();
        void put(PCB* pcb);

        int getSize ();

        Elem *head,*tail;
        Thread* getThByID(ID id);

        void removeTh(Thread* th);
        PCB* getHeadPCB();
        void clearWaiting();


    protected:
        void isLastEl();
        int sz;

};




#endif /* QUEUE_H_ */
