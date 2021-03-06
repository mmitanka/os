/*
 * system.h
 *
 *  Created on: Aug 17, 2020
 *      Author: OS1
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <dos.h>
#include <iostream.h>
#include "pcb.h"





void interrupt timer() ;

class Thread;
class PCB;

class System {
public:
    System(){}
    ~System(){}
    static Thread* mainT; static Idle* idle;
    static volatile PCB* running;

    static int cnt;
    static int usao;
    static unsigned int crit_section;
    static unsigned int ekspl_promena_cntx;

    static void setCritSection();
    static void resetCritSection();
    static unsigned int getCritSection();

    static void setContextSwitch();
    static void resetContextSwitch();
    static unsigned int getContextDemandFlag();

    static void dispatch();
    static void call_dispatch();

    static void inic();
    static void restore();


    static ID getRunningId();

    static void unblockWaiting();
protected:
    friend class Thread;
    friend class PCB;
    friend class QueueTime;
    friend class KernelSem;
    friend class Queue;
    friend class Idle;

};


#endif /* SYSTEM_H_ */
