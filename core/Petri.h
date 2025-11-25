/**
 * @file Petri.h
 * @brief Genetic algorithm evolution environment for Blackjack agents
 * 
 * Implements a "petri dish" environment where agents compete and evolve
 * through natural selection and genetic operations.
 */

#ifndef PETRI_H
#define PETRI_H

#include "Agent.h"
#include "Simulation.h"
#include "Tools.h"

/**
 * @brief Evolution environment for training Blackjack-playing agents
 * 
 * Runs a genetic algorithm that:
 * 1. Evaluates agent fitness through game simulations
 * 2. Removes poorly performing agents
 * 3. Creates new agents through reproduction of survivors
 * 4. Applies random mutations for exploration
 */
class Petri {
private:
    std::vector<Agent> agents;     // Population of agents
    std::vector<float> payouts;    // Fitness scores for each agent
    
    Simulation simulation;          // Game simulation engine
    Laboratory laboratory;          // Agent factory and mutation controller
    Deck deck;                      // Deck for simulations
    
    int n_agents;                   // Population size
    int n_decks;                    // Number of decks (unused, for future)
    int n_simulations;              // Number of shuffle cycles per evaluation
    int n_sim_rounds;               // Games per shuffle cycle
    int n_kill;                     // Number of agents to eliminate each generation
    
    float deck_penetration;         // Deck penetration for simulations
    float elite_rate;               // Fraction of agents to preserve
    
    std::uniform_int_distribution<int> random_parent;  // For selecting parents
    std::mt19937 rng;               // Local RNG
    
    /**
     * @brief Replace worst agents with offspring of best agents
     * 
     * Sorts agents by fitness, removes the worst n_kill agents,
     * and replaces them with offspring from the surviving elite.
     */
    void kill_and_sex() {
        std::vector<int> indices = sort_permutation(payouts, compare);
        
        // Replace worst agents with offspring of random elite parents
        for (int i = 0; i < n_kill; i++) {
            int parent_1 = random_parent(rng);
            int parent_2 = random_parent(rng);
            while (parent_1 == parent_2) {
                parent_2 = random_parent(rng);
            }
            agents[indices[i]] = laboratory.reproduce_agents(agents[parent_1], agents[parent_2]);
        }
    }
    
    /**
     * @brief Apply mutations to all agents
     */
    void mutate() {
        for (int i = 0; i < n_agents; i++) {
            laboratory.mutate_agent(agents[i]);
        }
    }
    
    /**
     * @brief Evaluate fitness of all agents through simulations
     */
    void get_payouts() {
        std::fill(payouts.begin(), payouts.end(), 0);
        for (int j = 0; j < n_simulations; j++) {
            deck.shuffle();
            for (int i = 0; i < n_agents; i++) {
                for (int k = 0; k < n_sim_rounds; k++) {
                    payouts[i] += simulation.run(agents[i], deck);
                }
            }
        }
    }
    
public:
    /**
     * @brief Construct a Petri dish environment
     * @param lab Laboratory for agent creation and mutation
     * @param deck Deck configuration for simulations
     * @param n_agents Population size
     * @param n_simulations Number of shuffle cycles per evaluation
     * @param n_sim_rounds Games per shuffle cycle
     * @param elite_rate Fraction of top performers to preserve (0.0-1.0)
     */
    Petri(Laboratory lab, Deck deck, int n_agents, int n_simulations, 
          int n_sim_rounds, float elite_rate)
        : laboratory(lab), deck(deck), n_agents(n_agents), 
          n_simulations(n_simulations), n_sim_rounds(n_sim_rounds), 
          elite_rate(elite_rate) {
        
        // Initialize population with random agents
        for (int i = 0; i < n_agents; i++) {
            Agent agent;
            laboratory.set_random_weights(agent);
            agents.push_back(agent);
        }
        payouts.resize(n_agents);
        
        // Calculate elimination count and parent selection range
        n_kill = static_cast<int>(n_agents * (1 - elite_rate));
        random_parent = std::uniform_int_distribution<int>(n_kill, n_agents - 1);
    }

    /**
     * @brief Run one generation of evolution
     * 
     * Evaluates all agents, performs selection, reproduction, and mutation.
     */
    void run_one() {
        get_payouts();
        kill_and_sex();
        mutate();
    }
};

#endif // PETRI_H
