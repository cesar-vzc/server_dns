# fakeDNS

## Description

*Création d'un serveur DNS (UDP). Un serveur DNS reçoit les requêtes du client et fait correspondre l'URL demandée à son adresse IP.*

*Le __DNS__ (Domain Name System) est le __service utilisé afin de traduire le nom de domaine en adresse IP__. Par exemple, un utilisateur veut aller sur google.fr. Il envoie sa requête qui sera récupéré par le DNS. Celui-ci va chercher l'adresse IP du site demandé afin de la renvoyé à l'utilisateur qui sera alors redirigé sur le site de Google.*

*Le DNS utilise le protocol UDP sur le port 53 par défaut.*

---

Afin de réaliser ce programme, on utilise :

- une socket : permet la connexion client/serveur.

- des threads : permet la connexion de plusieurs clients sur le serveur.

- des mutexs : protègent les variables.

- une table de hachage chaînée : range les informations dans un tableau et évite les collisions grâce aux listes chaînées.

###### Table de hachage

Ce procédé transforme une string en un entier qui définira son placement dans le tableau.

Afin de retrouver l'information placé dans le tableau il suffira d'envoyé la string recherchée. Le programme récupère l'emplacement dans le tableau grâce à la string et renvoie les données présentes à cette position.

Ici l'URL jouera le rôle de clef afin d'obtenir l'emplacement de son adresse IP.

###### Chaînage

Le tableau gère les collisions grâce aux listes chaînées de la manière suivante :

![Alt Image Text](https://user.oc-static.com/files/379001_380000/379571.png "Chaînage")

Si une adresse possède le même hach qu'une autre adresse alors elle est passé dans le chunk suivant de la liste.

###### TTL (Time To Live)

Chaque ajout dans la table de hachage possède une durée de vie (TTL) en secondes durant laquel l'information ajoutée sera conservée dans le tableau. A la fin de sa durée de vie, l'information sera alors supprimée du tableau et la mémoire allouée pour celle-ci sera libérée.

---

### Headers

Les headers sont disposés dans le dossier `inc/`.

### Sources

Les sources sont disposés dans le dossier `src/`.

### Makefile

Le `Makefile` compile les sources afin de créer l'exécutable du serveur `fakedns`. 

La compilation est effectuée avec les flags `-W -Wall -Werror -lpthread`.

`make clean` efface les fichiers `.o` créés dans le dossier `src/`.

`make distclean` supprime l'exécutable créer.

### Client

Le client nécessite une compilation manuelle :

	gcc -o client client.c -W -Wall -Werror -O0

(Créer l'exécutable `client`).

Le client possède plusieurs flags tels que :

- `-h` ou `--help`, permet d'afficher l'aide d'utilisation des commandes puis quitte le programme.

- `-v` ou `--version`, permet d'afficher la version du programme puis le quitte.

- `-p [PORT]` ou `--port [PORT]`, permet de modifier le port de connexion du client.

Exemple :

	./client -p 8888
	./client --port 8888

Il est possible d'utiliser netcat afin de communiquer avec le serveur. Il suffit de rentrer la commande :

	nc -u [IP] [PORT]

Le `-u` est nécessaire pour le protocol UDP.

Le client possède deux commandes :

- `put <URL/IP>`, permet d'ajouter une adresse IP correspondant à son URL dans le serveur DNS. Le client reçoit `Ajouté` si l'élément a bien été envoyé et `Échec` en cas d'erreur.

- `get <URL>`, permet de récuperer l'adresse IP correspondant à l'URL demandée. Le client reçoit l'adresse IP si l'élément a bien été trouvé dans le serveur et `NULL` si l'élément n'existe pas.

Exemple :

`put <google.fr/1.1.1.1>`

`get <google.fr>`

Remarque :

L'entrée URL demande uniquement le nom de domaine et l'extension ainsi `put <www.google.fr/1.1.1.1>` et `get <www.google.fr>` ne fonctionneront pas.

### Serveur

Le serveur possède plusieurs flags tels que :

- `-h` ou `--help`, permet d'afficher l'aide d'utilisation des commandes puis quitte le programme.

- `-v` ou `--version`, permet d'afficher la version du programme puis le quitte.

- `-p [PORT]` ou `--port [PORT]`, permet de modifier le port de connexion du serveur.

- `-m [NB_MAX]` ou `--max_conn [NB_MAX]`, permet de modifier le nombre de requête maximum reçu par le serveur.

Exemple :

	./fakedns -p 8888
	./fakedns --port 8888
>

	./fakedns -m 88
	./fakedns --max_conn 88

### Test

Un client test est présent `test.c` afin de vérifier si le serveur est fonctionnel. Le client envoie de commandes `put`et `get` automatiquement.

Pour le compiler il suffit d'entrer la commande :

	gcc -o test test.c -lpthread
	
Enfin, pour l'exécuter :

	./test -p 8888

### Extinction du serveur

Lors de l'extinction du serveur, celui-ci vous renvoie le nombre de commandes traitées, non traitées, le total des commandes reçues et enfin le pourcentage des commandes traitées.

>![Alt Image Text](https://image.noelshack.com/fichiers/2019/41/2/1570531155-fakedns.png "Moyenne")

### Conclusion

J'ai beaucoup aimé faire ce projet, il m'a permit d'apprendre des nouvelles notions comme les tables de hachage, le TTL et les mutexs. La réalisation de ce projet est intéressante car cela m'a fait comprendre comment fonctionne un serveur DNS et de plus, cela m'a permit de mettre en application toutes les notions vu au préalable et de me familiariser avec celles-ci.

Auteur : *César VENZAC*

Tuteurs : *Nicolas VILMAIN et Vincent KHERBACHE*