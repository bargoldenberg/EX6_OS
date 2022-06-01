#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

using namespace std;

class guard{
    public:
        guard(){}
        guard(pthread_mutex_t* m){
            mutex = m;
            pthread_mutex_lock(mutex);
        }
        ~guard(){
            pthread_mutex_unlock(mutex);
        }
    private:
        pthread_mutex_t* mutex;
};

size_t counter =0;
pthread_mutex_t m=PTHREAD_MUTEX_INITIALIZER;

//increment global counter
void increment(int* id){
    guard g(&m);
    counter++;
    cout<<counter<<", Thread #"<<*id<<endl;
}

//threads main function.
void* threadrunner(void* id){
    for(size_t i=0;i<1000;i++){
        increment((int*)id);
    }
}


int main(){
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