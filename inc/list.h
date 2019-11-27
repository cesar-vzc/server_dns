#ifndef __LIST_H__
#define __LIST_H__

int list_del_by_data(struct hashtab **hashtab, const char *addr);
void list_print(struct hashtab *hashtab);
int list_push(struct hashtab **hashtab, const char *addr, const char *ip);

#endif /* ! __LIST_H__ */
