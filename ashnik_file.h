#ifndef ASHNIK_FILE_H
#define ASHNIK_FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

typedef int StackElem_t;
const int SHAG_V_REALOC = 2;

enum Ochibki_Stacka
{
    NET_OSHIBOK = 0,
    UKAZTENEL_NA_STRUKTURU_POEHAL = 1,
    UKAZTENEL_NA_MASSIV_POEHAL = 2,
    VACANTNOE_MESTO_POEHALO = 3,
    OSHIBKA_V_VINIMANII_ZNACHENIA_IS_STEKA = 4,
    UKAZTENEL_NA_BUFER_HASH_POEHAL = 5,
    PIZDA_HAKERI_SPIZDILI_CHO_TO = 6
};

struct Stack_t
{
    StackElem_t* array_data;
    size_t vacant_place;
    size_t capacity;
};

//enum Ochibki_Stacka Sosi_Haker(Stack_t* stk, long long *bufer_hash);
//long long Stack_Hash(Stack_t* stk);
enum Ochibki_Stacka StackConstrtor(Stack_t* stk, size_t razmer);
enum Ochibki_Stacka StackPush(Stack_t* stk, StackElem_t complement);
size_t StackRecalloc(Stack_t* stk);
enum Ochibki_Stacka StackPop(Stack_t* stk, StackElem_t* last_recorded_value);
int StackDtor(Stack_t* stk);
enum Ochibki_Stacka StackError(Stack_t* stk);

#endif