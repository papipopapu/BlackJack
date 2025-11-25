/**
 * @file Hand.h
 * @brief Hand representation for player and dealer in Blackjack
 * 
 * Implements hand scoring with soft/hard ace handling,
 * splitting, and game state checks (blackjack, bust, etc.).
 */

#ifndef HAND_H
#define HAND_H

#include "Types.h"
#include "Deck.h"
#include <cassert>

/**
 * @brief Represents a hand of cards in Blackjack
 * 
 * Automatically manages ace values (11 or 1) to avoid busting when possible.
 * Tracks available actions (hit, stand, double, split) based on game state.
 */
class Hand {
private:
    std::vector<Card> cards;  // Cards in hand
    int score;                // Current hand score
    int high_aces;            // Number of aces counted as 11

public:
    bool can_split;   // True if hand can be split (pair of same rank)
    bool can_double;  // True if doubling down is allowed
    bool can_hit;     // True if hitting is allowed
    
    /**
     * @brief Default constructor - creates empty hand
     */
    Hand();
    
    /**
     * @brief Construct hand with initial card
     * @param card First card in hand
     */
    Hand(Card card);
    
    /**
     * @brief Add a card to the hand
     * @param card Card to add
     * @note Automatically adjusts ace values to prevent busting
     */
    void add(Card card);
    
    /**
     * @brief Split the hand into two separate hands
     * @param deck Deck to draw new cards from
     * @param hands Stack to push new hands onto
     * @pre Hand must be a pair (is_pair() == true)
     */
    void split(Deck &deck, std::stack<Hand> &hands);
    
    /**
     * @brief Get current hand score
     * @return Score value (1-21 normally, can exceed 21 if busted)
     */
    int get_score() { return score; }
    
    /**
     * @brief Check if hand is soft (has ace counted as 11)
     * @return true if hand contains at least one high ace
     */
    bool is_soft() { return high_aces > 0; }
    
    /**
     * @brief Check if hand is a pair that can be split
     * @return true if hand has exactly 2 cards of same rank
     */
    bool is_pair() { return cards.size() == 2 && cards[0] == cards[1]; }
    
    /**
     * @brief Check if hand is busted (over 21)
     * @return true if score exceeds 21
     */
    bool is_busted() { return score > 21; }
    
    /**
     * @brief Check if hand is a natural blackjack
     * @return true if hand is exactly 21 with 2 cards
     */
    bool is_blackjack() { return score == 21 && cards.size() == 2; }
    
    /**
     * @brief Check if dealer must stand (score >= 17)
     * @return true if dealer should stop hitting
     */
    bool is_house_stands() { return score >= 17; }
    
    /**
     * @brief Stream output operator for debugging
     */
    friend std::ostream& operator<<(std::ostream& os, const Hand& hand) {
        for (size_t i = 0; i < hand.cards.size(); i++) {
            os << hand.cards[i] << " ";
        }
        return os;
    }
};

// Implementation
Hand::Hand() : score(0), high_aces(0), can_split(false), can_double(true), can_hit(true) {
    cards.reserve(5);
}

Hand::Hand(Card card) 
    : score(card), high_aces(card == ACE ? 1 : 0), 
      can_split(false), can_double(true), can_hit(true) {
    cards.reserve(5);
    cards.push_back(card);
}

void Hand::add(Card card) {
    cards.push_back(card);
    score += card;
    if (card == ACE) {
        high_aces++;
    }
    // Convert high ace to low ace if busted
    if (score > 21 && high_aces > 0) {
        score -= 10;
        high_aces--;
    }
}

void Hand::split(Deck &deck, std::stack<Hand> &hands) {
    // Create two new hands from the pair
    Hand hand1 = Hand(cards[0]);
    hand1.add(deck.draw());
    Hand hand2 = Hand(cards[1]);
    hand2.add(deck.draw());
    
    // Special rule: split aces get only one card each
    if (cards[0] == ACE) {
        hand1.can_double = false;
        hand2.can_double = false;
        hand1.can_hit = false;
        hand2.can_hit = false;
    }
    hands.push(hand1);
    hands.push(hand2);
}

#endif // HAND_H
