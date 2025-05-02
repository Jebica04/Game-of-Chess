#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>

#define BOARD_SIZE 8
#define BOARD_START 'a'
#define BOARD_END 'h'
#define BOARD_BOTTOM 1
#define BOARD_TOP 8

char colorTurn = 'w';
int countTurn = 0;

int _Kw = 0; // number of king pieces
int _Kb = 0;

int _Qw = 0; // number of queen pieces
int _Qb = 0;

int _Rw = 0; // number of rook pieces
int _Rb = 0;

int _Bw = 0; // number of bishop pieces
int _Bb = 0;

int _Nw = 0; // number of knight pieces
int _Nb = 0;

int _Pw = 0; // number of pawn pieces
int _Pb = 0;

int _Dw = 0; // double pieces
int _Db = 0;

int _Sw = 0; // blocked (static) pieces
int _Sb = 0;

int _Iw = 0; // isolated pieces
int _Ib = 0;

int _Mw = 0; // mobility of freedom
int _Mb = 0;

char board[8][8][3] = {
    {"br", "bn", "bb", "bq", "bk", "bb", "bn", "br"},
    {"bp", "bp", "bp", "bp", "bp", "bp", "bp", "bp"},
    {"s", "s", "s", "s", "s", "s", "s", "s"},
    {"s", "s", "s", "s", "s", "s", "s", "s"},
    {"s", "s", "s", "s", "s", "s", "s", "s"},
    {"s", "s", "s", "s", "s", "s", "s", "s"},
    {"wp", "wp", "wp", "wp", "wp", "wp", "wp", "wp"},
    {"wr", "wn", "wb", "wq", "wk", "wb", "wn", "wr"},
};

/*A char board[8][8][3] = {
    // 0    1   2   3   4   5   6   7
    {"s", "s", "s", "s", "s", "s", "s", "s"},   // 0
    {"s", "s", "s", "s", "s", "s", "s", "s"},   // 1
    {"s", "s", "s", "s", "s", "s", "wq", "s"},  // 2
    {"s", "s", "s", "s", "s", "s", "s", "s"},   // 3
    {"s", "s", "s", "s", "s", "s", "s", "s"},   // 4
    {"s", "s", "wn", "s", "s", "wk", "s", "s"}, // 5
    {"s", "s", "s", "s", "s", "s", "s", "s"},   // 6
    {"wp", "s", "s", "s", "s", "s", "s", "s"},  // 7
};*/

void printBoard()
{
    setlocale(LC_CTYPE, "");
    wchar_t King = 0x2654;
    wchar_t Queen = 0x2655;
    wchar_t Rooke = 0x2656;
    wchar_t Bishop = 0x2657;
    wchar_t Knight = 0x2658;
    wchar_t Pawn = 0x2659;
    wchar_t whole = 0x25A0;
    wchar_t bhole = 0x25A1;
    int colour;

    int i = 0;
    int j = 0;

    wprintf(L"  a b c d e f g h  \n");

    for (i = 0; i < 8; i++)
    {

        wprintf(L"%d ", i + 1);

        for (j = 0; j < 8; j++)
        {
            if (board[i][j][0] == 'w')
            {
                colour = 1;
            }
            else
            {
                colour = 0;
            }

            if (board[i][j][1] == 'r')
            {
                wprintf(L"%lc ", Rooke + colour * 6);
            }
            else if (board[i][j][1] == 'n')
            {
                wprintf(L"%lc ", Knight + colour * 6);
            }
            else if (board[i][j][1] == 'p')
            {
                wprintf(L"%lc ", Pawn + colour * 6);
            }
            else if (board[i][j][1] == 'b')
            {
                wprintf(L"%lc ", Bishop + colour * 6);
            }
            else if (board[i][j][1] == 'k')
            {
                wprintf(L"%lc ", King + colour * 6);
            }
            else if (board[i][j][1] == 'q')
            {
                wprintf(L"%lc ", Queen + colour * 6);
            }

            else
            {
                if ((i + j) % 2 == 0)
                {
                    wprintf(L"%lc ", whole);
                }

                else
                {
                    wprintf(L"%lc ", bhole);
                }
            }
        }
        wprintf(L"%d ", i + 1);
        wprintf(L"\n");
    }
    wprintf(L"  a b c d e f g h  \n");
}

