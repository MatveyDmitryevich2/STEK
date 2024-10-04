#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "stack.h"
// FIXME нейминг

int main(void)
{
    Stack_t stk = {};

    if (StackConstrtor(&stk, 2) > 0) assert(0);
    
    //fprintf(stderr, "%d\n", StackPush(&stk, 2));
    if (StackPush(&stk, 13) > 0) assert(0);
    if (StackPush(&stk, 5) > 0) assert(0);
    if (StackPush(&stk, 6) > 0) assert(0);
    if (StackPush(&stk, 7) > 0) assert(0);
    if (StackPush(&stk, 8) > 0) assert(0);
    if (StackPush(&stk, 9) > 0) assert(0);
    if (StackPush(&stk, 7) > 0) assert(0);
    if (StackPush(&stk, 7) > 0) assert(0);
    if (StackPush(&stk, 7) > 0) assert(0);
    if (StackPush(&stk, 7) > 0) assert(0);
    if (StackPush(&stk, 7) > 0) assert(0);
    if (StackPush(&stk, 7) > 0) assert(0);
    if (StackPush(&stk, 7) > 0) assert(0);
    if (StackPush(&stk, 8) > 0) assert(0);
    if (StackPush(&stk, 8) > 0) assert(0);
    if (StackPush(&stk, 8) > 0) assert(0);
    if (StackPush(&stk, 8) > 0) assert(0);
    if (StackPush(&stk, 8) > 0) assert(0);
    
    StackDump(&stk);

    StackElem_t last_recorded_value = 0;
    
    if (StackPop(&stk, &last_recorded_value) > 0) assert(0);
    if (StackPop(&stk, &last_recorded_value) > 0) assert(0);
    if (StackPop(&stk, &last_recorded_value) > 0) assert(0);
    if (StackPop(&stk, &last_recorded_value) > 0) assert(0);
    if (StackPop(&stk, &last_recorded_value) > 0) assert(0);
    if (StackPop(&stk, &last_recorded_value) > 0) assert(0);
    if (StackPop(&stk, &last_recorded_value) > 0) assert(0);
    if (StackPop(&stk, &last_recorded_value) > 0) assert(0);
    if (StackPop(&stk, &last_recorded_value) > 0) assert(0);
    if (StackPop(&stk, &last_recorded_value) > 0) assert(0);


    StackDump(&stk);

    StackDtor(&stk);

    return 0;
}