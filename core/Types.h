/**
 * @file Types.h
 * @brief Core type definitions for the Blackjack simulation
 * 
 * Defines card values, player actions, and common type aliases
 * used throughout the simulation.
 */

#ifndef TYPES_H
#define TYPES_H

#include <random>

/**
 * @brief Enumeration representing card values in Blackjack
 * 
 * Card values are assigned their point values directly.
 * Face cards (Jack, Queen, King) all have value 10.
 * Ace is initially valued at 11 (soft) but can be reduced to 1 (hard).
 */
enum Card {
    ACE = 11,
    TWO = 2,
    THREE,
    FOUR,
    FIVE,
    SIX, 
    SEVEN,
    EIGHT,
    NINE,
    TEN = 10,
    JACK = 10,
    QUEEN = 10,
    KING = 10,
    NO_CARD = 0
};

/**
 * @brief Enumeration of possible player actions in Blackjack
 * 
 * HIT - Draw another card
 * STAND - Keep current hand, end turn
 * DOUBLE - Double the bet, receive exactly one more card
 * SPLIT - Split a pair into two separate hands
 * ACTION_N - Total number of actions (used for array sizing)
 */
enum Action {
    HIT = 0,
    STAND,
    DOUBLE,
    SPLIT,
    ACTION_N
};

/**
 * @brief Type alias for representing game payouts
 * 
 * Positive values indicate winnings, negative values indicate losses.
 * Standard win: +1.0, Standard loss: -1.0, Blackjack: +1.5
 */
using Payout = float;

#endif // TYPES_H
