#ifndef SEMA_H
#define SEMA_H

#include <iostream>
#include <semaphore.h>
#include "thread.h"
#include <vector>
using namespace std;

#define SEM_VALUE_MAX 65536


struct sema{
    int count;
    sem_t* sem;

};

vector<sema*> semas;
int currID=0;

int sem_init(sem_t *sem, int pshared, unsigned int value);
int sem_destroy(sem_t *sem);
int sem_wait(sem_t *sem);
int sem_post(sem_t *sem);


#endif