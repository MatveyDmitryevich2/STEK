#include "ashnik_file.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 
#include <assert.h>
#include <stdint.h>

// long long Stack_Hash(Stack_t* stk)
// {
//     enum Ochibki_Stacka oshibka1 = StackError(stk);
//     if(oshibka1 > 0) return oshibka1;

//     long long hash =  5381;

//     for(int i = 0; i <= stk->vacant_place - 1; i++)
//     {
//         hash = hash * 33 + stk->array_data[i];
//     }

//     return hash;
// }

// enum Ochibki_Stacka Sosi_Haker(Stack_t* stk, long long *bufer_hash)
// {
//     enum Ochibki_Stacka oshibka1 = StackError(stk);
//     if(oshibka1 > 0) return oshibka1;
//     if((stk) == NULL) return UKAZTENEL_NA_BUFER_HASH_POEHAL;

//     if(*bufer_hash != Stack_Hash(stk)) return PIZDA_HAKERI_SPIZDILI_CHO_TO;;

//     return NET_OSHIBOK;
// }

enum Ochibki_Stacka StackProverkaKonoreek(Stack_t* stk)
{
    if((*(StackElem_t*)((char*)stk->array_data - 8) != KONOREYKA)             ||
       (*(stk->array_data + stk->capacity) != KONOREYKA) ||
       (stk->konoreyka_left != KONOREYKA)                ||
       (stk->konoreyka_right != KONOREYKA)) return UKAZTENEL_NA_STRUKTURU_POEHAL;

    return NET_OSHIBOK;
}

enum Ochibki_Stacka StackConstrtor(Stack_t* stk, size_t razmer)
{
    if((stk) == NULL) return UKAZTENEL_NA_STRUKTURU_POEHAL;

    stk->capacity = razmer;
    stk->vacant_place = 0;
    stk->array_data = (StackElem_t*)calloc(stk->capacity * sizeof(StackElem_t) + 2*sizeof(uint64_t), 1);
    memcpy(stk->array_data, &KONOREYKA, sizeof(uint64_t));
    //stk->array_data = (StackElem_t*)((char*)(stk->array_data + 2));
    stk->array_data = (StackElem_t*)((char*)stk->array_data + 8);
    memcpy(stk->array_data + stk->capacity, &KONOREYKA, sizeof(uint64_t));

    if(StackProverkaKonoreek(stk) > 0) return StackProverkaKonoreek(stk);
    return StackError(stk);
}

enum Ochibki_Stacka StackPush(Stack_t* stk, StackElem_t complement)
{
    enum Ochibki_Stacka oshibka1 = StackError(stk);
    if(oshibka1 > 0) return oshibka1;
    if(StackProverkaKonoreek(stk) > 0) return StackProverkaKonoreek(stk);

    if(stk->vacant_place == stk->capacity - 1)           
    {
        stk->capacity = StackRecalloc(stk);
    }

    stk->array_data[stk->vacant_place] = complement;
    stk->vacant_place++;

    //printf("\n%lld\n", Stack_Hash(stk));
    if(StackProverkaKonoreek(stk) > 0) return StackProverkaKonoreek(stk);
    return StackError(stk);
}

enum Ochibki_Stacka StackPop(Stack_t* stk, StackElem_t* last_recorded_value)
{
    if(StackProverkaKonoreek(stk) > 0) return StackProverkaKonoreek(stk);
    enum Ochibki_Stacka oshibka = StackError(stk);
    if(oshibka > 0) return oshibka;
    if((last_recorded_value) == NULL) return OSHIBKA_V_VINIMANII_ZNACHENIA_IS_STEKA;

    --stk->vacant_place;
    *last_recorded_value = stk->array_data[stk->vacant_place];

    if(StackProverkaKonoreek(stk) > 0) return StackProverkaKonoreek(stk);
    return StackError(stk);
}

int StackDtor(Stack_t* stk)
{
    if(StackProverkaKonoreek(stk) > 0) return StackProverkaKonoreek(stk);

    enum Ochibki_Stacka oshibka = StackError(stk);
    if(oshibka > 0) return oshibka;

    for (size_t i = 0; i < stk->capacity; i++)
    {
        stk->array_data[i] = 0;
    }
    
    free(stk->array_data - 2);
    stk->array_data = NULL;

    return 0;
}

enum Ochibki_Stacka StackError(Stack_t* stk)
{
    if((stk) == NULL)
    {
        fprintf(stderr, "UKAZTENEL_NA_STRUKTURU_POEHAL");
        return UKAZTENEL_NA_STRUKTURU_POEHAL;
    }

    if((stk->array_data) == NULL) 
    {
        fprintf(stderr, "UKAZTENEL_NA_MASSIV_POEHAL");
        return UKAZTENEL_NA_MASSIV_POEHAL;
    }

    if(stk->vacant_place > stk->capacity) 
    {
        fprintf(stderr, "VACANTNOE_MESTO_POEHALO");
        return VACANTNOE_MESTO_POEHALO;
    }

    return NET_OSHIBOK;
}

size_t StackRecalloc(Stack_t* stk)
{
    stk->array_data = (StackElem_t*)((char*)stk->array_data - 8);
    stk->array_data = (StackElem_t*)realloc(stk->array_data, stk->capacity * SHAG_V_REALOC * sizeof(StackElem_t) + 2*sizeof(uint64_t));
    if((stk->array_data) == NULL) return UKAZTENEL_NA_MASSIV_POEHAL;

    stk->array_data = (StackElem_t*)((char*)stk->array_data + 8);
    size_t capacity_bufer = stk->capacity * SHAG_V_REALOC;

    memset(stk->array_data + stk->capacity, 0, 8 + stk->capacity * sizeof(StackElem_t));

    memcpy(stk->array_data + capacity_bufer, &KONOREYKA, sizeof(uint64_t));

    return capacity_bufer;
}