/**
 * @file Agent.h
 * @brief AI agent for Blackjack decision making using genetic algorithms
 * 
 * Implements an agent that learns optimal play through genetic algorithms.
 * The Laboratory class handles agent reproduction, mutation, and evolution.
 */

#ifndef AGENT_H
#define AGENT_H

#include "Types.h"
#include "Deck.h"
#include "Hand.h"
#include "Tools.h"
#include <random>
#include <algorithm>
#include <iterator>
#include <numeric>

// State space dimensions for the agent's weight matrix
const int PLAYER_SPACE = 26;   // Player scores: 4-20 hard + 12-20 soft = 26 states
const int HOUSE_SPACE = 10;     // Dealer showing: 2-11 = 10 states
const int DECK_SPACE = 10;      // Card types: 2-11 = 10 probabilities
const int IS_SOFT_BOOST = 13;   // Offset for soft hand indexing

// Forward declaration
class Laboratory;

/**
 * @brief AI agent that uses weighted scoring to decide Blackjack actions
 * 
 * The agent maintains a 4D weight matrix indexed by:
 * - Action type (HIT, STAND, DOUBLE, SPLIT)
 * - Player hand state (score + soft/hard)
 * - Dealer's visible card
 * - Card probability in deck
 * 
 * Actions are chosen by computing weighted sums and selecting the highest score.
 */
class Agent { 
protected:
    // Weight matrix for action scoring
    // Dimensions: [action][player_state][dealer_card][deck_card_prob]
    float weights[ACTION_N][PLAYER_SPACE][HOUSE_SPACE][DECK_SPACE] = {};

public:
    Agent() {}
    
    /**
     * @brief Get a specific weight value
     * @param action Action type
     * @param player_idx Player state index
     * @param house_idx Dealer card index
     * @param deck_idx Deck card type index
     * @return Weight value
     */
    float get_weight(Action action, int player_idx, int house_idx, int deck_idx) {
        return weights[action][player_idx][house_idx][deck_idx];
    }
    
    /**
     * @brief Calculate the score for a given action in current state
     * 
     * Computes a weighted sum using the current deck composition
     * to evaluate how favorable an action is.
     * 
     * @param action Action to evaluate
     * @param deck Current deck state
     * @param player Player's hand
     * @param house Dealer's visible hand
     * @return Score value (higher is better)
     */
    float get_action_score(Action action, Deck &deck, Hand player, Hand house) {
        // Calculate state indices
        int player_idx = player.is_soft() ? player.get_score() + IS_SOFT_BOOST : player.get_score() - 4;
        int house_idx = house.get_score() - 2;
        
        // Sum weighted card probabilities
        float score = 0;
        for (int i = 0; i < 10; i++) {
            score += weights[action][player_idx][house_idx][i] * deck.get_card_prob(i);
        }
        return score;
    }
    
    /**
     * @brief Determine the best action for the current game state
     * 
     * Evaluates all valid actions and returns the one with highest score.
     * Respects game rules (can_double, can_hit, can_split).
     * 
     * @param deck Current deck state
     * @param player Player's hand
     * @param house Dealer's visible hand
     * @return Best action to take
     */
    Action get_action(Deck &deck, Hand player, Hand house) {
        // Start with STAND as baseline
        Action best_action = STAND;
        float best_score = get_action_score(STAND, deck, player, house);

        // Check DOUBLE if allowed
        if (player.can_double) {
            float double_score = get_action_score(DOUBLE, deck, player, house);
            if (double_score > best_score) {
                best_action = DOUBLE;
                best_score = double_score;
            }
        }

        // Check HIT if allowed
        if (player.can_hit) {
            float hit_score = get_action_score(HIT, deck, player, house);
            if (hit_score > best_score) {
                best_action = HIT;
                best_score = hit_score;
            }
        }

        // Check SPLIT if allowed
        if (player.can_split) {
            float split_score = get_action_score(SPLIT, deck, player, house);
            if (split_score > best_score) {
                best_action = SPLIT;
                best_score = split_score;
            }
        }

        return best_action;
    }

    friend class Laboratory;
};

/**
 * @brief Factory and evolution controller for Agent instances
 * 
 * Handles genetic algorithm operations:
 * - Creating random agents
 * - Reproducing agents (crossover)
 * - Mutating agent weights
 */
class Laboratory {
private:
    std::uniform_real_distribution<float> relative_mutation;  // Mutation magnitude
    std::uniform_real_distribution<float> unit_distribution;  // [0, 1] generator
    std::uniform_int_distribution<int> mutation_location;     // Random weight selector
    
