#ifndef THREAD_H
#define THREAD_H

#include "sched.h"

static long int i64_ptr_mangle(long int p)
   {
        long int ret;
        asm(" mov %1, %%rax;\n"
            " xor %%fs:0x30, %%rax;"
            " rol $0x11, %%rax;"
            " mov %%rax, %0;"
        : "=r"(ret)
        : "r"(p)
        : "%rax"
        );
        return ret;
   }

Scheduler sched;
bool firstTime=true;
struct sigaction timer;
sigset_t alarm_set;

void lock();
void unlock();
void wrapper();
void startup();
void schedHandler(int signal);
pthread_t pthread_self(void);
int pthread_create(pthread_t *thread, const pthread_attr_t *attr,void *(*start_routine)(void*), void *arg);
void pthread_exit(void* value_ptr);
int pthread_join(pthread_t thread, void **value_ptr);

#endif