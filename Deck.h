#ifndef DECK_H
#define DECK_H
#include "Types.h"
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
        int card_list[10];
        int card_index, card_count;
        const int seed = 0;
        // create mt19937 object and seed it with 0


    public:
        Deck();
        float get_card_prob(int card_idx) { // card_idx := (int)(card - 2)
            return (float)card_list[card_idx] / card_count;
        }
        void set_cards(int card_list[10]);
        void shuffle(std::mt19937 &rng);
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
            if (card_count == 0) throw std::runtime_error("No cards left in deck");
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
Deck::Deck() : card_index(0) {

}
void Deck::set_cards(int card_list[10]) {
    card_count = 0;
    for (int i = 0; i < 10; i++) {
        this->card_list[i] = card_list[i];
        card_count += card_list[i];
    }
    cards.clear();
    cards.reserve(card_count);
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < card_list[i]; j++) {
            cards.push_back(static_cast<Card>(i + 2));
        }
    }
}
void Deck::shuffle(std::mt19937 &rng) {
    card_index = 0;
    std::shuffle(cards.begin(), cards.end(), rng);
}



#endif