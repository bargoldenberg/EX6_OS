#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Queue.h"
/**
 * ACTIVE OBJECT
 * AUTHORS : BAR GOLDENBERG & SAPPIR BOHBOT
 * 
 */

//STRUCT DEFINITIONS//
struct _active_object;
typedef struct _active_object activeObject;

//FUNCTION DECLARATIONS//
void* runAO(void* AO);
activeObject* createAO(queue* q, void (*func1)(void*), void (*func2)(void*));
void destroyAO(activeObject* AO);

