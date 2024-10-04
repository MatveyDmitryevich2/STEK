#include "stack.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 
#include <assert.h>
#include <stdint.h>

uint64_t Stack_Hash(char* ukazatel, size_t razmer)
{
    uint64_t hash = 5381;

    for (size_t i = 0; i <= razmer; i++)
    {
        hash = hash * 33 + (uint64_t)ukazatel[i];
    }

    return hash;
}

enum Ochibki_Stacka StackProverkaKonoreek(Stack_t* stk)
{
    if ((*(StackElem_t*)((char*)stk->array_data - sizeof(KONOREYKA)) != KONOREYKA) 
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

    stk->konoreyka_left = KONOREYKA;
    stk->konoreyka_right = KONOREYKA;
    stk->capacity = razmer;
    stk->vacant_place = 0;

    stk->array_data = (StackElem_t*)calloc(stk->capacity * sizeof(StackElem_t) + 2 * sizeof(KONOREYKA), 1);
    if (stk->array_data == NULL)
    {
        return UKAZTENEL_NA_MASSIV_POEHAL;
    }

    memcpy(stk->array_data, &KONOREYKA, sizeof(KONOREYKA));
    stk->array_data = (StackElem_t*)((char*)stk->array_data + sizeof(KONOREYKA));
    memcpy(stk->array_data + stk->capacity, &KONOREYKA, sizeof(KONOREYKA));

    enum Ochibki_Stacka err = Pereschot_Hasha_v_konce_funccii(stk);
    if (err > 0)
    {
        return err;
    }

    return StackError(stk); //FIXME крос лайн
}

enum Ochibki_Stacka StackPush(Stack_t* stk, StackElem_t complement)
{
    enum Ochibki_Stacka oshibka1 = StackError(stk);
    if (oshibka1 > 0)
    {
        return oshibka1;
    }
    enum Ochibki_Stacka err = Proverka_Hasha_v_nachale_funccii(stk);
    if (err > 0)
    {
        return err;
    }

    if (stk->vacant_place == stk->capacity)           
    {
        err = StackRecalloc(stk);
        if (err > 0)
        {
            return err;
        }
    }

    stk->array_data[stk->vacant_place] = complement;
    stk->vacant_place++;

    err = Pereschot_Hasha_v_konce_funccii(stk);
    if (err > 0)
    {
        return err;
    }

    return StackError(stk);
}

enum Ochibki_Stacka StackPop(Stack_t* stk, StackElem_t* last_recorded_value)
{
    enum Ochibki_Stacka oshibka = StackError(stk);
    if (oshibka > 0) 
    {
        return oshibka;
    }
    if ((last_recorded_value) == NULL) 
    {
        return OSHIBKA_V_VINIMANII_ZNACHENIA_IS_STEKA;
    }
    enum Ochibki_Stacka err = Proverka_Hasha_v_nachale_funccii(stk);
    if (err > 0)
    {
        return err;
    }

    --stk->vacant_place;
    *last_recorded_value = stk->array_data[stk->vacant_place];

    if (stk->vacant_place <= stk->capacity/4)
    {
        err = StackUmenshenieRealloc(stk);
        if (err > 0)
        {
            return err;
        }
    }

    err = Pereschot_Hasha_v_konce_funccii(stk);
    if (err > 0)
    {
        return err;
    }