    float min_relative_mutation;  // Minimum mutation multiplier
    float max_relative_mutation;  // Maximum mutation multiplier
    float mutation_rate;          // Probability of mutation occurring
    float mutation_extent;        // Fraction of weights to mutate
    int mutation_n;               // Number of weights to mutate per agent

public:
    /**
     * @brief Construct a laboratory with evolution parameters
     * @param min_relative_mutation Minimum mutation factor (e.g., 0.1 = reduce to 10%)
     * @param max_relative_mutation Maximum mutation factor (e.g., 2.0 = double)
     * @param mutation_rate Probability of mutation per generation (0.0-1.0)
     * @param mutation_extent Fraction of weights to mutate when mutation occurs
     */
    Laboratory(float min_relative_mutation, float max_relative_mutation, 
               float mutation_rate, float mutation_extent)
        : min_relative_mutation(min_relative_mutation), 
          max_relative_mutation(max_relative_mutation), 
          mutation_rate(mutation_rate), 
          mutation_extent(mutation_extent) {
        
        // Calculate total number of weights to mutate
        mutation_n = static_cast<int>(mutation_extent * ACTION_N * PLAYER_SPACE * HOUSE_SPACE * DECK_SPACE);
        
        // Initialize distributions
        relative_mutation = std::uniform_real_distribution<float>(min_relative_mutation, max_relative_mutation);
        unit_distribution = std::uniform_real_distribution<float>(0, 1);
        mutation_location = std::uniform_int_distribution<int>(0, ACTION_N * PLAYER_SPACE * HOUSE_SPACE * DECK_SPACE - 1);
    }
    
    /**
     * @brief Attempt to mutate an agent's weights
     * @param agent Agent to mutate
     * @return true if mutation occurred, false otherwise
     */
    bool mutate_agent(Agent &agent) {
        // Check if mutation should occur
        if (unit_distribution(GLOBAL_RNG) > mutation_rate) {
            return false;
        }
        
        // Apply mutations to random weight locations
        for (int i = 0; i < mutation_n; i++) {
            int location = mutation_location(GLOBAL_RNG);
            float relative = relative_mutation(GLOBAL_RNG);
            
            // Convert flat index to 4D indices
            int action_idx = location / (PLAYER_SPACE * HOUSE_SPACE * DECK_SPACE);
            int player_idx = (location / (HOUSE_SPACE * DECK_SPACE)) % PLAYER_SPACE;
            int house_idx = (location / DECK_SPACE) % HOUSE_SPACE;
            int deck_idx = location % DECK_SPACE;
            
            agent.weights[action_idx][player_idx][house_idx][deck_idx] *= relative;
        }
        return true;
    }
    
    /**
     * @brief Initialize an agent with random weights
     * @param agent Agent to initialize
     */
    void set_random_weights(Agent &agent) {
        int N = ACTION_N * PLAYER_SPACE * HOUSE_SPACE * DECK_SPACE;
        for (int i = 0; i < N; i++) {
            // Convert flat index to 4D indices
            int action_idx = i / (PLAYER_SPACE * HOUSE_SPACE * DECK_SPACE);
            int player_idx = (i / (HOUSE_SPACE * DECK_SPACE)) % PLAYER_SPACE;
            int house_idx = (i / DECK_SPACE) % HOUSE_SPACE;
            int deck_idx = i % DECK_SPACE;
            
            // Random weight in range [-1, 1]
            agent.weights[action_idx][player_idx][house_idx][deck_idx] = 
                2 * unit_distribution(GLOBAL_RNG) - 1;
        }
    }  

    /**
     * @brief Create a child agent by combining two parent agents
     * 
     * Uses weighted averaging with a random split factor.
     * 
     * @param agent_1 First parent
     * @param agent_2 Second parent
     * @return Child agent with blended weights
     */
    Agent reproduce_agents(Agent &agent_1, Agent &agent_2) {
        Agent child;
        float random_split = unit_distribution(GLOBAL_RNG);
        
        for (int action = 0; action < ACTION_N; action++) {
            for (int player_s = 0; player_s < PLAYER_SPACE; player_s++) {
                for (int house_s = 0; house_s < HOUSE_SPACE; house_s++) {
                    for (int card = 0; card < 10; card++) {
                        // Blend weights from both parents
                        child.weights[action][player_s][house_s][card] = 
                            random_split * agent_1.weights[action][player_s][house_s][card] +
                            (1 - random_split) * agent_2.weights[action][player_s][house_s][card];
                    }
                }
            }
        }
        return child;
    }

    /**
     * @brief Get reference to the global RNG
     * @return Reference to GLOBAL_RNG
     */
    std::mt19937& get_rng() {
        return GLOBAL_RNG;
    }
};

#endif // AGENT_H
