#ifndef SPOOKY_CHESS_GAME_H
#define SPOOKY_CHESS_GAME_H

#include <string>
#include "Game.h"
#include "ChessPiece.h"
#include "Enumerations.h"

/*
Chess game where a non-allied ghost piece teleports around the board at random taking pieces it encounters
*/

class SpookyChessGame : public Game {

public:

    // Creates new game in standard start-of-game state
    SpookyChessGame();

    // Creates game with state indicated in specified file
    SpookyChessGame(std::string filename);

    // Perform a move from the start Position to the end Position
    // The method returns an integer with the status
    // >= 0 is SUCCESS, < 0 is failure
    virtual int make_move(Position start, Position end) override;

    // Reports whether the chess game is over
    virtual bool game_over() const override;

protected:

    // Create all needed factories for the kinds of pieces
    // used in chess (doesn't make the actual pieces)
    virtual void initialize_factories();

    virtual void save_file() const;

    virtual std::string return_game_type() const override { return "spooky";}

//private methods
private:

    unsigned int _random_calls;

    bool move_ghost();

    unsigned int _ghost_location;

};

#endif // CHESS_GAME_H
