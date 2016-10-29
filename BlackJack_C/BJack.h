#ifndef _BJACK_H
#define _BJACK_H

#ifdef __cplusplus
	#define _BJACK_CAN_WORK
#elif __STDC_VERSION__ >= 199901L
	#define _BJACK_CAN_WORK
#endif
#ifndef _BJACK_CAN_WORK
#error "BlackJack requires C99 or C++ to work!"
#else

#include "GenList.h"

// BlackJack State
typedef enum BJS {
	BJS_NOTHING,
	BJS_YOU_BUSTED,
	BJS_COM_BUSTED,
	BJS_BOTH_BUSTED,
	BJS_YOU_EXACT21,
	BJS_COM_EXACT21,
	BJS_BOTH_EXACT21,
	BJS_YOU_5CARD,
	BJS_COM_5CARD,
	BJS_BOTH_5CARD,
	BJS_YOU_SCORE_BETTER,
	BJS_COM_SCORE_BETTER,
	BJS_SAME_SCORE
} BJS;

BJS  BJack_State();
void BJack_TakeCard();
void BJack_Deal();
void BJack_Reset();
void BJack_Dispose();

// PARAM1: Pointer to int array of size 5
// RETURN: Number of cards
int  BJack_GetPlayerCards(int*);
int  BJack_GetComCards(int*);

int  BJack_GetPlayerPoint();
int  BJack_GetComPoint();

#endif//_BJACK_CAN_WORK
#endif//_BJACK_H
