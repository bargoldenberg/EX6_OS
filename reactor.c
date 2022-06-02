#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

typedef struct _reactor{
    struct pollfd fds[256];
    void (*funcs[256])(void*);
    int size;
    pthread_t thread_id;
} Reactor;


void* thread_runner(void* arg){
    Reactor* r = (Reactor*)arg;
    while(1){
        int i = poll(r->fds, r->size, -1);
        for(i=0;i<r->size;i++){
            if(r->fds[i].revents & POLLIN){
                r->funcs[i](&(r->fds[i].fd));
            }
        }
    }
}

void* NewReactor(){
    Reactor* r = (Reactor*)malloc(sizeof(Reactor));
    r->size = 0;
    r->thread_id = 0;
    return r;
}

void installHandler(Reactor* r,void (*func)(void *),int fd){
    if(r->size>=256){
        return;
    }
    if(r->size==0){
        r->fds[r->size].fd = fd;
        r->fds[r->size].events = POLLIN;
        r->funcs[r->size] = func;
        r->size++;
        pthread_create(&r->thread_id, NULL, thread_runner, r);
    }else{
        int i;
        for(i=0;i<r->size;i++){
            if(r->fds[i].fd==fd){
                return;
            }
        }
        r->fds[r->size].fd = fd;
        r->fds[r->size].events = POLLIN;
        r->funcs[r->size] = func;
        r->size++;
    }
}

void RemoveHandler(void *reactor, int fd){
    Reactor* r = (Reactor*)reactor;
    int i;
    for(i=0;i<r->size;i++){
        if(r->fds[i].fd==fd){
            r->fds[i].fd = -1;
            r->fds[i].events = 0;
            r->funcs[i] = NULL;
            r->size--;
            return;
        }
    }
}

void DeleteReactor(Reactor* r){
    free(r);
}

