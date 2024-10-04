#include "stack.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 
#include <assert.h>
#include <stdint.h>

enum Oshibki_Stacka StackConstrtor(Stack_t* stk, size_t razmer)
{
    if ((stk) == NULL) { return UKAZTENEL_NA_STRUKTURU_POEHAL; }

    stk->konoreyka_left = KONOREYKA;
    stk->konoreyka_right = KONOREYKA;
    stk->capacity = razmer;
    stk->vacant_place = 0;

    stk->array_data = (StackElem_t*)calloc(stk->capacity * sizeof(StackElem_t) + 2 * sizeof(KONOREYKA), 1);
    if (stk->array_data == NULL) { return UKAZTENEL_NA_MASSIV_POEHAL; }

    memcpy(stk->array_data, &KONOREYKA, sizeof(KONOREYKA));
    stk->array_data = (StackElem_t*)((char*)stk->array_data + sizeof(KONOREYKA));
    memcpy(stk->array_data + stk->capacity, &KONOREYKA, sizeof(KONOREYKA));


    enum Oshibki_Stacka err = Pereschot_Hasha_v_konce_funccii(stk);
    if (err != NET_OSHIBOK) { return err; }
    return StackError(stk);
}

enum Oshibki_Stacka StackPush(Stack_t* stk, StackElem_t complement)
{
    enum Oshibki_Stacka oshibka1 = StackError(stk);
    if (oshibka1 != NET_OSHIBOK) { return oshibka1; }


    if (stk->vacant_place == stk->capacity)           
    {
        oshibka1 = StackRecalloc(stk);
        if (oshibka1 != NET_OSHIBOK) { return oshibka1; }
    }

    stk->array_data[stk->vacant_place] = complement;
    stk->vacant_place++;


    oshibka1 = Pereschot_Hasha_v_konce_funccii(stk);
    if (oshibka1 != NET_OSHIBOK) { return oshibka1; }

    return StackError(stk);
}

enum Oshibki_Stacka StackPop(Stack_t* stk, StackElem_t* last_recorded_value)
{
    enum Oshibki_Stacka oshibka = StackError(stk);
    if (oshibka != NET_OSHIBOK) { return oshibka; }
    if (last_recorded_value == NULL) { return OSHIBKA_V_VINIMANII_ZNACHENIA_IS_STEKA; }
    if (stk->vacant_place <= stk->capacity / OBRATNIY_SHAG_V_REALOC)
    {
        oshibka = StackUmenshenieRealloc(stk);
        if (oshibka != NET_OSHIBOK) { return oshibka; }
    }


    --stk->vacant_place;
    *last_recorded_value = stk->array_data[stk->vacant_place];


    oshibka = Pereschot_Hasha_v_konce_funccii(stk);
    if (oshibka != NET_OSHIBOK) { return oshibka; }

    return StackError(stk);
}

enum Oshibki_Stacka StackRecalloc(Stack_t* stk)
{
    enum Oshibki_Stacka oshibka = StackError(stk);
    if (oshibka != NET_OSHIBOK) { return oshibka; }


    stk->array_data = (StackElem_t*)((char*)stk->array_data - sizeof(KONOREYKA));
    stk->array_data = (StackElem_t*)realloc(stk->array_data, stk->capacity * SHAG_V_REALOC 
                                                             * sizeof(StackElem_t) + 2*sizeof(uint64_t));


    if ((stk->array_data) == NULL) { return UKAZTENEL_NA_MASSIV_POEHAL; }
    stk->array_data = (StackElem_t*)((char*)stk->array_data + sizeof(KONOREYKA));


    size_t capacity_bufer = stk->capacity * SHAG_V_REALOC;
    memset(stk->array_data + stk->capacity, 0, sizeof(KONOREYKA) + stk->capacity * sizeof(StackElem_t));
    memcpy(stk->array_data + capacity_bufer, &KONOREYKA, sizeof(KONOREYKA));
    stk->capacity = capacity_bufer;


    oshibka = Pereschot_Hasha_v_konce_funccii(stk);
    if (oshibka != NET_OSHIBOK) { return oshibka; }

    return StackError(stk);
}

enum Oshibki_Stacka StackUmenshenieRealloc(Stack_t* stk)
{
    enum Oshibki_Stacka oshibka = StackError(stk);
    if (oshibka != NET_OSHIBOK) { return oshibka; }
    

