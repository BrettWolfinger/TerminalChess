#ifndef PROMPTS_H
#define PROMPTS_H

#include <iostream>
#include <string>

#include "Enumerations.h"


// There is no need to modify existing code in this class; you may,
// however, add to this class if you wish.

class Prompts {

public:

    static const char *get_player_name(Player pl) {
        if (pl == WHITE)
            return "White";
        else
            return "Black";
    }

    static void game_choice() {
        std::cout << "Which game would you like to play?\n"
            << "1. Standard Chess\n"
            << "2. King of the Hill Chess\n"
            << "3. Spooky Chess\n";

    }

    static void new_or_load_choice() {
        std::cout
            << "1. Start a new game\n"
            << "2. Load a saved game\n";
    }

    static void load_game() {
        std::cout << "Enter name of file from which to load:\n";
    }

    static void save_game() {
        std::cout << "Enter name of file to save:\n";
    }

    static void load_failure() {
        std::cout << "Failed to load game\n";
    }

    static void save_failure() {
        std::cout << "Failed to save game\n";
    }

    static void player_prompt(Player pl, int turn) {
        std::cout << get_player_name(pl) << " turn " << turn << ':' << std::endl;
    }

    static void check(Player pl) {
        std::cout << get_player_name(pl) << " checks!" << std::endl;
    }

    static void capture(Player pl) {
        std::cout << get_player_name(pl) << " captures a piece." << std::endl;
    }

    static void ghost_capture() {  // Spooky Chess only
        std::cout << "The ghost captures a piece." << std::endl;
    }

    static void checkmate(Player pl) {
        std::cout << get_player_name(pl) << " checkmates!!!\n";
    }

    static void stalemate() {
        std::cout << "Stalemate!\n";
    }

    static void parse_error() {
        std::cout << "Error: couldn't parse your move.\n";
    }

    static void out_of_bounds() {
        std::cout << "Error: out of bounds.\n";
    }

    static void no_piece() {
        std::cout << "Error: you do not have a piece there.\n";
    }

    static void blocked() {
        std::cout << "Error: the piece blocked by another piece.\n";
    }

    static void cannot_castle() {
        std::cout << "Error: castling cannot be performed.\n";
    }

    static void must_handle_check() {
        std::cout << "Error: your king is checked.\n";
    }

    static void cannot_expose_check() {
        std::cout << "Error: you cannot expose your king to check.\n";
    }

    static void illegal_move() {
        std::cout << "Error: illegal move.\n";
    }

    static void win(Player pl, int turn) {
        std::cout << get_player_name(pl) << " wins on turn " << turn << ".\n";
    }

    static void game_over() {
        std::cout << "Game over. Goodbye!\n";
    }

    static void conquered(Player pl) {  //King of the Hill Chess only
        std::cout << get_player_name(pl) << "'s king has reached the hill!!!\n";
    }

};

#endif // PROMPTS_H
