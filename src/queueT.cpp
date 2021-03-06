#include "queueT.h"
#include "pcb.h"
#include "SCHEDULE.H"
#include "kernelSe.h"

QueueTime::QueueTime() {
    head=0; sz=0;

}

QueueTime::~QueueTime() {
	delete head;
}

void QueueTime::put (PCB* pcb,Time tm) {
    if(!pcb || tm<0) return ;
    if(isThere(pcb))
        return;
    
    Elem* el= new Elem(pcb, tm);

    if(getSize()==0) {
        head=el; sz++;
    } else{
        Elem *cur; cur=head;
        Elem* prev=0;
        int t=head->timeToWait;
        while(el->timeToWait>t &&cur) {
            prev=cur;
            cur=cur->next;
            if(cur) {
            	t+=cur->timeToWait;
            }

        }
        if(prev) {
            if(cur) { //nije puklo zbog kraja
                el->timeToWait-=(t-cur->timeToWait);
                cur->timeToWait-=el->timeToWait;
                prev->next=el;
                el->next=cur;
            } else {
                el->timeToWait-=t;
                prev->next=el;
            }
        }else { //nase vreme za cekanje je manje od vremena prvog el
            head->timeToWait-=el->timeToWait;
            el->next=head;
            head=el;
        }

        sz++;
        cur=prev=0;
    }

}
void QueueTime::unblockWaiting() {
    if(head==0) return; 
    Elem* cur; cur=head;
    cur->timeToWait--;
    PCB *p=cur->pcb;
    while(head&& head->timeToWait<=0){
    	p=head->pcb;
    	Elem* posl=0;
    	if(!head->next) {

    		delete head; head=0;
    	}else {
    		cur=head;
    		head=head->next;
    		delete cur;

    	}
        sz--;
    	if(cur->pcb==0) break;
        cur->pcb->phase=PCB::READY;

        PCB* p2=p;


        p2->returnVal=0;
        Scheduler::put(p2);


    }

}

int QueueTime::getSize() {
    return sz;
}

int QueueTime::isThere(PCB* p) {
    Elem* cur; 
    cur=head; 
    int ret=0;

    while(cur) {
        if(cur->pcb==p){
            ret=1;
            break;
        }
        cur=cur->next;
    }
    return ret; 

}

void QueueTime::remove(PCB* p) {
    Elem* cur; 
    cur=head;
    Elem* prev=0;
    while(cur && cur->pcb->id!=p->id){
        prev=cur; cur=cur->next;

    }
    if(cur) {
        if(prev){
            if(cur->next) {
            cur->next->timeToWait+=cur->timeToWait;
            prev->next=cur->next;

            delete cur;
            } else {//poslednji el
                prev->next=0; delete cur;
            }

        }else { //ovde mi je prev=0 a to je u slucaju da smo na prvom elemntu
            if(head->next){
                head->next->timeToWait+=cur->timeToWait;
                head=head->next;

                 delete cur;
            }else {//imamo jedan el u listi
                delete head;
                 head=0;
            }
        } 
        sz--;
    }
}

PCB* QueueTime::getHeadPCB() {
    if(!head) return 0;
    return head->pcb;
}
