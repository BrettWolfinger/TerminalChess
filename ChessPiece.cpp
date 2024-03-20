#include <cstdlib>
#include "Enumerations.h"
#include "Piece.h"
#include "ChessPiece.h"
#include <vector>


    // Returns an integer representing move shape validity
    // where a value >= 0 means valid, < 0 means invalid.
    // also populates a vector of Positions with the trajectory
    // followed by the Piece from start to end
int Pawn::valid_move_shape(Position start, Position end, std::vector<Position>& trajectory) const {
        //pawn may only move forward (except when capturing which is checked elsewhere)
        if (start.x != end.x) {
          return -1;
        }
        if (_owner == Player::WHITE) {
          //may move forward 2 squares as its first move
          if (start.y == 1) {
            if ((int) end.y - (int) start.y == 2) {
              trajectory.push_back(Position(start.x, start.y + 1));
              return 1;
            }
          }
          //otherwise it can only move one
          if ((int) end.y - (int) start.y == 1) {
            return 1;
          }
        } else {
          //same logic from black start position
          if (start.y == 6) {
            if ((int) end.y - (int) start.y == -2) {
              trajectory.push_back(Position(start.x, start.y - 1));
              return 1;
            }
          }
          if ((int) end.y - (int) start.y == -1) {
            return 1;
          }
        }
        return -1;
}

/*
 Defines valid move shapes for each of the different chess pieces following the standard rules of chess
*/
int Rook::valid_move_shape(Position start, Position end, std::vector<Position>& trajectory) const {
        //Rook moves vertically
        if (start.x == end.x && end.y != start.y) {
          //up the board
          if (end.y > start.y) {
            for (unsigned int i = start.y + 1; i < end.y; i++) {
              trajectory.push_back(Position(start.x, i));
            }
            //down the board
          } else {
            for (unsigned int i = end.y + 1; i < start.y; i++) {
              trajectory.push_back(Position(start.x, i));
            }
          }
          return 1;
          //Rook moves Horizontally
        } else if (start.y == end.y && start.x != end.x) {
          //right on board
          if (end.x > start.x) {
            for (unsigned int i = start.x + 1; i < end.x; i++) {
              trajectory.push_back(Position(i, start.y));
            }
            //left on board
          } else {
            for (unsigned int i = end.x + 1; i < start.x; i++) {
              trajectory.push_back(Position(i, start.y));
            }
          }
          return 1;
        }
        return -1;
}



int Knight::valid_move_shape(Position start, Position end, std::vector<Position>& trajectory) const {
        trajectory.clear();
        //move must follow the L shape of a knight
        if ((abs((int) start.x - (int) end.x) == 1 && abs((int) start.y - (int) end.y) == 2) ||
            (abs((int) start.x - (int) end.x) == 2 && abs((int) start.y - (int) end.y) == 1)) {
          return 1;
        }
        return -1;
}


int Bishop::valid_move_shape(Position start, Position end, std::vector<Position>& trajectory) const {
        //Must move an equal number of spaces horizontally and vertically (to make the diagonal)
        if (abs((int) start.x - (int) end.x) == abs((int) start.y - (int) end.y)  && start.x != end.x) {
          //horizontally right
          if (((int) end.x - (int) start.x) > 0) {
            //Vertically up
            if (((int) end.y - (int) start.y) > 0) {
              for (unsigned int i = 1; i < (end.x -start.x);  i++) {
                trajectory.push_back(Position(start.x + i, start.y + i));
              }
              //vertically down
            } else {
              for (unsigned int i = 1; i < (end.x -start.x);  i++) {
                trajectory.push_back(Position(start.x + i, start.y - i));
              }
            }
            //horizontally left
          } else {
            //vertically up
            if (((int) end.y - (int) start.y) > 0) {
              for (unsigned int i = 1; i < (start.x - end.x);  i++) {
                trajectory.push_back(Position(start.x - i, start.y + i));
              }
              //vertically down
            } else {
              for (unsigned int i = 1; i < (start.x - end.x);  i++) {
                trajectory.push_back(Position(start.x - i, start.y - i));
              }
            }
          }
          return 1;
        }
        return -1;
}


int Queen::valid_move_shape(Position start, Position end, std::vector<Position>& trajectory) const {
        //Diagonal move
        if (abs((int) start.x - (int) end.x) == abs((int) start.y - (int) end.y) && start.x != end.x) {
          //horizontally right
          if (((int) end.x - (int) start.x) > 0) {
            //vertically up
            if (((int) end.y - (int) start.y) > 0) {
              for (unsigned int i = 1; i < (end.x -start.x);  i++) {
                trajectory.push_back(Position(start.x + i, start.y + i));
              }
              //vertically down
            } else {
              for (unsigned int i = 1; i < (end.x -start.x);  i++) {
                trajectory.push_back(Position(start.x + i, start.y - i));
              }
            }
            //horizontally left
          } else {
            //vertically up
            if (((int) end.y - (int) start.y) > 0) {
              for (unsigned int i = 1; i < (start.x - end.x);  i++) {
                trajectory.push_back(Position(start.x - i, start.y + i));
              }
              //vertically down
            } else {
              for (unsigned int i = 1; i < (start.x - end.x);  i++) {
                trajectory.push_back(Position(start.x - i, start.y - i));
              }
            }
          }
          return 1;
          //horizontal move
        } else if (start.x == end.x && start.y != end.y) {
          //up on board
          if (end.y > start.y) {
            for (unsigned int i = start.y + 1; i < end.y; i++) {
              trajectory.push_back(Position(start.x, i));
	    }
          //down on board
          } else {
            for (unsigned int i = end.y + 1; i < start.y; i++) {
              trajectory.push_back(Position(start.x, i));
	    }
          }
          return 1;
          //vertical move
        } else if (start.y == end.y && start.x != end.x) {
          //right on board
          if (end.x > start.x) {
            for (unsigned int i = start.x + 1; i < end.x; i++) {
              trajectory.push_back(Position(i, start.y));
            }
          //left on board
          } else {
            for (unsigned int i = end.x + 1; i < start.x; i++) {
              trajectory.push_back(Position(i, start.y));
            }
          }
          return 1;
        }
        return -1;
}


int King::valid_move_shape(Position start, Position end, std::vector<Position>& trajectory) const {
      trajectory.clear();
      //can only move one square but in any direction
      if ((abs((int) start.x - (int) end.x) == 1 && abs((int) start.y - (int) end.y) <= 1) || (abs(start.y - end.y) == 1 && abs((int) start.x - (int) end.x) <= 1)) {
          return 1;
        }
        return -1;
}


int Ghost::valid_move_shape(Position start, Position end, std::vector<Position>& trajectory) const {
      //ghost can move however it likes
      start = start;
      end = end;
      trajectory.clear();
      return -1;
}



