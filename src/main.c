#include "../inc/fakedns.h"
#include "../inc/server.h"
#include "../inc/command.h"
#include "../inc/hashtab.h"
#include "../inc/list.h"
#include "../inc/utils.h"

void need_help(void)
{
    printf("Usage :\n\n\t-v, --version\t\t\tAffiche la version du programme.\n\n\t");
    printf("-p, --port PORT_NUMBER\t\tConnexion sur le port envoyé.\n\n\t");
    printf("-m, --max-conn MAX_NUMBER\tLimite le nombre de connexion client.\n\n");
}

void need_version(void)
{
    printf("Fake Server DNS (UDP)\n\n\tVersion 1.0\n\n");
}

int main(int argc, char **argv)
{
    int opt = 0;
    int port = 53;
    int max_conn = 5;
    
    static struct option long_options[] = {
					   {"help", no_argument, 0, 'h'},
					   {"version", no_argument, 0, 'v'},
					   {"port", required_argument, 0, 'p'},
					   {"max-conn", required_argument, 0, 'm'},
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
	case 'm' : max_conn = atoi(optarg);
	    break;
	}
    }

    server(port, max_conn);
    return (0);
}
