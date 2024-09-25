#pragma once

#include "Utils.c"
// interface for board utils
// JGreyscales - Wed Sept 25, 2025

void print_game_board(char playBoard[128]);
int letter_to_int(char letter[2]);
int piece_selection(int confirmFlag);
int calculate_possible_moves(int pieceOffset, 
                             char playboard[128],
                             char validMoves[98]);
void decode_movement_pattern(int pieceOffset,
                            char playboard[128],
                            char validMoves[98],
                            char copiedPlayboard[128],
                            char currentSide);
void switch_sides(char* currentSide);
void validate_final_move(char playboard[128],
                         int* validatedMove,
                         int selectedPiece);
void update_gameboard_with_move(char playBoard[128],
                                int selectedPiece,
                                int moveToSpot);