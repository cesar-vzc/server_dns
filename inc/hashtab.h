#ifndef __HASHTAB_H__
#define __HASHTAB_H__

#include "fakedns.h"

struct hashtab {
    char addr[URL_SIZE];
    char ip[IP4_SIZE];
    int ttl;
    int timestamp;
    struct hashtab *next;
};

struct hashtab **xmalloc(int tabsize);
struct hashtab **cache_init(int tabsize);
int cache_search(const char *key, char *ip);
int cache_add(const char *addr, const char *ip, int ttl);
int cache_delete(const char *key);
int cache_free(void);
int hash_function(const char *str);

#endif /* ! __HASHTAB_H__ */
