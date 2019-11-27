#ifndef __SERVER_H__
#define __SERVER_H__

#include "fakedns.h"

struct answer {
    int sock;
    int max;
    int conn;
    int count;
    int count_max;
    pthread_mutex_t mutex_conn;
    pthread_mutex_t mutex_tab;
};

struct client {
    struct answer *answer;
    char buffer[512];
    struct sockaddr_in cli;
    socklen_t src;
};

void INThandler(int i);
int server(int pot, int max_conn);

#endif /* ! __SERVER_H__ */
