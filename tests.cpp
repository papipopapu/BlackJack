#include "Hand.h"
#include "Simulation.h"
#include "Agent.h"
#include "Tools.h"
#include "Petri.h"
// test all methods of Hand
void test_hand() {
    // test constructor
    Hand hand = Hand();
    assert(hand.get_score() == 0);
    assert(hand.is_busted() == false);
    assert(hand.is_blackjack() == false);
    assert(hand.is_house_stands() == false);
    assert(hand.can_double == true);
    assert(hand.can_hit == true);
    assert(hand.can_split == false);
    // test add
    hand.add(Card(ACE));
    assert(hand.get_score() == 11);
    assert(hand.is_busted() == false);
    assert(hand.is_blackjack() == false);
    assert(hand.is_house_stands() == false);
    assert(hand.can_double == true);
    assert(hand.can_hit == true);
    assert(hand.can_split == false);
    hand.add(Card(ACE));
    assert(hand.get_score() == 12);
    assert(hand.is_busted() == false);
    assert(hand.is_blackjack() == false);
    assert(hand.is_house_stands() == false);
    assert(hand.is_pair() == true);
    // test split
    Deck deck = Deck();
    deck.set_cards(DEFAULT_DECK);

    deck.shuffle();
    std::stack<Hand> hands = std::stack<Hand>();
    hand.split(deck, hands);
    // print hands
    std::cout << "hand1: " << hands.top() << std::endl;
    hands.pop();
    std::cout << "hand2: " << hands.top() << std::endl;
    hands.pop();
}
void test_deck() {
    Deck deck = Deck(0.5);
    deck.set_cards(DEFAULT_DECK);
    deck.shuffle();
    for (int i=0; i<100; i++) {
        std::cout << deck.draw() << std::endl;
        std::cout << "card_count: " << deck.get_card_count() << std::endl;

    }
}
void test_simulation() {
    Simulation sim;
    const int N = 1000;
    // run N simulations and get average payout
    Payout payout = 0;
    Deck deck = Deck();

    int DECK[10] = {5, 4, 4, 4, 4, 4, 4, 4, 16, 4};
    deck.set_cards(DECK);
    Card house_card = NINE, c_1 = TWO, c_2 = TWO;


    // copy deck
    Deck deck_copy = deck;
    deck_copy.print_cards();
    Payout p = sim.run_on(deck_copy, c_1, c_2, house_card);
    payout += p;
    //std::cout << "payout: " << p << std::endl;
        
    std::cout << "average payout: " << payout << std::endl;
}
void test_agent() {
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

    lab.set_random_weights(agent1);
    lab.set_random_weights(agent2);
    // print 20 example weights
    std::cout << "weights1: ";
    for (int i = 0; i < 10; i++) {
        std::cout << agent1.get_weight(HIT, 0, 0, i) << " ";
    }
    std::cout << std::endl;

    std::cout << "weights2: ";
    for (int i = 0; i < 10; i++) {
        std::cout << agent2.get_weight(HIT, 0, 0, i) << " ";
    }
    std::cout << std::endl;


    float stand_score1 = agent1.get_action_score(STAND, deck, player, house);
    std::cout << "hit score1: " << stand_score1 << std::endl;
    float stand_score2 = agent2.get_action_score(STAND, deck, player, house);
    std::cout << "hit score2: " << stand_score2 << std::endl;

    Agent agent3 = lab.reproduce_agents(agent1, agent2);

    std::cout << "child weights: ";
    for (int i = 0; i < 10; i++) {
        std::cout << agent3.get_weight(HIT, 0, 0, i) << " ";
    }
    std::cout << std::endl;

    float stand_score3 = agent3.get_action_score(STAND, deck, player, house);
    std::cout << "child hit score: " << stand_score3 << std::endl;

    // test mutation
    if (lab.mutate_agent(agent3)) {
        std::cout << "mutation succesful" << std::endl;
    }

    std::cout << "mutated weights: ";
    for (int i = 0; i < 10; i++) {
        std::cout << agent3.get_weight(HIT, 0, 0, i) << " ";
    }
    std::cout << std::endl;

    // test simulation with agent
    Payout payout = sim.run(agent3, deck);
    std::cout << "payout: " << payout << std::endl;

}

void test_petri() {
    Laboratory lab(-5, 5, 0.1, 0.2);
    Simulation sim;
    Deck deck;
    deck.set_cards(DEFAULT_DECK);
    
    Petri petri(lab, deck, 1000, 100, 1, 0.2); // se sale si elite rate < 0.5 ????
    petri.print_member_variables();
    for (int i = 0; i < 100; i++) {
        petri.run_one();
        std::cout << "generation: " << i << std::endl;
    }
}



int main() {
    /* test_hand();
    test_simulation();
    test_agent();
    test_deck(); */
    test_petri();
    return 0;
}

