/**
 * @file Deck.h
 * @brief Deck management for the Blackjack simulation
 * 
 * Implements a deck of cards with support for multiple decks,
 * shuffling, card counting, and deck penetration.
 */

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

/**
 * @brief Default single deck configuration
 * 
 * Array indices represent cards: 2, 3, 4, 5, 6, 7, 8, 9, 10/Face, Ace
 * Standard deck has 4 of each rank, except 16 tens (10, J, Q, K).
 */
inline int DEFAULT_DECK[10] = {4, 4, 4, 4, 4, 4, 4, 4, 16, 4};

/**
 * @brief Represents a deck of playing cards for Blackjack
 * 
 * Manages card dealing, shuffling, and provides probability calculations
 * for card counting strategies. Supports configurable deck penetration.
 */
class Deck {
protected:
    std::vector<Card> cards;           // Physical cards for dealing
    int card_list[10];                 // Current count of each card type
    int card_list_0[10];               // Initial count of each card type (for reset)
    int card_index = 0;                // Current position in shuffled deck
    int card_count;                    // Total remaining cards
    int min_card_count;                // Minimum cards before reshuffle (based on penetration)
    float deck_penetration;            // How deep into deck before reshuffle (0.0-1.0)

public:
    /**
     * @brief Construct a deck with specified penetration
     * @param deck_penetration Fraction of deck to deal before reshuffle (default 1.0 = full)
     */
    Deck(float deck_penetration);
    
    /**
     * @brief Default constructor with full deck penetration
     */
    Deck() : Deck(1) {}
    
    /**
     * @brief Calculate probability of drawing a specific card type
     * @param card_idx Index of card type (0=2, 1=3, ..., 8=10/Face, 9=Ace)
     * @return Probability (0.0 to 1.0)
     */
    float get_card_prob(int card_idx) {
        return (float)card_list[card_idx] / card_count;
    }
    
    /**
     * @brief Get total remaining card count
     * @return Number of cards remaining in deck
     */
    int get_card_count() {
        return card_count;
    }
    
    /**
     * @brief Initialize deck with specific card counts
     * @param card_list Array of 10 integers representing count of each card type
     */
    void set_cards(int card_list[10]);
    
    /**
     * @brief Reset deck to initial state without shuffling
     */
    void reset();
    
    /**
     * @brief Shuffle the deck using the global RNG
     */
    void shuffle();
    
    /**
     * @brief Print current card counts to stdout
     */
    void print_card_list() {
        for (int i = 0; i < 10; i++) {
            std::cout << card_list[i] << " ";
        }
        std::cout << std::endl;
    }
    
    /**
     * @brief Print all cards in deck order to stdout
     */
    void print_cards() {
        for (size_t i = 0; i < cards.size(); i++) {
            std::cout << cards[i] << " ";
        }
        std::cout << std::endl;
    }
    
    /**
     * @brief Draw the top card from the deck
     * @return The drawn card
     * @note Automatically resets deck if penetration limit reached
     */
    Card draw() {
        if (card_count <= min_card_count) reset();
        Card card = cards[card_index++];
        card_list[card - 2]--;
        card_count--;
        return card;
    }
    
    /**
     * @brief Peek at the top card without drawing
     * @return The top card
     */
    Card peek() {
        return cards[card_index];
    }
    
    friend class Laboratory;
};

// Implementation
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

#endif // DECK_H
