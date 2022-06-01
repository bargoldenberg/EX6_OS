#include <iostream>
#include <pthread.h>
#include <unistd.h>
using namespace std;

//Singleton Class.
template<typename t> class singleton{
    private:
        static t* myinstance;
        static pthread_mutex_t m;
        singleton(){
            cout<<"constructor called"<<endl;
        }
        ~singleton(){
            cout<<"destructor called"<<endl;
        }
    public:
        static t* Instance(){
            if(myinstance == NULL){
                pthread_mutex_init(&m, NULL);
                pthread_mutex_lock(&m);
                myinstance = new t();
                pthread_mutex_unlock(&m);
            }
            return myinstance;
        }
        static void Destroy(){
            if(myinstance != NULL){
                pthread_mutex_destroy(&m);
                delete myinstance;
                myinstance = NULL;
            }
        }
};