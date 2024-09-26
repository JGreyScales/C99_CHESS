#include <stdio.h>
#include <string.h>

// implementation of boad Utils
// JGreyscales - Wed Sept 25, 2025

#define ASCII_NUMBER_OFFSET     48
#define ASCII_CAPITAL_OFFSET    64
#define ASCII_LOWER_OFFSET      96
#define FOREFEIT_CODE           129
#define INVALID_SELECTION       1
#define ASCII_CHAR_DIFFERENCE   32

void print_game_board(char playBoard[128])
{

    //board is a 128 chars long
    //each row is 16 chars
    // prints y column
    printf("  a  b  c  d  e  f  g  h\n");
    for (int xOffset = 0; xOffset < 128; xOffset += 16)
    {
        // prints x column
        printf("%d", (xOffset / 16) + 1);
        for (int x = 0; x < 16; x += 2)
        {
            printf(" %c", playBoard[xOffset + x]);
            printf("%c", playBoard[xOffset + x + 1]);
        }
        printf("\n");
    }
}

int letter_to_int(char letter[2])
{
    int x = (int)letter[0];

    // if the ascii number is a capital letter
    if (65 <= x && 90 >= x){
        x -= ASCII_CAPITAL_OFFSET;

    // lower case numbers
    } else if (97 <= x && 122 >= x){
        x -= ASCII_LOWER_OFFSET;
    } else {
        return 0;
    }

    return x;
}

int piece_selection(int confirmFlag)
{

    int playBoardOffset = -1;

    do
    {
        char userPieceSeletion[2] = "  ";

        if (confirmFlag){
            printf("Please select ur piece (example: a7)\n");
        } else {
            printf("Please select ur desired move\n");
        }
        printf("Type (FF) to forfeit\n");

        // handles assignment error
        if (1 != scanf("%2s", userPieceSeletion))
        {
            printf("Invalid input data, please try again\n");
            continue;
        }

        // handles forefeits
        // == only compares address location, not string content
        // strcmp required from string.h to compensate
        // 0 is return value from strcmp when equal
        if (0 == strcmp("FF", userPieceSeletion) ||
            0 == strcmp("ff", userPieceSeletion))
        {
            return FOREFEIT_CODE;
        }

        // handles 2nd char out of range
        if (((int)userPieceSeletion[1] - ASCII_NUMBER_OFFSET) < 1 ||
            ((int)userPieceSeletion[1] - ASCII_NUMBER_OFFSET) > 8)
        {
            printf("Improper 2nd character used, please try again\n");
            continue;
        }

        // handles 1st char out of range
        int yColumn = letter_to_int(userPieceSeletion) - 1;
        if (-1 == yColumn)
        {
            printf("Improper 1st character used, please try again\n");
            continue;
        }

        // take user input, convert first letter to int
        int xColumn = (int)userPieceSeletion[1] - ASCII_NUMBER_OFFSET - 1;

        playBoardOffset = (xColumn * 16) + (yColumn * 2);
    } while (-1 == playBoardOffset);

    return playBoardOffset;
}

int calculate_possible_moves(int pieceOffset, char playboard[128],
                            char validMoves[205]){
    char selectedType = playboard[pieceOffset + 1];
    switch (selectedType)
    {
    // xy
    // indicates a "path" segement
    // there is an issue with the negative values
    case 'k':
        sprintf(validMoves, "|%d,|%s|,%s|,%s", 10, "01", "-10", "0-1");
        break;
    case 'q':
        sprintf(validMoves, "|%d,%d,%d,%d,%d,%d,%d|,%d,%d,%d,%d,%d,%d,%d|,%s,%s,%s,%s,%s,%s,%s|,%d,%d,%d,%d,%d,%d,%d|,%s,%s,%s,%s,%s,%s,%s|,%s,%s,%s,%s,%s,%s,%s|,%s,%s,%s,%s,%s,%s,%s|,%s,%s,%s,%s,%s,%s,%s", 11, 22, 33, 44, 55, 66, 77, -11, -22, -33, -44, -55, -66, -77, "01", "02", "03", "04", "05", "06", "07", 10, 20, 30, 40, 50, 60, 70, "-1-1", "-2-2", "-3-3", "-4-4", "-5-5", "-6-6", "-7-7", "1-1", "2-2", "3-3", "4-4", "5-5", "6-6", "7-7", "0-1", "0-2", "0-3", "0-4", "0-5", "0-6", "0-7", "-10", "-20", "-30", "-40", "-50", "-60", "-70");
        break;
    case 'n':
        sprintf(validMoves, "|%s|,%s|,%s|,%s|%s|,%s|,%s|,%s", "-12", "12", "21", "2-1", "1-2", "-1-2", "-2-1", "-21");
        break;
    case 'b':
        sprintf(validMoves, "|%d,%d,%d,%d,%d,%d,%d|,%d,%d,%d,%d,%d,%d,%d|,%s,%s,%s,%s,%s,%s,%s|,%s,%s,%s,%s,%s,%s,%s|", 11, 22, 33, 44, 55, 66, 77, -11, -22, -33, -44, -55, -66, -77, "-1-1", "-2-2", "-3-3", "-4-4", "-5-5", "-6-6", "-7-7", "1-1", "2-2", "3-3", "4-4", "5-5", "6-6", "7-7", "0-1", "0-2", "0-3", "0-4", "0-5", "0-6", "0-7");
        break;
    case 'r':
        sprintf(validMoves, "|%s,%s,%s,%s,%s,%s,%s|,%s,%s,%s,%s,%s,%s,%s|%s,%s,%s,%s,%s,%s,%s|%s,%s,%s,%s,%s,%s,%s", "01", "02", "03", "04", "05", "06", "07", "10", "20", "30", "40", "50", "60", "70", "0-1", "0-2", "0-3", "0-4", "0-5", "0-6", "0-7", "-10", "-20", "-30", "-40", "-50", "-60", "-70");
        break;
    case 'p':
        sprintf(validMoves, "|%s", "01");
        break;
    // ensures the player is not selecting an empty space
    default:
        return INVALID_SELECTION;
    }

    return 0;

}

