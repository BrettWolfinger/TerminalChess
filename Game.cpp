#include <iostream>
#include <cassert>
#include <cctype>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>
#include <ctype.h>

#include "Game.h"
#include "Prompts.h"
#include "Piece.h"
#include "Terminal.h"
#include "Enumerations.h"

Game::~Game() {

    // Delete the factories used to generate pieces
    for (size_t i = 0; i < _registered_factories.size(); i++) {
        delete _registered_factories[i];
    }

    // Delete any other dynamically-allocated resources here


    for (unsigned int i = 0; i < _width * _height; i++) {
      if (_pieces[i] != nullptr) {
        Piece* temp = _pieces[i];
        delete temp;
      }
    }

}

// Create a Piece on the board using the appropriate factory.
// Returns true if the piece was successfully placed on the board.
bool Game::init_piece(int piece_type, Player owner, Position pos) {
    Piece* piece = new_piece(piece_type, owner);
    if (!piece) return false;

    // Fail if the position is out of bounds
    if (!valid_position(pos)) {
        Prompts::out_of_bounds();
        return false;
    }

    // Fail if the position is occupied
    if (get_piece(pos)) {
        Prompts::blocked();
        return false;
    }
    _pieces[index(pos)] = piece;
    return true;
}

// Get the Piece at a specified Position.  Returns nullptr if no
// Piece at that Position or if Position is out of bounds.
Piece* Game::get_piece(Position pos) const {
    if (valid_position(pos))
        return _pieces[index(pos)];
    else {
        Prompts::out_of_bounds();
        return nullptr;
    }
}

//Take text input from the player in the form of
// startPosition endPosition
//using chess rank and file notation (ie a2 a3 will move the starting leftmost pawn up one square)
bool Game::process_move(std::string line ) {
  std::istringstream is(line);
  std::string pos_one, pos_two;
  //not enough inputs were given
  if (!(is >> pos_one) || !(is >> pos_two)) {
    Prompts::parse_error();
    return true;
  }
  //check input is the correct size
  if (pos_one.length() < 2 || pos_two.length() < 2) {
    Prompts::parse_error();
    return true;
  }
  std::string::size_type temp1, temp2 ;
  //check that input is the correct format
  if (!isdigit(pos_one[1]) || !isdigit(pos_two[1]) || !isalpha(pos_one[0]) || !isalpha(pos_two[0])) {
    Prompts::parse_error();
    return true;
  }
  //parse input
  int x1 = pos_one[0] - 97;
  int x2 = pos_two[0] - 97;
  int y1 = std::stoi(pos_one.substr(1), &temp1) - 1;
  int y2 = std::stoi(pos_two.substr(1), &temp2) - 1;
  //
  if (temp1 != pos_one.length() - 1 || temp2 != pos_two.length() - 1) {
    Prompts::parse_error();
    return true;
  }
  //given coordinates don't exist on the board
  if (x1 < 0 || (unsigned int) x1 >= _width || x2 < 0 || (unsigned int) x2 >= _width ||
      y1 < 0 || (unsigned int) y1 >= _height || y2 < 0 || (unsigned int) y2 >= _height) {
    Prompts::out_of_bounds();
    return true;
  }
  //attempt to move
  int result = make_move(Position(x1, y1), Position(x2, y2));
  if (result < status::MOVE_CHECKMATE) {
    return true;
  }
  //if game is flagged to print the board than do so
  if (_print_board) {
    print_board();
  }
  Prompts::game_over();
  return false;
}


//handle different types of input from the player
bool Game::process_input(std::string line) {
  std::istringstream is(line);
  std::string input = "";
  is >> input;
  //quit
  if (input == "q") {
    return false;
    //set show board flag
  } else if (input == "board") {
    if (_print_board) {
      _print_board = false;
    } else {
      _print_board = true;
    }
    return true;
    //bring prompts for saving game state
  } else if (input == "save") {
    save_file();
    return true;
    //forfeit game
  } else if (input == "forfeit") {
    Prompts::win(static_cast<Player>(turn() % 2), turn());
    Prompts::game_over();
    return false;
  }
  return process_move(line);
}

