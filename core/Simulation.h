/**
 * @file Simulation.h
 * @brief Blackjack game simulation engine
 * 
 * Implements the complete Blackjack game logic including:
 * - Card dealing and hand resolution
 * - Player vs dealer gameplay
 * - Support for AI agents and custom policies
 */

#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>
#include <stack>
#include <tuple>
#include <algorithm>
#include <iostream>
#include <random>
#include "Types.h"
#include "Deck.h"
#include "Hand.h"
#include "Agent.h"

/**
 * @brief Update hand's split eligibility based on game rules
 * @param hand Hand to update
 * @param max_hands Maximum allowed hands from splitting
 * @param num_hands Current number of hands
 */
inline void set_split_rights(Hand &hand, int max_hands, int num_hands) {
    if (num_hands < max_hands && hand.is_pair()) {
        hand.can_split = true;
    } else {
        hand.can_split = false;
    }
}

/**
 * @brief Blackjack game simulation engine
 * 
 * Runs complete Blackjack games with configurable rules.
 * Supports both AI agents and predefined card scenarios.
 */
class Simulation {
public:
    int max_hands;         // Maximum hands from splitting (default: 4)
    bool hit_on_soft_17;   // Dealer hits on soft 17 (default: true)
    float bk_payout;       // Blackjack payout ratio (default: 1.5)
    
    /**
     * @brief Default constructor with standard casino rules
     */
    Simulation() : max_hands(4), hit_on_soft_17(true), bk_payout(1.5) {}
    
    /**
     * @brief Run a game with an AI agent
     * 
     * Draws initial cards from deck and plays a complete hand
     * using the agent's decision-making.
     * 
     * @param agent AI agent to make decisions
     * @param deck Deck to draw cards from
     * @return Payout result (positive = win, negative = loss)
     */
    Payout run(Agent &agent, Deck deck) {
        // Draw initial cards
        Card c_1 = deck.draw(), c_2 = deck.draw(), house_card = deck.draw();

        std::stack<Hand> hands;
        float payout = 0;
        int num_hands = 1;
        
        // Set up dealer's initial hand and check for blackjack peek
        Hand house_initial = Hand(house_card);
        if (house_card == TEN) {
            Card second_card = deck.peek();
            if (second_card == ACE) house_initial.add(deck.draw());
        } else if (house_card == ACE) {
            Card second_card = deck.peek();
            if (second_card == TEN) house_initial.add(deck.draw());
        }

        // Set up player's initial hand
        Hand initial_hand = Hand(c_1);
        initial_hand.add(c_2);
        hands.push(initial_hand);

        // Check for natural blackjacks
        if (initial_hand.is_blackjack()) {
            if (!house_initial.is_blackjack()) return bk_payout;
            else return 0;  // Push
        } else if (house_initial.is_blackjack()) {
            return -1;
        } 

        // Complete dealer's hand
        Hand house_final = house_initial;
        while (!house_final.is_house_stands()) {
            house_final.add(deck.draw());
        }
        
        // Process all player hands
        while (!hands.empty()) {
            Hand hand = hands.top();
            hands.pop();
            set_split_rights(hand, max_hands, num_hands);
            Action action = agent.get_action(deck, hand, house_initial);
            
            if (action == HIT) {
                hand.add(deck.draw());
                if (hand.is_busted()) {
                    payout -= 1;
                } else {
                    hand.can_double = false;
                    hands.push(hand);
                }
            } else if (action == STAND) {
                if (house_final.is_busted()) {
                    payout += 1;
                } else if (house_final.get_score() < hand.get_score()) {
                    payout += 1;
                } else if (house_final.get_score() > hand.get_score()) {
                    payout -= 1;
                }
                // Equal scores = push (no payout change)
            } else if (action == DOUBLE) {
                hand.add(deck.draw());
                if (hand.is_busted()) {
                    payout -= 2;
                } else {
                    if (house_final.is_busted()) {
                        payout += 2;
                    } else if (house_final.get_score() < hand.get_score()) {
                        payout += 2;
                    } else if (house_final.get_score() > hand.get_score()) {
                        payout -= 2;
                    }
                }
            } else if (action == SPLIT) {
                hand.split(deck, hands);
                num_hands++;
            }
        }
        
        return payout;
    }
    
    /**
     * @brief Run a game with specific initial cards (for testing)
     * 
     * Allows specifying exact starting cards for reproducible testing.
     * Uses a simple stand-only policy for the player.
     * 
     * @param deck Deck state
     * @param c_1 Player's first card
     * @param c_2 Player's second card
     * @param house_card Dealer's up card
     * @return Payout result
     */
    Payout run_on(Deck deck, Card c_1, Card c_2, Card house_card) {
        std::stack<Hand> hands;
        float payout = 0;
        
        // Set up dealer's initial hand with blackjack peek
        Hand house_initial = Hand(house_card);
        if (house_card == TEN) {
            Card second_card = deck.peek();
            if (second_card == ACE) house_initial.add(deck.draw());
        } else if (house_card == ACE) {
            Card second_card = deck.peek();
            if (second_card == TEN) house_initial.add(deck.draw());
        }

        // Set up player's initial hand
        Hand initial_hand = Hand(c_1);
        initial_hand.add(c_2);
        hands.push(initial_hand);

        // Check for natural blackjacks
        if (initial_hand.is_blackjack()) {
            if (!house_initial.is_blackjack()) return bk_payout;
            else return 0;
        } else if (house_initial.is_blackjack()) {
            return -1;
        } 

        // Complete dealer's hand
        Hand house_final = house_initial;
        while (!house_final.is_house_stands()) {
            house_final.add(deck.draw());
        }
        
        // Process player hands (simple stand policy for testing)
        while (!hands.empty()) {
            Hand hand = hands.top();
            hands.pop();
            
            // Simple policy: always stand
            if (house_final.is_busted()) {
                payout += 1;
            } else if (house_final.get_score() < hand.get_score()) {
                payout += 1;
            } else if (house_final.get_score() > hand.get_score()) {
                payout -= 1;
            }
        }
        
        return payout;
    }
};

#endif // SIMULATION_H
