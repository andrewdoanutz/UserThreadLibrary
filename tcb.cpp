#include "tcb.h"

TCB::TCB(){
    status=READY;
    stackPtr=new unsigned long [STACK_SIZE];
    arg=nullptr;
    start_routine=nullptr;
    returnVal=nullptr;
    esp=stackPtr+STACK_SIZE/8-5;
    TID=0;
    SID=-1;
    joinedBy=-1;
    joinedOn=-1;
}


TCB::~TCB(){
    cout<<"deleting tcb\n";
    esp=nullptr;
    delete[] stackPtr;
}