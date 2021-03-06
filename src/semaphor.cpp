#include "semaphor.h"
#include "kernelSe.h"
#include "system.h"

Semaphore::Semaphore(int init) {

    myImpl=new KernelSem(this, init);
}

Semaphore::~Semaphore(){
	delete myImpl;
}

int Semaphore::wait(Time maxTime) {
    return myImpl->wait(maxTime);
}

int Semaphore::signal(int n) {
    return myImpl->signal(n);
}

int Semaphore::val() const{
    return myImpl->val();
}

