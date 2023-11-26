#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

#define TRUE 1
#define FALSE 0
#define NORTH 90
#define WEST 0
#define EAST 180
#define SOUTH 270
#define MAX_LENGTH 100
#define ROW 30
#define COL 60
#define SPEED 900

int direction = WEST;
int segment[MAX_LENGTH][2] = {{COL / 2, ROW / 2}, {COL / 2 + 1, ROW / 2}, {COL / 2 + 2, ROW / 2}};
int foodPos[2] = {-1, -1};
int curentLength = 3;
int score = 0;

void init() {
    char temp[100];
    sprintf(temp,"mode con cols=%d lines=%d | title snake", COL, ROW);
    system(temp);
    
    CONSOLE_CURSOR_INFO curInfo;
    curInfo.dwSize = 1;
    curInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);

    srand(time(0));


}

enum ConsolColor{ BLACK, DARKBLUE, DARKGREEN,
    DARKCYAN, DARKRED, DARKMAGENTA, DARKYELLOW,
    GRAY, DARKGRAY, BLUE, GREEN, CYAN,
    RED, MAGENTA, YELLOW, WHITE, LIGHTGRAY = 7,
    ORIGINAL = 7, ORIGINALFONT = 7, ORIGINALBG = 0
}; 


void gotoxy(int x, int y) {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(consoleHandle, pos);
}

void setColor(int color) { 
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (info.wAttributes&0xf0) | (color &0xf));
}

void keyInput() {
    int ch = _kbhit();
    if (ch == 0) return;
    char c = _getch();
    //printf("%c",c);
    //c = _getch();
    //printf("%c",c);
    switch(c) {
        case 'w': case 'W':
            if (direction != SOUTH) direction = NORTH;
            break;
        case 'a': case 'A':
            if (direction != WEST) direction = EAST;
            break;
        case 's': case 'S':
            if (direction != NORTH) direction = SOUTH;
            break;
        case 'd': case 'D':
            if (direction != EAST) direction = WEST;
            break;
    }
}

void drawSnake() {
    setColor(GREEN);
    for (int i = curentLength - 1; i >= 0; i--) {
        gotoxy(segment[i][0], segment[i][1]);
        printf("#");
    }
}

void move(int dir) {
    for (int i = curentLength - 1; i > 0; i--) {
        int nx = segment[i-1][0], ny = segment[i-1][1];
        segment[i][0] = nx;
        segment[i][1] = ny;
    }
    switch (dir) {
        case NORTH:
            segment[0][1]--;
            break;
        case WEST:
            segment[0][0]++;
            break;
        case EAST:
            segment[0][0]--;
            break;
        case SOUTH:
            segment[0][1]++;
            break;
    }

}

void extendSnake() {
    int x = segment[curentLength-1][0], y = segment[curentLength-1][1];
    segment[curentLength][0] = x;
    segment[curentLength][1] = y;
}

void drawFood() {
    setColor(RED);
    gotoxy(foodPos[0], foodPos[1]);
    printf("@");
}

void food() {
    int x = rand() % (COL - 1) + 1;
    int y = rand() % (ROW - 1) + 1;
    foodPos[0] = x;
    foodPos[1] = y;
    drawFood();
}

void foodDetection() {
    if (segment[0][0] == foodPos[0] && segment[0][1] == foodPos[1]) {
        score++;
        curentLength++;
        extendSnake();
        food();
    }
}

void drawScore() {
    setColor(WHITE);
    gotoxy(0, 0);
    printf("score : %3d",score);
}


void drawGame()
{
    drawSnake();
    drawFood();
    drawScore();
}


void initSnake() {
    drawSnake();
    food();
}

int isGameOver() {
    
    
    
    if (segment[0][0] < 0 || segment[0][0] > COL || segment[0][1] < 0 || segment[0][1] > ROW) return TRUE;
    return FALSE;
}

void game() {
    init();
    initSnake();
    while (TRUE) {
        Sleep(1000 - SPEED);
        system("cls");
        keyInput();
        move(direction);
        foodDetection();
        drawGame();
        if (isGameOver()) {
            break;
        }
    }
    drawGame();
    gotoxy(COL / 2 - 6, ROW / 2);
    setColor(DARKRED);
    printf("GAME OVER");
    _getch();
}



int main() {
    init();
    initSnake();
    game();
}
