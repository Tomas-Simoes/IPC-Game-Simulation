#ifndef COMMON_H
#define COMMON_H


#define MSG_KEY 1234

struct message {
    long type;           // message type
    char text[100];      // message content
};

#endif