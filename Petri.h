#ifndef PETRI_H
#define PETRI_H
#include "Agent.h"
#include "Simulation.h"
#include "Tools.h"



class Petri {
    private:
    std::vector<Agent> agents;
    std::vector<float> payouts;


    Simulation simulation;
    Laboratory laboratory;
    Deck deck;
    int n_agents, n_decks, n_simulations, n_sim_rounds, n_kill;
    float deck_penetration, elite_rate;
    std::uniform_int_distribution<int> random_parent;
    std::mt19937 rng;
    void kill_and_sex() {
        std::vector<int> indices = sort_permutation(payouts, compare);
        // print elements in indices with corresponding payouts
        // now change the n_kill worst agents by randomly reproducing the best agents
        int parent = n_agents - 1;
        for (int i = 0; i < n_kill; i++) {
            int parent_1 = (parent == -1) ? n_agents - 1 : parent--;
            int parent_2 = (parent == -1) ? n_agents - 1 : parent--;
            /* while (parent_1 == parent_2) { // maybe remove this idk
                parent_2 = random_parent(rng);
            } */
            agents[indices[i]] = laboratory.reproduce_agents(agents[parent_1], agents[parent_2]);
            /* std::cout << "Parent 1: " << std::endl;
            agents[parent_1].print_first_10_weights();

            std::cout << "Parent 2: " << std::endl;
            agents[parent_2].print_first_10_weights();

            std::cout << "Child: " << std::endl;
            agents[indices[i]].print_first_10_weights(); */
        }
    }
    void mutate() {
        for (int i = 0; i < n_agents; i++) {
            laboratory.mutate_agent(agents[i]);
        }
    }
    void get_payouts() {
        std::fill(payouts.begin(), payouts.end(), 0);
        float avg = 0;
        for (int j = 0; j < n_simulations; j++) {
            deck.shuffle();
            for (int i = 0; i < n_agents; i++) {
                for (int k = 0; k < n_sim_rounds; k++) {
                    payouts[i] += simulation.run(agents[i], deck);
                    avg += payouts[i];
                }
            }
        }
        std::cout << "Average payout: " << avg / n_agents  << std::endl;
    }
    public:
    Petri(Laboratory lab, Deck deck, int n_agents, int n_simulations, int n_sim_rounds, float elite_rate) :
        laboratory(lab), deck(deck), n_agents(n_agents), n_simulations(n_simulations), n_sim_rounds(n_sim_rounds), elite_rate(elite_rate) {
        for (int i = 0; i < n_agents; i++) {
            Agent agent;
            laboratory.set_random_weights(agent);
            agents.push_back(agent);
        }
        n_kill = n_agents * (1 - elite_rate);
        random_parent = std::uniform_int_distribution<int>(n_kill, n_agents - 1);
        payouts = std::vector<float>(n_agents);
    }

    void run_one() {
        get_payouts();
        kill_and_sex();
        mutate();
    }

    void print_member_variables() {
        std::cout << "n_agents: " << n_agents << std::endl;
        std::cout << "n_simulations: " << n_simulations << std::endl;
        std::cout << "n_sim_rounds: " << n_sim_rounds << std::endl;
        std::cout << "elite_rate: " << elite_rate << std::endl;
        std::cout << "n_kill: " << n_kill << std::endl;
    }
    
        
};


// bruh

#endif