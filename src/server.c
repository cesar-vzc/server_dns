#include "../inc/fakedns.h"
#include "../inc/server.h"
#include "../inc/command.h"
#include "../inc/hashtab.h"
#include "../inc/list.h"
#include "../inc/utils.h"


static struct answer *pans;

void INThandler(int i)
{
    int total;
    float moyenne;

    signal(i, SIG_IGN);

    printf("\n\nCtrl+C reçu, fermeture du programme.\n\n");

    pthread_mutex_destroy(&pans->mutex_conn);
    pthread_mutex_destroy(&pans->mutex_tab);    
    cache_free();

    printf("* Commandes traitees : [ %d ]\n\n* Commandes non traitees : [ %d ]\n\n",
	   pans->count_max, pans->count);

    total = pans->count + pans->count_max;
    
    printf("* Nombre de commande : [ %d ]\n\n",
	   total);

    moyenne = (pans->count_max / (double)total) * 100;
    
    printf("Pourcentage des commandes traitees : [ %.2f ", moyenne);
    putchar('%');
    puts(" ]\n");
    
    puts("Serveur eteint.");
    close(pans->sock);
    exit(0);
}

int server(int port, int max_conn)
{
    pthread_t thread;
    struct client *client = NULL;
    char tmp[URL_SIZE];
    int n;
    struct answer answer;
    
    cache_init(TAB_SIZE);
    pans = &answer;

    answer.sock = 0;
    answer.max = max_conn;
    answer.conn = 0;
    pthread_mutex_init(&answer.mutex_conn, NULL);
    pthread_mutex_init(&answer.mutex_tab, NULL);

    struct sockaddr_in servaddr, cliaddr;

    socklen_t src_addr;
    src_addr = sizeof(servaddr);

    if ((answer.sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
	perror("socket");
	return (-1);
    }

    int buf = 1;
    // Permet de relancer le server directement.
    if ((setsockopt(answer.sock, SOL_SOCKET,
		    SO_REUSEADDR, &buf, sizeof (int)) < 0)) {
	perror("sockopt");
	return (-1);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);

    int sock_err;
    if ((sock_err = bind(answer.sock,
			 (const struct sockaddr *) &servaddr,
			 sizeof(servaddr))) < 0) {
	perror("bind");
	return (-1);
    }

    puts("Serveur allume.");

    pans->count = 0;
    pans->count_max = 0;
    
    signal(SIGINT, INThandler);
    while (1) {
	memset(tmp, 0, sizeof(tmp));
	if ((n = recvfrom(answer.sock, (char *) tmp, URL_SIZE-1, 0,
			  (struct sockaddr *) &cliaddr, &src_addr)) < 0) {
	    perror("recvfrom");
	    return (-1);
	}
		
	if (answer.conn >= answer.max) {
	    puts("Le nombre de requete maximum est atteint.");
	    pans->count++;
	} else {
	    client = malloc(sizeof(struct client));
	    client->answer = &answer;
	    strncpy(client->buffer, tmp, URL_SIZE-1);
	    client->buffer[n] = 0;
	    client->src = src_addr;
	    client->cli = cliaddr;

	    pthread_mutex_lock(&answer.mutex_conn);
	    client->answer->conn++;
	    pthread_mutex_unlock(&answer.mutex_conn);
	    pthread_create(&thread, NULL, client_handler, (void *) client);
	    if (pthread_detach(thread) != 0)
		puts("detach");
	    pans->count_max++;
	}
    }
    close(answer.sock);
    return (0);
}