// Execute the main gameplay loop.
void Game::run() {
  std::string line;
  std::cin.clear();
  Prompts::player_prompt(player_turn(), turn());
  std::getline(std::cin, line);
  std::getline(std::cin, line);
  while (process_input(line)) {
    if (_print_board) {
      print_board();
    }
    Prompts::player_prompt(player_turn(), turn());
    std::getline(std::cin, line);
    std::transform(line.begin(), line.end(), line.begin(), ::tolower);
  }
}



// Search the factories to find a factory that can translate
//`piece_type' into a Piece, and use it to create the Piece.
// Returns nullptr if factory not found.
Piece* Game::new_piece(int piece_type, Player owner) {

    PieceGenMap::iterator it = _registered_factories.find(piece_type);

    if (it == _registered_factories.end()) { // not found
        std::cout << "Piece type " << piece_type << " has no generator\n";
        return nullptr;
    } else {
        return it->second->new_piece(owner);
    }
}



// Add a factory to the Board to enable producing
// a certain type of piece. Returns whether factory
// was successfully added or not.
bool Game::add_factory(AbstractPieceFactory* piece_gen) {
    // Temporary piece to get the ID
    Piece* p = piece_gen->new_piece(WHITE);
    int piece_type = p->piece_type();
    delete p;

    PieceGenMap::iterator it = _registered_factories.find(piece_type);
    if (it == _registered_factories.end()) { // not found, so add it
        _registered_factories[piece_type] = piece_gen;
        return true;
    } else {
        std::cout << "Piece type " << piece_type << " already has a generator\n";
        return false;
    }


}

//print green and red board to terminal with pieces setup on it
void Game::print_board() {
  for (unsigned int i = 0; i < _height; i++) {
    std::cout << " " << _height - i;
    if ((_height - i) < 10) {
      std::cout << " ";
    }
    for (unsigned int j = 0; j < _width; j++) {
      if ((i + j) % 2 == 0) {
        Terminal::color_bg(Terminal::Color::GREEN);
      } else {
        Terminal::color_bg(Terminal::Color::RED);
      }
      std::cout << " ";
      print_piece(_pieces[index(Position(j, _height - i - 1))]);
      std::cout << " ";
    }
    Terminal::set_default();
    std::cout << std::endl;
  }
  Terminal::set_default();
  std::cout << "   ";
  for (unsigned int x = 0; x <_width; x++) {
    std::cout << "  " << (char) (97 + x) << " ";
  }
  std::cout << std::endl;
  print_graveyard();
}
  