int isWithinTheTable(int x2, int y2) // Function to check if a move is valid
{
    // char piece = board[x1][y1];

    // Check if the end coordinates are within the bounds of the board
    if (x2 < 0 || x2 >= 8 || y2 < 0 || y2 >= 8)
    {
        return 0;
    }
}

int isFriendly(char color, int x, int y)
{
    if (board[x][y][0] == color && board[x][y][0] != 's')
    {
        return 1;
    }
    return 0;
}

int isEnemy(char color, int x, int y)
{
    if (board[x][y][0] != color && board[x][y][0] != 's')
    {
        return 1;
    }
    return 0;
}


void parseCoordinates(wchar_t* pos, int* y, int* x)
{
    char letter = pos[0];
    *y = pos[1] - '1';

    if (letter >= 'A' && letter <= 'Z')
        *x = letter - 'A';
    else if (letter >= 'a' && letter <= 'z')
        *x = letter - 'a';
    else
        *x = -1;
}


int validateMoveCoordinates(int x1, int y1, int x2, int y2) // validare pereche de coordonate
{
    // Validate each coordinate individually
    int valid_x1 = validateCoordinate(x1, BOARD_START-1, BOARD_END-1);
    int valid_y1 = validateCoordinate(y1, BOARD_BOTTOM-1, BOARD_TOP-1);
    int valid_x2 = validateCoordinate(x2, BOARD_START-1, BOARD_END-1);
    int valid_y2 = validateCoordinate(y2, BOARD_BOTTOM-1, BOARD_TOP-1);

    // verific daca ambele coordonate sunt valabile si inegale
    return (valid_x1 && valid_y1 && valid_x2 && valid_y2 && !(x1 == x2 && y1 == y2));
}

int validateCoordinate(int coordinate, int x, int y)
{
    return (coordinate >= x && coordinate <= y);
}

