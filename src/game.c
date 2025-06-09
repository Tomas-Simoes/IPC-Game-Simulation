#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "../include/game.h"
#include <stdbool.h>

// ~ -> Water
// AC -> Aircraft carrier [5]
// BS -> Battleship [4]
// C -> Cruiser [3]
// S -> Submarine [3]
// D -> Destroyer [2]

// Rules:
// Each player chooses the places wehre they want to set the different ships.
// . The ships cant be setted in a diagonal way, only vertically or horizontally

int letter_to_num(char letter) {
    if (letter >= 'a' && letter <= 'j') {
        letter -= 32;
    }
    if (letter >= 'A' && letter <= 'J') {
        return letter - 'A'; 
    }
    return -1;
}

void initShips(struct Ship ships[SHIP_COUNT]) {
    struct Ship temp[SHIP_COUNT] = {
        {"Aircraft Carrier", 5, -1, -1, 0, 5, 'A'},
        {"Battleship", 4, -1, -1, 0, 4, 'B'},
        {"Cruiser", 3, -1, -1, 0, 3, 'C'},
        {"Submarine", 3, -1, -1, 0, 3, 'S'},
        {"Destroyer", 2, -1, -1, 0, 2, 'D'}
    };
    for (int i = 0; i < SHIP_COUNT; i++) {
        ships[i] = temp[i];
    }
}

int canPlaceShip(char board[BOARD_SIZE][BOARD_SIZE], int row, int col, int size, int orientation) {
    if (orientation == 0) {
        if (col + size > BOARD_SIZE) return 0;
        for (int i = 0; i < size; i++) {
            if (board[row][col + i] != '~') return 0;
        }
    } else {
        if (row + size > BOARD_SIZE) return 0;
        for (int i = 0; i < size; i++) {
            if (board[row + i][col] != '~') return 0;
        }
    }
    return 1;
}

void initializeBoard(char board[BOARD_SIZE][BOARD_SIZE]) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = '~';  
        }
    }
}

void printBoard(char board[BOARD_SIZE][BOARD_SIZE]) {
    printf("  ");
    for (int i = 0; i < BOARD_SIZE; i++)
        printf("%d ", i + 1);
    printf("\n");

    for (int i = 0; i < BOARD_SIZE; i++) {
        printf("%c ", 'A' + i);
        for (int j = 0; j < BOARD_SIZE; j++) {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
}

void placeShipOnBoard(char board[BOARD_SIZE][BOARD_SIZE], int row, int col, int size, int orientation, char name) {
    for (int i = 0; i < size; i++) {
        if (orientation == 0) {
            board[row][col + i] = name;
        } else {
            board[row + i][col] = name;
        }
    }
}

void placeShips(char board[BOARD_SIZE][BOARD_SIZE], struct Ship ships[SHIP_COUNT]) {
    char dir;
    char pos[10];
    int row, col;
    int orientation;

    printf("Please set the positions of each ship.\n");

    for (int i = 0; i < SHIP_COUNT; i++) {
        bool valid = false;
        while (!valid) {
            printf("--> Placing %s which has size %d.\n", ships[i].name, ships[i].size);
            printf("Enter the position (A1...J10): ");
            scanf("%s", pos);
            row = letter_to_num(pos[0]);
            col = atoi(&pos[1]) - 1;

            if (row == -1 || col < 0 || col >= BOARD_SIZE || row >= BOARD_SIZE) {
                printf("Invalid Position.\n");
                continue;
            }

            printf("Enter the orientation (H - Horizontally, V - Vertically): ");
            scanf(" %c", &dir);

            orientation = (dir == 'H' || dir == 'h') ? 0 : 1;

            if (!canPlaceShip(board, row, col, ships[i].size, orientation)) {
                printf("You can't place %s in those coordinates.\n", ships[i].name);
                continue;
            }

            ships[i].row = row;
            ships[i].collumn = col;
            ships[i].orientation = orientation;

            placeShipOnBoard(board, row, col, ships[i].size, orientation, ships[i].abrv);
            printf("\n");
            printBoard(board);
            valid = true;
        }
    }
}


int checkAllShips(struct Ship ships[SHIP_COUNT]) {
    for (int i = 0; i < SHIP_COUNT; i++) {
        if(ships[i].life > 0) {
            return 1; 
        }
    }
    return 0;  
}

// 0 -> Miss
// 1 -> Hit
// 2 -> Sunk a ship
// 3 -> Sunk

int damageShip(struct Ship ships[SHIP_COUNT], int index) {
    ships[index].life--;
    
    if (ships[index].life <= 0) {
        ships[index].life = 0;
        if (checkAllShips(ships) == false) {
            return 3; 
        }
        return 2;
    } 
    
    return 1;  // Hit
}

// A
// B
// C
// S
// D

int handleAttack(char board[BOARD_SIZE][BOARD_SIZE], int row, int collumn, struct Ship ships[SHIP_COUNT]) {
    char letter = board[row][collumn];
    int attackedShipIndex;
    int result;
    board[row][collumn] = '~';
    switch(letter) {
        case '~':
            return 0;
            break;
        case 'A':
            attackedShipIndex = 0;
            result = damageShip(ships, attackedShipIndex);
            return result;
            break;
        case 'B':
            attackedShipIndex = 1;
            result = damageShip(ships, attackedShipIndex);
            return result;
            break;
        case 'C':
            attackedShipIndex = 2;
            result = damageShip(ships, attackedShipIndex);
            return result;
            break;
        case 'S':
            attackedShipIndex = 3;
            result = damageShip(ships, attackedShipIndex);
            return result;
            break;
        case 'D':
            attackedShipIndex = 3;
            result = damageShip(ships, attackedShipIndex);
            return result;
            break;
    }
    return -1;
}