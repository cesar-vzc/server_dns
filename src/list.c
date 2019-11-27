#include "../inc/fakedns.h"
#include "../inc/server.h"
#include "../inc/command.h"
#include "../inc/hashtab.h"
#include "../inc/list.h"
#include "../inc/utils.h"

void list_print(struct hashtab *hashtab)
{
    struct hashtab *tmp = hashtab;

    while (tmp != NULL) {
	printf("%s / %s -> ", tmp->addr, tmp->ip);
	tmp = tmp->next;
    }
    printf("NULL\n\n");
}

int list_push(struct hashtab **hashtab, const char *addr, const char *ip)
{
    int bool = 0;
    int hash = 0;

    hash = hash_function(addr);

    struct hashtab *tmp = NULL;
    struct hashtab *new = NULL;

    if (hashtab[hash]->ip[0] != '\0' && hashtab[hash]->addr[0] != '\0') {
	tmp = hashtab[hash];
	while (tmp->next != NULL)
	    tmp = tmp->next;
	new = malloc(sizeof(struct hashtab));
	memset(new, 0, sizeof(struct hashtab));
	tmp->next = new;
	strncpy(new->ip, ip, IP4_SIZE);
	strncpy(new->addr, addr, URL_SIZE);
    } else {
	strcpy(hashtab[hash]->ip, ip);
	strcpy(hashtab[hash]->addr, addr);
    }

    hashtab[hash]->timestamp = time(NULL);

    if (bool == 0)
	return (0);
    else
	return (-1);
}

int list_del_by_data(struct hashtab **hashtab, const char *addr)
{
  int bool = 0;
  int hash = 0;
  struct hashtab *tmp = NULL;
  struct hashtab *tmp2 = NULL;

  hash = hash_function(addr);

  tmp = hashtab[hash];
  tmp2 = hashtab[hash];

  if (strcmp(addr, hashtab[hash]->addr) == 0) {
      hashtab[hash] = hashtab[hash]->next;
      free(tmp);
      return (0);
  }

  while (tmp != NULL) {
    if (strcmp(addr, tmp->addr) == 0) {
      tmp2->next = tmp->next;
      free(tmp);
      bool = 0;
      break;
    }
    bool = 1;
    tmp2 = tmp;
    tmp = tmp->next;
  }

  if (bool == 0)
    return (0);
  else
    return (-1);
}