int validateKingMove(int x1, int x2, int y1, int y2)
{
    if (abs(x1 - x2) <= 1 || abs(y1 - y2) <= 1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int validateBishopMove(int x1, int x2, int y1, int y2)
{
    if (abs(x1 - x2) == abs(y1 - y2))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int validateRookMove(int x1, int x2, int y1, int y2)
{
    if ((x1 == x2 && y1 == y2) && (x1 == x2 || y1 == y2))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int validateQueenMove(int x1, int x2, int y1, int y2)
{
    if (abs(x1 - x2) == abs(y1 - y2))
    {
        return 1;
    }
    else if ((x1 == x2 && y1 == y2) && (x1 == x2 || y1 == y2))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int validateKnightMove(int x1, int x2, int y1, int y2)
{
    if ((abs(x1 - x2) == 1 && abs(y1 - y2) == 2) || (abs(x1 - x2) == 2 && abs(y1 - y2) == 1))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int validatePawnMove(int x1, int y1, int x2, int y2) //(int x1, int x2, int y1, int y2)
{
    char color = board[x1][y1][0];

    if (isFriendly(color, x2, y2))
    {
        return 0;
    }
    if ((color == 'w' && x2 > x1) || (color == 'b' && x2 < x1))
    {
        return 0;
    }
    if ((color == 'w' && x1 == 6) || (color == 'b' && x1 == 1))
    {
        wprintf(L"%c\n", board[x1 + 1][y2][0]);
        if (abs(x1 - x1) <= 2 && y1 == y2 && board[x2][y2][0] == 's' && board[x1 + 1][y2] == 's')
        {
            return 1;
        }
    }
    if (abs(x1 - x1) == 1 && y1 == y2 && board[x2][y2][0] == 's')
    {
        return 1;
    }
    else if (abs(x1 - x1) == 1 && (abs(y1 - y1) == 1))
    {
        if (board[x2][y2][0] != 's')
        {
            return 1;
        }
    }

    return 0;

    /* if(board[x1][y1]== 'bp' && ((x1-1==x2) || ((x1-2==x2) && y1==1))&& y1==y2)
    {
        return 1;
    }
    else if(board[x1][y1]== 'wp' && ((x1+1==x2) || ((x1+2==x2) && y1==6))&& y1==y2)
    {
        return 1;
    }
    else
    {
        return 0;
    } */
}

int validateMove(char piece[2], char startPos[2], char endPos[2])
{
    int y1 = startPos[0] - 'a';
    int x1 = 8 - (startPos[1] - '0');
    int y2 = endPos[0] - 'a';
    int x2 = 8 - (endPos[1] - '0');

    if (isWithinTheTable(x1, y1) || isWithinTheTable(x2, y2))
    {
        return 1;
    }
    else if (isFriendly(piece[0], x2, y2))
    {
        return 1;
    }
    else
    {
        if (board[x1][y1][1] == 'k' && piece[1] == 'k' && validateKingMove(x1, y1, x2, y2))
        {
            return 1;
        }
        if (board[x1][y1][1] == 'b' && piece[1] == 'b' && validateBishopMove(x1, y1, x2, y2))
        {
            return 1;
        }
        if (board[x1][y1][1] == 'r' && piece[1] == 'r' && validateRookMove(x1, y1, x2, y2))
        {
            return 1;
        }
        if (board[x1][y1][1] == 'q' && piece[1] == 'q' && validateQueenMove(x1, y1, x2, y2))
        {
            return 1;
        }
        if (board[x1][y1][1] == 'n' && piece[1] == 'n' && validateKnightMove(x1, y1, x2, y2))
        {
            return 1;
        }
        if (board[x1][y1][1] == 'p' && piece[1] == 'p' && validatePawnMove(x1, y1, x2, y2))
        {
            return 1;
        }

        return 0;
    }
}


int validatePieceMove(wchar_t piece, int x1, int y1, int x2, int y2)
{
    switch (piece)
    {
    case 'k':
        return validateKingMove(x1, x2, y1, y2);
        break;
    case 'b':
        return validateBishopMove(x1, x2, y1, y2);
        break;
    case 'r':
        return validateRookMove(x1, x2, y1, y2);
        break;
    case 'q':
        return validateQueenMove(x1, x2, y1, y2);
        break;
    case 'n':
        return validateKnightMove(x1, x2, y1, y2);
        break;
    case 'p':
        return validatePawnMove(x1, y1, x2, y2);
        break;
    default:
        wprintf(L"\t!!! Invalid piece !!!\n");
        return 0;
        break;
    }       
}

void locatePieces(char color)
{
    char startPosition[2];
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (board[i][j][0] == color)
            {
                startPosition[0] = j + 'a';
                startPosition[1] = 8 - i + '0';
                generateMove(board[i][j], startPosition);
            }
        }
    }
}

int generateKing(int x, int y)
{
    int counter = 0;

    if (validateKingMove(x, y, x + 1, y))
    {
        counter++;
        // wprintf(L"%d %d\n", x + 1, y);
    }
    if (validateKingMove(x, y, x - 1, y))
    {
        counter++;
        // wprintf(L"%d %d\n", x - 1, y);
    }
    if (validateKingMove(x, y, x, y + 1))
    {
        counter++;
        // wprintf(L"%d %d\n", x, y + 1);
    }
    if (validateKingMove(x, y, x, y - 1))
    {
        counter++;
        // wprintf(L"%d %d\n", x, y - 1);
    }
    if (validateKingMove(x, y, x + 1, y + 1))
    {
        counter++;
        // wprintf(L"%d %d\n", x + 1, y + 1);
    }
    if (validateKingMove(x, y, x - 1, y + 1))
    {
        counter++;
        // wprintf(L"%d %d\n", x - 1, y + 1);
    }
    if (validateKingMove(x, y, x - 1, y - 1))
    {
        counter++;
        // wprintf(L"%d %d\n", x - 1, y - 1);
    }
    if (validateKingMove(x, y, x + 1, y - 1))
    {
        counter++;
        // wprintf(L"%d %d\n", x + 1, y - 1);
    }
}

int generateBishop(int x, int y)
{
    int j1 = y;
    int j2 = y;

    for (int i = x + 1; i < 8; i++)
    {
        j1++;
        j2--;
        if (validateBishopMove(x, y, i, j1) && j1 < 8)
        {
            wprintf(L"%d %d\n", i, j1);
        }
        if (validateBishopMove(x, y, i, j2) && j2 >= 8)
        {
            wprintf(L"%d %d\n", i, j2);
        }
    }

    j1 = y;
    j2 = y;

    for (int i = x - 1; i >= 0; i--)
    {
        j1++;
        j2--;
        if (validateBishopMove(x, y, i, j1) && j1 < 8)
        {
            wprintf(L"%d %d\n", i, j1);
        }
        if (validateBishopMove(x, y, i, j2) && j2 >= 0)
        {
            wprintf(L"%d %d\n", i, j2);
        }
    }
}

int generateRook(int x, int y)
{
    for (int i = 0; i < 8; i++)
    {
        if (validateRookMove(x, y, i, y) == 1)
        {
            wprintf(L"%d %d\n", i, y);
        }
    }
    for (int i = 0; i < 8; i++)
    {
        if (validateRookMove(x, y, x, i) == 1)
        {
            wprintf(L"%d %d\n", x, i);
        }
    }
}

int generateQueen(int x, int y)
{
    int counter = 0;
    counter += generateBishop(x, y);
    counter += generateRook(x, y);
    return counter;
}

int generateKnight(int x, int y)
{
    int counter = 0;

    if (validateKnightMove(x, y, x + 1, y + 2))
    {
        counter++;
        // wprintf(L"%d %d\n", x + 1, y + 2);
    }
    if (validateKnightMove(x, y, x + 2, y + 1))
    {
        counter++;
        // wprintf(L"%d %d\n", x + 2, y + 1);
    }
    if (validateKnightMove(x, y, x + 2, y - 1))
    {
        counter++;
        // wprintf(L"%d %d\n", x + 2, y - 1);
    }
    if (validateKnightMove(x, y, x + 1, y - 2))
    {
        counter++;
        // wprintf(L"%d %d\n", x + 1, y - 2);
    }
    if (validateKnightMove(x, y, x - 1, y - 2))
    {
        counter++;
        // wprintf(L"%d %d\n", x - 1, y - 2);
    }
    if (validateKnightMove(x, y, x - 2, y - 1))
    {
        counter++;
        // wprintf(L"%d %d\n", x - 2, y - 1);
    }
    if (validateKnightMove(x, y, x - 2, y + 1))
    {
        counter++;
        // wprintf(L"%d %d\n", x - 2, y + 1);
    }
    if (validateKnightMove(x, y, x - 1, y + 2))
    {
        counter++;
        // wprintf(L"%d %d\n", x - 1, y + 2);
    }
}

int generatePawn(int x, int y)
{
    int counter = 0;
    if (validatePawnMove(x, y, x + 2, y))
    {
        counter++;
        // wprintf(L"%d %d\n", x + 2, y);
    }
    if (validatePawnMove(x, y, x + 1, y))
    {
        counter++;
        // wprintf(L"%d %d\n", x + 1, y);
    }
    if (validatePawnMove(x, y, x + 1, y + 1))
    {
        counter++;
        // wprintf(L"%d %d\n", x + 1, y + 1);
    }
    if (validatePawnMove(x, y, x + 1, y - 1))
    {
        counter++;
        // wprintf(L"%d %d\n", x + 1, y - 1);
    }
    if (validatePawnMove(x, y, x - 2, y))
    {
        counter++;
        // wprintf(L"%d %d\n", x - 2, y);
    }
    if (validatePawnMove(x, y, x - 1, y))
    {
        counter++;
        // wprintf(L"%d %d\n", x - 1, y);
    }
    if (validatePawnMove(x, y, x - 1, y + 1))
    {
        counter++;
        // wprintf(L"%d %d\n", x - 1, y + 1);
    }
    if (validatePawnMove(x, y, x - 1, y - 1))
    {
        counter++;
        // wprintf(L"%d %d\n", x - 1, y - 1);
    }
}

int generateMove(char piece[2], char startPos[2]) //(char pieceType, int x1, int y1)
{
    int counter = 0;
    int y1 = startPos[0] - 'a';
    int x1 = 8 - (startPos[1] - '0');
    if (board[x1][y1][1] == 'k' && piece[1] == 'k')
    {
        counter = generateKing(x1, y1);
    }
    if (board[x1][y1][1] == 'b' && piece[1] == 'b')
    {
        counter = generateBishop(x1, y1);
    }

    if (board[x1][y1][1] == 'r' && piece[1] == 'r')
    {
        counter = generateRook(x1, y1);
    }

    if (board[x1][y1][1] == 'q' && piece[1] == 'q')
    {
        counter = generateQueen(x1, y1);
    }

    if (board[x1][y1][1] == 'n' && piece[1] == 'n')
    {
        counter = generateKnight(x1, y1);
    }

    if (board[x1][y1][1] == 'p' && piece[1] == 'p')
    {
        counter = generatePawn(x1, y1);
    }
    if (piece[0] == 'w')
    {
        _Mw += counter;
    }
    else if (piece[0] = 'b')
    {
        _Mb += counter;
    }
}

int countDoublePawn(int x, int y)
{
    char color = board[x][y][0];
    if (color == 'w' && board[x - 1][y][0] == 'w' && board[x][y][1] == board[x - 1][y][1])
    {
        _Dw++;
    }
    if (color == 'b' && board[x + 1][y][0] == 'b' && board[x][y][1] == board[x + 1][y][1])
    {
        _Db++;
    }
}

int countBlockedPawns(int x, int y)
{
    char color = board[x][y][0];
    if (color == 'w' && board[x - 1][y][0] == 'b' && board[x][y][1] == board[x - 1][y][1])
    {
        _Sw++;
    }
    if (color == 'b' && board[x + 1][y][0] == 'w' && board[x][y][1] == board[x - 1][y][1])
    {
        _Sb++;
    }
}

int countIsolatedPawns(int x, int y)
{
    int k = 0;
    char color = board[x][y][0];
    for (int i = 0; i < 8; i++)
    {
        if (board[x][y][1] == board[x][y - 1][1] && color == board[x][y - 1][0] || board[x][y][1] == board[x][y + 1][1] && color == board[x][y + 1][0])
        {
            k = 1;
        }
    }

    if (k == 0 && color == 'w')
    {
        _Iw++;
    }
    if (k == 0 && color == 'b')
    {
        _Ib++;
    }
}

int countScore()
{
    float score = 0;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (board[i][j][0] == 'w')
            {
                if (board[i][j][1] == 'k')
                {
                    _Kw++;
                }
                if (board[i][j][1] == 'q')
                {
                    _Qw++;
                }
                if (board[i][j][1] == 'r')
                {
                    _Rw++;
                }
                if (board[i][j][1] == 'b')
                {
                    _Bw++;
                }
                if (board[i][j][1] == 'n')
                {
                    _Nw++;
                }
                if (board[i][j][1] == 'p')
                {
                    _Pw++;
                    countDoublePawn(i, j);
                    countBlockedPawns(i, j);
                    countIsolatedPawns(i, j);
                }
            }

            if (board[i][j][0] == 'b')
            {
                if (board[i][j][1] == 'k')
                {
                    _Kb++;
                }
                if (board[i][j][1] == 'q')
                {
                    _Qb++;
                }
                if (board[i][j][1] == 'r')
                {
                    _Rb++;
                }
                if (board[i][j][1] == 'b')
                {
                    _Bb++;
                }
                if (board[i][j][1] == 'n')
                {
                    _Nb++;
                }
                if (board[i][j][1] == 'p')
                {
                    _Pb++;
                    countDoublePawn(i, j);
                    countBlockedPawns(i, j);
                    countIsolatedPawns(i, j);
                }
            }
        }
    }

    locatePieces('w');
    locatePieces('b');

    wprintf(L"%d %d %d %d %d %d %d %d %d %d\n", _Kw, _Qw, _Bw, _Rw, _Nw, _Pw, _Dw, _Sw, _Iw, _Mw);
    wprintf(L"%d %d %d %d %d %d %d %d %d %d\n", _Kb, _Qb, _Bb, _Rb, _Nb, _Pb, _Db, _Sb, _Ib, _Mb);

    score = 200 * (_Kw - _Kb) + 9 * (_Qw - _Qb) + 5 * (_Rw - _Rb) + 
            3 * (_Bw - _Bb + _Nw - _Nb) + +1 * (_Pw - _Pb) - 
            -0.5 * (_Dw - _Db + _Sw - _Sb + _Iw - _Ib) + 0.1 * (_Mw - _Mb);
}

int PvP()
{
    wprintf(L"\n~~~ Changing turns ~~~\n\n");
    printBoard();
    wprintf(L"\n");

    if (colorTurn == 'w')
    {
        wprintf(L"White's turn!\n");
    }
    else
    {
        wprintf(L"Black's turn!\n");
    }

    wchar_t startPos[2];
    wchar_t endPos[2];
    int x1, y1;
    int x2, y2;
    wchar_t selectedPiece;
    wchar_t selectedPColor;

    while (1)
    {
        wprintf(L"Select a piece by giving coordinates: ");
        wscanf(L"%ls", &startPos);

        parseCoordinates(startPos, &x1, &y1);

        wprintf(L"Enter the destination: ");
        wscanf(L"%ls", &endPos);

        parseCoordinates(endPos, &x2, &y2);

        int validMoveCoord = validateMoveCoordinates(x1, y1, x2, y2);

        if (validMoveCoord != 1)
        {
            wprintf(L"\t !!! Bad coordinates, try again !!!\n");
            continue;
        }

        // printf(L"%d %d %d %d\n", x1, y1, x2, y2);

        selectedPColor = board[x1][y1][0];
        selectedPiece = board[x1][y1][1];

        if (selectedPColor == 's')
        {
            wprintf(L"\t !!! Inexistent piece, try again !!!\n");
            continue;
        }

        if (selectedPColor != colorTurn)
        {
            wprintf(L"\t!!! Incorect color piece (%lc%lc), try again !!!\n", selectedPColor, selectedPiece);
            continue;
        }

        if (isFriendly(selectedPColor, x2, y2))
        {
            wprintf(L"\t!!! Destination ocupied by your piece, try again !!!\n", selectedPColor, selectedPiece);
            continue;
        }

        // validare miscare piesa indiferent daca destiatia e ocupata sau nu
        if (validatePieceMove(selectedPiece, x1, y1, x2, y2) != 1)
        {
            wprintf(L"\t!!! Illegal move of the selected piece !!!\n", selectedPColor, selectedPiece);
            continue;
        }

        break;
    }

    // GAME IS HAPPENING

    board[x1][y1][0] = 's';
    board[x1][y1][1] = '\0';
    board[x2][y2][0] = selectedPColor;
    board[x2][y2][1] = selectedPiece;

    if (colorTurn == 'w')
    {
        colorTurn = 'b';
    }
    else
    {
        colorTurn = 'w';
        countTurn++;
    }
}

int PvC()
{

}

int saveBoard(){
    FILE *fp;
    fp = fopen("board.save","w+");
    if(fp == NULL){
        printf("Failed to save board\n");
        return 0;
    }
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            fprintf(fp,"%s ",board[i][j]);
        }
        fprintf(fp,"\n");
    }

}

int loadBoard(){
    FILE *fp;
    fp = fopen("board.save","r");
    if(fp == NULL){
        printf("Failed to load board\n");
        return 0;
    }
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            fscanf(fp,"%s",board[i][j]);
        }
    }

}

int main()
{
    /*int option;
    wprintf(L"Chose the game mode:\n");
    wprintf(L"1. Player vs Player\n");
    wprintf(L"2. Player vs Computer\n");

    scanf("%d", &option);

    if (option == 1)
    {
        PvP();
    }    
    else if (option == 2)
    {
        PvC();
    }

    system("clear");*/

    printBoard();
    loadBoard();
    printBoard();

    return 0;
}