    stk->array_data = (StackElem_t*)((char*)stk->array_data - sizeof(KONOREYKA));
    stk->array_data = (StackElem_t*)realloc(stk->array_data, (stk->capacity / OBRATNIY_SHAG_V_REALOC) 
                                                             * sizeof(StackElem_t) + 2*sizeof(uint64_t));


    if ((stk->array_data) == NULL) { return UKAZTENEL_NA_MASSIV_POEHAL; }


    stk->array_data = (StackElem_t*)((char*)stk->array_data + sizeof(KONOREYKA));
    stk->capacity = stk->capacity / OBRATNIY_SHAG_V_REALOC;
    memcpy(stk->array_data + stk->capacity, &KONOREYKA, sizeof(KONOREYKA));


    return StackError(stk);
}

enum Oshibki_Stacka StackDump(Stack_t* stk)
{
    enum Oshibki_Stacka oshibka = StackError(stk);
    if (oshibka != NET_OSHIBOK) { return oshibka; }
    

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

uint64_t Stack_Hash(const uint8_t* ukazatel, size_t razmer)
{
    if ((ukazatel) == NULL) { return UKAZTENEL_NA_STRUKTURU_POEHAL; }

    uint64_t hash = 5381;

    for (size_t i = 0; i <= razmer; i++) { hash = hash * 33 + (uint64_t)ukazatel[i]; }

    return hash;
}

enum Oshibki_Stacka Proverka_Hasha_v_nachale_funccii(Stack_t* stk)
{
    if ((stk) == NULL) { return UKAZTENEL_NA_STRUKTURU_POEHAL; }


    uint64_t bufer_hash_strucktura = stk->hash_strucktura;
    uint64_t bufer_hash_massiv = stk->hash_massiv;
    stk->hash_strucktura = 0;
    stk->hash_massiv = 0;

    if (bufer_hash_massiv != Stack_Hash((uint8_t*)stk->array_data, stk->capacity * sizeof(StackElem_t))
        || bufer_hash_strucktura != Stack_Hash((uint8_t*)stk, sizeof(stk))) 
    {return PIZDA_HAKERI_SPIZDILI_CHO_TO; }

    stk->hash_massiv = bufer_hash_massiv;
    stk->hash_strucktura = bufer_hash_strucktura;


    return NET_OSHIBOK;
}

enum Oshibki_Stacka Pereschot_Hasha_v_konce_funccii(Stack_t* stk)
{
    if ((stk) == NULL) { return UKAZTENEL_NA_STRUKTURU_POEHAL; }


    stk->hash_massiv = Stack_Hash((uint8_t*)stk->array_data, stk->capacity * sizeof(StackElem_t));
    stk->hash_strucktura = Stack_Hash((uint8_t*)stk, sizeof(stk));


    return NET_OSHIBOK;
}

enum Oshibki_Stacka StackProverkaKonoreek(Stack_t* stk)
{
    if ((stk) == NULL) { return UKAZTENEL_NA_STRUKTURU_POEHAL; }


    if ((memcmp((char*)stk->array_data - sizeof(KONOREYKA), &KONOREYKA, sizeof(KONOREYKA)))
        || (memcmp(&stk->konoreyka_left, &KONOREYKA, sizeof(KONOREYKA)))                  
        || (memcmp(stk->array_data + stk->capacity, &KONOREYKA, sizeof(KONOREYKA)))
        || (memcmp(&stk->konoreyka_right, &KONOREYKA, sizeof(KONOREYKA))))    
    { 
        return UKAZTENEL_NA_STRUKTURU_POEHAL; 
    }


    return NET_OSHIBOK;
}

int StackDtor(Stack_t* stk)
{
    enum Oshibki_Stacka oshibka = StackError(stk);
    if (oshibka != NET_OSHIBOK) { return oshibka; }


    for (size_t i = 0; i < stk->capacity; i++) { stk->array_data[i] = 0; }

    free(stk->array_data - 2);
    stk->array_data = NULL;


    return 0;
}

enum Oshibki_Stacka StackError(Stack_t* stk)
{
    enum Oshibki_Stacka err = Proverka_Hasha_v_nachale_funccii(stk);
    if (err != NET_OSHIBOK) { return err; }

    err = StackProverkaKonoreek(stk);
    if (err != NET_OSHIBOK)
    {
        fprintf(stderr, "KONREYKI_POEHALI");
        return err;
    }

//# ## x-macro

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
