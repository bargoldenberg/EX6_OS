#include <iostream>
#include "guard.cpp"
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

using namespace std;

size_t counter =0;
pthread_mutex_t m;


void increment(int* id){
    guard g(&m);
    counter++;
    cout<<counter<<", Thread #"<<*id<<endl;
}

void* threadrunner(void* id){
    for(size_t i=0;i<1000;i++){
        increment((int*)id);
    }
}


int main(){
    pthread_mutex_init(&m,NULL);
    pthread_t t1;
    pthread_t t2;
    int id1 = 1;
    int id2 = 2;
    pthread_create(&t1,NULL,threadrunner,&id1);
    pthread_create(&t2,NULL,threadrunner,&id2);
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    return 0;
}