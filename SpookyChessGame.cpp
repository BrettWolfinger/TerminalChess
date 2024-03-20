#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>

#include "Game.h"
#include "SpookyChessGame.h"
#include "Prompts.h"
#include "Piece.h"

#define SEED 322

// Set up the chess board with standard initial pieces
SpookyChessGame::SpookyChessGame(): Game(), _random_calls(0), _ghost_location(32) {
    initialize_factories();
    std::vector<int> pieces {
        ROOK_ENUM, KNIGHT_ENUM, BISHOP_ENUM, QUEEN_ENUM,
        KING_ENUM, BISHOP_ENUM, KNIGHT_ENUM, ROOK_ENUM
    };
    for (size_t i = 0; i < pieces.size(); ++i) {
        init_piece(PAWN_ENUM, WHITE, Position(i, 1));
        init_piece(pieces[i], WHITE, Position(i, 0));
        init_piece(pieces[i], BLACK, Position(i, 7));
        init_piece(PAWN_ENUM, BLACK, Position(i, 6));
    }
    init_piece(GHOST_ENUM, NO_ONE, Position(0, 4));
    srand(SEED);
}


// Set up the chess board with game state loaded from file
SpookyChessGame::SpookyChessGame(const std::string filename) : Game() {

    initialize_factories();

    std::ifstream input_file;
    input_file.open(filename);
    if (!input_file.is_open()) {
      Prompts::load_failure();
      exit(1);
    }
    std::string temp;
    input_file >> temp;
    if (temp != "spooky") {
      Prompts::load_failure();
      input_file.close();
      exit(1);
    }
    int turn = 0;
    input_file >> turn;
    _turn = turn + 1;
    unsigned int calls = 0;
    input_file >> calls;
    _random_calls = calls;
    srand(SEED);
    for (unsigned int i = 0; i < calls; i++) {
      //I don't know if this in an issue with c++ but in Data structures,
      // they told us that if you call a function in java but don't actually assign
      // it to anything, Java will be tricky and stop actually executing it
      turn = rand();
    }
    int player, piece_type;
    std::string coordinate;
    while (input_file >> player) {
      input_file >> coordinate;
      input_file >> piece_type;
      int x = (int) coordinate[0] - 97;
      int y = coordinate[1] - 49;
      if (piece_type == PieceEnum::GHOST_ENUM) {
        _ghost_location = index(Position(x, y));
      }
      init_piece(static_cast<PieceEnum>(piece_type), static_cast<Player>(player), Position(x, y));
    }
    input_file.close();
}

void SpookyChessGame::save_file() const {
  Prompts::save_game();
  std::string filename;
  std::getline(std::cin, filename);
  std::ofstream output_file;
  output_file.open(filename);
  if (output_file.is_open()) {
    output_file << return_game_type() << "\n";
    output_file << turn() - 1  << "\n";
    output_file << _random_calls << "\n";
    for (unsigned int x = 0; x < _height; x++) {
      for (unsigned int y = 0; y < _width; y++) {
        Piece * p = _pieces[index(Position(y, x))];
        if (p != nullptr) {
          output_file << p->owner() << " ";
          output_file << (char) (y + 97) << x + 1 << " ";
          output_file << p->piece_type() << "\n";
        }
      }
    }
    output_file.close();
  } else {
    Prompts::save_failure();
  }
}

// Perform a move from the start Position to the end Position
// The method returns an integer with the status
// >= 0 is SUCCESS, < 0 is failure
int SpookyChessGame::make_move(Position start, Position end) {
  int result = make_move_helper(start, end);
  if (result < 0) {
    return result;
  }
  Player opponent = static_cast<Player>(1 - player_turn());
  if (checked(opponent, _pieces) && !player_can_move(opponent)) {
    Prompts::checkmate(player_turn());
    Prompts::win(player_turn(), _turn);
    return status::MOVE_CHECKMATE;
  }
  if (!player_can_move(opponent)) {
    Prompts::stalemate();
    return status::MOVE_STALEMATE;
  }
  if (move_ghost()) {
    Prompts::ghost_capture();
  }
  //wow the ghost can f*** s*** up
  if (checked(opponent, _pieces) && !player_can_move(opponent)) {
    Prompts::checkmate(player_turn());
    Prompts::win(player_turn(), _turn);
    return status::MOVE_CHECKMATE;
  }
  if (checked(player_turn(), _pieces) && !player_can_move(player_turn())) {
    Prompts::checkmate(opponent);
    Prompts::win(opponent, _turn);
    return status::MOVE_CHECKMATE;
  }
  if (!player_can_move(opponent) || !player_can_move(player_turn())) {
    Prompts::stalemate();
    return status::MOVE_STALEMATE;
  }
  if (checked(opponent, _pieces) && checked(player_turn(), _pieces)) {
    Prompts::check(player_turn());
    Prompts::check(opponent);
    _turn++;
    return status::MOVE_CHECK;
  }
  if (checked(opponent, _pieces)) {
    Prompts::check(player_turn());
    _turn++;
    return status::MOVE_CHECK;
  }
  if (checked(player_turn(), _pieces)) {
    Prompts::checkmate(opponent);
    Prompts::win(opponent, _turn);
    return status::MOVE_CHECKMATE;
  }
  _turn++;
  return result;
}

//handles movement for the ghost
bool SpookyChessGame::move_ghost() {
  unsigned int spot = rand() % (_height * _width);
  _random_calls++;
  while (_pieces[spot] != nullptr && _pieces[spot]->piece_type() == PieceEnum::KING_ENUM) {
    spot = rand() % (_height * _width);
    _random_calls++;
  }
  if (spot == _ghost_location) {
    return false;
  }
  Piece* g = _pieces[_ghost_location];
  Piece* q = _pieces[spot];
  _pieces[_ghost_location] = nullptr;
  _pieces[spot] = g;
  _ghost_location = spot;
  if (q != nullptr) {
    delete q;
    return true;
  }
  return false;
}


// Report whether the chess game is over
bool SpookyChessGame::game_over() const {
  Player opponent = static_cast<Player>(1 - player_turn());
  return !player_can_move(opponent) || !player_can_move(player_turn());
}


// Prepare the game to create pieces to put on the board
void SpookyChessGame::initialize_factories() {

    // Add all factories needed to create Piece subclasses
    add_factory(new PieceFactory<Pawn>(PAWN_ENUM));
    add_factory(new PieceFactory<Rook>(ROOK_ENUM));
    add_factory(new PieceFactory<Knight>(KNIGHT_ENUM));
    add_factory(new PieceFactory<Bishop>(BISHOP_ENUM));
    add_factory(new PieceFactory<Queen>(QUEEN_ENUM));
    add_factory(new PieceFactory<King>(KING_ENUM));
    add_factory(new PieceFactory<Ghost>(GHOST_ENUM));
}




