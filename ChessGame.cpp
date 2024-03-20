#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>

#include "Game.h"
#include "ChessGame.h"
#include "Prompts.h"
#include "Piece.h"


// Set up the chess board with standard initial pieces
ChessGame::ChessGame(): Game() {
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
}


// Set up the chess board with game state loaded from file
ChessGame::ChessGame(const std::string filename) : Game() {

    initialize_factories();

    std::ifstream input_file;
    input_file.open(filename);
    if (!input_file.is_open()) {
      Prompts::load_failure();
      exit(1);
    }
    std::string temp;
    input_file >> temp;
    //make sure save file is of the correct game type
    if (temp != "chess") {
      Prompts::load_failure();
      input_file.close();
      exit(1);
    }
    int turn = 0;
    input_file >> turn;
    _turn = turn + 1;
    int player, piece_type;
    std::string coordinate;
    //create all pieces according to file output
    while (input_file >> player) {
      input_file >> coordinate;
      input_file >> piece_type;
      int x = (int) coordinate[0] - 97;
      int y = coordinate[1] - 49;
      init_piece(static_cast<PieceEnum>(piece_type), static_cast<Player>(player), Position(x, y));
    }
    input_file.close();
}

//Saves game state out to file
//Format:
//gametype (chess)
//turnnumber (5)
//player location pieceType (0 a1 1) means there is a white rook on square a1
void ChessGame::save_file() const {
  Prompts::save_game();
  std::string filename;
  std::getline(std::cin, filename);
  std::ofstream output_file;
  output_file.open(filename);
  if (output_file.is_open()) {
    output_file << return_game_type() << "\n";
    output_file << turn() - 1  << "\n";
    //Note, override this function for spooky to make sure it's different
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
int ChessGame::make_move(Position start, Position end) {
  int result = make_move_helper(start, end);
  if (result < 0) {
    return result;
  }
  Player opponent = static_cast<Player>(1 - player_turn());
  if (checked(opponent, _pieces)) {
    if (!player_can_move(opponent)) {
      Prompts::checkmate(player_turn());
      Prompts::win(player_turn(), _turn);
      return status::MOVE_CHECKMATE;
    }
    Prompts::check(player_turn());
    _turn++;
    return status::MOVE_CHECK;
  }
  if (!player_can_move(opponent)) {
    Prompts::stalemate();
    return status::MOVE_STALEMATE;
  }
  _turn++;
  return result;
}

// Report whether the chess game is over
bool ChessGame::game_over() const {
  Player opponent = static_cast<Player>(1 - player_turn());
  return !player_can_move(opponent);
}


// Prepare the game to create pieces to put on the board
void ChessGame::initialize_factories() {

    // Add all factories needed to create Piece subclasses
    add_factory(new PieceFactory<Pawn>(PAWN_ENUM));
    add_factory(new PieceFactory<Rook>(ROOK_ENUM));
    add_factory(new PieceFactory<Knight>(KNIGHT_ENUM));
    add_factory(new PieceFactory<Bishop>(BISHOP_ENUM));
    add_factory(new PieceFactory<Queen>(QUEEN_ENUM));
    add_factory(new PieceFactory<King>(KING_ENUM));
}




