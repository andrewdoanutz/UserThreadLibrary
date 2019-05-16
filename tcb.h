#ifndef TCB_H
#define TCB_H

#include <setjmp.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <iostream>
#include <cstdio>
#include <string>
#include <cerrno>


#define STACK_SIZE 32767
#define MAX_NUM_THREADS 128
#define READY 10
#define RUNNING 11
#define BLOCK 12
#define DEAD 13
#define SBLOCK 14
#define ZOMBIE 15
#define stackInd 6
#define progInd 7

using namespace std;

class TCB {
    public:
    TCB();
    ~TCB();
    int status;
    pthread_t TID;
    int SID;
    jmp_buf jumpBuff;
    unsigned long * stackPtr;
    unsigned long * esp;
    void *(*start_routine)(void*);
    void *arg;
    void *returnVal;
    int joinedBy;
    int joinedOn;
    private:
};

#endif