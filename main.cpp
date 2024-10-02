#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "ashnik_file.h"
// FIXME нейминг

int main(void) //сделать чтобы память уменьшалась если ее дохуя жеска
{
    Stack_t stk = {};
    long long bufer_hash = 0;

    if(StackConstrtor(&stk, 2) > 0) assert(0);
    
    if(StackPush(&stk, 13) > 0) assert(0);
    if(StackPush(&stk, 5) > 0) assert(0);

    for(int i = stk.vacant_place - 1; i >= 0; i--)
        printf("%lu\n", stk.array_data[i]);

    StackElem_t last_recorded_value = 0;
    if(StackPop(&stk, &last_recorded_value) > 0) assert(0);

    printf("\n %d \n", last_recorded_value);

    StackDtor(&stk);

    return 0;
}