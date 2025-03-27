#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>  
#include <windows.h>

#define WIDTH 35
#define HEIGHT 15
#define NUM_X 8
#define SCREEN_WIDTH 172
#define SCREEN_HEIGHT 20

void initGame(char gameArea[HEIGHT][WIDTH], int *x, int *y);
void printGame(char gameArea[HEIGHT][WIDTH]);
void setColor(int color);
void gotoxy(int x, int y);
extern void moveCharacter(int *x, int *y, char gameArea[HEIGHT][WIDTH], char input);  

int main() {
    char gameArea[HEIGHT][WIDTH];
    int x, y;  
    char input;
    system("cls"); 
    initGame(gameArea, &x, &y);  

    while (1) {   

        if (_kbhit()) {  
            input = _getch();
            printf("%c", input);
            moveCharacter(&x, &y, gameArea, input);
            system("cls"); 
            printGame(gameArea); 
        }
    }

    return 0;
}

// Inicializa el area de juego y coloca al jugador
void initGame(char gameArea[HEIGHT][WIDTH], int *x, int *y) {
    // Llenar con espacios
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            gameArea[i][j] = ' ';
        }
    }

    // Dibujar bordes
    for (int i = 0; i < WIDTH; i++) {
        gameArea[0][i] = '#';
        gameArea[HEIGHT - 1][i] = '#';
    }
    for (int i = 0; i < HEIGHT; i++) {
        gameArea[i][0] = '#';
        gameArea[i][WIDTH - 1] = '#';
    }

    // Colocar los 'P' en posiciones fijas en los bordes
    gameArea[0][5] = 'P';  
    gameArea[HEIGHT - 1][15] = 'P'; 
    gameArea[7][0] = 'P';  
    gameArea[3][WIDTH - 1] = 'P'; 

    // Colocar 'X' en posiciones aleatorias dentro del area de juego
    srand(time(NULL));
    for (int i = 0; i < NUM_X; i++) {
        int x, y;
        do {
            x = rand() % (WIDTH - 2) + 1;
            y = rand() % (HEIGHT - 2) + 1;
        } while (gameArea[y][x] != ' ');
        gameArea[y][x] = 'X';
    }

    // Colocar al jugador 'C' en el centro
    *x = WIDTH / 2;
    *y = HEIGHT / 2;
    gameArea[*y][*x] = 'C';

    printGame(gameArea);
}

// Imprime el area de juego
void printGame(char gameArea[HEIGHT][WIDTH]) 
{ 
    int startX = (SCREEN_WIDTH - WIDTH) / 2;
    int startY = (SCREEN_HEIGHT - HEIGHT) / 2;

    // Titulo del juego
    gotoxy(SCREEN_WIDTH / 2 - 14, startY - 2);
    setColor(3);
    printf("Empuja las X por la salida!");

    for (int i = 0; i < HEIGHT; i++) 
    {
        gotoxy(startX, startY + i);
        for (int j = 0; j < WIDTH; j++) 
        {
            if (gameArea[i][j] == '#')
                setColor(13);
            else if (gameArea[i][j] == 'C')
                setColor(2);
            else if (gameArea[i][j] == 'X')
                setColor(4);
            else if(gameArea[i][j] == 'P')
                setColor(6);
            else
                setColor(7);

            printf("%c", gameArea[i][j]);
        }
        printf("\n");
    }
}

// Seleccionar el color con el cual se imprimira el caracter
void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

// Mueve el cursor a una posición específica
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
