//**NODESTRUCTURE**//
struct _node;
typedef struct _node node;

node* createNode();
void destroyNode(node* n);


//**QUEUEUSTRUCTURE**//
struct  _queue;
typedef struct _queue queue;

queue* createQ();
void destroyQ(queue* q);
void enQ(queue* q, void* data);
void* deQ(queue* q);
int isEmpty(queue* q);
void updatefd(queue* q,int fd);
int getFD(queue* q);
