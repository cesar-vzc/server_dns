#include "../inc/fakedns.h"
#include "../inc/server.h"
#include "../inc/command.h"
#include "../inc/hashtab.h"
#include "../inc/list.h"
#include "../inc/utils.h"

int addr_verif(const char *addr)
{
    int addr_verif = 0;

    for (long unsigned i = 0; i < strlen(addr); i++) {
	if (addr[i] == '.')
	    addr_verif++;
    }
    return (addr_verif);
}
