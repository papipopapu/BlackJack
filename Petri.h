#ifndef PETRI_H
#define PETRI_H
#include "Agent.h"
#include "Simulation.h"

class Petri {

    std::vector<Agent> agents;
    Simulation simulation;
    Laboratory laboratory;
    int n_agents, n_decks;
    float deck_penetration, elite_rate;
};




#endif