#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <mutex>
#include <fstream>

using namespace std;

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;


template<typename T>
class Singleton {
private:
    static Singleton *myinstance;
    T object;
    Singleton(T temp){
        object = temp;
    }

public:
    static Singleton *Instance(T temp){
    pthread_mutex_lock(&mtx);
    if (myinstance == nullptr) {
        myinstance = new Singleton(temp);
    }
    pthread_mutex_unlock(&mtx);
    return myinstance;
    }

    void Destroy(){
        pthread_mutex_lock(&mtx);
        delete myinstance;
        myinstance = nullptr;
        pthread_mutex_unlock(&mtx);
    }
};

template<typename T> Singleton<T> *Singleton<T>::myinstance = nullptr;




int main() {
    FILE *fptr = fopen("test.txt", "w");
    Singleton<FILE *> *sing1 = Singleton<FILE *>::Instance(fptr); // Ok
    Singleton<FILE *> *sing2 = Singleton<FILE *>::Instance(fptr);
    std::cout << sing1 << std::endl;
    std::cout << sing2 << std::endl;
    if(sing1==sing2){
        cout<<"Singleton Works!"<<endl;
    }else{
        cout<<"Singleton doesn't work!"<<endl;
    }
    sing1->Destroy();
}