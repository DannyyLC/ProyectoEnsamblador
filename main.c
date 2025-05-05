#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>  
#include <windows.h>

#define WIDTH 35
#define HEIGHT 15

// Variable global para el tiempo restante
int remainingTime = 0;
int gameRunning;

void initGame(char gameArea[HEIGHT][WIDTH], int *x, int *y, int numX, int remainingX);
void printGame(char gameArea[HEIGHT][WIDTH], int remainingX);
void setColor(int color);
void gotoxy(int x, int y);
extern void moveCharacter(int *x, int *y, char gameArea[HEIGHT][WIDTH], char input, int *remainingX);
void selectDifficulty(int *numX, int *time);
void printFinalMessage(int result);
DWORD WINAPI timerThread(LPVOID lpParam);

int main() {
    char gameArea[HEIGHT][WIDTH];
    int x, y, numX, time, op;  
    char input;

    do{
        gameRunning = 1;
        system("cls");
        selectDifficulty(&numX, &time);
        remainingTime = time;
        int remainingX = numX; 
    
        system("cls"); 
        initGame(gameArea, &x, &y, numX, remainingX);  
    
        // Iniciar el hilo del tiempo
        HANDLE hTimerThread = CreateThread(NULL, 0, timerThread, NULL, 0, NULL);
    
        while (gameRunning) {   
    
            if (_kbhit()) {  
                input = _getch();
                moveCharacter(&x, &y, gameArea, input, &remainingX);
                system("cls");
                printGame(gameArea, remainingX);
                if (!remainingX) {
                    printFinalMessage(1);     
                } 
                else if (remainingTime <= 0) {
                    printFinalMessage(0);
                }
            }
        }
    
        // Esperar a que termine el hilo del temporizador
        WaitForSingleObject(hTimerThread, INFINITE);
        CloseHandle(hTimerThread);
    
        setColor(7);  // Restaurar color normal
        printf("\n\nJuego terminado\n0. Salir\n1. Volver a iniciar\nSeleccion: ");
        fflush(stdin);
        scanf("%d", &op);
    }while(op != 0);

    return 0;
}

// Función para el hilo del temporizador
DWORD WINAPI timerThread(LPVOID lpParam) {
    while (remainingTime > 0 && gameRunning) {
        Sleep(1000);  // Esperar 1 segundo
        remainingTime--;
    }
    return 0;
}

// Inicializa el area de juego y coloca al jugador
void initGame(char gameArea[HEIGHT][WIDTH], int *x, int *y, int numX, int remainingX) {
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
    for (int i = 0; i < numX; i++) {
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

    printGame(gameArea, remainingX);
}

void printFinalMessage(int result){
    // Obtener el tamaño actual de la consola
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    
    // Calcular la posición de inicio para centrar el tablero
    int startX = (consoleWidth - WIDTH) / 2;
    int startY = (consoleHeight - HEIGHT) / 2;
   
    // Asegurar que no sea negativo
    if (startX < 0) startX = 0;
    if (startY < 0) startY = 0;
    
    int messageLenght = 20;
    int xCord = startX + (WIDTH - messageLenght) / 2;
    int yCord = startY + HEIGHT + 2;

    gotoxy(xCord, yCord);
    if (result){
        setColor(10);
        printf("Felicidades! Has ganado");
    }else{
        setColor(12);
        printf("Se acabo el tiempo!");
    }

    Sleep(3000);
    gameRunning = FALSE;
}

// Imprime el area de juego
void printGame(char gameArea[HEIGHT][WIDTH], int remainingX) 
{ 
    // Obtener el tamaño actual de la consola
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    
    // Calcular la posición de inicio para centrar el tablero
    int startX = (consoleWidth - WIDTH) / 2;
    int startY = (consoleHeight - HEIGHT) / 2;
   
    // Asegurar que no sea negativo
    if (startX < 0) startX = 0;
    if (startY < 0) startY = 0;

    // Calculamos la posicion del titulo
    int titleLength = 27;
    int titleX = startX + (WIDTH - titleLength) / 2;
    int titleY = startY - 5;    

    // Imprimir titulo del juego
    gotoxy(titleX, titleY);
    setColor(3);
    printf("Empuja las X por la salida!");

    //Calculamos la posicion del contador de X restantes
    int counterLength = 23;
    int counterX = startX + (WIDTH - counterLength) / 2;
    int counterY = startY - 3; 

    // Imprimir contador de x
    gotoxy(counterX, counterY);
    setColor(3);
    printf("Objetivos restantes: %d", remainingX);

    // Imprimir contador de tiempo
    gotoxy(counterX, counterY + 1);
    setColor(remainingTime <= 10 ? 12 : 3);  // Rojo si quedan 10 segundos o menos
    printf("Tiempo restante: %d s", remainingTime);

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

void selectDifficulty(int *numX, int *time){
    printf("Selecciona la dificultad!");
    printf("\n1. Facil");
    printf("\n2. Normal");
    printf("\n3. Heroico");
    printf("\n4. Legendario");
    printf("\nSeleccion: ");
    
    int selection;
    
    scanf("%d", &selection);

    switch (selection)
    {
    case 1:
        *numX = 6;
        *time = 60;
        break;

    case 2:
        *numX = 8;
        *time = 45;
        break; 

    case 3:
        *numX = 10;
        *time = 30;
        break; 

    case 4:
        *numX = 12;
        *time = 15;
        break; 

    default:
        *numX = 8;
        *time = 45;
        break;
    }
}