    return StackError(stk);
}
// FIXME realloc вниз

int StackDtor(Stack_t* stk)
{
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
    enum Ochibki_Stacka err = StackProverkaKonoreek(stk);
    if (err > 0)
    {
        fprintf(stderr, "KONREYKI_POEHALI");
        return err;
    }

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

enum Ochibki_Stacka StackRecalloc(Stack_t* stk)
{
    enum Ochibki_Stacka oshibka = StackError(stk);
    if (oshibka > 0) 
    {
        return oshibka;
    }

    stk->array_data = (StackElem_t*)((char*)stk->array_data - sizeof(KONOREYKA));
    stk->array_data = (StackElem_t*)realloc(stk->array_data, stk->capacity * SHAG_V_REALOC 
                                                             * sizeof(StackElem_t) + 2*sizeof(uint64_t));
    if ((stk->array_data) == NULL)
    {
        return UKAZTENEL_NA_MASSIV_POEHAL;
    }
    stk->array_data = (StackElem_t*)((char*)stk->array_data + sizeof(KONOREYKA));

    size_t capacity_bufer = stk->capacity * SHAG_V_REALOC;
    memset(stk->array_data + stk->capacity, 0, sizeof(KONOREYKA) + stk->capacity * sizeof(StackElem_t));
    memcpy(stk->array_data + capacity_bufer, &KONOREYKA, sizeof(KONOREYKA));
    stk->capacity = capacity_bufer;

    return StackError(stk);
}

enum Ochibki_Stacka StackUmenshenieRealloc(Stack_t* stk)
{
    enum Ochibki_Stacka oshibka = StackError(stk);
    if (oshibka > 0) 
    {
        return oshibka;
    }
    
    stk->array_data = (StackElem_t*)((char*)stk->array_data - sizeof(KONOREYKA));
    stk->array_data = (StackElem_t*)realloc(stk->array_data, (stk->capacity / OBRATNIY_SHAG_V_REALOC) 
                                                             * sizeof(StackElem_t) + 2*sizeof(uint64_t));
    if ((stk->array_data) == NULL)
    {
        return UKAZTENEL_NA_MASSIV_POEHAL;
    }
    stk->array_data = (StackElem_t*)((char*)stk->array_data + sizeof(KONOREYKA));
    stk->capacity = stk->capacity / OBRATNIY_SHAG_V_REALOC;

    memcpy(stk->array_data + stk->capacity, &KONOREYKA, sizeof(KONOREYKA));

    return StackError(stk);
}

enum Ochibki_Stacka Proverka_Hasha_v_nachale_funccii(Stack_t* stk)
{
    enum Ochibki_Stacka oshibka = StackError(stk);
    if (oshibka > 0) 
    {
        return oshibka;
    }

    uint64_t bufer_hash_strucktura = stk->hash_strucktura;
    uint64_t bufer_hash_massiv = stk->hash_massiv;
    stk->hash_strucktura = 0;
    stk->hash_massiv = 0;

    if (bufer_hash_massiv != Stack_Hash((char*)stk->array_data, stk->capacity * sizeof(StackElem_t))
        || bufer_hash_strucktura != Stack_Hash((char*)stk, sizeof(stk)))
    {
        return PIZDA_HAKERI_SPIZDILI_CHO_TO;
    }

    return StackError(stk);
}

enum Ochibki_Stacka Pereschot_Hasha_v_konce_funccii(Stack_t* stk)
{
    enum Ochibki_Stacka oshibka = StackError(stk);
    if (oshibka > 0) 
    {
        return oshibka;
    }

    stk->hash_massiv = 0;
    stk->hash_strucktura = 0;

    stk->hash_massiv = Stack_Hash((char*)stk->array_data, stk->capacity * sizeof(StackElem_t));
    stk->hash_strucktura = Stack_Hash((char*)stk, sizeof(stk));

    return StackError(stk);
}

enum Ochibki_Stacka StackDump(Stack_t* stk)
{
    enum Ochibki_Stacka oshibka = StackError(stk);
    if (oshibka > 0) 
    {
        return oshibka;
    }
    
    fprintf(stderr, "\nЛевая конорейка структуры = %lu\nАдрес левой конорейки структуры = %p",
                     stk->konoreyka_left, &stk->konoreyka_left);
    fprintf(stderr, "\nАдрес начала массива = %p", (char*)stk->array_data - sizeof(KONOREYKA));
    fprintf(stderr, "\nЛевая конорейка массива = %lu\nАдрес левой конорейки массива = %p", 
                     (long unsigned)*(StackElem_t*)((char*)stk->array_data - sizeof(KONOREYKA)),
                     ((StackElem_t*)((char*)stk->array_data - sizeof(uint64_t))));
    fprintf(stderr, "\nРазмер массива в StackElem_t = %lu", (stk->capacity * sizeof(StackElem_t) 
                                                             + 2*sizeof(KONOREYKA)) / sizeof(StackElem_t));
    fprintf(stderr, "\nРазмер массива в байтах = %lu", stk->capacity * sizeof(StackElem_t) 
                                                       + 2*sizeof(KONOREYKA));
    fprintf(stderr, "\nАдрес начала стека = %p", stk->array_data);
    fprintf(stderr, "\nРазмер стека в StackElem_t = %lu", stk->vacant_place);
    fprintf(stderr, "\nРазмер стека в байтах = %lu\n", stk->vacant_place * sizeof(StackElem_t));
    fprintf(stderr, "Элементы массива\n{\n");
    for (long long i = (long long)stk->vacant_place - 1; i >= 0; i--)
    {
        fprintf(stderr, "   [%lld] = ", i);
        fprintf(stderr, "%lu\n", (long unsigned)stk->array_data[i]);
    }
    fprintf(stderr,"}");
    fprintf(stderr, "\nАдрес конца стека = %p", stk->array_data + stk->vacant_place);
    fprintf(stderr, "\nПравая конорейка массива = %lu\nАдрес правой конорейки массива = %p",
                     (long unsigned)*(stk->array_data + stk->capacity), stk->array_data + stk->capacity);
    fprintf(stderr, "\nАдрес конца массива = %p", (char*)(stk->array_data + stk->capacity) 
                                                  + sizeof(KONOREYKA));
    fprintf(stderr, "\nПравая конорейка структуры = %lu\nАдрес правой конорейки структуры = %p\n",
                     stk->konoreyka_right, &stk->konoreyka_right);

    return StackError(stk);
}