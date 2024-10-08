#include "ashnik_file.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 
#include <assert.h>
#include <stdint.h>

uint64_t Stack_Hash(size_t razmer)
{
    uint64_t hash = 5381;

    for (size_t i = 0; i <= razmer; i++)
    {
        hash = hash * 33 + sizeof(uint8_t);
    }

    return hash;
}

// enum Ochibki_Stacka Sosi_Haker(Hash_t* hash_stuckt)
// {
//     // enum Ochibki_Stacka oshibka1 = StackError(stk);
//     // if(oshibka1 > 0) return oshibka1;
//     // if((stk) == NULL) return UKAZTENEL_NA_BUFER_HASH_POEHAL;

//     if (hash_stuckt->hash_nachalo != hash_stuckt->hash_nachalo) 
//     {
//         return PIZDA_HAKERI_SPIZDILI_CHO_TO;
//     }

//     return NET_OSHIBOK;
// }

enum Ochibki_Stacka StackProverkaKonoreek(Stack_t* stk)
{
    if ((*(StackElem_t*)((char*)stk->array_data - sizeof(uint64_t)) != KONOREYKA) 
       || (*(stk->array_data + stk->capacity) != KONOREYKA)          
       || (stk->konoreyka_left != KONOREYKA)                         
       || (stk->konoreyka_right != KONOREYKA)) 
    {
       return UKAZTENEL_NA_STRUKTURU_POEHAL;
    }

    return NET_OSHIBOK;
}

enum Ochibki_Stacka StackConstrtor(Stack_t* stk, size_t razmer)
{
    if ((stk) == NULL) 
    {
        return UKAZTENEL_NA_STRUKTURU_POEHAL;
    }

    stk->capacity = razmer;
    stk->vacant_place = 0;
    stk->array_data = (StackElem_t*)calloc(stk->capacity * sizeof(StackElem_t) + 2 * sizeof(uint64_t), 1);
    memcpy(stk->array_data, &KONOREYKA, sizeof(uint64_t));
    //stk->array_data = (StackElem_t*)((char*)(stk->array_data + 2));
    stk->array_data = (StackElem_t*)((char*)stk->array_data + sizeof(uint64_t));
    memcpy(stk->array_data + stk->capacity, &KONOREYKA, sizeof(uint64_t));

    if (StackProverkaKonoreek(stk) > 0) return StackProverkaKonoreek(stk);
    {
    return StackError(stk); //макрос лайн
    }
}

enum Ochibki_Stacka StackPush(Stack_t* stk, StackElem_t complement, Hash_t* hash_stuckt)
{
    if(stk->vacant_place > 0)
    {
        if (hash_stuckt->hash_konec != Stack_Hash(stk->capacity * sizeof(StackElem_t)))
        {
            return PIZDA_HAKERI_SPIZDILI_CHO_TO;
        }
    }

    enum Ochibki_Stacka oshibka1 = StackError(stk);
    if (oshibka1 > 0) 
    {
        return oshibka1;
    }
    if (StackProverkaKonoreek(stk) > 0) 
    {
        return StackProverkaKonoreek(stk);
    }
    if(stk->vacant_place == stk->capacity)           
    {
        stk->capacity = StackRecalloc(stk);
    }

    stk->array_data[stk->vacant_place] = complement;
    stk->vacant_place++;

    //printf("\n%lld\n", Stack_Hash(stk));
    if (StackProverkaKonoreek(stk) > 0) 
    {
        return StackProverkaKonoreek(stk);
    }

    hash_stuckt->hash_konec = Stack_Hash(stk->capacity * sizeof(StackElem_t));

    return StackError(stk);
}

enum Ochibki_Stacka StackPop(Stack_t* stk, StackElem_t* last_recorded_value, Hash_t* hash_stuckt)
{
    if(stk->vacant_place > 0)
    {
        if (hash_stuckt->hash_konec != Stack_Hash(stk->capacity * sizeof(StackElem_t)))
        {
            return PIZDA_HAKERI_SPIZDILI_CHO_TO;
        }
    }

    if (StackProverkaKonoreek(stk) > 0) 
    {
        return StackProverkaKonoreek(stk);
    }
    enum Ochibki_Stacka oshibka = StackError(stk);
    if (oshibka > 0) 
    {
        return oshibka;
    }
    if ((last_recorded_value) == NULL) 
    {
        return OSHIBKA_V_VINIMANII_ZNACHENIA_IS_STEKA;
    }

