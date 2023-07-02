
#ifndef HAND_H
#define HAND_H

#include "Types.h"
#include "Deck.h"
#include <cassert>



class Hand {
    private:
        std::vector<Card> cards;
        int score, high_aces; // updated automatically
    public:
        bool can_split, can_double, can_hit; // updated manually
        Hand();
        Hand(Card card);
        void add(Card card);
        void split(Deck &deck, std::stack<Hand> &hands);
        int get_score() { return score; }
        bool is_pair() { return cards.size() == 2 && cards[0] == cards[1]; }
        bool is_busted() { return score > 21; }
        bool is_blackjack() { return score == 21 && cards.size() == 2; }
        bool is_house_stands() { return score >= 17; }
        // overload stream
        friend std::ostream& operator<<(std::ostream& os, const Hand& hand) {
            for (int i = 0; i < hand.cards.size(); i++) {
                os << hand.cards[i] << " ";
            }
            return os;
        }
       
};

Hand::Hand() : score(0), high_aces(0), can_split(false), can_double(true), can_hit(true) {
    cards.reserve(5);
}
Hand::Hand(Card card) {
    score = card;
    high_aces = (card == ACE);
    cards.reserve(5);
    cards.push_back(card);
}
void Hand::add(Card card) {
    cards.push_back(card);
    score += card;
    if (card == ACE) {
        high_aces++;
    }
    if (score > 21 && high_aces > 0) {
        score -= 10;
        high_aces--;
    }
}

void Hand::split(Deck &deck, std::stack<Hand> &hands) {
    // it is assumed that the hand can be split
    Hand hand1 = Hand(cards[0]);
    hand1.add(deck.draw());
    Hand hand2 = Hand(cards[1]);
    hand2.add(deck.draw());
    if (cards[0] == ACE) {
        hand1.can_double = false;
        hand2.can_double = false;
        hand1.can_hit = false;
        hand2.can_hit = false;
    }
    hands.push(hand1);
    hands.push(hand2);
}

#endif


