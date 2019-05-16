#include "sched.h"
#include <vector>
Scheduler::Scheduler(){
    head=nullptr;
    tail=nullptr;
    currTID=0;
}

Scheduler::~Scheduler(){
    sNode* temp=head;
    sNode* temp2=nullptr;
    while(temp){
        temp2=temp;
        temp=temp->next;
        delete temp2;
    }
}

sNode* Scheduler::findThread(pthread_t TID){
    sNode* temp=head;
    while(temp){
        if(temp->info->TID==TID){
            return temp;
        }
        temp=temp->next;
    }
    return nullptr;
}

void Scheduler::printIDS(){
    sNode*temp=head;
    vector<int>res;
    int count=0;
    while(temp){
   
        if(temp->info->status==READY|| temp->info->status==RUNNING || temp->info->status==BLOCK || temp->info->status==SBLOCK){
            res.push_back(temp->info->TID);
        }
        temp=temp->next;
    }
    cout<<"IDS: ";
    for (const auto& i: res)
        std::cout << i << ' ';
    cout<<endl;
 }

void Scheduler::pushThread(TCB* info, pthread_t TID){
    sNode* temp=new sNode();
    if(!temp){
        eMess("Full stack");
        exit(1);
    }
    else{
        (info->TID)=currTID;
        currTID++;
    }
    temp->info=info;
    if(head==nullptr){
        head=temp;
        tail=temp;
        return;
    }
    //TA fix WIP
/*     if(head->next==nullptr){
        head->next=temp;
        tail=temp;
        return;
    }
    temp=head->next->next;
    head->next=temp; */
    // passes with leaks
    tail->next=temp;
    tail=temp;
}


void Scheduler::incrementSched(){
    if(head->info->TID==0){
        cout<<"TID "<<head->info->TID<<" joinedOn: "<<head->info->joinedOn<<" joinedBy: "<<head->info->joinedBy<<endl;
    }
    if(empty()){
        eMess("Empty Schedule");
        exit(1);
    }
    else if(head==tail){ //only 1 in sched
        cout<<"1 in Schedule\n";
        return;
    }
    else {
        sNode* temp=head;
        head=head->next;
        temp->next=nullptr;
        tail->next=temp;
        tail=temp;
        if (tail->info->status ==RUNNING) {
				tail->info->status = READY;
		}
        while(head->info->status==DEAD || head->info->status==BLOCK || head->info->status==SBLOCK || head->info->status==ZOMBIE){
            if (head->info->status==BLOCK){
                if(sched.findThread(head->info->joinedOn)){
                    if(sched.findThread(head->info->joinedOn)->info->status==ZOMBIE){
                        cout<<"changing status to run since dead for "<<head->info->TID<<endl;
                        head->info->status=RUNNING;
                        return;
                    }
                }
            }
            if(head->info->status==DEAD){
                sched.popThread();
            }
            else{
                sNode* temp=head;
                head=head->next;
                temp->next=nullptr;
                tail->next=temp;
                tail=temp;
            }
        }
        if (tail->info->status ==RUNNING) {
				tail->info->status = READY;
		}
        if(head->info->status==READY){
                cout<<"changing status to run for "<<head->info->TID<<endl;
                head->info->status=RUNNING;
        }
    }
    // else{
    //     sNode* temp=head;
    //     head=head->next;
    //     temp->next=nullptr;
    //     tail->next=temp;
    //     tail=temp;
    //     if(head->info->status==DEAD || head->info->status==BLOCK || head->info->status==SBLOCK){
    //         incrementSched();
    //     }
    //     if(tail->info->status==RUNNING){
    //         tail->info->status=READY;
    //     }
    //     if(head->info->status==READY){
    //         head->info->status=RUNNING;
    //         cout<<"changing status to run for "<<head->info->TID<<endl;
    //     }
                
       //TA fix WIP
       /*
        sNode* temp=tail;
        tail=tail->parent;
        tail->next=nullptr;
        temp->parent=nullptr;
        head->parent=temp;
        temp->next=head;
        head=temp;
        tail->info->status=READY;
        head->info->status=RUNNING;
        */
   // }
    cout<<"Done incrementing\n";
}


void Scheduler::popThread(){
    if(empty()){
        eMess("Empty Schedule");
        exit(1);
    }
    else if(head==tail){
        delete head;
        head=nullptr;
        tail=nullptr;
    }
    else{
        sNode* temp=head;
        head=head->next;
        delete temp; 
        
    } 
}

void Scheduler::setZombie(){
    if(empty()){
        eMess("Empty Schedule");
        exit(1);
    }
    else if(head==tail){
        head->info->status=ZOMBIE;
    }
    else{
        head->info->status=ZOMBIE;
        kill(getpid(),SIGALRM);
    } 
}

