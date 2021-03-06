/*
 * idle.h
 *
 *  Created on: Aug 24, 2020
 *      Author: OS1
 */

#ifndef IDLE_H_
#define IDLE_H_

#include "thread.h"


class Idle: public Thread {

public:
    Idle();
    virtual ~Idle();

    virtual void run();

    void start();

protected:
    friend class Thread;
    friend class PCB;
    friend class System;

};



#endif /* IDLE_H_ */
