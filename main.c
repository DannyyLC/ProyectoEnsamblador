#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>  

#define WIDTH 35
#define HEIGHT 15
#define NUM_X 8

void initGame(char gameArea[HEIGHT][WIDTH], int *x, int *y);
void printGame(char gameArea[HEIGHT][WIDTH]);
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
void printGame(char gameArea[HEIGHT][WIDTH]) { 
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            printf("%c", gameArea[i][j]);
        }
        printf("\n");
    }
}
