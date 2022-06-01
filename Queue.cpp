#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>


//NODE_STRUCT//
typedef struct _node{
    void* data;
    struct _node *next;
} node;

/**
 * @brief Create a Node object
 * 
 * @return node* 
 */
node* createNode(){
    node* n = (node*)malloc(sizeof(node));
    n->next = NULL;
    return n;
}

/**
 * @brief Destroy the node object
 * 
 * @param n 
 */
void destroyNode(node* n){
    free(n);
}

//QUEUE_STRUCT//
typedef struct _queue {
    node* head;
    node* tail;
    size_t size;
    pthread_mutex_t lock;
    pthread_cond_t cond;
    int* fd;
} queue;

int isEmpty(queue* q){
    return q->size == 0;
}

queue* createQ(){
    queue* q = (queue*)malloc(sizeof(queue));
    if (pthread_mutex_init(&(q->lock), NULL) != 0) {
        printf("\n mutex init has failed\n");
        return NULL;
    }
    q->cond = PTHREAD_COND_INITIALIZER;
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

void destroyQ(queue* q) {
    pthread_mutex_lock(&(q->lock));
    node* n = q->head;
    while(n != NULL){
        node* temp = n;
        n = n->next;
        destroyNode(temp);
    }
    pthread_mutex_unlock(&(q->lock));
    pthread_mutex_destroy(&(q->lock));
    free(q->fd);
    free(q);
}


void enQ(queue* q,void* data){
    pthread_mutex_lock(&(q->lock));
    pthread_cond_signal(&(q->cond));
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
    pthread_mutex_unlock(&(q->lock));
}

void* deQ(queue* q){
    pthread_mutex_lock(&(q->lock));
    if(isEmpty(q)){
        pthread_cond_wait(&(q->cond), &(q->lock));
    }
    if(q->head == NULL){
        return NULL;
    }
    node* n = q->head;
    void* data = n->data;
    q->head = n->next;
    destroyNode(n);
    q->size--;
  
    pthread_mutex_unlock(&(q->lock));
    return data;
}

void updatefd(queue* q,int fd){
    pthread_mutex_lock(&(q->lock));
    int* fd_ptr = (int*)malloc(sizeof(int));
    *fd_ptr = fd;
    q->fd = fd_ptr;
    pthread_mutex_unlock(&(q->lock));
}

int getFD(queue* q){
    return *(q->fd);
}