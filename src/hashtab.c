#include "../inc/fakedns.h"
#include "../inc/server.h"
#include "../inc/command.h"
#include "../inc/hashtab.h"
#include "../inc/list.h"
#include "../inc/utils.h"

static struct hashtab **hashtab = NULL;

int hash_function(const char *str)
{
    int hash = 0;

    for (int i = 0; str[i] != '\0'; i++)
	hash += str[i];
    hash %= TAB_SIZE;

    return (hash);
}

struct hashtab **xmalloc(int tabsize)
{
    if (tabsize == 0) {
	puts("Erreur: la taille du tableau n'est pas valide.\n");
	exit(87);
    }

    hashtab = malloc(tabsize * sizeof(struct hashtab *));

    if (hashtab != NULL) {
	return (hashtab);
    } else {
	puts("Erreur: allocation mémoire impossible.\n");
	exit(87);
    }
}

struct hashtab **cache_init(int tabsize)
{
    struct hashtab **tab = NULL;

    tab = xmalloc(tabsize);
    for (int i = 0; i < tabsize; i++) {
	tab[i] = malloc(sizeof(struct hashtab));
	tab[i]->ip[0] = 0;
	tab[i]->addr[0] = 0;
	tab[i]->timestamp = 0;
	tab[i]->next = NULL;
    }

    return(tab);
}

int cache_free(void)
{
    if (hashtab == NULL)
	return (0);

    for (int i = 0; i < TAB_SIZE; i++) {
	struct hashtab *tmp = hashtab[i];
	while (hashtab[i] != NULL) {
	    tmp = hashtab[i];
	    hashtab[i] = hashtab[i]->next;
	    free(tmp);
	}
    }
    free(hashtab);
    puts("Tableau free.\n");
    hashtab = NULL;
    return (0);
}

int cache_add(const char *addr, const char *ip, int ttl)
{
    int hash = 0;
    char buffer[INET_ADDRSTRLEN];

    if (addr == 0 || ip == 0) {
	puts("Erreur: adresse ou IP initialisée à NULL.\n");
	return (-1);
    }

    if (inet_pton(AF_INET, ip, buffer) == 0) {
	puts("Erreur: l'IP n'est pas valide.\n");
	return (-1);
    }

    if (strlen(addr) > 255 || strlen(addr) < 4 || addr_verif(addr) < 1 || addr_verif(addr) > 2) {
	puts("Erreur: l'adresse n'est pas valide.\n");
	return (-1);
    }

    hash = hash_function(addr);

    if (hash > TAB_SIZE) {
	printf("Erreur: %s ne peut être ajoutée au tableau.\n", addr);
	return (-1);
    }

    if (ttl < 1) {
	puts("Erreur: TTL invalide.\n");
	return (-1);
    }

    if (hashtab[hash] == NULL) {
	hashtab[hash] = malloc(sizeof(struct hashtab));
	hashtab[hash]->ip[0] = 0;
	hashtab[hash]->addr[0] = 0;
	hashtab[hash]->timestamp = 0;
	hashtab[hash]->next = NULL;
    }

    hashtab[hash]->ttl = ttl;
    list_push(hashtab, addr, ip);
    printf("Add au hash %d.\n", hash);
    //list_print(hashtab[hash]);
    return (0);
}

int cache_delete(const char *key)
{
    int hash = 0;

    if (hashtab == NULL) {
	puts("Tableau vide.\n");
	return (0);
    }

    if (strlen(key) > 255 || strlen(key) < 4 || addr_verif(key) != 1) {
	puts("Erreur: l'adresse est invalide.\n");
	return (-1);
    }

    hash = hash_function(key);

    if (hash > TAB_SIZE) {
	printf("Erreur: %s n'est pas dans le tableau.\n", key);
	return (-1);
    }

    list_del_by_data(hashtab, key);

    return (0);
}

int cache_search(const char *key, char *ip)
{
    int hash = 0;
    int tmp = 0;

    if (strlen(key) > 255 || strlen(key) < 4 || addr_verif(key) != 1) {
	puts("Erreur: l'adresse n'est pas valide.\n");
	return (-1);
    }

    hash = hash_function(key);

    if (hash > TAB_SIZE || hash < 0) {
	printf("Erreur: %s n'est pas dans le tableau.\n", key);
	return (-1);
    }

    if (hashtab == NULL) {
	puts("Tableau vide.\n");
	return (0);
    }

    if (hashtab[hash] == NULL) {
	puts("Erreur: l'élément n'existe pas.\n");
	return (-2);
    }

    tmp = time(NULL);

    if (hashtab[hash]->timestamp <= 0) {
	puts("Erreur: l'élément n'existe pas.\n");
	return (-1);
    }

    if (difftime(tmp, hashtab[hash]->timestamp) > hashtab[hash]->ttl) {
	cache_delete(key);
	return (-2);
    }

    if (hashtab[hash]->ip[0] != 0) {
	struct hashtab *ptr = hashtab[hash];
	do {
	    if (strcmp(ptr->addr, key) == 0) {
		strncpy(ip, ptr->ip, IP4_SIZE-1);
		return 0;
		list_print(hashtab[hash]);
	    }
	    ptr = ptr->next;
	} while (ptr);
    }

    return (-1);
}
