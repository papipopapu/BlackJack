# BlackJack Simulation

A C++ Blackjack simulation with a genetic algorithm-based AI agent. This project implements a complete Blackjack game engine and uses evolutionary algorithms to train AI agents that learn optimal play strategies.

## Features

- **Complete Blackjack Rules**: Supports standard casino rules including:
  - Hit, Stand, Double Down, and Split
  - Soft/Hard hand handling
  - Natural blackjack detection
  - Configurable deck penetration

- **Genetic Algorithm AI**: Evolves agents through:
  - Weighted decision scoring based on game state
  - Agent reproduction with crossover
  - Random mutation for exploration
  - Selection pressure via fitness-based culling

- **Modular Design**: Clean separation of components:
  - `Types.h` - Core type definitions
  - `Tools.h` - Utility functions and RNG
  - `Deck.h` - Deck management
  - `Hand.h` - Hand scoring and actions
  - `Agent.h` - AI agent and Laboratory (evolution controller)
  - `Simulation.h` - Game simulation engine
  - `Petri.h` - Evolution environment

## Project Structure

```
BlackJack/
├── CMakeLists.txt      # Build configuration
├── README.md           # This file
├── main.cpp            # Main entry point
├── tests.cpp           # Unit tests
└── core/               # Header-only library
    ├── Types.h         # Card, Action, Payout types
    ├── Tools.h         # Utility functions, GLOBAL_RNG
    ├── Deck.h          # Deck class
    ├── Hand.h          # Hand class
    ├── Agent.h         # Agent and Laboratory classes
    ├── Simulation.h    # Simulation class
    └── Petri.h         # Petri class (evolution environment)
```

## Building

### Prerequisites

- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.14 or higher

### Build Instructions

```bash
# Create build directory
mkdir build && cd build

# Configure
cmake ..

# Build
cmake --build .

# Run tests
ctest --output-on-failure
```

### Manual Build (without CMake)

```bash
# Build main executable
g++ -std=c++17 -o blackjack main.cpp

# Build tests
g++ -std=c++17 -o blackjack_tests tests.cpp

# Run
./blackjack
./blackjack_tests
```

## Usage

### Basic Example

```cpp
#include "core/Types.h"
#include "core/Deck.h"
#include "core/Hand.h"
#include "core/Agent.h"
#include "core/Simulation.h"

int main() {
    // Create and shuffle a deck
    Deck deck;
    deck.set_cards(DEFAULT_DECK);
    deck.shuffle();
    
    // Create an agent with random weights
    Laboratory lab(0.1, 2.0, 0.1, 0.2);
    Agent agent;
    lab.set_random_weights(agent);
    
    // Run a simulation
    Simulation sim;
    Payout result = sim.run(agent, deck);
    
    return 0;
}
```

### Training Agents

```cpp
#include "core/Petri.h"

int main() {
    // Create laboratory and deck
    Laboratory lab(0.1, 2.0, 0.1, 0.2);
    Deck deck;
    deck.set_cards(DEFAULT_DECK);
    
    // Create petri dish with 100 agents
    Petri petri(lab, deck, 
                100,    // n_agents
                10,     // n_simulations
                100,    // n_sim_rounds
                0.5);   // elite_rate (keep top 50%)
    
    // Run evolution for 1000 generations
    for (int gen = 0; gen < 1000; gen++) {
        petri.run_one();
    }
    
    return 0;
}
```

## Configuration

### Laboratory Parameters

| Parameter | Description | Example Value |
|-----------|-------------|---------------|
| `min_relative_mutation` | Minimum mutation factor | 0.1 |
| `max_relative_mutation` | Maximum mutation factor | 2.0 |
| `mutation_rate` | Probability of mutation | 0.1 |
| `mutation_extent` | Fraction of weights to mutate | 0.2 |

### Simulation Parameters

| Parameter | Description | Default |
|-----------|-------------|---------|
| `max_hands` | Maximum hands from splitting | 4 |
| `hit_on_soft_17` | Dealer hits on soft 17 | true |
| `bk_payout` | Blackjack payout ratio | 1.5 |

## License

This project is provided as-is for educational purposes.

## Contributing

Contributions are welcome! Please feel free to submit issues and pull requests.
