#include <iostream>
#include "Prompts.h"
#include "Game.h"
#include "ChessGame.h"
#include "KOTHChessGame.h"
#include "SpookyChessGame.h"

using std::cin;
using std::string;

// Game variant enumeration
enum GameName {STANDARD_CHESS = 1, KING_OF_THE_HILL, SPOOKY_CHESS};


// Ask user which game they want to play
int collect_game_choice() {
    Prompts::game_choice();
    int game_choice;
    cin >> game_choice;
    return game_choice;
}

// Ask user whether to launch new game or load existing one
int determine_new_or_load() {
    Prompts::new_or_load_choice();
    int new_or_load;
    cin >> new_or_load;
    return new_or_load;
}

// Ask user for name of file where game state is stored
string collect_filename() {
    Prompts::load_game();
    string f;
    cin >> f;
    return f;
}



int main() {

    // Determine which game to play, and how to begin it
    int game_choice = collect_game_choice();
    int new_or_load_choice = determine_new_or_load();

    // Set up the desired game
    Game *g = nullptr;
    if (game_choice == STANDARD_CHESS && new_or_load_choice == 1) {  //new standard chess
        g = new ChessGame();
    } else if (game_choice == STANDARD_CHESS && new_or_load_choice == 2) { //load standard chess
        string filename = collect_filename();
        g = new ChessGame(filename);
    } else if (game_choice == KING_OF_THE_HILL && new_or_load_choice == 1) {  //new standard chess
        g = new KOTHChessGame();
    } else if (game_choice == KING_OF_THE_HILL && new_or_load_choice == 2) { //load standard chess
        string filename = collect_filename();
        g = new KOTHChessGame(filename);
    } else if (game_choice == SPOOKY_CHESS && new_or_load_choice == 1) {  //new standard chess
        g = new SpookyChessGame();
    } else if (game_choice == SPOOKY_CHESS && new_or_load_choice == 2) { //load standard chess
        string filename = collect_filename();
        g = new SpookyChessGame(filename);
    }


    // You'll want to add additional alternatives here for chess game variants


    else {
      std::cout << "Invalid option(s) selected. Exiting the program. \n" << std::endl;
      return 1;
    }

    // Begin play of the selected game!
    g->run();

    // Nothing else to do here in main, so clean up
    delete g;

    return 0;
}

