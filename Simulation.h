#ifndef SIMULATION_H
#define SIMULATION_H
#include "include.h"

Action policy(Deck &deck, Hand player, Hand house) {
    return STAND;
}

void set_split_rights(Hand &hand, int max_hands, int num_hands) {
    if (num_hands < max_hands && hand.is_pair()) hand.can_split = true;
    else hand.can_split = false;
}

class Simulation {
    public:
        int max_hands;
        bool hit_on_soft_17;
        float bk_payout;
        // etc, different rules
        Simulation() : max_hands(4), hit_on_soft_17(true), bk_payout(1.5) {}
        Payout run_on(Deck &deck) {
            
            std::stack<Hand> hands;
            float payout = 0;
            float num_hands = 1;
            
            Card house_card = deck.draw();
            Hand house = Hand(house_card);
            if (house_card >= TEN) house.add(deck.draw()); // peek at second card
            Hand player = Hand(deck.draw());
            player.add(deck.draw());
            hands.push(player);

            if (player.is_blackjack()) {
                if (!house.is_blackjack()) return bk_payout;
                else return 0;
            } else if (house.is_blackjack()) {
                return -1;
            } 

            while (!hands.empty()) {
               
                Hand hand = hands.top();
                hands.pop();
                set_split_rights(player, max_hands, num_hands); // maybe to do update all rights
                Action action = policy(deck, hand, house); 
                //std::cout << "action: " << action << std::endl;
                if (action == HIT) {
                    hand.add(deck.draw());
                    if (hand.is_busted()) {
                        payout -= 1;
                    } else {
                        hand.can_double = false;
                        hands.push(hand);
                    }
                } else if (action == STAND) {
                    //std::cout << "house score: " << house.get_score() << std::endl;
                    // print cards of player
                    //std::cout << "player cards: " << hand << std::endl;
                    while (!house.is_house_stands()) {
                        house.add(deck.draw());
                        // print score
                        //std::cout << "house score: " << house.get_score() << std::endl;
                        }
                    if (house.is_busted()) {
                        payout += 1;
                    } else if (house.get_score() < hand.get_score()) {
                        payout += 1;
                    } else if (house.get_score() > hand.get_score()) {
                        payout -= 1;
                    }
                } else if (action == DOUBLE) {
                    hand.add(deck.draw());
                        if (hand.is_busted()) {
                            payout -= 2;
                        } else {
                            while (!house.is_house_stands()) house.add(deck.draw());
                            if (house.is_busted()) {
                                payout += 2;
                            } else if (house.get_score() < hand.get_score()) {
                                payout += 2;
                            } else if (house.get_score() > hand.get_score()) {
                                payout -= 2;
                            }
                        }
                } else if (action == SPLIT) {
                    hand.split(deck, hands);
                    num_hands++;
                }
            }
            
            return payout;

        } // returns payout of 1 round
        Payout run_on(Deck &deck, Card c_1, Card c_2, Card house_card) {
            // same but now the two cards of the player are given and the first card of the house

            std::stack<Hand> hands;
            float payout = 0;
            float num_hands = 1;
            
            Hand house = Hand(house_card);
            if (house_card >= TEN) house.add(deck.draw()); // peek at second card
            Hand player = Hand(c_1);
            player.add(c_2);
            hands.push(player);

            if (player.is_blackjack()) {
                if (!house.is_blackjack()) return bk_payout;
                else return 0;
            } else if (house.is_blackjack()) {
                return -1;
            } 

            while (!hands.empty()) {
               
                Hand hand = hands.top();
                hands.pop();
                set_split_rights(player, max_hands, num_hands); // maybe to do update all rights
                Action action = policy(deck, hand, house); 
                //std::cout << "action: " << action << std::endl;
                if (action == HIT) {
                    hand.add(deck.draw());
                    if (hand.is_busted()) {
                        payout -= 1;
                    } else {
                        hand.can_double = false;
                        hands.push(hand);
                    }
                } else if (action == STAND) {
                    //std::cout << "player cards: " << hand << std::endl;
                    //std::cout << "house score: " << house.get_score() << std::endl;
                    // print cards of player
                    
                    while (!house.is_house_stands()) {
                        house.add(deck.draw());
                        // print score
                        //std::cout << "house score: " << house.get_score() << std::endl;
                        }
                    if (house.is_busted()) {
                        payout += 1;
                    } else if (house.get_score() < hand.get_score()) {
                        payout += 1;
                    } else if (house.get_score() > hand.get_score()) {
                        payout -= 1;
                    }
                } else if (action == DOUBLE) {
                    hand.add(deck.draw());
                        if (hand.is_busted()) {
                            payout -= 2;
                        } else {
                            while (!house.is_house_stands()) house.add(deck.draw());
                            if (house.is_busted()) {
                                payout += 2;
                            } else if (house.get_score() < hand.get_score()) {
                                payout += 2;
                            } else if (house.get_score() > hand.get_score()) {
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
};









#endif 