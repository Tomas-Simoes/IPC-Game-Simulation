#ifndef GAME_H
#define GAME_H

#define BOARD_SIZE 10
#define SHIP_COUNT 5

struct Ship {
    char name[20];
    int size;
    int row;
    int collumn;
    int orientation; // 0 -> Horizontal / 1 -> Vertical
    int life;
    char abrv;
};

int letter_to_num(char letter);
void initShips(struct Ship ships[SHIP_COUNT]);
int canPlaceShip(char board[BOARD_SIZE][BOARD_SIZE], int row, int collumn, int size, int orientation);
void initializeBoard(char board[BOARD_SIZE][BOARD_SIZE]);
void printBoard(char board[BOARD_SIZE][BOARD_SIZE]);
void placeShipOnBoard(char board[BOARD_SIZE][BOARD_SIZE], int row, int collumn, int size, int orientation, char name);
void placeShips(char board[BOARD_SIZE][BOARD_SIZE], struct Ship ships[SHIP_COUNT]);
int handleAttack(char board[BOARD_SIZE][BOARD_SIZE], int row, int collumn, struct Ship ships[SHIP_COUNT]);

#endif