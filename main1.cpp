/*
** server.c -- a stream socket server demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include "Queue.h"
#include "AO.h"

#include <pthread.h> // --->>> This is the Thread functions header.


#define PORT "3491"  // the port users will be connecting to

#define BACKLOG 10   // how many pending connections queue will hold
//initialize global queues.
queue* q1 = createQ();
queue* q2 = createQ();
queue* q3 = createQ();

//Functions for pipline
void ceaser_cypher(void* data){
    int i;
    char* str = (char*)data;
    for(i=0;i<strlen(str);i++){
        if(str[i]>='a' && str[i]<='z'){
            str[i] = (str[i]-'a'+1)%26+'a';
        }
        else if(str[i]>='A' && str[i]<='Z'){
            char c = str[i];
            str[i] = (c-'A'+1)%26+'A';
        }
    }
}

void switchCase(void* str){
    int i;
    char* data = (char*)str;
    for(i=0;i<strlen(data);i++){
        if(data[i]>='a' && data[i]<='z'){
            data[i] = (data[i]-'a')+'A';
        }
        else if(data[i]>='A' && data[i]<='Z'){
            data[i] = (data[i]-'A')+'a';
        }
    }
}

void enQ2(void* str){
    char* data = (char*)str;
    enQ(q2,data);
}

void enQ3(void* str){
    char* data = (char*)str;
    enQ(q3,data);
}

void sendToClient(void* str){
    char* data = (char*)str;
    printf("The File Discriptor is %d\n",getFD(q3));
    int numbytes = send(getFD(q3),data,strlen(data),0);
    if(numbytes<0){
        perror("send");
        exit(1);
    }
    free(data);
}

void sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}


// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

/**
 * @brief Conection Thread Function, in each new connection,  we will call this funtion using new thread, so it will run and
 * we still be able to accept more client and serve them as well
 * @param pClient -> pointer to the File Descriptor that contain the sock addr
 */
void *connection_handler(void *pClient)
{
    int new_fd = *((int*)pClient);
    free(pClient); // No more use for the pointer -> free as fast as we can so we wont miss that. 
       
    //Active Object initialization
    activeObject* AO1=NULL;
    activeObject* AO2=NULL;
    activeObject* AO3=NULL;
    while(1){
        char* buffer=(char*)malloc(sizeof(char)*1024);
        bzero(buffer,1024);
        printf("Waiting for client to send data fd: %d\n",new_fd);
        int n = recv(new_fd,buffer,1024,0);
        if(!strncmp(buffer,"EXIT",4)){
            break;
        }
        enQ(q1,buffer);
        updatefd(q3,new_fd);
        printf("%d\n",isEmpty(q1));
        if(AO1==NULL||AO2==NULL||AO3==NULL){
            AO1 = createAO(q1,ceaser_cypher,enQ2);
            AO2 = createAO(q2,switchCase,enQ3);
            AO3 = createAO(q3,sendToClient,NULL);
        }
    }
    destroyAO(AO1);
    destroyAO(AO2);
    destroyAO(AO3);
    close(new_fd);
}


int main(void)
{
    int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    printf("server: waiting for connections...\n");

    while(1) {  // main accept() loop
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }


        inet_ntop(their_addr.ss_family,
            get_in_addr((struct sockaddr *)&their_addr),
            s, sizeof s);
        printf("server: got connection from %s\n", s);

        printf("File Descriptor = %d\n",new_fd);
        // close(sockfd);
        
        pthread_t t;
        int *pClient = (int*)malloc(sizeof(int));
        *pClient = new_fd; 
        pthread_create(&t,NULL,connection_handler,pClient);
    }
    destroyQ(q1);
    destroyQ(q2);
    destroyQ(q3);
    return 0;
}