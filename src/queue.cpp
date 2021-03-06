/*
 * queue.cpp
 *
 *  Created on: Aug 14, 2020
 *      Author: OS1
 */
#include "queue.h"
#include "pcb.h"
#include "thread.h"
#include "SCHEDULE.H"

Queue::Queue(){
    head=tail=0;
    sz=0;
}

Queue::~Queue(){
    Elem* tek=head;
    Elem* preth=0;
    while(tek){
       preth=tek;

       tek=tek->next;
       delete preth;


    }
    sz=0;
    delete head; delete tail; delete tek; delete preth;
}

void Queue::put(PCB* pcb){
    if(!pcb)  return;

    Elem *el;
    el=head;
    for(;el;el=el->next)
    	if(el->pcb==pcb)
    		return;


    el=new Elem(pcb);


    if(getSize()==0) {head=el; tail=el; sz++;}
    else {
        tail->next=el; tail=el;
        sz++;
    }
}
int Queue::getSize() {
	return sz;
}

PCB* Queue::get()
{
    if(getSize()==0 || !head) return 0;

    Elem *el=head; head=head->next;
    if(!head) tail=0;
    PCB* p=el->pcb;
    sz--;
    delete el;
    return p;

}

Thread* Queue::getThByID(ID id) {
    if(id<0 || getSize()<=0) return 0;

    Elem *cur=head;
    Thread* t=0;
    while(cur) {
        if(cur->pcb->id==id){
        	t=cur->pcb->myThr;
        	break;
        }


        cur=cur->next;
    }

    return t;

}

void Queue::removeTh(Thread * th) {
    PCB* pp=th->myPCB;
    Elem *cur=head, *prev=0;

    while(cur && cur->pcb!=pp) {
        prev=cur;
        cur=cur->next;
    }

    if(!cur) return;
    if(cur==head) {
    	if(head==tail)
    		tail=head->next;
        head=head->next;

    } else{

        prev->next=cur->next;
    	if(cur==tail)
    		tail=prev;
    }
    sz--;
    cur->next=0;
    delete cur;


}

PCB* Queue::getHeadPCB() {
    if( !head) return 0;

    PCB* p=head->pcb;
    return p;
}

void Queue::clearWaiting() {
	if(!head) return;
	Elem *tek= head;
	Elem* preth=0;
	head=head->next;
	while(getSize()!=0){
		tek->pcb->phase=PCB::READY;
		Scheduler::put(tek->pcb);
		preth=tek; sz--;

		tek=tek->next;
		delete preth;
	}
	if(!head)
		tail=0;

}
