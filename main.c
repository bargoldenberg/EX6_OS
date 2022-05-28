#include<stdio.h>
#include "Queue.h"

int main(){
    queue *q = createQ();
    enQ(q, 1);
    enQ(q, 2);
    enQ(q, 3);
    printf("%d\n", deQ(q));
    printf("%d\n", deQ(q));
    destroyQ(q);
}