//prints graveyard of all captured pieces for the current game
void Game::print_graveyard() {
  int white_pawn = 8, black_pawn = 8;
  int white_rook = 2, black_rook = 2;
  int white_bishop = 2, black_bishop = 2;
  int white_queen = 1, black_queen = 1;
  int white_knight = 2, black_knight = 2;
  int type;

  //iterates through all the existing pieces still in play and 
  //subtracts values from the total pieces defined above so 
  //we know how many pieces have fallen and need to be put in graveyard
  for (unsigned int i = 0; i < _pieces.size(); i++) {
    if (_pieces[i] != nullptr && _pieces[i]->owner() == Player::WHITE) {
      type = _pieces[i]->piece_type();
      switch (type) {
      case PieceEnum::PAWN_ENUM:
        white_pawn--;
        break;
      case PieceEnum::ROOK_ENUM:
        white_rook--;
        break;
      case PieceEnum::KNIGHT_ENUM:
        white_knight--;
        break;
      case PieceEnum::BISHOP_ENUM:
        white_bishop--;
        break;
      case PieceEnum::QUEEN_ENUM:
        white_queen--;
	      break;
      }
    }
    else if (_pieces[i] != nullptr && _pieces[i]->owner() == Player::BLACK) {
      type = _pieces[i]->piece_type();
      switch (type) {
      case PieceEnum::PAWN_ENUM:
        black_pawn--;
        break;
      case PieceEnum::ROOK_ENUM:
        black_rook--;
        break;
      case PieceEnum::KNIGHT_ENUM:
        black_knight--;
        break;
      case PieceEnum::BISHOP_ENUM:
        black_bishop--;
        break;
      case PieceEnum::QUEEN_ENUM:
        black_queen--;
	break;
      }
    }
  }
  //print white pieces using unicode values of chess pieces
  std::cout << "White Graveyard: ";
  Terminal::color_all(0, Terminal::Color::WHITE, Terminal::Color::RED);
  for (int i = 0; i < white_pawn; i++) {
    std::cout << "\u2659 ";
  }
  for (int i = 0; i < white_rook; i++) {
    std::cout << "\u2656 ";
  }
  for (int i = 0; i < white_knight; i++) {
    std::cout << "\u2658 ";
  }
  for (int i = 0; i < white_bishop; i++) {
    std::cout << "\u2657 ";
  }
  for (int i = 0; i < white_queen; i++) {
    std::cout << "\u2655 ";
  }
  Terminal::set_default();
    //print Black pieces using unicode values of chess pieces
  std::cout << std::endl << "Black Graveyard: ";
  Terminal::color_all(0, Terminal::Color::BLACK, Terminal::Color::GREEN);
  for (int i = 0; i < black_pawn; i++) {
    std::cout << "\u265F ";
  }
  for (int i = 0; i < black_rook; i++) {
    std::cout << "\u265C ";
  }
  for (int i = 0; i < black_knight; i++) {
    std::cout << "\u265E ";
  }
  for (int i = 0; i < black_bishop; i++) {
    std::cout << "\u265D ";
  }
  for (int i = 0; i < black_queen; i++) {
    std::cout << "\u265B ";
  }
  Terminal::set_default();
  std::cout << std::endl;
}


//Print piece onto the chessboard using unicode values for chess pieces
//could be simplified tremendously with dictionaries
void Game::print_piece(Piece* p) {
  if (p == nullptr) {
    std::cout << "  ";
    return;
  }
  int type = p->piece_type();
  //process white pieces
  if (p->owner() == Player::WHITE) {
    Terminal::color_fg(0, Terminal::Color::WHITE);
    switch (type) {
      case PieceEnum::PAWN_ENUM:
        std::cout << "\u2659";
        break;
      case PieceEnum::ROOK_ENUM:
        std::cout << "\u2656";
        break;
      case PieceEnum::KNIGHT_ENUM:
        std::cout << "\u2658";
        break;
      case PieceEnum::BISHOP_ENUM:
        std::cout << "\u2657";
        break;
      case PieceEnum::QUEEN_ENUM:
        std::cout << "\u2655";
        break;
      case PieceEnum::KING_ENUM:
        std::cout << "\u2654";
        break;
    }
    std::cout << " ";
    //print black pieces
  } else if (p->owner() == Player::BLACK) {
    Terminal::color_fg(0, Terminal::Color::BLACK);
    switch (type) {
      case PieceEnum::PAWN_ENUM:
        std::cout << "\u265F";
        break;
      case PieceEnum::ROOK_ENUM:
        std::cout << "\u265C";
        break;
      case PieceEnum::KNIGHT_ENUM:
        std::cout << "\u265E";
        break;
      case PieceEnum::BISHOP_ENUM:
        std::cout << "\u265D";
        break;
      case PieceEnum::QUEEN_ENUM:
        std::cout << "\u265B";
        break;
      case PieceEnum::KING_ENUM:
        std::cout << "\u265A";
        break;
    }
    std::cout << " ";
  } else if (p->owner() == Player::NO_ONE) {
    if (type == PieceEnum::GHOST_ENUM) {
      Terminal::color_fg(0, Terminal::Color::BLACK);
      std::cout << "\u2603 ";
    }
  }
}

