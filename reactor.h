struct _pollfd;
typedef struct _pollfd pollfd;
struct _reactor;
typedef struct _reactor Reactor;

void* NewReactor();
void installHandler(Reactor* r,void (*func)(void *),int fd);
void RemoveHandler(void *reactor, int fd); 
void DeleteReactor(Reactor *r);