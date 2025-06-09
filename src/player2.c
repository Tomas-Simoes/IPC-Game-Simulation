#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <errno.h>
#include "common.h"
#include "game.h"
#include <stdbool.h>

char board[BOARD_SIZE][BOARD_SIZE];
struct Ship ships[SHIP_COUNT];

int main() {
    int id = msgget(MSG_KEY, 0666 | IPC_CREAT);
    if (id == -1) {
        perror("msgget failed");
        exit(1);
    }
    
    struct message m;
    initializeBoard(board);
    initShips(ships);
    placeShips(board, ships);
    
    printf("--> PLAYER 2 - Battleship <--\n");
    printf("Your board:\n");
    printBoard(board);
    printf("Waiting for Player 1 to make the first move...\n\n");

    while (1) {
        // gets player move
        if (msgrcv(id, &m, sizeof(m.text), 1, 0) == -1) {
            perror("msgrcv failed");
            continue;
        }

        if (strcmp(m.text, "exit") == 0) {
            printf("Player 1 has left the game!\n");
            break;
        }

        printf("Player 1 attacked: %s\n", m.text);

        // handles player 1 attack
        int row = letter_to_num(m.text[0]);
        int col = atoi(&m.text[1]) - 1;

        int result = handleAttack(board, row, col, ships);

        // handles answers
        m.type = 2;
        switch(result) {
            case 0:
                sprintf(m.text, "Miss on %c%d", m.text[0], col + 1);
                break;
            case 1:
                sprintf(m.text, "Hit on %c%d", m.text[0], col + 1);
                break;
            case 2:
                sprintf(m.text, "Hit and sank ship on %c%d", m.text[0], col + 1);
                break;
            case 3:
                strcpy(m.text, "Win");
                break;
        }

        // sends the result to player 1
        if (msgsnd(id, &m, strlen(m.text) + 1, 0) == -1) {
            perror("msgsnd failed");
            continue;
        }

        if (result == 3) {
            printf("Player 1 wins! All your ships have been sunk.\n");
            break;
        }

        printf("Your board after attack:\n");
        printBoard(board);

        printf("\nYour turn!\n");
        printf("Your move: ");
        if (scanf("%99s", m.text) != 1) {
            printf("Input error\n");
            continue;
        }

        if (strcmp(m.text, "exit") == 0) {
            m.type = 2;
            msgsnd(id, &m, strlen(m.text) + 1, 0);
            break;
        }

        // send to player 1
        m.type = 2;
        if (msgsnd(id, &m, strlen(m.text) + 1, 0) == -1) {
            perror("msgsnd failed");
            continue;
        }

        
        if (msgrcv(id, &m, sizeof(m.text), 1, 0) == -1) {
            perror("msgrcv failed");
            continue;
        }

        printf("Result: %s\n", m.text);
        
        if (strcmp(m.text, "Win") == 0) {
            printf("You win! All opponent ships sunk.\n");
            break;
        }

        printf("\nWaiting for Player 1's move...\n");

        printf("\n");
    }

    printf("Game ended.\n");
    return 0;
}