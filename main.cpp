/**
 * @file main.cpp
 * @brief Main entry point for the Blackjack simulation
 * 
 * Demonstrates basic usage of the simulation components.
 */

#include "core/Types.h"
#include "core/Tools.h"
#include "core/Deck.h"
#include "core/Hand.h"
#include "core/Agent.h"
#include "core/Simulation.h"
#include "core/Petri.h"
#include <iostream>
#include <chrono>

int main() {
    std::cout << "=== Blackjack Simulation ===" << std::endl;
    
    // Demonstrate basic card comparison
    Card card = TEN, card2 = TWO;
    std::cout << "Card comparison: TEN(" << card << ") vs TWO(" << card2 << "): ";
    if (card >= card2) {
        std::cout << "TEN >= TWO" << std::endl;
    } else {
        std::cout << "TEN < TWO" << std::endl;
    }
    
    // Demonstrate deck creation
    Deck deck;
    deck.set_cards(DEFAULT_DECK);
    deck.shuffle();
    std::cout << "\nDeck created with " << deck.get_card_count() << " cards" << std::endl;
    
    // Demonstrate hand creation
    Hand hand;
    hand.add(deck.draw());
    hand.add(deck.draw());
    std::cout << "Initial hand: " << hand << " (Score: " << hand.get_score() << ")" << std::endl;
    
    std::cout << "\nSimulation ready!" << std::endl;
    return 0;
}