//**NODESTRUCTURE**//
struct _node;
typedef struct _node node;

node* createNode(int data);
void destroyNode(node* n);




//**QUEUEUSTRUCTURE**//
struct  _queue;
typedef struct _queue queue;

queue* createQ();
void destroyQ(queue* q);
void enQ(queue* q, int data);
int deQ(queue* q);
