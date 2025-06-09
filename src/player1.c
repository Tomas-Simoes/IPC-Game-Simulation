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

    struct message temp_msg;
    while (msgrcv(id, &temp_msg, sizeof(temp_msg.text), 0, IPC_NOWAIT) != -1) {
    }
    
    struct message m;
    memset(&m, 0, sizeof(m));
    initializeBoard(board);
    initShips(ships);
    placeShips(board, ships);
    
    printf("--> PLAYER 1 - Battleship <--\n");
    printf("Your board:\n");
    printBoard(board);
    printf("You make the first move!\n");
    printf("Enter moves like 'A1', 'B4', etc. Type 'exit' to quit.\n\n");

    while (1) {
        printf("Your move: ");
        if (scanf("%99s", m.text) != 1) {
            printf("Input error\n");
            continue;
        }

        if (strcmp(m.text, "exit") == 0) {
            m.type = 1;
            msgsnd(id, &m, strlen(m.text) + 1, 0);
            break;
        }

        // send to player 2
        m.type = 1;
        if (msgsnd(id, &m, strlen(m.text) + 1, 0) == -1) {
            perror("msgsnd failed");
            continue;
        }
        
        // gets result from player 2
        if (msgrcv(id, &m, sizeof(m.text), 2, 0) == -1) {
            perror("msgrcv failed");
            continue;
        }

        printf("Result: %s\n", m.text);
        
        if (strcmp(m.text, "Win") == 0) {
            printf("You win! All opponent ships sunk.\n");
            break;
        }

        printf("\nWaiting for Player 2's move...\n");
        
        // gets player 2 move
        if (msgrcv(id, &m, sizeof(m.text), 2, 0) == -1) {
            perror("msgrcv failed");
            continue;
        }

        if (strcmp(m.text, "exit") == 0) {
            printf("Player 2 has left the game!\n");
            break;
        }

        printf("Player 2 attacked: %s\n", m.text);

        // handle attack from player 2
        int row = letter_to_num(m.text[0]);
        int col = atoi(&m.text[1]) - 1;

        int result = handleAttack(board, row, col, ships);

        m.type = 1;
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

        // send result to player 2
        if (msgsnd(id, &m, strlen(m.text) + 1, 0) == -1) {
            perror("msgsnd failed");
            continue;
        }

        if (result == 3) {
            printf("Player 2 wins! All your ships have been sunk.\n");
            break;
        }

        printf("Your board after attack:\n");
        printBoard(board);
        printf("\n");
    }

    printf("Game ended.\n");
    msgctl(id, IPC_RMID, NULL);
    return 0;
}