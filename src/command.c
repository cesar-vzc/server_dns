#include "../inc/fakedns.h"
#include "../inc/server.h"
#include "../inc/command.h"
#include "../inc/hashtab.h"
#include "../inc/list.h"
#include "../inc/utils.h"

void *put(struct client *client, char *buffer)
{
    int ttl = 10;
    int i = 0;
    int j = 0;
    int verif = 0;
    int verifchar = 0;
    char tmp[URL_SIZE];
    char tmp2[URL_SIZE];
    char add[] = "Ajoute\n";
    char err[] = "Echec\n";
    char err_cmd[] = "Commande invalide.\n";
    
    tmp[0] = '\0';
    tmp2[0] = '\0';

    for (int i = 0; buffer[i]; i++) {
	if (buffer[i] == '<' || buffer[i] == '/' || buffer[i] == '>' || buffer[i] == '.') {
	    if (buffer[i] == '<' || buffer[i] == '>')
		verifchar++;
	    verif++;
	}

    }

    if (verif != 7 || verifchar != 2) {
	sendto(client->answer->sock, (char *) err, 5, 0,
	       (const struct sockaddr *) &client->cli, client->src);
	return (NULL);
    }

    while (buffer[i] != '<') {
	if (buffer[i] == '\0') {
	    sendto(client->answer->sock, (char *) err_cmd, 19, 0,
		   (const struct sockaddr *) &client->cli, client->src);
	    return (NULL);
	}
	i++;
    }
    
    i = i + 1;

    while (buffer[i])
	tmp[j++] = buffer[i++];

    i = 0;
    j = 0;

    while (tmp[i] != '/')
	i++;

    i = i + 1;

    while (tmp[i] != '>') {
	if (tmp[i] == '\0') {
	    sendto(client->answer->sock, (char *) err_cmd, 19, 0,
		   (const struct sockaddr *) &client->cli, client->src);
	    return (NULL);
	}
	tmp2[j++] = tmp[i++];
    }
    
    tmp2[j] = '\0';

    j = j + 1;

    tmp[i - j] = '\0';

    pthread_mutex_lock(&client->answer->mutex_tab);
    if (cache_add(tmp, tmp2, ttl) == 0)
	sendto(client->answer->sock, (char *) add, 7, 0,
	       (const struct sockaddr *) &client->cli, client->src);
    else
	sendto(client->answer->sock, (char *) err, 6, 0,
	       (const struct sockaddr *) &client->cli, client->src);
    pthread_mutex_unlock(&client->answer->mutex_tab);

    return (NULL);
}

void *get(struct client *client, char *buffer)
{
    int i = 0;
    int j;
    int verif = 0;
    char tmp[URL_SIZE];
    char ip[IP4_SIZE];
    char err[] = "NULL\n";
    char err_cmd[] = "NULL [commande invalide].\n";
    char err_ttl[] = "NULL [TTL termine].\n";
    
    tmp[0] = '\0';

    for (int i = 0; buffer[i]; i++) {
	if (buffer[i] == '<' || buffer[i] == '>' || buffer[i] == '.')
	    verif++;
    }

    if (verif != 3) {
	sendto(client->answer->sock, (char *) err, 5, 0,
	       (const struct sockaddr *) &client->cli, client->src);
	return (NULL);
    }

    while (buffer[i] != '<') {
	if (buffer[i] == '\0') {
	    sendto(client->answer->sock, (char *) err_cmd, 26, 0,
		   (const struct sockaddr *) &client->cli, client->src);
	    return (NULL);
	}
	i++;
    }

    i = i + 1;

    memset(tmp, 0, URL_SIZE);
    j = 0;
    while (buffer[i] != '>') {
	if (buffer[i] == '\0') {
	    sendto(client->answer->sock, (char *) err_cmd, 26, 0,
		   (const struct sockaddr *) &client->cli, client->src);
	    return (NULL);
	}
	tmp[j++] = buffer[i++];
    }

    tmp[strlen(tmp)] = '\0';

    pthread_mutex_lock(&client->answer->mutex_tab);
    if (cache_search(tmp, ip) == 0) {
	int len = strlen(ip);
	sendto(client->answer->sock, (char *) ip, len, 0,
	       (const struct sockaddr *) &client->cli, client->src);
	printf("L'adresse IP demandée est : %s\n", ip);
    } else if (cache_search(tmp, ip) == -2)
	sendto(client->answer->sock, (char *) err_ttl, 19, 0,
	       (const struct sockaddr *) &client->cli, client->src);
    else
	sendto(client->answer->sock, (char *) err, 5, 0,
	       (const struct sockaddr *) &client->cli, client->src);
    pthread_mutex_unlock(&client->answer->mutex_tab);
    
    return (NULL);
}

void quit(void) {
    puts("\nLe client a quitté le programme.\n");
}

void *client_handler(void *ans)
{
    struct client *client = (struct client*) ans;
    
    printf("\nCommande du client :\n\n%s\n", client->buffer);
    
    if (strncmp(client->buffer, "get ", 4) == 0)
	get(client, client->buffer);
    else if (strncmp(client->buffer, "put ", 4) == 0)
	put(client, client->buffer);
    
    pthread_mutex_lock(&client->answer->mutex_conn);
    client->answer->conn--;
    pthread_mutex_unlock(&client->answer->mutex_conn);
    free(client);
    return (NULL);
}
