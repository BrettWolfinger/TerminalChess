#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H

#include <cstdlib>
#include "Enumerations.h"
#include "Piece.h"
#include <vector>

class Pawn : public Piece {

protected:
    friend PieceFactory<Pawn>;
    Pawn(Player owner, int piece_type) : Piece(owner, piece_type) {}

public:
    // Returns an integer representing move shape validity
    // where a value >= 0 means valid, < 0 means invalid.
    // also populates a vector of Positions with the trajectory 
    // followed by the Piece from start to end
    virtual int valid_move_shape(Position start, Position end, std::vector<Position>& trajectory) const override;
};


class Rook : public Piece {
  
 protected:
  friend PieceFactory<Rook>;
 Rook(Player owner, int piece_type) : Piece(owner, piece_type) {}
  
 public:
  // Returns an integer representing move shape validity
    // where a value >= 0 means valid, < 0 means invalid.
    // also populates a vector of Positions with the trajectory
    // followed by the Piece from start to end
    virtual int valid_move_shape(Position start, Position end, std::vector<Position>& trajectory) const override; 
};



class Knight : public Piece {

protected:
    friend PieceFactory<Knight>;
    Knight(Player owner, int piece_type) : Piece(owner, piece_type) {}

public:
    // Returns an integer representing move shape validity
    // where a value >= 0 means valid, < 0 means invalid.
    // also populates a vector of Positions with the trajectory
    // followed by the Piece from start to end
    virtual int valid_move_shape(Position start, Position end, std::vector<Position>& trajectory) const override;
};


class Bishop : public Piece {

protected:
    friend PieceFactory<Bishop>;
    Bishop(Player owner, int piece_type) : Piece(owner, piece_type) {}

public:
    // Returns an integer representing move shape validity
    // where a value >= 0 means valid, < 0 means invalid.
    // also populates a vector of Positions with the trajectory
    // followed by the Piece from start to end
    virtual int valid_move_shape(Position start, Position end, std::vector<Position>& trajectory) const override;
};


class Queen : public Piece {

protected:
    friend PieceFactory<Queen>;
    Queen(Player owner, int piece_type) : Piece(owner, piece_type) {}

public:
    // Returns an integer representing move shape validity
    // where a value >= 0 means valid, < 0 means invalid.
    // also populates a vector of Positions with the trajectory
    // followed by the Piece from start to end
    virtual int valid_move_shape(Position start, Position end, std::vector<Position>& trajectory) const override;
};


class King : public Piece {

protected:
    friend PieceFactory<King>;
    King(Player owner, int piece_type) : Piece(owner, piece_type) {}

public:
    // Returns an integer representing move shape validity
    // where a value >= 0 means valid, < 0 means invalid.
    // also populates a vector of Positions with the trajectory
    // followed by the Piece from start to end
    virtual int valid_move_shape(Position start, Position end, std::vector<Position>& trajectory) const override;
};


class Ghost : public Piece {

protected:
    friend PieceFactory<Ghost>;
    Ghost(Player owner, int piece_type) : Piece(owner, piece_type) {}

public:
    // Returns an integer representing move shape validity
    // where a value >= 0 means valid, < 0 means invalid.
    // also populates a vector of Positions with the trajectory
    // followed by the Piece from start to end
    virtual int valid_move_shape(Position start, Position end, std::vector<Position>& trajectory) const override;
};


#endif // CHESS_PIECE_H
