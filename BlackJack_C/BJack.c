#include "BJack.h"

// Private declaration -----

#define BJ_CARD_MIN		1
#define BJ_CARD_MAX		13
#define RND(min, max)	(rand() % ((max)-(min)+1) + (min))

typedef GENLIST CardList;
CardList unused_cards, player_cards, com_cards;
BJS  bjs;
int  want_deal;

int  GetPointOf(CardList const);
int  GetRandomCard();
void ComTurn();
void UpdateState();

// Public function declaration -----

BJS  BJack_State() {
	return bjs;
}

void BJack_TakeCard() {
	if (bjs != BJS_NOTHING) { return; }
	int card = GetRandomCard();
	GenList_Add(player_cards, &card);
	ComTurn();
	UpdateState();
}

void BJack_Deal() {
	if (bjs != BJS_NOTHING) { return; }
	want_deal = 1;
	ComTurn();
	UpdateState();
}

void BJack_Reset() {
	bjs = BJS_NOTHING;
	want_deal = 0;
	// Create CardList as needed
	if (!unused_cards) {GenList_Create(&unused_cards, sizeof(int), true);}
	if (!player_cards) {GenList_Create(&player_cards, sizeof(int), true);}
	if (!com_cards) {GenList_Create(&com_cards, sizeof(int), true);}
	do {
		// Prepare unused cards
		GenList_Clear(unused_cards);
		for (int i = BJ_CARD_MIN; i <= BJ_CARD_MAX; ++i) {
			for (int j = 0; j < 4; ++j) {
				GenList_Add(unused_cards, &i);
			}
		}
		// Pass 2 cards to Player
		int card;
        GenList_Clear(player_cards);
        card = GetRandomCard();
        GenList_Add(player_cards, &card);
        card = GetRandomCard();
	    GenList_Add(player_cards, &card);
	    // Pass 2 cards to COM
	    GenList_Clear(com_cards);
        card = GetRandomCard();
        GenList_Add(com_cards, &card);
        card = GetRandomCard();
	    GenList_Add(com_cards, &card);
	    // If any player busted at this point, reset
	} while (BJack_GetComPoint() >= 21 || BJack_GetPlayerPoint() >= 21);
}

void BJack_Dispose() {
	if (unused_cards) {GenList_Destroy(unused_cards);}
	if (player_cards) {GenList_Destroy(player_cards);}
	if (com_cards) {GenList_Destroy(com_cards);}
}

int  BJack_GetPlayerCards(int *arr) {
	size_t count;
	GenList_GetInfo(player_cards, &count, NULL, NULL);
	for (size_t i = 0; i < count; ++i) {
		GenList_GetAt(player_cards, i, (void**)&arr[i]);
	}
	return count;
}

int  BJack_GetComCards(int *arr) {
	size_t count;
	GenList_GetInfo(com_cards, &count, NULL, NULL);
	for (size_t i = 0; i < count; ++i) {
		GenList_GetAt(com_cards, i, (void**)&arr[i]);
	}
	return count;
}

int  BJack_GetPlayerPoint() {
	return GetPointOf(player_cards);
}

int  BJack_GetComPoint() {
	return GetPointOf(com_cards);
}

// Private function definition -----

int  GetPointOf(CardList const cards) {
	int point = 0;
	size_t card_count, ace_count = 0;
	GenList_GetInfo(cards, &card_count, NULL, NULL);
	for (size_t i = 0; i < card_count; ++i) {
		int card;
		GenList_GetAt(cards, i, (void**)&card);
		if (card == 1) {
			++ ace_count;
			point += 11;
		}
		else if (card > 10) {
			point += 10;
		}
		else {
			point += card;
		}
	}
	while (point > 21 && ace_count > 0) {
		-- ace_count;
		point -= 10;
	}
	return point;
}

int  GetRandomCard() {
	int card;
	size_t count;
	GenList_GetInfo(unused_cards, &count, NULL, NULL);
	size_t i = RND(0, count - 1);
	GenList_GetAt(unused_cards, i, (void**)&card);
	GenList_RemoveAt(unused_cards, i);
	return card;
}

void ComTurn() {
	double rate = (20 - BJack_GetComPoint()) / 20.0;
    if (rate + (RND(1,5) * 0.1) > 0.5) {    
		int card = GetRandomCard();        	
		GenList_Add(com_cards, &card);
    }
}

void UpdateState() {
	int player_point = BJack_GetPlayerPoint();
	int com_point = BJack_GetComPoint();
	
	size_t player_cards_count, com_cards_count;
	GenList_GetInfo(player_cards, &player_cards_count, NULL, NULL);
	GenList_GetInfo(com_cards, &com_cards_count, NULL, NULL);
	
	if (player_point > 21 && com_point > 21) {
		bjs = BJS_BOTH_BUSTED;
	}
	else if (player_cards_count >= 5 && com_cards_count >= 5) {
		bjs = BJS_BOTH_5CARD;
	}
	else if (player_point > 21) {
		bjs = BJS_YOU_BUSTED;
	}
	else if (player_point == 21) {
		bjs = BJS_YOU_EXACT21;
	}
	else if (player_cards_count >= 5) {
		bjs = BJS_YOU_5CARD;
	}
	else if (com_point > 21) {
		bjs = BJS_COM_BUSTED;
	}
	else if (com_point == 21) {
		bjs = BJS_COM_EXACT21;
	}
	else if (com_cards_count >= 5) {
		bjs = BJS_COM_5CARD;
	}
	else if (want_deal) {
		if (player_point > com_point) {
			bjs = BJS_YOU_SCORE_BETTER;
		}
		else if (com_point > player_point) {
			bjs = BJS_COM_SCORE_BETTER;
		}
		else {
			bjs = BJS_SAME_SCORE;
		}
	}
}

