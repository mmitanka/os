/*
 * system.cpp
 *
 *  Created on: Aug 17, 2020
 *      Author: OS1
 */
#include "system.h"
#include "pcb.h"
#include "thread.h"
#include "queue.h"
#include "SCHEDULE.H"
#include <dos.h>
#include <iostream.h>
#include "idle.h"
#include "kernelSe.h"
#include "listSem.h"

extern void tick();

unsigned tss,tsp,tbp;

unsigned oldOFF, oldSEG;
int System::usao=-1;

unsigned int System::crit_section=1;
int System::cnt=1;
unsigned int System::ekspl_promena_cntx=0;

Thread* System::mainT=0;
volatile PCB* System::running=0; Idle* System::idle=0;


void interrupt timer() {
    if(System::getContextDemandFlag()!=0) {
        System::usao=1;
    }else {
        tick();

	System::unblockWaiting();


        if(System::cnt>0)
        System::cnt--;
        System::usao=0;

    }
    if(( System::running->st.timeSl>0  && System::cnt==0 )|| System::getContextDemandFlag()==1) {
        if (System::getCritSection()==1) {

            System::resetContextSwitch();
			#ifndef BCC_BLOCK_IGNORE
            asm {
                mov tsp, sp
                mov tss, ss
                mov tbp,bp
		    }
			#endif
            System::running->st.ss=tss;
            System::running->st.sp=tsp;
            System::running->st.bp=tbp;


            if(System::running->phase==PCB::READY) Scheduler::put((PCB*)System::running);

            System::running=Scheduler::get();
            if(!System::running){
            	System::running=System::idle->myPCB;
            }
            tss=System::running->st.ss;
            tsp=System::running->st.sp;
            tbp=System::running->st.bp;

            System::cnt=System::running->st.timeSl;

			#ifndef BCC_BLOCK_IGNORE
            asm {
                mov sp, tsp
                mov ss, tss
                mov bp, tbp
		    }
			#endif
        }else  {
            System::setContextSwitch();
        }

    }
    if(!System::usao) {

    	asm int 60h;

    	}
}


void System::setCritSection() {
    crit_section=0;
}
void System::resetCritSection() {
    crit_section=1;
}

unsigned int System::getCritSection() {
    return crit_section;
}

void System::setContextSwitch(){
    ekspl_promena_cntx=1;
}
void System::resetContextSwitch(){
    ekspl_promena_cntx=0;

}

unsigned int System::getContextDemandFlag() {

    return ekspl_promena_cntx;
}


void System::dispatch() {
	asm {
		pushf
		cli
	}
      System::ekspl_promena_cntx=1;
      timer();
    asm popf
}
void System::call_dispatch() {
	if(System::getContextDemandFlag())
		System::dispatch();
}

ID System::getRunningId() {
    return System::running->id;
}

void System::inic() {
         asm {
       	  pushf
		  cli
         }
		#ifndef BCC_BLOCK_IGNORE
    	asm{
	  		cli
	  		push es
	  		push ax

	  		mov ax,0   //  ; inicijalizuje rutinu za tajmer
	  		mov es,ax

	  		mov ax, word ptr es:0022h //; pamti staru rutinu
	  		mov word ptr oldSEG, ax
	  		mov ax, word ptr es:0020h
	  		mov word ptr oldOFF, ax

	  		mov word ptr es:0022h, seg timer	 //postavlja
	  		mov word ptr es:0020h, offset timer //novu rutinu

	  		mov ax, oldSEG	 //	postavlja staru rutinu
	  		mov word ptr es:0182h, ax //; na int 60h
	  		mov ax, oldOFF
	  		mov word ptr es:0180h, ax

	  		pop ax
	  		pop es
	  		sti
	  	}
		#endif



    	mainT=new Thread(1024, 1);
    	mainT->myPCB->phase=PCB::READY;

    	System::running=(PCB*)System::mainT->myPCB;
    	idle=new Idle();
    	idle->start();

    	asm {
    		popf
    	}


}

void System::restore() {
	if((PCB*)System::running!=System::mainT->myPCB) return;
    asm {
  	  pushf
	  cli
    }
		#ifndef BCC_BLOCK_IGNORE
    	asm {
	  		cli
	  		push es
	  		push ax

	  		mov ax,0
	  		mov es,ax


	  		mov ax, word ptr oldSEG
	  		mov word ptr es:0022h, ax
	  		mov ax, word ptr oldOFF
	  		mov word ptr es:0020h, ax

	  		pop ax
	  		pop es
	  		sti
	  	}
		#endif


          delete PCB::pcb_id_queue;
          delete KernelSem::allSemaphores;
          delete mainT;
          delete idle;
          asm {
        	  popf
          }



}

void System::unblockWaiting() {
		for(ListSem::Elem* tek=KernelSem::allSemaphores->head;tek;tek=tek->next)
					if(tek->kSem->timeBlock ->head)
					{
						tek->kSem->timeBlock->head->timeToWait--;
							 	while(tek->kSem->timeBlock ->head->timeToWait<=0)
							 			{
							 		PCB *ret=tek->kSem->timeBlock->getHeadPCB();
							 		
							 		if(ret) {
							 						tek->kSem->timeBlock->remove(ret);
													
													
							 						ret->returnVal=0;
							 						tek->kSem->value++;
													ret->phase=PCB::READY;
							 						Scheduler::put(ret);
							 			}
								}
					}

}



