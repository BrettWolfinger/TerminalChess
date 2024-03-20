#ifndef KOTH_CHESS_GAME_H
#define KOTH_CHESS_GAME_H

#include <string>
#include "Game.h"
#include "ChessPiece.h"

/*
Chess game with alternate win condition of moving king into the middle of the hill
*/

class KOTHChessGame : public Game {

public:

    // Creates new game in standard start-of-game state
    KOTHChessGame();

    // Creates game with state indicated in specified file
    KOTHChessGame(std::string filename);

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

    virtual std::string return_game_type() const override { return "king";}

//private methods
private:

    bool conquered_hill(Player play) const;

};

#endif // CHESS_GAME_H
