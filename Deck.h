#ifndef DECK_H
#define DECK_H
#include "Types.h"
#include <vector>
#include <stack>
#include <tuple>
#include <algorithm>
#include <iostream>
#include <random>


int DECK_1[12] = { 52, 4, 4, 4, 4, 4, 4, 4, 4, 4, 16, 4,};
class Deck {
    private:
        std::vector<Card> cards;
        int card_list[12];
        int card_index;
        const int seed = 0;
        std::mt19937 rng;
    public:
        Deck();
        void set_cards(int card_list[12]);
        void shuffle();
        void print_card_list() {
            for (int i = 0; i < 12; i++) {
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
            return cards[card_index++];
        }
};
Deck::Deck() : card_index(0) {
    rng.seed(seed);
}
void Deck::set_cards(int card_list[12]) {
    for (int i = 0; i < 12; i++) {
        this->card_list[i] = card_list[i];
    }
    cards.clear();
    cards.reserve(card_list[0]);
    for (int i = 2; i < 12; i++) {
        for (int j = 0; j < card_list[i]; j++) {
            cards.push_back(static_cast<Card>(i));
        }
    }
}
void Deck::shuffle() {
    std::shuffle(cards.begin(), cards.end(), rng);
}



#endif