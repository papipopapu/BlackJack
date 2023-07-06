#ifndef TYPES_H
#define TYPES_H
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
    KING = 10
};

enum Action {
    HIT,
    STAND,
    DOUBLE,
    SPLIT
};


using Payout = float;
#endif 