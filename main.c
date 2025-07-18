#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>  
#include <windows.h>
#include <string.h>

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
void drawRotatingDonut(int centerX, int centerY, int frame);
DWORD WINAPI donutAnimationThread(LPVOID lpParam);

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
        
        // Posicionar el mensaje a la izquierda del tablero de juego
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        int consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        
        // Calcular la posición del tablero (igual que en printGame)
        int boardStartX = (consoleWidth - WIDTH) / 2;
        int boardStartY = (consoleHeight - HEIGHT) / 2;
        
        if (boardStartX < 0) boardStartX = 0;
        if (boardStartY < 0) boardStartY = 0;
        
        // Posicionar el mensaje a la izquierda del tablero, centrado verticalmente
        int messageX = boardStartX - 25; // 25 caracteres a la izquierda del tablero
        int messageY = boardStartY + HEIGHT / 2 - 2; // Centrado verticalmente en el tablero
        
        // Asegurar que no se salga de los límites
        if (messageX < 0) messageX = 2;
        if (messageY < 0) messageY = 2;
        
        gotoxy(messageX, messageY);
        setColor(14); // Amarillo para el título
        printf("Juego terminado");
        
        gotoxy(messageX, messageY + 2);
        setColor(12); // Rojo para salir
        printf("0. Salir");
        
        gotoxy(messageX, messageY + 3);
        setColor(10); // Verde para continuar
        printf("1. Volver a jugar");
        
        gotoxy(messageX, messageY + 5);
        setColor(7); // Blanco para la selección
        printf("Seleccion: ");
        
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
            x = rand() % (WIDTH - 4) + 2;
            y = rand() % (HEIGHT - 4) + 2;
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

// Variables globales para la animacion de la dona
int donutCenterX, donutCenterY;
int donutFrame = 0;
int animationRunning = 0;

// Función para dibujar la dona giratoria
void drawRotatingDonut(int centerX, int centerY, int frame) {
    const char* donutFrames[][9] = {
        {
            "    @@@@@@    ",
            "   @@    @@   ",
            "  @@      @@  ",
            " @@        @@ ",
            " @@        @@ ",
            " @@        @@ ",
            "  @@      @@  ",
            "   @@    @@   ",
            "    @@@@@@    "
        },
        {
            "    ######    ",
            "   ##    ##   ",
            "  ##      ##  ",
            " ##        ## ",
            " ##        ## ",
            " ##        ## ",
            "  ##      ##  ",
            "   ##    ##   ",
            "    ######    "
        },
        {
            "    ******    ",
            "   **    **   ",
            "  **      **  ",
            " **        ** ",
            " **        ** ",
            " **        ** ",
            "  **      **  ",
            "   **    **   ",
            "    ******    "
        },
        {
            "    oooooo    ",
            "   oo    oo   ",
            "  oo      oo  ",
            " oo        oo ",
            " oo        oo ",
            " oo        oo ",
            "  oo      oo  ",
            "   oo    oo   ",
            "    oooooo    "
        }
    };
    
    const int colors[] = {12, 14, 10, 11}; // Rojo, Amarillo, Verde, Cyan
    
    // Limpiar el área de la dona anterior
    for (int i = 0; i < 9; i++) {
        gotoxy(centerX - 7, centerY - 4 + i);
        printf("               "); // 15 espacios para limpiar
    }
    
    // Dibujar la nueva dona
    setColor(colors[frame % 4]);
    for (int i = 0; i < 9; i++) {
        gotoxy(centerX - 7, centerY - 4 + i);
        printf("%s", donutFrames[frame % 4][i]);
    }
}

// Hilo para animar la dona
DWORD WINAPI donutAnimationThread(LPVOID lpParam) {
    while (animationRunning) {
        drawRotatingDonut(donutCenterX, donutCenterY, donutFrame);
        donutFrame++;
        Sleep(300); // Cambiar frame cada 300ms
    }
    return 0;
}

void selectDifficulty(int *numX, int *time){
    // Limpiar pantalla
    system("cls");
    
    // Obtener el tamaño actual de la consola
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    
    // Calcular la posición de inicio para centrar el menú
    int menuWidth = 25; // Ancho aproximado del menú
    int menuHeight = 6; // Altura del menú (6 líneas)
    int startX = (consoleWidth - menuWidth) / 2;
    int startY = (consoleHeight - menuHeight) / 2 - 6; // Mover más hacia arriba
    
    // Asegurar que no sea negativo
    if (startX < 0) startX = 0;
    if (startY < 0) startY = 3;
    
    // Dibujar el menú primero
    gotoxy(startX, startY);
    setColor(11); // Color cyan brillante
    printf("Selecciona la dificultad!");
    
    gotoxy(startX, startY + 2);
    setColor(10); // Color verde
    printf("1. Facil");
    
    gotoxy(startX, startY + 3);
    setColor(14); // Color amarillo
    printf("2. Normal");
    
    gotoxy(startX, startY + 4);
    setColor(12); // Color rojo
    printf("3. Heroico");
    
    gotoxy(startX, startY + 5);
    setColor(13); // Color magenta
    printf("4. Legendario");
    
    // Configurar la animación de la dona (bien separada del menú)
    donutCenterX = consoleWidth / 2;
    donutCenterY = startY + menuHeight + 8; // Más separación del menú
    animationRunning = 1;
    donutFrame = 0;
    
    // Iniciar el hilo de animación
    HANDLE hDonutThread = CreateThread(NULL, 0, donutAnimationThread, NULL, 0, NULL);
    
    // Esperar un poco para que la dona aparezca
    Sleep(100);
    
    gotoxy(startX, startY + 7);
    setColor(7); // Color blanco
    printf("Seleccion: ");
    
    int selection;
    scanf("%d", &selection);
    
    // Detener la animación
    animationRunning = 0;
    WaitForSingleObject(hDonutThread, INFINITE);
    CloseHandle(hDonutThread);

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
    
    setColor(7); // Restaurar color normal
}