#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

class guard{
    public:
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
