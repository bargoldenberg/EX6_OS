#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "Queue.h"
/**
 * ACTIVE OBJECT
 * AUTHORS : BAR GOLDENBERG & SAPPIR BOHBOT
 * 
 */

//ACTIVE_OBJECT_STRUCT//
typedef struct _active_object {
    queue *q;
    pthread_t thread;
    void (*AOfunc1)(void *);
    void (*AOfunc2)(void *);
}activeObject;

//Active Object Functions//
void* runAO(void * vAO){
  activeObject* AO = (activeObject*)vAO;
  void* data ;
  while(1){
    data = deQ(AO->q);
    if(AO->AOfunc1!=NULL){
       AO->AOfunc1(data);
    }
    if(AO->AOfunc2!=NULL){
       AO->AOfunc2(data);
    }
  }
}

//Active Object Functions//
activeObject* createAO(queue* q, void (*func1)(void *), void (*func2)(void* )){
    activeObject* AO = (activeObject*)malloc(sizeof(activeObject));
    AO->q=q;
    AO->AOfunc1=func1;
    AO->AOfunc2=func2;
    pthread_create(&AO->thread, NULL,runAO,AO);
    return AO;
}


//Active Object Functions//
void destroyAO(activeObject* AO){
    pthread_join(AO->thread, NULL);
    pthread_cancel(AO->thread);
    destroyQ(AO->q);
    free(AO);
}

