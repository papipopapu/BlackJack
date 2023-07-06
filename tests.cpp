#include "Hand.h"
#include "Simulation.h"

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
    deck.set_cards(DECK_1);
    deck.shuffle();
    std::stack<Hand> hands = std::stack<Hand>();
    hand.split(deck, hands);
    // print hands
    std::cout << "hand1: " << hands.top() << std::endl;
    hands.pop();
    std::cout << "hand2: " << hands.top() << std::endl;
    hands.pop();
}
void test_simulation() {
    Simulation sim;
    const int N = 1000;
    // run N simulations and get average payout
    Payout payout = 0;
    Deck deck = Deck();
    int DECK[12] = { 49, 4, 4, 4, 3, 4, 4, 4, 4, 2, 16, 4,};
    deck.set_cards(DECK);
    Card house_card = NINE, c_1 = NINE, c_2 = NINE;



    deck.shuffle();
    //std::cout << "//////////////////////" << std::endl; 
    // copy deck
    Deck deck_copy = deck;
    Payout p = sim.run_on(deck_copy, c_1, c_2, house_card);
    payout += p;
    //std::cout << "payout: " << p << std::endl;
        
    std::cout << "average payout: " << payout << std::endl;
}




int main() {
    test_hand();
    test_simulation();
    return 0;
}

