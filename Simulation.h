#ifndef SIMULATION_H
#define SIMULATION_H
#include "include.h"


int i = 0;
Action policy(Deck &deck, Hand player, Hand house) {
    if (i==0) {
        i++;
        return SPLIT;
    }
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
        
        Payout run_on(Deck &deck, Card c_1, Card c_2, Card house_card) {
            // same but now the two cards of the player are given and the first card of the house

            std::stack<Hand> hands;
            float payout = 0;
            float num_hands = 1;
            
            // Initial house hand + peek at second card, if second card gives blackjack, add it so its accounted for in natural blackjack check
            Hand house_initial = Hand(house_card);
            if (house_card == TEN) {
                Card second_card = deck.peek();
                if (second_card == ACE) house_initial.add(deck.draw());
            } else if (house_card == ACE) {
                Card second_card = deck.peek();
                if (second_card == TEN) house_initial.add(deck.draw());
            }

            // Initial player hand
            Hand player = Hand(c_1);
            player.add(c_2);
            hands.push(player);

            // Check for natural blackjacks
            if (player.is_blackjack()) {
                if (!house_initial.is_blackjack()) return bk_payout;
                else return 0;
            } else if (house_initial.is_blackjack()) {
                return -1;
            } 

            // Get final house hand after drawing cards
            Hand house_final = house_initial;
            while (!house_final.is_house_stands()) { // get house 
                house_final.add(deck.draw());
                //std::cout << "house score: " << house_final.get_score() << std::endl;
            }
            
            // Keep doing shit until all hands are resolved (so much for splitting lmao)
            while (!hands.empty()) {
                //std::cout << "//////////////////////" << std::endl;
                Hand hand = hands.top();
                hands.pop();
                set_split_rights(player, max_hands, num_hands); // maybe to do update all rights
                Action action = policy(deck, hand, house_initial); // must only know the first* card of the house
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
             
                    if (house_final.is_busted()) {
                        payout += 1;
                    } else if (house_final.get_score() < hand.get_score()) {
                        payout += 1;
                    } else if (house_final.get_score() > hand.get_score()) {
                        payout -= 1;
                    }
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
};









#endif 