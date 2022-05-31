#include <iostream>
#include "guard.cpp"
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

using namespace std;

size_t counter =0;
pthread_mutex_t m;


void increment(){
    guard g(&m);
    counter++;
    cout<<counter<<endl;
}

void* threadrunner(void* arg){
    for(size_t i=0;i<1000;i++){
        increment();
    }
}


int main(){
    pthread_mutex_init(&m,NULL);
    pthread_t t1;
    pthread_t t2;
    pthread_create(&t1,NULL,threadrunner,NULL);
    pthread_create(&t2,NULL,threadrunner,NULL);
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    return 0;
}