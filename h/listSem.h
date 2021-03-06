#ifndef LISTSEM_H_
#define LISTSEM_H_
#include "kernelSe.h"


class ListSem {
    public:
        struct Elem {
            Elem *next; KernelSem *kSem;
            Elem(KernelSem *p) {next=0; kSem=p;}

        };

        ListSem();
        ~ListSem();

        KernelSem* get();
        void put(KernelSem* ksem);

        int getSize ();
        void removeSem(KernelSem *ks);
        
        Elem *head,*tail;
        friend class KernelSem;
    protected:
        int sz;

};
#endif
