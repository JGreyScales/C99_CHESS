// A terminal Based Chess game

// GreyScales Sept 18th 2024
#include <stdio.h>
#include <string.h>

// define ascii characters with circles around text for possible movesets
#define EMPTY_MOVEABLE_SPACE "OO"
#define WHITE_MOVEABLE_PIECE "W"
#define BLACK_MOVEABLE_PIECE "B"
#define KING_MOVEABLE_PIECE "K"
#define QUEEN_MOVEABLE_PIECE "Q"
#define BISHOP_MOVEABLE_PIECE "B"
#define KNIGHT_MOVEABLE_PIECE "N"
#define ROOK_MOVEABLE_PIECE "R"
#define PAWN_MOVEABLE_PIECE "P"
#define EMPTY_SPACE "--"

#define ASCII_NUMBER_OFFSET 48
#define FOREFEIT_CODE 129
#define INVALID_SELECTION 1
#define ILLEGAL_SELECTION_CODE "---"
#define ASCII_CHAR_DIFFERENCE 32

void print_game_board(char playBoard[128])
{

    printf("  a  b  c  d  e  f  g  h\n");
    for (int xOffset = 0; xOffset < 128; xOffset += 16)
    {
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
    int x = 0;
    switch (letter[0])
    {
    case 'a':
    case 'A':
        x = 1;
        break;
    case 'b':
    case 'B':
        x = 2;
        break;
    case 'c':
    case 'C':
        x = 3;
        break;
    case 'd':
    case 'D':
        x = 4;
        break;
    case 'e':
    case 'E':
        x = 5;
        break;
    case 'f':
    case 'F':
        x = 6;
        break;
    case 'g':
    case 'G':
        x = 7;
        break;
    case 'h':
    case 'H':
        x = 8;
        break;
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

int calculate_possible_moves(
    int pieceOffset,
    char playboard[128],
    char currentSide)
{
    char validMoves[95] = "";
    int directionModifier = -1;
    char selectedSide = playboard[pieceOffset];
    char selectedType = playboard[pieceOffset + 1];
    char copiedPlayboard[128];
    int piecePlacementOnRow = pieceOffset % 16;
    int pieceMovementToRight = 16 - piecePlacementOnRow;
    int pieceMovementToLeft = 16 - pieceMovementToRight;
    int pathBlocked = 0;
    strncpy(copiedPlayboard, playboard, 128);

    if ('b' == selectedSide)
    {
        directionModifier = 1;
    }

    // ensures the player is only moving pieces that belong to them
    if (selectedSide != currentSide)
    {
        return INVALID_SELECTION;
    }
    // take the piece type (index 1)
    // get valid moves for that piece type
    // handle cases include Pawn
    switch (selectedType)
    {
    // xy
    // indicates a "path" segement
    // there is an issue with the negative values
    case 'k':
        sprintf(validMoves, "%d,%s", 10, "01");
        break;
    case 'q':
        sprintf(validMoves, "|%d,%d,%d,%d,%d,%d,%d|,%d,%d,%d,%d,%d,%d,%d|,%s,%s,%s,%s,%s,%s,%s|,%d,%d,%d,%d,%d,%d,%d", 11, 22, 33, 44, 55, 66, 77, -11, -22, -33, -44, -55, -66, -77, "01", "02", "03", "04", "05", "06", "07", 10, 20, 30, 40, 50, 60, 70);
        break;
    case 'n':
        sprintf(validMoves, "%s,%s,%s,%s", "-12", "12", "21", "2-1");
        break;
    case 'b':
        sprintf(validMoves, "|%d,%d,%d,%d,%d,%d,%d|,%d,%d,%d,%d,%d,%d,%d", 11, 22, 33, 44, 55, 66, 77, -11, -22, -33, -44, -55, -66, -77);
        break;
    case 'r':
        sprintf(validMoves, "|%s,%s,%s,%s,%s,%s,%s|,%d,%d,%d,%d,%d,%d,%d", "01", "02", "03", "04", "05", "06", "07", 10, 20, 30, 40, 50, 60, 70);
        break;
    case 'p':
        sprintf(validMoves, "%s", "01");
        break;
    // ensures the player is not selecting an empty space
    default:
        return INVALID_SELECTION;
    }

    // decode the movement pattern
    for (int i = 0; strlen(validMoves) >= i; i += 3)
    {

        if ('|' == validMoves[i]){
            pathBlocked = 0;
        }

        if (pathBlocked){
            continue;
        }
        int xModifier = 1;
        int yModifier = 1;

        if ('-' == validMoves[i])
        {
            xModifier = -1;
            i++;
        }

        int x = ((int)validMoves[i] - ASCII_NUMBER_OFFSET) * xModifier;

        if ('-' == validMoves[i + 1])
        {
            yModifier = -1;
            i++;
        }


        int y = ((int)validMoves[i + 1] - ASCII_NUMBER_OFFSET) * yModifier;


        for (int recursionModifier = -1;
            recursionModifier <= 1;
            recursionModifier += 2)
        {
            // 16 chars per row
            // tile length of 2
            int selectedPiece = pieceOffset + (recursionModifier * (y * 16)) + (recursionModifier * (x * 2));
            // handle off the board errors & side checking

            if (0 > selectedPiece || 128 < selectedPiece ||
            currentSide == playboard[selectedPiece] ||
            // ensures the x values does not go over the side of the board left
            pieceOffset - pieceMovementToLeft > pieceOffset + (recursionModifier * (x * 2)) ||
            // ensures x values does not go over the side of the board right
            pieceOffset + pieceMovementToRight <= pieceOffset + (recursionModifier * (x * 2))
            )
            {
                // stops processing of other options along the path
                pathBlocked = 1;
                continue;
            }

            // prevents pawns from going backwards
            if ('p' == selectedType && recursionModifier != directionModifier)
            {
                continue;
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
    }
    
    print_game_board(copiedPlayboard);

    int selectedMove = piece_selection(1);
    return 0;
}

int main(void)
{

    int gameActive = 0;
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
        int selectedPiece = piece_selection(0);

        // handle forefeits
        if (FOREFEIT_CODE == selectedPiece)
        {
            return 0;
        }

        // calculate possible moves & reprint the board
        // if statement handles if the user selected an invalid piece
        if (calculate_possible_moves(selectedPiece, playBoard, currentSide))
        {
            printf("Invalid selection, please try again\n");
            continue;
        }

        // gather what piece the user wants to move to, and handle errors inside


        // update gameboard

        // switch the players each turn
        if ('b' == currentSide)
        {
            currentSide = 'w';
        }
        else
        {
            currentSide = 'b';
        }
    } while (gameActive);

    return 0;
}