int Game::make_move_helper(Position start, Position end) {
    if (!valid_position(start) || !valid_position(end)) {
      Prompts::out_of_bounds();
      return status::MOVE_ERROR_OUT_OF_BOUNDS;
    }
    Piece* p = get_piece(start);
    Piece* q = get_piece(end);
    //determine if the move is legal based on rules of chess
    int status = can_make_move(start, end, player_turn());
    //saves a lot of if statements
    switch (status) {
      case status::MOVE_ERROR_NO_PIECE:
        Prompts::no_piece();
        return status;
      case status::MOVE_ERROR_ILLEGAL:
        Prompts::illegal_move();
        return status;
      case status::MOVE_ERROR_BLOCKED:
        Prompts::blocked();
        return status;
      case status::MOVE_ERROR_CANT_EXPOSE_CHECK:
        Prompts::cannot_expose_check();
        return status;
      case status::MOVE_ERROR_MUST_HANDLE_CHECK:
        Prompts::must_handle_check();
        return status;
      case status::MOVE_ERROR_CANT_CASTLE:
        Prompts::cannot_castle();
        return status;
    }
    if (status < 0) {
      return status;
    }
    //Handle castling movement
    if (p->piece_type() == PieceEnum::KING_ENUM && abs((int) start.x - (int) end.x) == 2) {
      _pieces[index(end)] = p;
      _pieces[index(start)] = nullptr;
      if (end.x > start.x) {
        Piece *r = _pieces[index(Position(start.x + 3, start.y))];
        _pieces[index(Position(start.x + 3, start.y))] = nullptr;
        _pieces[index(Position(start.x + 1, start.y))] = r;
      } else {
	Piece *r = _pieces[index(Position(start.x -4, start.y))];
        _pieces[index(Position(start.x - 4, start.y))] = nullptr;
        _pieces[index(Position(start.x - 1, start.y))] = r;
      }
      //simple movement for any other piece
    } else {
      _pieces[index(end)] = p;
      _pieces[index(start)] = nullptr;
    }
    //pawn turns into queen at end of board
    if (p->piece_type() == PieceEnum::PAWN_ENUM) {
      if ((player_turn() == Player::WHITE && end.y == _height -1) || (player_turn() == Player::BLACK && end.y == 0)) {
        delete p;
        _pieces[index(end)] = nullptr;
        init_piece(PieceEnum::QUEEN_ENUM, player_turn(), end);
        p = _pieces[index(end)];
      }
    }
    p->set_moved(true);
    if (q != nullptr) {
      Prompts::capture(player_turn());
      delete q;
      return status::MOVE_CAPTURE;;
    }
    return status::SUCCESS;
}



//many different checks to see if move is allowed
//check types indicated by status messages
int Game::can_make_move(Position start, Position end, Player play) const {
    Piece* p = get_piece(start);
    Piece* q = get_piece(end);
    if (p == nullptr || p->owner() != play) {
      return status::MOVE_ERROR_NO_PIECE;
    }
    std::vector<Piece*> copy = _pieces;
    std::vector<Position> trajectory;
    Player opponent = static_cast<Player>(1 - play);
    if (p->valid_move_shape(start, end, trajectory) < 0) {
      if (p->piece_type() == PieceEnum::KING_ENUM && abs((int) start.x - (int) end.x) == 2 && start.y == end.y) {
        if (can_castle(start, end, play, copy)) {
          return status::SUCCESS;
        } else {
          return status::MOVE_ERROR_CANT_CASTLE;
        }
      }
      if (p->piece_type() != PieceEnum::PAWN_ENUM || check_pawn_capture(start, end, p->owner(), copy) < 0) {
         return status::MOVE_ERROR_ILLEGAL;
      }
    }
    if (p->piece_type() == PieceEnum::PAWN_ENUM && check_pawn_capture(start, end, p->owner(), copy) < 0 && q != nullptr && q->owner() != play) {
      return status::MOVE_ERROR_BLOCKED;
    }
    if ((q != nullptr && q->owner() != opponent) || check_blocked(trajectory, copy)) {
      return status::MOVE_ERROR_BLOCKED;
    }
    if (!checked(play, copy)) {
      copy[index(end)] = p;
      copy[index(start)] = nullptr;
      if (checked(play, copy)) {
        copy[index(start)] = p;
        copy[index(end)] = q;
        return status::MOVE_ERROR_CANT_EXPOSE_CHECK;
      }
    } else {
      copy[index(end)] = p;
      copy[index(start)] = nullptr;
      if (checked(play, copy)) {
        copy[index(start)] = p;
        copy[index(end)] = q;
        return status::MOVE_ERROR_MUST_HANDLE_CHECK;
      }
    }
    copy[index(start)] = p;
    copy[index(end)] = q;
    return status::SUCCESS;
}