void decode_movement_pattern(int pieceOffset,
                            char playboard[128],
                            char validMoves[205],
                            char copiedPlayboard[128],
                            char currentSide){

    char selectedSide = playboard[pieceOffset];
    char selectedType = playboard[pieceOffset + 1];
    int directionModifier = -1;
    int piecePlacementOnRow = pieceOffset % 16;
    int pieceMovementToRight = 16 - piecePlacementOnRow;
    int pieceMovementToLeft = 16 - pieceMovementToRight;
    int pathBlocked = 0;

    // if playing as black
    if ('b' == selectedSide)
        directionModifier = 1;

    printf("%s", validMoves);

    // begin looping through the components of the validMoves
    for (int MoveIndex = 0; MoveIndex <= strlen(validMoves); MoveIndex += 3){
        int xModifier = 1;
        int yModifier = 1;

        printf("%d %c   - %d", (int)validMoves[MoveIndex], validMoves[MoveIndex], MoveIndex);


        if ('|' == validMoves[MoveIndex - 1]){
                        pathBlocked = 0;
                        printf("hit");

        }            

        if (',' == validMoves[MoveIndex]){

            MoveIndex++;
        }


        //unblock the path each time we come across a new one
        if ('|' == validMoves[MoveIndex]){
            MoveIndex++;
            pathBlocked = 0;
            printf("hit");

        }
        printf("\n");

        // determine if the first digit is negative
        if ('-' == validMoves[MoveIndex]){
            xModifier = -1;
            MoveIndex++;
        }

        // set the x value
        int x = ((int)validMoves[MoveIndex] - ASCII_NUMBER_OFFSET) * xModifier;


        // determine if the second digit is negative
        if ('-' == validMoves[MoveIndex + 1]){
            yModifier = -1;
            MoveIndex++;
        }

        // ensure pawn is moving in the right direction
        if ('p' == selectedType && 'w' == currentSide)
            yModifier = yModifier * -1;

        int y = ((int)validMoves[MoveIndex + 1] - ASCII_NUMBER_OFFSET) * yModifier;

        int selectedPiece = pieceOffset + (y * 16) + (x * 2);

        if (0 > selectedPiece || 128 < selectedPiece || pathBlocked ||
        pieceOffset - pieceMovementToLeft > pieceOffset + (x * 2) || 
        pieceOffset + pieceMovementToRight <= pieceOffset + (x * 2)){
            continue;
        }

        if ('-' != playboard[selectedPiece]){
            pathBlocked = 1;
            if (currentSide == playboard[selectedPiece]){
                continue;
            }
        }

        if ('-' == copiedPlayboard[selectedPiece])
        {
            copiedPlayboard[selectedPiece] = 'O';
            copiedPlayboard[selectedPiece + 1] = 'O';
        }
        else
        {
            copiedPlayboard[selectedPiece] = copiedPlayboard[selectedPiece] - ASCII_CHAR_DIFFERENCE;
            copiedPlayboard[selectedPiece + 1] = copiedPlayboard[selectedPiece + 1] - ASCII_CHAR_DIFFERENCE;
        }

    }

   return;
}

void switch_sides(char* currentSide){
    if ('b' == *currentSide)
        *currentSide = 'w';
    else
        *currentSide = 'b';
}

void validate_final_move(char playboard[128], int* validatedMove,
                       int selectedPiece){

    if (playboard[selectedPiece] == 'O' || 
    (65 <= (int)playboard[selectedPiece] && 90 >= (int)playboard[selectedPiece]))
    {
        *validatedMove = 1;
        return;
    } else {
        return;
    }
}

void update_gameboard_with_move(char playBoard[128], int selectedPiece,
                                int moveToSpot){

    playBoard[moveToSpot] = playBoard[selectedPiece];
    playBoard[moveToSpot + 1] = playBoard[selectedPiece + 1];

    playBoard[selectedPiece] = '-';
    playBoard[selectedPiece + 1] = '-';

    return;
}
