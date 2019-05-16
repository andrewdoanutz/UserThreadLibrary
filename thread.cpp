#include "thread.h"


void lock(){

    sigprocmask(SIG_BLOCK, &alarm_set, NULL);
}

void unlock(){
    sigprocmask(SIG_UNBLOCK, &alarm_set, NULL);

}

void startup(){
    cout<<"starting\n";
    lock();
    firstTime=false;
    TCB* firstInfo=new TCB();
    firstInfo->status=RUNNING;
    sched.pushThread(firstInfo,0);
    timer.sa_handler=schedHandler;
    timer.sa_flags= SA_NODEFER; //| SA_RESTART;
    sigemptyset(&timer.sa_mask);
    sigaction(SIGALRM,&timer,NULL);
    sigemptyset(&alarm_set);
	sigaddset(&alarm_set, SIGALRM);
    ualarm(50000,50000);
    unlock();
    
}

void schedHandler(int signal){
    cout<<"handling for "<<sched.top()->info->TID<<endl;
    lock();
    TCB* info=sched.top()->info;
    if(!setjmp(info->jumpBuff)){
        sched.incrementSched();
        sched.printIDS();
        cout<<"jumping in handler\n";
        if(firstTime){
            unlock();
        }
        longjmp(sched.top()->info->jumpBuff,1);
        unlock();
    }
}

pthread_t pthread_self(void){
    lock();
    if(sched.empty()){
        eMess("Empty Schedule when getting TID");
        unlock();
        exit(0);
    }
    pthread_t temp=(pthread_t)(sched.top()->info->TID);
    unlock();
    return temp;
}


int pthread_create(pthread_t *thread, const pthread_attr_t *attr,void *(*start_routine)(void*), void *arg){
    if(firstTime){
        startup();
    }
    lock();
    TCB* info=new TCB();
    setjmp(info->jumpBuff);
    *((unsigned long*)&(info->jumpBuff)+stackInd)=i64_ptr_mangle((unsigned long)(unsigned long*)(info->esp));
    *((unsigned long*)&(info->jumpBuff)+progInd)=i64_ptr_mangle((unsigned long)(&wrapper));
    info->arg=arg;
    info->start_routine=start_routine;
    sched.pushThread(info,(pthread_t)thread);
    sched.printIDS();
    *thread=info->TID;
    unlock();
    cout<<"creating "<<info->TID<<endl;
    return 0;
}

void wrapper(){
    cout<<"wrapping "<<pthread_self()<<endl;
    lock();
    TCB* info=sched.top()->info;
    unlock();
    void *(*start_routine)(void*)=info->start_routine;
    void *arg=info->arg;
    pthread_exit(start_routine(arg));
 
}



void pthread_exit(void *value_ptr){
    cout<<"exiting: "<<pthread_self()<<endl;
    lock();
    TCB* info=sched.top()->info;
    if(info->joinedBy!=-1){
        cout<<"Unblocking "<<info->joinedBy<<endl;
        sched.findThread(info->joinedBy)->info->status=READY;
    }
    if(info->TID!=0){
        info->returnVal=value_ptr;
        cout<<"popping\n";
        sched.setZombie();
        cout<<"ready to jump: "<<sched.top()->info->TID<<endl;
        unlock();
        longjmp(sched.top()->info->jumpBuff,1);
    }
    cout<<"failed exit"<<endl;
    unlock();
    exit(0);
}


int pthread_join(pthread_t thread, void **value_ptr){
    cout<<pthread_self()<<" joining "<<thread<<" who has status "<<sched.findThread(thread)->info->status<<endl;
    if(sched.findThread(thread)==nullptr || sched.findThread(thread)->info->joinedBy!=-1){
        eMess("ESRCH");
        return ESRCH;
    }
    if(sched.findThread(thread)->info->status==ZOMBIE){
        cout<<"joining dead\n";
        if(value_ptr!=NULL){
            cout<<"assigning dead\n";
            *value_ptr=sched.findThread(thread)->info->returnVal;
            cout<<"assigned "<<&sched.findThread(thread)->info->returnVal<<endl;
            sched.findThread(thread)->info->status=DEAD;
       }
        return 0;
    }
    lock();
    TCB* info=sched.top()->info;
    info->status=BLOCK;
    sched.findThread(thread)->info->joinedBy=info->TID;
    info->joinedOn=thread;
    schedHandler(0);
    if(value_ptr!=NULL){
        cout<<"assigning\n";
        *value_ptr=sched.findThread(thread)->info->returnVal; 
        cout<<"assigned "<<&sched.findThread(thread)->info->returnVal<<endl;
        sched.findThread(thread)->info->status=DEAD;
    }
    unlock();
    return 0;
}