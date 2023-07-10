#ifndef DECK_H
#define DECK_H
#include "Types.h"
#include "Tools.h"
#include <vector>
#include <stack>
#include <tuple>
#include <algorithm>
#include <iostream>
#include <random>


int DEFAULT_DECK[10] = {4, 4, 4, 4, 4, 4, 4, 4, 16, 4}; // 2, 3, 4, 5, 6, 7, 8, 9, 10, ACE
// Cards: 2, 3, 4, 5, 6, 7, 8, 9, 10, ACE
// Corresponding index: 0, 1, 2, 3, 4, 5, 6, 7, 8, 9

class Deck {
    protected:
        std::vector<Card> cards;
        int card_list[10], card_list_0[10];
        int card_index = 0, card_count, min_card_count;
        float deck_penetration;


    public:
        Deck(float deck_penetration); // !!!
        Deck() : Deck(1) {} // full penetration 
        float get_card_prob(int card_idx) { // card_idx := (int)(card - 2)
            return (float)card_list[card_idx] / card_count;
        }
        int get_card_count() {
            return card_count;
        }
        void set_cards(int card_list[10]);
        void reset();
        void shuffle();
        void print_card_list() {
            for (int i = 0; i < 10; i++) {
                std::cout << card_list[i] << " ";
            }
            std::cout << std::endl;
        }
        void print_cards() {
            for (int i = 0; i < cards.size(); i++) {
                std::cout << cards[i] << " ";
            }
            std::cout << std::endl;
        }
        Card draw() {
            if (card_count <= min_card_count) reset(); // might change to shuffle
            Card card = cards[card_index++];
            card_list[card - 2]--;
            card_count--;
            return card;
        }
        Card peek() {
            return cards[card_index];
        }
    friend class Laboratory;
};
Deck::Deck(float deck_penetration) : deck_penetration(deck_penetration) {
}
void Deck::shuffle() {
    card_index = 0;
    card_count = 0;
    for (int i = 0; i < 10; i++) {
        card_list[i] = card_list_0[i];
        card_count += card_list[i];
    }
    std::shuffle(cards.begin(), cards.end(), GLOBAL_RNG);
}
void Deck::reset() {
    card_index = 0;
    card_count = 0;
    for (int i = 0; i < 10; i++) {
        card_list[i] = card_list_0[i];
        card_count += card_list[i];
    }
}
void Deck::set_cards(int card_list[10]) {
    card_count = 0;
    for (int i = 0; i < 10; i++) {
        this->card_list[i] = card_list[i];
        this->card_list_0[i] = card_list[i];
        card_count += card_list[i];
    }
    cards.clear();
    cards.reserve(card_count);

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < card_list[i]; j++) {
            cards.push_back(static_cast<Card>(i + 2));
        }
    }
    min_card_count = (int)((1 - deck_penetration) * card_count);
}


#endif