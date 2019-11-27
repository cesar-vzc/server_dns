// CLIENT

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <getopt.h>

#define URL_SIZE 255

void need_help(void)
{
    printf("Usage :\n\n\t-v, --version\t\t\tAffiche la version du programme.\n\n\t");
    printf("-p, --port PORT_NUMBER\t\tConnexion sur le port envoyé.\n\n");
}

void need_version(void)
{
    printf("Fake Server DNS (UDP)\n\n\tVersion 1.0\n\n");
}

int main(int argc, char **argv) {
    int opt = 0;
    int port = 53;
    int sock;
    char buffer[URL_SIZE];
    char rbuffer[URL_SIZE];
    char errp[] = "put <error>\n";
    char errg[] = "get <error>\n";

    long unsigned int j;

    static struct option long_options[] = {
					   {"help", no_argument, 0, 'h'},
					   {"version", no_argument, 0, 'v'},
					   {"port", required_argument, 0, 'p'},
					   {0, 0, 0, 0}
    };

    int long_index = 0;

    while ((opt = getopt_long(argc, argv, "hvp:m:",
			      long_options, &long_index)) != -1) {
	switch (opt) {
	case 'h' :
	    need_help();
	    return (0);
	case 'v' :
	    need_version();
	    return (0);
	case 'p' : port = atoi(optarg);
	    break;
	}
    }
    
    struct sockaddr_in servaddr;
    socklen_t dest_addr;
    dest_addr = sizeof(servaddr);
    
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
	perror("socket");
	return (-1);
    }
    
    memset(&servaddr, 0, sizeof(servaddr));
    
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    
    while (1) {
	puts("\nEntrer une commande :\n");
	memset(buffer, 0, sizeof(buffer));
	read(STDIN_FILENO, buffer, sizeof(buffer));

	j = strlen(buffer);

	if (j != 255) {
	    if (sendto(sock, (char *) buffer, URL_SIZE, 0,
		       (const struct sockaddr *) &servaddr, dest_addr) < 0) {
		perror("sendto");
		return (-1);
	    }
	} else if (strncmp(buffer, "put ", 4) == 0) {
	    sendto(sock, (char *) errp, 12, 0,
		   (const struct sockaddr *) &servaddr, dest_addr);
	} else if (strncmp(buffer, "get ", 4) == 0) {
	    sendto(sock, (char *) errg, 12, 0,
		   (const struct sockaddr *) &servaddr, dest_addr);
	}

	memset(rbuffer, 0, sizeof(rbuffer));

	if (strncmp(buffer, "put ", 4) == 0) {
	    recvfrom(sock, (char *) rbuffer, URL_SIZE, 0,
		     (struct sockaddr *) &servaddr, &dest_addr);
	    printf("\n%s\n", rbuffer);
	} else if (strncmp(buffer, "get ", 4) == 0) {
	    recvfrom(sock, (char *) rbuffer, URL_SIZE, 0,
		     (struct sockaddr *) &servaddr, &dest_addr);
	    printf("\nL'IP demandée est : %s\n", rbuffer);
	} else {
	    puts("\nCommande invalide.");
	}
    }

    close(sock);
    return (0);
}
