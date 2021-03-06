#ifndef KERNELSE_H_
#define KERNELSE_H_

#include "semaphor.h"
#include "listSem.h"
#include "queue.h"
#include "queueT.h"

class ListSem;
class QueueTime;

class KernelSem {
    public:

		static ListSem* allSemaphores;

        KernelSem(Semaphore *sem, int v=1);
        ~KernelSem();

        int signal(int n);
        int wait(Time maxTime);

        int val() const;

        void releaseSem();

        int value;
        Semaphore *mySem;
        Queue *blocked;
        QueueTime *timeBlock;



        friend class QueueTime;
        friend class Queue;
        
};
#endif
