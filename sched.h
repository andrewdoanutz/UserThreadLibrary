#ifndef SCHED_H
#define SCHED_H



#include "tcb.h"


void eMess(string message){
    fprintf(stderr,"ERROR: %s\n",message.c_str());
}
struct sNode{
    TCB* info;
    sNode* next;
    sNode(){
        info=nullptr;
        next=nullptr;
    }
    ~sNode(){
        cout<<"deleting node from scheduler\n";
        delete info;
    }
};

class Scheduler{
    public:
        Scheduler();
        ~Scheduler();
        void pushThread(TCB* info,pthread_t TID);
        void popThread();
        void setZombie();
        void incrementSched();
        void printIDS();
        sNode* findThread(pthread_t TID);
        bool empty(){
            if(head){
                return false;
            }
            return true;
        };
        sNode* top(){
            if(empty()){
                eMess("Empty Schedule at top");
                exit(1);
            }
            else{
                return head;
            }
        };
    private:
        pthread_t currTID;
        sNode* head;
        sNode* tail;
};

#endif