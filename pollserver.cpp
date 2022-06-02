/** 
  * pollserver.c -- a cheezy multiperson chat server 
  * A basic Chat server implementation using my reactor and beej server template 
 */ 
  
 #include <stdio.h> 
 #include <stdlib.h> 
 #include <string.h> 
 #include <unistd.h> 
 #include <sys/socket.h> 
 #include <netinet/in.h> 
 #include <arpa/inet.h> 
 #include <netdb.h> 
 #include<signal.h> 
 #include "reactor.h" 
 #include <pthread.h>
 #include <poll.h>
  
 #define PORT "3491"   // Port we're listening on 

//REACTOR_STRUCT
typedef struct _reactor{
    struct pollfd fds[256];
    void (*funcs[256])(void*);
    int size;
    pthread_t thread_id;
} Reactor;

//GLOBAL VARIABLES
Reactor* r; 
  

  
 /** 
  * Get sockaddr, IPv4 or IPv6: 
  * @param sa 
  * @return return the ip of the sockaddr struct 
  */ 
 void *get_in_addr(struct sockaddr *sa) { 
     if (sa->sa_family == AF_INET) { 
         return &(((struct sockaddr_in *) sa)->sin_addr); 
     } 
  
     return &(((struct sockaddr_in6 *) sa)->sin6_addr); 
 } 
  
 /** 
  * This function initial a new listener fd 
  * @return listening socket 
  */ 
 int get_listener_socket() { 
     int listener;     // Listening socket descriptor 
     int yes = 1;        // For setsockopt() SO_REUSEADDR, below 
     int rv; 
  
     struct addrinfo hints, *ai, *p; 
  
     // Get us a socket and bind it 
     memset(&hints, 0, sizeof hints); 
     hints.ai_family = AF_UNSPEC; 
     hints.ai_socktype = SOCK_STREAM; 
     hints.ai_flags = AI_PASSIVE; 
     if ((rv = getaddrinfo(NULL, PORT, &hints, &ai)) != 0) { 
         fprintf(stderr, "selectserver: %s\n", gai_strerror(rv)); 
         exit(1); 
     } 
  
     for (p = ai; p != NULL; p = p->ai_next) { 
         listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol); 
         if (listener < 0) { 
             continue; 
         } 
  
         // Lose the pesky "address already in use" error message 
         setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)); 
  
         if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) { 
             close(listener); 
             continue; 
         } 
  
         break; 
     } 
  
     freeaddrinfo(ai); // All done with this 
  
     // If we got here, it means we didn't get bound 
     if (p == NULL) { 
         return -1; 
     } 
  
     // Listen 
     if (listen(listener, 10) == -1) { 
         return -1; 
     } 
  
     return listener; 
 } 
  
 /** 
  * This function is used to handle the new connection
  * @param newfd - client file descriptor 
  */

 void handleClient(void *vfd) { 
     char buff[1024]; 
     int* newfd = (int*)vfd;
     memset(buff, '\0', 1024); 
     int bytes = recv(*newfd, buff, 1024,0); 
     if (bytes < 0) { 
         perror("receive error"); 
     } 
     else if (bytes == 0){ 
         printf("pollserver: client disconected \n"); 
         RemoveHandler((void *)r, *newfd); 
         close(*newfd); 
     } 
     else { 
         for (int i = 0; i < r->size; ++i) { 
             if ((r->funcs[i] == handleClient)&&(r->fds[i].fd != *newfd)) {  
                 send(r->fds[i].fd, buff, bytes, 0); 
             } 
         } 
     } 
  
 } 
  
 void newClient(void *vlistener) { 
     int* listener = (int*)vlistener;
     int newfd; // Newly accept()ed socket descriptor 
     struct sockaddr_storage remoteaddr; // Client address 
     char remoteIP[INET6_ADDRSTRLEN]; 
     socklen_t addrlen = sizeof(remoteaddr); 
  
     newfd = accept(*listener, (struct sockaddr *) &remoteaddr, &addrlen); 
  
     if (newfd == -1) { 
         perror("accept"); 
     } else { 
         installHandler(r,handleClient, newfd); 
  
         printf("pollserver: new connection from %s on socket %d\n", 
                inet_ntop(remoteaddr.ss_family, get_in_addr((struct sockaddr *) &remoteaddr), remoteIP, 
                          INET6_ADDRSTRLEN), newfd); 
     } 
 } 
void onExit(int signal){ 
     DeleteReactor(r); 
     exit(0); 
 } 
  
 int main() { 
  
     signal(SIGINT, onExit); 
     r = (Reactor*) NewReactor(); 
     int listener = get_listener_socket(); 
     if (listener == -1) { 
         fprintf(stderr, "error getting listening socket\n"); 
         exit(1); 
     } 
     installHandler(r, newClient, listener); 
     printf("pollserver: waiting for new connections...\n"); 
     pthread_join((r->thread_id), NULL); 
     close(listener); 
  
     return 0; 
 }