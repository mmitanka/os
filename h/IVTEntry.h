#ifndef IVTENTRY_H_
#define IVTENTRY_H_
#include "event.h"
#include "kernelE.h"
#include "system.h"
#include "pcb.h"


#define PREPAREENTRY(ivtNo, callOld) \
void interrupt inter##ivtNo(...); \
IVTEntry newEntry##ivtNo(ivtNo, inter##ivtNo); \
void interrupt inter##ivtNo(...) \
{\
  newEntry##ivtNo.signal();\
  if (callOld == 1)\
  newEntry##ivtNo.callOldR();\
}

class KernelEv;
class Event;
class IVTEntry{
    public:

    IVTEntry(int ivtNo,void interrupt(*)(...));
    ~IVTEntry();

    static IVTEntry* IVTtab[256];


    static IVTEntry* getIVTEntry(IVTNo n);
	void callOldR();
	void signal();

    KernelEv* myKEv;
    void interrupt (*oldRout)(...);

    unsigned ivtno;
    friend class KernelEv;
    friend class Event;

};

#endif
