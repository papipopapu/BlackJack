#ifndef AGENT_H
#define AGENT_H

#include "Types.h"
#include "Deck.h"
#include "Hand.h"
#include <random>
#include <algorithm>
#include <iterator>

const int PLAYER_SPACE = 26, HOUSE_SPACE = 10, DECK_SPACE = 10;
const int IS_SOFT_BOOST = 13; // 17 hards - 4 to shift to 0 the lowest score


 

class Laboratory;   

class Agent { 
    // player scores 4...20 hards + 12...20 softs = 26
    // house scores 2...11 = 10
    // deck cards: 2...11 = 10
    protected:
    // action weights
    float weights[ACTION_N][PLAYER_SPACE][HOUSE_SPACE][DECK_SPACE] = {};
    public:
    // last state output
    Agent(){};
    float get_weight(Action action, int player_idx, int house_idx, int deck_idx) {
        return weights[action][player_idx][house_idx][deck_idx];
    }
    float get_action_score(Action action, Deck &deck, Hand player, Hand house) {
        int player_idx = player.is_soft() ? player.get_score() + IS_SOFT_BOOST : player.get_score() - 4;
        int house_idx = house.get_score() - 2;
        float score = 0;
        for (int i = 0; i < 10; i++) {
            score += weights[action][player_idx][house_idx][i] * deck.get_card_prob(i);
        }
        return score;
    }
    Action get_action(Deck &deck, Hand player, Hand house) {
        int player_idx = player.is_soft() ? player.get_score() + IS_SOFT_BOOST : player.get_score() - 4;
        int house_idx = house.get_score() - 2;

        Action best_action = STAND;
        float best_score = get_action_score(STAND, deck, player, house);

        if (player.can_double) {
            if (get_action_score(DOUBLE, deck, player, house) > best_score) {
                best_action = DOUBLE;
                best_score = get_action_score(DOUBLE, deck, player, house);
            }
        }

        if (player.can_hit) {
            if (get_action_score(HIT, deck, player, house) > best_score) {
                best_action = HIT;
                best_score = get_action_score(HIT, deck, player, house);
            }
        }

        if (player.can_split) {
            if (get_action_score(SPLIT, deck, player, house) > best_score) {
                best_action = SPLIT;
                best_score = get_action_score(SPLIT, deck, player, house);
            }
        }

        return best_action;

    }

    friend class Laboratory;



};


class Laboratory {
    private:
    std::uniform_real_distribution<float> relative_mutation, unit_distribution;
    std::uniform_int_distribution<int> mutation_location;
    std::mt19937 rng;
    float min_relative_mutation, max_relative_mutation, mutation_rate, mutation_extent;
    int mutation_n;
    public:
    Laboratory(float min_relative_mutation, float max_relative_mutation, float mutation_rate, float mutation_extent)
     : min_relative_mutation(min_relative_mutation), max_relative_mutation(max_relative_mutation), mutation_rate(mutation_rate), mutation_extent(mutation_extent) {
        rng.seed(0);
        mutation_n = mutation_extent * (int)ACTION_N * PLAYER_SPACE * HOUSE_SPACE * DECK_SPACE;
        relative_mutation = std::uniform_real_distribution<float>(min_relative_mutation, max_relative_mutation);
        unit_distribution = std::uniform_real_distribution<float>(0, 1);
        mutation_location = std::uniform_int_distribution<int>(0, ACTION_N * PLAYER_SPACE * HOUSE_SPACE * DECK_SPACE - 1);
    }
    bool mutate_agent(Agent &agent) {
        // first generate a random number to see if we mutate, musat have mutation_rate chance
        if (unit_distribution(rng) > mutation_rate) { // remove for testing
            return false;
        }
        // now mutate
        // iterate mutation_n times
        // now mutate
        // iterate mutation_n times
        for (int i = 0; i < mutation_n; i++) {
            // get a random location
            int location = mutation_location(rng); // i for testing
            // get a random relative mutation
            float relative = relative_mutation(rng);

            
            agent.weights[location / (PLAYER_SPACE * HOUSE_SPACE * DECK_SPACE)][(location / (HOUSE_SPACE * DECK_SPACE)) % PLAYER_SPACE][(location / DECK_SPACE) % HOUSE_SPACE][location % DECK_SPACE] *= relative;
        }
        return true;
    }
    void set_random_weights(Agent &agent) {
        int N = ACTION_N * PLAYER_SPACE * HOUSE_SPACE * DECK_SPACE;
        for (int i = 0; i < N; i++) {
            agent.weights[i / (PLAYER_SPACE * HOUSE_SPACE * DECK_SPACE)][(i / (HOUSE_SPACE * DECK_SPACE)) % PLAYER_SPACE][(i / DECK_SPACE) % HOUSE_SPACE][i % DECK_SPACE] = 2 * unit_distribution(rng) - 1;
        }
    }  

    Agent reproduce_agents(Agent &agent_1, Agent &agent_2) {
        Agent child;
        float random_split = unit_distribution(rng);
        for (int action=0; action < ACTION_N; action++) {
            for (int player_s=0; player_s < PLAYER_SPACE; player_s++) {
                for (int house_s=0; house_s < HOUSE_SPACE; house_s++) {
                    for (int card=0; card < 10; card++) {
                        child.weights[action][player_s][house_s][card] = random_split * agent_1.weights[action][player_s][house_s][card]
                                                     + (1 - random_split) * agent_2.weights[action][player_s][house_s][card];
                    }
                }
            }
        }
        return child;
    }

    std::mt19937& get_rng() {
        return rng;
    }
};

#endif