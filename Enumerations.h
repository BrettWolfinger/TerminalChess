#ifndef ENUMERATIONS_H
#define ENUMERATIONS_H

// Player name enumeration.
enum Player {
    WHITE = 0,
    BLACK = 1,
    NO_ONE = 2
};


// A struct to represent a position on the Game board.
struct Position {
    unsigned int x, y;
    Position(unsigned int xx = 0 , unsigned int yy = 0) : x(xx) , y(yy) { }
};



#endif // ENUMERATIONS_H
