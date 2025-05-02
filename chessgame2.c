#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #include <io.h> 
    #include <fcntl.h>
#endif

#define BOARD_SIZE 8
#define BOARD_START 0
#define BOARD_END 8
#define BOARD_BOTTOM 1
#define BOARD_TOP 8

char colorTurn = 'w';
int countTurn = 0;

// Y = horizontal lines 
// X = vertical lines 
char board[8][8][3] = {
    {"br","bn","bb","bq","bk","bb","bn","br"},
    {"bp","bp","bp","bp","bp","bp","bp","bp"},
    {"s","s","s","s","s","s","s","s"},
    {"s","s","s","s","s","s","s","s"},
    {"s","s","s","s","s","s","s","s"},
    {"s","s","s","s","s","s","s","s"},
    {"wp","wp","wp","wp","wp","wp","wp","wp"},
    {"wr","wn","wb","wq","wk","wb","wn","wr"},
    };

void printBoard();
int validateMoveCoordinates(int x1, int y1, int x2, int y2);
int validateCoordinate(int coordinate, int x, int y);
void parseCoordinates(wchar_t* pos, int* x, int* y);
int isFriendly(wchar_t color, int x, int y);

int validateKingMove(int x1, int x2, int y1, int y2);
int validateBishopMove(int x1, int x2, int y1, int y2);
int validateRookMove(int x1, int x2, int y1, int y2);
int validateQueenMove(int x1, int x2, int y1, int y2);
int validateKnightMove(int x1, int x2, int y1, int y2);
int validatePawnMove(int x1, int y1, int x2, int y2);
int validatePieceMove(wchar_t piece, int x1, int y1, int x2, int y2);
int validateDiagonalMove(int x1, int y1, int x2, int y2);
int validateOrthogonalMove(int x1, int y1, int x2, int y2);

int PVP();
int PVC();
int saveBoard();
int loadBoard();

int main()
{
    #ifdef _WIN32
        _setmode(_fileno(stdout), 0x00020000);
    #endif
    setlocale(LC_CTYPE, "");

    int option;
    wprintf(L"1.Chose the game mode:\n");
    wprintf(L"2. Save game\n");
    wprintf(L"3. Load game\n");

    scanf("%d", &option);

    if (option == 1)
    {
        wprintf(L"11. Player vs Player\n");
        wprintf(L"22. Player vs Computer\n");
        scanf("%d", &option);

        if(option == 11)
        {
            PVP();
        }
        else
        {
            PVC();
        }
    }

    if(option == 2)
    {
        saveBoard();
    }

    if(option == 3)
    {
        loadBoard();
    }    
    
    system("clear");
    
    return 0;
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

int isFriendly(wchar_t color, int x, int y)
{
    if (board[x][y][0] == color)
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

int validateCoordinate(int coordinate, int x, int y)
{
    return (coordinate >= x && coordinate <= y);
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

int validateKingMove(int x1, int x2, int y1, int y2)
{
    wprintf(L"King piece is selected\n");

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
    wprintf(L"Bishop piece is selected\n");

    if (abs(x1 - x2) != abs(y1 - y2))
    {
        return 0;
    }

    return validateDiagonalMove(x1, y1, x2, y2);
}

int validateRookMove(int x1, int x2, int y1, int y2)
{
    wprintf(L"Rook piece is selected\n");

    if ((x1 == x2 && y1 == y2) && (x1 == x2 || y1 == y2))
    {
        // do nothing
    }
    else
    {
        return 0;
    }

    return validateOrthogonalMove(x1, y1, x2, y2);
}

int validateQueenMove(int x1, int x2, int y1, int y2)
{
    wprintf(L"Queen piece is selected\n");

    if (x1 != x2 && y1 != y2 && abs(x1 - x2) != abs(y1 - y2))
    {
        return 0;
    }

    if (!(x1 != x2 && y1 == y2) && !(x1 == x2 && y1 != y2))
    {
        return 0;
    }
 
    if(x1 == x2 || y1 == y2)
    {
        return validateOrthogonalMove(x1, y1, x2, y2);
    }
    else if(x1 != x2 && y1 != y2)
    {
        return validateDiagonalMove(x1, y1, x2, y2);
    }

    return 1;
}

int validateKnightMove(int x1, int x2, int y1, int y2)
{
    wprintf(L"Knight piece is selected\n");

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
    wprintf(L"Pawn piece is selected\n");

    char selfColor = board[x1][y1][0];

    if (selfColor == 'w')
    {
        // must go up
        if (x1 < x2)
        {
            // wprintf(L"x1 must be bigger than x2\n");
            return 0;
        }
    }
    else
    {
        // (black) must go down
        if (x1 > x2)
        {
            // wprintf(L"x1 must be smaller than x2\n");
            return 0;
        }
    }

    int availablejumps = 1;
    if (countTurn == 0)
    {
        availablejumps = 2;
    }

    if (abs(x2 - x1) <= availablejumps && y1 == y2 && board[x2][y2][0] == 's')
    {
        return 1;
    }

    if (board[x2][y2][0] != 's' && !isFriendly(selfColor,x2, y2) &&  abs(y2 - y1) == 1 && abs(x2 - x1) ==1)
    {
        return 1;
    }

    return 0;
}

int validateOrthogonalMove(int x1, int y1, int x2, int y2)
{
    int xi = x1;
    int yi = y1;

    int dx = 0;
    if (x2-x1 > 0)
    {
        dx = 1;
    }
    else if(x1 - x2 > 0)
    {
        dx = -1;
    }

    int dy = 0;
    if (y2-y1 > 0)
    {
        dy = 1;
    }
     if (y1-y2 > 0)
    {
        dy = -1;
    }

    while((xi != x2 && dy == 0) || (yi != y2 && dx == 0))
    {
        xi = xi + dx;
        yi = yi + dy;

        if (xi == x2 && yi == y2)
        {
            break;
        }

        if (board[xi][yi][0] != 's')
        {
            return 0;
        }
    }

    return 1;
}

int validateDiagonalMove(int x1, int y1, int x2, int y2)
{
    int xi = x1;
    int yi = y1;

    int dx = -1;
    if (x2-x1 > 0)
    {
        dx = 1;
    }

    int dy = -1;
    if (y2-y1 > 0)
    {
        dy = 1;
    }

    while(xi != x2 && yi != y2)
    {
        xi = xi + dx;
        yi = yi + dy;

        if (xi == x2 && yi == y2)
        {
            break;
        }

        if (board[xi][yi][0] != 's')
        {
            return 0;
        }
    }

    return 1;
}

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

int PVP()
{
    while (1)
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
}

int PVC()
{
    return 0;
}

int saveBoard()
{
    FILE *fp;
    fp = fopen("board.save","w+");
    if(fp == NULL)
    {
        printf("Failed to save board\n");
        return 0;
    }
    for (int i = 0; i < 8; ++i) 
    {
        for (int j = 0; j < 8; ++j) 
        {
            fprintf(fp,"%s ",board[i][j]);
        }
        fprintf(fp,"\n");
    }
    fclose(fp);
}

int loadBoard(){
    FILE *fp;
    fp = fopen("board.save","r");
    if(fp == NULL)
    {
        printf("Failed to load board\n");
        return 0;
    }
    for (int i = 0; i < 8; ++i) 
    {
        for (int j = 0; j < 8; ++j) 
        {
            fscanf(fp,"%s",board[i][j]);
        }
    }
    fclose(fp);
}