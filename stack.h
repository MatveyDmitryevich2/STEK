#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdint.h>

typedef int StackElem_t;
const int SHAG_V_REALOC = 2;
const int OBRATNIY_SHAG_V_REALOC = 4;
const uint64_t KONOREYKA = 29304148;

enum Ochibki_Stacka
{
    NET_OSHIBOK                            = 0,
    UKAZTENEL_NA_STRUKTURU_POEHAL          = 1,
    UKAZTENEL_NA_MASSIV_POEHAL             = 2,
    VACANTNOE_MESTO_POEHALO                = 3,
    OSHIBKA_V_VINIMANII_ZNACHENIA_IS_STEKA = 4,
    UKAZTENEL_NA_BUFER_HASH_POEHAL         = 5,
    PIZDA_HAKERI_SPIZDILI_CHO_TO           = 6,
    KONREYKI_POEHALI                       = 7
};

struct Stack_t
{
    uint64_t konoreyka_left;
    StackElem_t* array_data;
    size_t vacant_place;
    size_t capacity;
    uint64_t konoreyka_right;
    uint64_t hash_massiv;
    uint64_t hash_strucktura;
};

//enum Ochibki_Stacka Sosi_Haker(Hash_t* hash_stuckt);
enum Ochibki_Stacka StackUmenshenieRealloc(Stack_t* stk);
enum Ochibki_Stacka Proverka_Hasha_v_nachale_funccii(Stack_t* stk);
enum Ochibki_Stacka Pereschot_Hasha_v_konce_funccii(Stack_t* stk);
uint64_t Stack_Hash(char* ukazatel, size_t razmer);
enum Ochibki_Stacka StackProverkaKonoreek(Stack_t* stk);
enum Ochibki_Stacka StackConstrtor(Stack_t* stk, size_t razmer);
enum Ochibki_Stacka StackPush(Stack_t* stk, StackElem_t complement);
enum Ochibki_Stacka StackRecalloc(Stack_t* stk);
enum Ochibki_Stacka StackPop(Stack_t* stk, StackElem_t* last_recorded_value);
int StackDtor(Stack_t* stk);
enum Ochibki_Stacka StackError(Stack_t* stk);
enum Ochibki_Stacka StackDump(Stack_t* stk);

#endif // STACK_H