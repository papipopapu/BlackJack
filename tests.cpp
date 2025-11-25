/**
 * @file tests.cpp
 * @brief Unit tests for the Blackjack simulation components
 * 
 * Tests for Hand, Deck, Agent, and Simulation classes.
 */

#include "core/Hand.h"
#include "core/Simulation.h"
#include "core/Agent.h"
#include <cassert>
#include <iostream>

/**
 * @brief Test Hand class methods
 */
void test_hand() {
    std::cout << "Testing Hand class..." << std::endl;
    
    // Test constructor
    Hand hand = Hand();
    assert(hand.get_score() == 0);
    assert(hand.is_busted() == false);
    assert(hand.is_blackjack() == false);
    assert(hand.is_house_stands() == false);
    assert(hand.can_double == true);
    assert(hand.can_hit == true);
    assert(hand.can_split == false);
    
    // Test adding an ace
    hand.add(Card(ACE));
    assert(hand.get_score() == 11);
    assert(hand.is_busted() == false);
    assert(hand.is_blackjack() == false);
    assert(hand.is_house_stands() == false);
    assert(hand.can_double == true);
    assert(hand.can_hit == true);
    assert(hand.can_split == false);
    
    // Test adding second ace (should become soft 12)
    hand.add(Card(ACE));
    assert(hand.get_score() == 12);
    assert(hand.is_busted() == false);
    assert(hand.is_blackjack() == false);
    assert(hand.is_house_stands() == false);
    assert(hand.is_pair() == true);
    
    // Test split
    Deck deck = Deck();
    deck.set_cards(DEFAULT_DECK);
    deck.shuffle();
    
    std::stack<Hand> hands = std::stack<Hand>();
    hand.split(deck, hands);
    
    // Print hands
    std::cout << "  Split hand1: " << hands.top() << std::endl;
    hands.pop();
    std::cout << "  Split hand2: " << hands.top() << std::endl;
    hands.pop();
    
    std::cout << "  Hand tests PASSED!" << std::endl;
}

/**
 * @brief Test Simulation class methods
 */
void test_simulation() {
    std::cout << "Testing Simulation class..." << std::endl;
    
    Simulation sim;
    Payout payout = 0;
    Deck deck = Deck();

    int DECK[10] = {5, 4, 4, 4, 4, 4, 4, 4, 16, 4};
    deck.set_cards(DECK);
    Card house_card = NINE, c_1 = TWO, c_2 = TWO;

    // Copy deck and run simulation
    Deck deck_copy = deck;
    std::cout << "  Deck cards: ";
    deck_copy.print_cards();
    
    Payout p = sim.run_on(deck_copy, c_1, c_2, house_card);
    payout += p;
        
    std::cout << "  Payout result: " << payout << std::endl;
    std::cout << "  Simulation tests PASSED!" << std::endl;
}

/**
 * @brief Test Agent and Laboratory class methods
 */
void test_agent() {
    std::cout << "Testing Agent class..." << std::endl;
    
    Agent agent1, agent2;
    Laboratory lab(0.1, 2, 0.1, 0.2);
    Simulation sim;
    Deck deck;
    deck.set_cards(DEFAULT_DECK);
    deck.shuffle();
    
    Card house_card = NINE, c_1 = TWO, c_2 = TWO;
    Hand player, house;
    player.add(c_1);
    player.add(c_2);
    house.add(house_card);

    // Initialize agents with random weights
    lab.set_random_weights(agent1);
    lab.set_random_weights(agent2);
    
    // Print sample weights
    std::cout << "  Agent1 weights (first 10): ";
    for (int i = 0; i < 10; i++) {
        std::cout << agent1.get_weight(HIT, 0, 0, i) << " ";
    }
    std::cout << std::endl;

    std::cout << "  Agent2 weights (first 10): ";
    for (int i = 0; i < 10; i++) {
        std::cout << agent2.get_weight(HIT, 0, 0, i) << " ";
    }
    std::cout << std::endl;

    // Test action scoring
    float stand_score1 = agent1.get_action_score(STAND, deck, player, house);
    std::cout << "  Agent1 STAND score: " << stand_score1 << std::endl;
    float stand_score2 = agent2.get_action_score(STAND, deck, player, house);
    std::cout << "  Agent2 STAND score: " << stand_score2 << std::endl;

    // Test reproduction
    Agent agent3 = lab.reproduce_agents(agent1, agent2);
    std::cout << "  Child weights (first 10): ";
    for (int i = 0; i < 10; i++) {
        std::cout << agent3.get_weight(HIT, 0, 0, i) << " ";
    }
    std::cout << std::endl;

    float stand_score3 = agent3.get_action_score(STAND, deck, player, house);
    std::cout << "  Child STAND score: " << stand_score3 << std::endl;

    // Test mutation
    if (lab.mutate_agent(agent3)) {
        std::cout << "  Mutation successful" << std::endl;
    } else {
        std::cout << "  No mutation occurred (random chance)" << std::endl;
    }

    std::cout << "  Mutated weights (first 10): ";
    for (int i = 0; i < 10; i++) {
        std::cout << agent3.get_weight(HIT, 0, 0, i) << " ";
    }
    std::cout << std::endl;

    // Test simulation with agent
    Payout payout = sim.run(agent3, deck);
    std::cout << "  Simulation payout: " << payout << std::endl;
    
    std::cout << "  Agent tests PASSED!" << std::endl;
}

int main() {
    std::cout << "=== Blackjack Simulation Tests ===" << std::endl << std::endl;
    
    test_hand();
    std::cout << std::endl;
    
    test_simulation();
    std::cout << std::endl;
    
    test_agent();
    std::cout << std::endl;
    
    std::cout << "=== All tests completed! ===" << std::endl;
    return 0;
}
