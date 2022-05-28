#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
pthread_mutex_t lock;
//NODE_STRUCT//
typedef struct _node{
    int data;
    struct _node *next;
} node;

node* createNode(){
    node* n = (node*)malloc(sizeof(node));
    n->next = NULL;
    return n;
}

void destroyNode(node* n){
    free(n);
}

//QUEUE_STRUCT//
typedef struct _queue {
    node* head;
    node* tail;
    size_t size;
} queue;

queue* createQ(){
    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("\n mutex init has failed\n");
        return NULL;
    }
    queue* q = (queue*)malloc(sizeof(queue));
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

void destroyQ(queue* q) {
    node* n = q->head;
    while(n != NULL){
        node* temp = n;
        n = n->next;
        destroyNode(temp);
    }
    pthread_mutex_destroy(&lock);
    free(q);
}


void enQ(queue* q,int data){
    pthread_mutex_lock(&lock);
    node* n = createNode();
    n->data = data;
    if(q->head == NULL){
        q->head = n;
        q->tail = n;
    }
    else{
        q->tail->next = n;
        q->tail = n;
    }
    q->size++;
    pthread_mutex_unlock(&lock);
}

int deQ(queue* q){
    pthread_mutex_lock(&lock);
    if(q->head == NULL){
        return -1;
    }
    node* n = q->head;
    int data = n->data;
    q->head = n->next;
    destroyNode(n);
    q->size--;
    pthread_mutex_unlock(&lock);
    return data;
}