// A terminal Based Chess game, uses multiple lines.
// Ensure that your terminal is large enough

// GreyScales Sept 18th 2024
#include <stdio.h>
#include <string.h>
#include "Utils/Utils.h"

// define ascii characters with circles around text for possible movesets

int main(void)
{

    int gameActive = 1;
    char currentSide = 'w';

    // total length 128
    // each tile is length of 2
    // each row is length of 16
    char playBoard[128] = "brbnbbbkbqbbbnbrbpbpbpbpbpbpbpbp----------------------------------------------------------------wpwpwpwpwpwpwpwpwrwnwbwqwkwbwnwr";


    do
    {
        printf("It is %c's turn\n", currentSide);

        print_game_board(playBoard);

        // gather what piece the user wants to select, and handle errors inside
        int selectedPiece = piece_selection(1);

        // handle forefeits
        if (FOREFEIT_CODE == selectedPiece)
            return 0;
        if (playBoard[selectedPiece] != currentSide){
            printf("Invalid piece choosen, must be piece from your side\n");
            continue;
        }


        // calculate possible moves
        // overrides validMoves inside of func
        // returns 1 on failure
        char validMoves[205] = "";
        if (calculate_possible_moves(selectedPiece, playBoard, validMoves)){
            printf("Invalid selection, please try again\n");
            continue;
        }

        char movementPlayboard[128] = "";
        strncpy(movementPlayboard, playBoard, 128);

        // stores a copied of the playboard with moves highlighted onto 
        // copiedplayboard
        // does weird math shit to decode the valid movement storage method
        decode_movement_pattern(selectedPiece, playBoard, validMoves,
                                movementPlayboard, currentSide);

        // reprint the board with the moves highlighted
        print_game_board(movementPlayboard);

        // gather what piece the user wants to move to, and handle errors inside
        int validatedMove = 0;
        int moveToPiece = -1;
        do {
            moveToPiece = piece_selection(0);
            validate_final_move(movementPlayboard, &validatedMove, moveToPiece);
        } while(!validatedMove && -1 != moveToPiece);


        // update gameboard

        update_gameboard_with_move(playBoard, selectedPiece, moveToPiece);

        // switch the players each turn
        switch_sides(&currentSide);
        printf("\n\n\n\n");
    } while (gameActive);

    return 0;
}