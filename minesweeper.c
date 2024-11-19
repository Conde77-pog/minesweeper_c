#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

int board_width;
int board_height;
int number_of_bombs;

#define ANSI_COLOR_RED      "\x1b[31m"
#define ANSI_COLOR_GRAY     "\e[0;37m"
#define ANSI_COLOR_GREEN    "\x1b[32m"
#define ANSI_COLOR_BLUE     "\x1b[34m"

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
            Board[i][j].hidden = true;
            Board[i][j].number_of_bombs = 0;
        }
    }
}


void PlaceBombs(struct Cell Board[board_width][board_height], int number_of_bombs) {

    int placed_bombs = 0;


    while (placed_bombs < number_of_bombs) {

        int x = rand() % board_width;
        int y = rand() % board_height;

        if (!Board[x][y].bomb) {
            Board[x][y].bomb = true; 
            placed_bombs++;
        }
    }
}


void Check_for_Bomb(struct Cell Board[board_width][board_height]) {
    for (int i = 0; i < board_width; i++) {
        for (int j = 0; j < board_height; j++) {
            Board[i][j].number_of_bombs = 0;

            if (Board[i][j].bomb) {
                continue;
            }

            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    int ni = i + dx;
                    int nj = j + dy;

                    if (ni >= 0 && ni < board_width && nj >= 0 && nj < board_height) {
                        if (Board[ni][nj].bomb) {
                            Board[i][j].number_of_bombs++;
                        }
                    }
                }
            }
        }
    }
}


void PrintBoard(struct Cell Board[board_width][board_height]) {
    printf("  ");
    for(int i = 0; i < board_width; i++){printf("%d  ", i);}
    printf("\n");
    for (int i = 0; i < board_width; i++) {
        printf("%d ",i);
        for (int j = 0; j < board_height; j++) {
            if (Board[i][j].hidden) {
                printf("X  ");  // Célula escondida
            }
            else if (Board[i][j].bomb) {
                printf(ANSI_COLOR_RED "B  " ANSI_COLOR_GRAY);  // Célula com bomba
            }
            else {
                if (Board[i][j].number_of_bombs == 0) {
                    printf(ANSI_COLOR_BLUE "%d  " ANSI_COLOR_GRAY, Board[i][j].number_of_bombs);  // Sem bombas ao redor
                } else {
                    printf(ANSI_COLOR_GREEN "%d  " ANSI_COLOR_GRAY, Board[i][j].number_of_bombs);  // Número de bombas ao redor
                }
            }
        }
        printf("\n");
    }
}


void RevealCell(struct Cell Board[board_width][board_height], int x, int y)
{
    if (x < 0 || x >= board_width || y < 0 || y >= board_height || !Board[x][y].hidden) {
        return;
    }

    Board[x][y].hidden = false;

    if (Board[x][y].number_of_bombs == 0) {
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx == 0 && dy == 0) continue;

                RevealCell(Board, x + dx, y + dy);
            }
        }
    }
}

void Play(struct Cell Board[board_width][board_height], int x, int y)
{
    if(Board[x][y].bomb)
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
    else{
        RevealCell(Board,x,y);
    }
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    int x, y;

    if(argc != 4)
    {
        printf("./mine <x> <y> <number of bombs>");
    }


    board_width =atoi(argv[1]);
    board_height = atoi(argv[2]);
    number_of_bombs = atoi(argv[3]);

    struct Cell Board[board_width][board_height];

    InitializeBoard(Board);
    PlaceBombs(Board, number_of_bombs);
    Check_for_Bomb(Board);
    PrintBoard(Board);

    while (true)
    {
        printf("Escolha uma posição (y x):\n");
        scanf("%d %d", &x, &y);

        if (x < 0 || x >= board_width || y < 0 || y >= board_height){
            printf("Posição inválida. Tente novamente.\n");
            continue;
        }

        Play(Board, x, y);
        PrintBoard(Board);
    }

    return 0;
}