//return true if the trajectory is blocked and false if it's not
bool Game::check_blocked(std::vector<Position>& trajectory, std::vector<Piece*> &copy) const {
    for (std::vector<Position>::iterator it = trajectory.begin(); it != trajectory.end(); ++it) {
      Position pos = *it;
      if (copy[index(pos)] != nullptr) {
        return true;
      }
    }
    return false;
}

//checks if there are any valid moves left on the board for a player
bool Game::player_can_move(Player play) const{
  for (unsigned int i = 0; i < _height * _width; i++) {
    Position start(i % _width, i / _width);
    Piece *p = get_piece(start);
    if (p != nullptr && p->owner() == play) {
      for (unsigned int j = 0; j < _height * _width; j++) {
        Position end(j % _width, j / _width);
        if (can_make_move(start, end, play) >= 0) {
          return true;
        }
      }
    }
  }
  return false;
}

bool Game::checked(Player play, std::vector<Piece*> &copy) const{
  Position king_pos;
  for (unsigned int i = 0; i < _height * _width; i++) {
    if (copy[i] != nullptr && copy[i]->owner() == play && copy[i]->piece_type() == PieceEnum::KING_ENUM) {
      king_pos = Position(i % _width, i / _width);
    }
  }
  Player opponent = static_cast<Player>(1 - play);
  std::vector<Position> trajectory;
  for (unsigned int i = 0; i < _height * _width; i++) {
    if (copy[i] != nullptr && copy[i]->owner() == opponent) {
      trajectory.clear();
      if (copy[i]->piece_type() == PieceEnum::PAWN_ENUM) {
        if (check_pawn_capture(Position(i % _width, i / _width), king_pos, copy[i]->owner(), copy) >= 0) {
          return true;
        }
      } else {
	if (copy[i]->valid_move_shape(Position(i % _width, i / _width), king_pos, trajectory) >= 0  && !check_blocked(trajectory, copy)) {
          return true;
        }
      }
    }
  }
  return false;
}

//condition checks to make sure king can castle
bool Game::can_castle(Position start, Position end, Player play, std::vector<Piece*> &copy) const {
  Piece *k = copy[index(start)];
  if (k != nullptr && k->piece_type() == PieceEnum::KING_ENUM && !k->has_moved()) {
    if (start.y == end.y && abs((int) start.x - (int) end.x) == 2) {
      if (end.x > start.x) {
        if (copy[index(Position(start.x + 1, start.y))] == nullptr &&
            copy[index(Position(start.x + 2, start.y))] == nullptr) {
          Piece *r = copy[index(Position(start.x + 3, start.y))];
          if (r != nullptr && r->piece_type() == PieceEnum::ROOK_ENUM && !r->has_moved()) {
            if (check_castle_checking(start, end, play, copy)) {
              return true;
            }
          }
	}
      } else {
        if (copy[index(Position(start.x - 1, start.y))] == nullptr &&
            copy[index(Position(start.x - 2, start.y))] == nullptr &&
            copy[index(Position(start.x - 3, start.y))] == nullptr) {
          Piece *r = copy[index(Position(start.x - 4, start.y))];
          if (r != nullptr && r->piece_type() == PieceEnum::ROOK_ENUM && !r->has_moved()) {
            if (check_castle_checking(start, end, play, copy)) {
              return true;
            }
          }
	}
      }
    }
  }
  return false;
}