    --stk->vacant_place;
    *last_recorded_value = stk->array_data[stk->vacant_place];

    if (StackProverkaKonoreek(stk) > 0) 
    {
        return StackProverkaKonoreek(stk);
    }

    hash_stuckt->hash_konec = Stack_Hash(stk->capacity * sizeof(StackElem_t));

    return StackError(stk);
}

int StackDtor(Stack_t* stk)
{
    if (StackProverkaKonoreek(stk) > 0)
    {
         return StackProverkaKonoreek(stk);
    }

    enum Ochibki_Stacka oshibka = StackError(stk);
    if (oshibka > 0) 
    {
        return oshibka;
    }

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
    if ((stk) == NULL)
    {
        fprintf(stderr, "UKAZTENEL_NA_STRUKTURU_POEHAL");
        return UKAZTENEL_NA_STRUKTURU_POEHAL;
    }

    if ((stk->array_data) == NULL) 
    {
        fprintf(stderr, "UKAZTENEL_NA_MASSIV_POEHAL");
        return UKAZTENEL_NA_MASSIV_POEHAL;
    }

    if (stk->vacant_place > stk->capacity) 
    {
        fprintf(stderr, "VACANTNOE_MESTO_POEHALO");
        return VACANTNOE_MESTO_POEHALO;
    }

    return NET_OSHIBOK;
}

size_t StackRecalloc(Stack_t* stk)
{
    stk->array_data = (StackElem_t*)((char*)stk->array_data - sizeof(uint64_t));
    stk->array_data = (StackElem_t*)realloc(stk->array_data, stk->capacity * SHAG_V_REALOC * sizeof(StackElem_t) + 2*sizeof(uint64_t));
    if ((stk->array_data) == NULL)
    {
         return UKAZTENEL_NA_MASSIV_POEHAL;
    }

    stk->array_data = (StackElem_t*)((char*)stk->array_data + sizeof(uint64_t));
    size_t capacity_bufer = stk->capacity * SHAG_V_REALOC;

    memset(stk->array_data + stk->capacity, 0, sizeof(uint64_t) + stk->capacity * sizeof(StackElem_t));

    memcpy(stk->array_data + capacity_bufer, &KONOREYKA, sizeof(uint64_t));

    return capacity_bufer;
}

void StackDump(Stack_t* stk)
{
    fprintf(stderr, "\nЛевая конорейка структуры = %lu\nАдрес левой конорейки структуры = %p", stk->konoreyka_left, &stk->konoreyka_left);
    fprintf(stderr, "\nАдрес начала массива = %p", (char*)stk->array_data - sizeof(uint64_t));
    fprintf(stderr, "\nЛевая конорейка массива = %lu\nАдрес левой конорейки массива = %p", *(StackElem_t*)((char*)stk->array_data - sizeof(uint64_t)), ((StackElem_t*)((char*)stk->array_data - sizeof(uint64_t))));
    fprintf(stderr, "\nРазмер массива в StackElem_t = %lu", (stk->capacity * sizeof(StackElem_t) + 2*sizeof(uint64_t)) / sizeof(StackElem_t));
    fprintf(stderr, "\nРазмер массива в байтах = %lu", stk->capacity * sizeof(StackElem_t) + 2*sizeof(uint64_t));
    fprintf(stderr, "\nАдрес начала стека = %p", stk->array_data);
    fprintf(stderr, "\nРазмер стека в StackElem_t = %lu", stk->vacant_place);
    fprintf(stderr, "\nРазмер стека в байтах = %lu\n", stk->vacant_place * sizeof(StackElem_t));
    fprintf(stderr, "Элементы массива\n{\n");
    for (long long i = stk->vacant_place - 1; i >= 0; i--)
    {
        fprintf(stderr, "   [%lu] = ", i);
        fprintf(stderr, "%lu\n", stk->array_data[i]);
    }
    fprintf(stderr,"}");
    fprintf(stderr, "\nАдрес конца стека = %p", stk->array_data + stk->vacant_place);
    fprintf(stderr, "\nПравая конорейка массива = %lu\nАдрес правой конорейки массива = %p", *(stk->array_data + stk->capacity), stk->array_data + stk->capacity);
    fprintf(stderr, "\nАдрес конца массива = %p", (char*)(stk->array_data + stk->capacity) + sizeof(uint64_t));
    fprintf(stderr, "\nПравая конорейка структуры = %lu\nАдрес правой конорейки структуры = %p\n", stk->konoreyka_right, &stk->konoreyka_right);
}