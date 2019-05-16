#include "sema.h"

int sem_init(sem_t *sem, int pshared, unsigned int value){
    if(value>SEM_VALUE_MAX){
        eMess("Value greater than max");
        return EINVAL;
    }
    if(pshared!=0){
        return ENOSYS;
    }
    sema* temp=new sema;
    temp->count=value;
    sem->__align=currID++;
    temp->sem=sem;
    semas.push_back(temp);
    return 0;
}

int sem_destroy(sem_t *sem){
    int ind=sem->__align;
    if(semas.at(ind)==NULL){
        return EINVAL;
    }
    sema*temp=semas.at(ind);
    delete temp;
    semas.erase(semas.begin()+ind);
    return 0;
}
int sem_wait(sem_t *sem){
    int ind=sem->__align;
    if(semas.at(ind)==NULL){
        return EINVAL;
    }
    sema*temp=semas.at(ind);
    if(temp->count==0){
        lock();
        sched.top()->info->status=SBLOCK;
        sched.top()->info->SID=ind;
        unlock();
        kill(getpid(),SIGALRM);
    }
    else{
        temp->count--;
    }
    
    return 0;
}
int sem_post(sem_t *sem){
    int ind=sem->__align;
    if(semas.at(ind)==NULL){
        return EINVAL;
    }
    sema*temp=semas.at(ind);
    if(temp->count==0){
        sNode* tempNode=sched.top();
        while(tempNode){
            if(tempNode->info->status==SBLOCK && tempNode->info->SID==ind){
                cout<<"waking "<<tempNode->info->TID<<endl;
                tempNode->info->status=READY;
                tempNode->info->SID=-1;
                break;
            }
            tempNode=tempNode->next;
        }
    }

    else{
        temp->count++;
    }
    cout<<"count "<<temp->count<<endl;
    return 0;
}