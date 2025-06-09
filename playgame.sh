#!/bin/bash

# Config
MSG_KEY=1234
BOARD_SIZE=10
LETTERS=(A B C D E F G H I J)

# Clean previous message queue if any
ipcrm -Q $MSG_KEY 2>/dev/null

# Create message queue
ipcmk -Q -k $MSG_KEY >/dev/null

# Send function (to msg queue)
send_msg() {
    local type=$1
    local text=$2
    echo "$text" | msgsnd -q $MSG_KEY -t $type
}

# Receive function (from msg queue)
recv_msg() {
    local type=$1
    msgrcv -q $MSG_KEY -t $type
}

# Emulate Player 1 (attacker)
player1() {
    echo "[Player 1] Starting"
    local i=0
    for row in "${LETTERS[@]}"; do
        for col in $(seq 1 $BOARD_SIZE); do
            move="$row$col"
            echo "[Player 1] Attacking: $move"
            send_msg 1 "$move"
            recv_msg 2  # get result from Player 2
            result=$(recv_msg 2)
            echo "[Player 1] Got result: $result"

            if [[ "$result" == "Win" ]]; then
                echo "[Player 1] Victory!"
                return
            fi

            # Simulate waiting for Player 2 move
            recv_msg 2  # get their move
            echo "[Player 1] Received move from Player 2"
            send_msg 1 "Miss on A1"  # always miss for P2
        done
    done
}

# Emulate Player 2 (defender)
player2() {
    echo "[Player 2] Starting"
    total_hits=0
    win_threshold=1  # After this many moves, declare P1 won

    while true; do
        move=$(recv_msg 1)
        echo "[Player 2] Received attack: $move"
        total_hits=$((total_hits + 1))

        if [[ $total_hits -eq $win_threshold ]]; then
            send_msg 2 "Win"
            echo "[Player 2] Sending Win"
            return
        else
            send_msg 2 "Miss on $move"
        fi

        # Now simulate P2's move (we just send same move always)
        send_msg 2 "A1"
        recv_msg 1  # receive result from Player 1
    done
}

# Run both players in parallel
player2 &
P2_PID=$!
sleep 1
player1

# Cleanup
wait $P2_PID
ipcrm -Q $MSG_KEY
echo "Game over."