//Long function to check if king can castle or not
//has to check each position that the king might pass through
//while castling to make sure it isnt under attack
//probably should have been done recursively...
bool Game::check_castle_checking(Position start, Position end, Player play, std::vector<Piece*> &copy) const {
  if (!checked(play, copy)) {
    Piece *k = copy[index(start)];
    //going right
    if (end.x > start.x) {
      copy[index(start)] = nullptr;
      copy[index(Position(start.x + 1, start.y))] = k;
      if (!checked(play, copy)) {
        copy[index(Position(start.x + 1, start.y))] = nullptr;
        copy[index(Position(start.x + 2, start.y))] = k;
        if (!checked(play, copy)) {
          Piece *r = copy[index(Position(start.x + 3, start.y))];
          copy[index(Position(start.x + 1, start.y))] = r;
          copy[index(Position(start.x + 3, start.y))] = nullptr;
          if (!checked(play, copy)) {
            copy[index(start)] = k;
            copy[index(Position(start.x + 2, start.y))] = nullptr;
            copy[index(Position(start.x + 3, start.y))] = r;
            return true;
          }
          copy[index(start)] = k;
          copy[index(Position(start.x + 2, start.y))] = nullptr;
          copy[index(Position(start.x + 3, start.y))] = r;
        } else {
          copy[index(start)] = k;
          copy[index(Position(start.x + 2, start.y))] = nullptr;
        }
      } else {
        copy[index(start)] = k;
        copy[index(Position(start.x + 1, start.y))] = nullptr;
      }
      //going left
    } else {
      copy[index(start)] = nullptr;
      copy[index(Position(start.x - 1, start.y))] = k;
      if (!checked(play, copy)) {
        copy[index(Position(start.x - 1, start.y))] = nullptr;
        copy[index(Position(start.x - 2, start.y))] = k;
        if (!checked(play, copy)) {
          Piece *r = copy[index(Position(start.x - 4, start.y))];
          copy[index(Position(start.x - 1, start.y))] = r;
          copy[index(Position(start.x - 4, start.y))] = nullptr;
          if (!checked(play, copy)) {
            copy[index(start)] = k;
            copy[index(Position(start.x - 2, start.y))] = nullptr;
            copy[index(Position(start.x - 4, start.y))] = r;
            return true;
          }
          copy[index(start)] = k;
          copy[index(Position(start.x - 2, start.y))] = nullptr;
          copy[index(Position(start.x - 4, start.y))] = r;
        } else {
          copy[index(start)] = k;
          copy[index(Position(start.x - 2, start.y))] = nullptr;
        }
      } else {
	copy[index(start)] = k;
        copy[index(Position(start.x - 1, start.y))] = nullptr;
      }
    }
  }
  return false;
}



//return a value >= 0 if its a valid pawn capture move and < 0 if it's not
//need special pawn capture move because it captures slightly differently from how it moves
int Game::check_pawn_capture(Position start, Position end, Player play, std::vector<Piece*> &copy) const {
  Piece* p = copy[index(end)];
  if (play == Player::WHITE) {
    if (p != nullptr && p->owner() == BLACK) {
      if (((int) end.y - (int) start.y) == 1 && abs((int) end.x - (int) start.x) == 1) {
        return 1;
      }
    }
  } else if (play == Player::BLACK) {
    if (p != nullptr && p->owner() == WHITE) {
      if (((int) start.y - (int) end.y) == 1 && abs((int) end.x - (int) start.x) == 1) {
        return 1;
      }
    }
  }
  return -1;
}
