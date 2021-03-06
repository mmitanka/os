#include "event.h"
#include "system.h"
#include "kernelE.h"
#include "IVTEntry.h"
#include "kernelSe.h"
#include "pcb.h"

Event::Event(IVTNo n) {

    myImpl=new KernelEv(n);
}

Event::~Event() {
    delete myImpl;
}

void Event::wait() {
	asm {
		pushf
		cli
	}
    myImpl->wait();

    asm {
    	popf
    }
}

void Event::signal() {
	asm {
		pushf
		cli
	}
    myImpl->signal();

    asm {
    	popf
    }

}
