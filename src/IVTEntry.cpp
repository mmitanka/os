#include "IVTEntry.h"
#include <dos.h>
#include <iostream.h>
#include "system.h"
#include "kernelE.h"
#include "pcb.h"

IVTEntry* IVTEntry::IVTtab[256]={0};

IVTEntry::IVTEntry(int n,void interrupt (*nRoutine)(...)) {
    myKEv=0;
    ivtno=n;
    IVTtab[ivtno]=this;

    asm {
        pushf
        cli
    }
    oldRout = getvect(ivtno);
	setvect(ivtno, nRoutine);
    asm {
        popf
    }

}


IVTEntry::~IVTEntry() {
    asm {
        pushf
        cli
    }
    
	setvect(ivtno, oldRout);
    asm {
        popf
    }
}

IVTEntry* IVTEntry::getIVTEntry(IVTNo n) {
    return IVTtab[n];
}

void IVTEntry::callOldR() {
    (*oldRout)();
}

void IVTEntry::signal() {
    if(myKEv) {
        myKEv->signal();
    }
}
