#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

int board_width;
int board_height;
int number_of_bombs;

struct Cell {
    bool bomb;
    bool hidden;
    int number_of_bombs;
};

void InitializeBoard(struct Cell Board[board_width][board_height])
{
    for (int i = 0; i < board_width; i++)
    {
        for (int j = 0; j < board_height; j++)
        {
            Board[i][j].bomb = false;
            Board[i][j].hidden = true;
            Board[i][j].number_of_bombs = 0;
        }
    }
}


void PlaceBombs(struct Cell Board[board_width][board_height], int number_of_bombs)
{
    while (number_of_bombs > 0)
    {
        int x = rand() % board_width;
        int y = rand() % board_height;


        if (!Board[x][y].bomb)
        {
            Board[x][y].bomb = true;
            number_of_bombs--;
        }
    }
}

void PrintBoard(struct Cell Board[board_width][board_height])
{
    for (int i = 0; i < board_width; i++)
    {
        for (int j = 0; j < board_height; j++)
        {
            if (Board[i][j].hidden) {printf("X  ");}
            else if (Board[i][j].bomb) {printf("B  ");}
            else {printf("%d  ", Board[i][j].number_of_bombs);}
        }
        printf("\n");
    }
}

void Play(struct Cell Board[board_width][board_height], int x, int y)
{
    if (Board[x][y].bomb)
    {
        printf("Você perdeu!\n");
        for(int i = 0; i < board_width; i++)
        {
            for(int j = 0; j < board_height; j++)
            {
                if(Board[i][j].bomb == true) {Board[i][j].hidden = false;}
            }
        }
        PrintBoard(Board);
        exit(0);
    }
    else{Board[x][y].hidden = false;}
}

int main()
{
    srand(time(NULL));
    int x, y;

    board_width = 9;
    board_height = 9;
    number_of_bombs = 10;

    bool End_game = false;

    struct Cell Board[board_width][board_height];

    InitializeBoard(Board);

    PlaceBombs(Board, number_of_bombs);

    PrintBoard(Board);

    while (!End_game)
    {

        printf("Escolha uma posição (x y):\n");
        scanf("%d %d", &x, &y);

        if (x < 0 || x >= board_width || y < 0 || y >= board_height)
        {
            printf("Posição inválida. Tente novamente.\n");
            continue;
        }

        Play(Board, x, y);
        PrintBoard(Board);
    }

    return 0;
}