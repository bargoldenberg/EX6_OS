#include <iostream>
#include <pthread.h>
#include <unistd.h>
using namespace std;
class singleton{
    private:
        static singleton* myinstance;
        static pthread_mutex_t m;
        singleton(){
            cout<<"constructor called"<<endl;
        }
        ~singleton(){
            cout<<"destructor called"<<endl;
        }
    public:
        static singleton* Instance(){
            if(myinstance == NULL){
                pthread_mutex_init(&m, NULL);
                myinstance = new singleton();
            }
            return myinstance;
        }
        static void destroy(){
            if(myinstance != NULL){
                pthread_mutex_destroy(&m);
                delete myinstance;
                myinstance = NULL;
            }
        }
};
