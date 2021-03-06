#include "listSem.h"
#include "kernelSe.h"


ListSem::ListSem() {
    head=tail=0;
    sz=0;
}

ListSem::~ListSem() {

    Elem* tek=head;
    Elem* preth=0;
    while(tek){
       preth=tek;

       tek=tek->next;
       delete preth;


    }
    sz=0;
    delete head; delete tail; delete tek; delete preth;
}
KernelSem* ListSem::get(){
    if(getSize()==0 || !head) return 0;

    Elem *el=head; head=head->next;
    if(!head) tail=0;
    KernelSem* p=el->kSem;
    sz--;
    delete el;
    return p;

}

void ListSem::put(KernelSem *s){
    if(!s)  return;

    Elem *el=head;
    for(;el;el=el->next)
    	if(el->kSem==s)
    		return;


    el=new Elem(s);


    if(getSize()==0) {head=tail=el; sz++;}
    else {
        tail->next=el; tail=el;
        sz++;
    }
}

int ListSem::getSize() {
	return sz;
}
void ListSem::removeSem(KernelSem *ks) {

    Elem* cur=head, *prev=0;

    while(cur && cur->kSem!=ks) {
        prev=cur;
        cur=cur->next;
    }

    if(!cur) return;
    if(cur==head) {
    	if(head==tail)
    		tail=head->next;
        head=head->next;

    } else{
        prev->next=cur->next;
    	if(cur==tail)
    		tail=prev;
    }
    sz--;
    cur->next=0;
    delete cur;
}

