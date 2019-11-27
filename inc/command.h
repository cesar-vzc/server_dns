#ifndef __COMMAND_H__
#define __COMMAND_H__

#include "hashtab.h"

void *client_handler(void *ans);
void quit(void);
void *get(struct client *client, char *buffer);
void *put(struct client *client, char *buffer);

#endif /* ! __COMMAND_H_ */
