#include "libnet.h"
#include "libthread.h"
#include "libgame.h"
#include "liblab.h"
#include "libutil.h"
#include <signal.h>

char adresse_g[100]; //Adresse IP du serveur
char port_g[10];	 //Port utilisé par le serveur
game partie;		 //Structure pour enregistrer le nom du serveur et le port
int connecte;		 //Variable d'état pour savoir si le client a réussi à se connecter
int fin_partie;		 //Variable d'état pour savoir si le client souhaite mettre fin à la partie
struct pollfd descripteurs[MAX_DESCRIPTEUR];
int s;			  //Socket pour la communication
char idClient[8]; //Pour enregistrer l'ID du client
objet2D *obj;	  //Pour enregister les objets 2D

pthread_t tid[2]; //Ids des threads

/********** Fonction pour nettoyer le programme avant son arrêt **********/
void clean(void)
{
	printf("\nLe jeu est en cours d'arrêt\n");

	pthread_cancel(tid[0]);
	pthread_cancel(tid[1]);

	fermerFenetre();

	printf("\nAu revoir\n");
}

/********** Gestionnaire de signaux **********/
void hand(int sig)
{
	if (sig == SIGINT)
	{
		clean();
		exit(SIGINT);
	}
	else if (sig == SIGQUIT)
	{
		clean();
		exit(SIGQUIT);
	}
}

//Fonction pour choisir un pseudo et se connecter au jeu
void connexionJeu()
{
	char tmp[MAX_TAMPON], tampon[MAX_TAMPON];
	while (!connecte)
	{
		int reponse = 0;
		printf("\nVeuillez choisir votre pseudo : (les caractères spéciaux ne sont pas acceptés)(taille max 63 caractères)\n");

		int nb = poll(descripteurs, 2, -1);

		while (!reponse)
		{
			nb = poll(descripteurs, 2, -1);
			if (nb < 0)
			{
				perror("main.poll");
				exit(EXIT_FAILURE);
			}

			/* Si quelque chose est tapé sur le terminal (entrée standard 0), 
    			on écrit ça sur la socket pour que ça soit envoyé au serveur */
			if ((descripteurs[1].revents & POLLIN) != 0)
			{
				int taille = read(0, tampon, MAX_TAMPON);
				strcpy(tmp, "CONNEXION ");
				strcat(tmp, tampon);
				if (taille <= 0)
					break;
				write(s, tmp, taille + 10);
				taille = 0;
				while (!reponse)
				{
					nb = poll(descripteurs, 2, -1);

					/* Si quelque chose est reçu sur la socket s, 
    					on l'écrit sur la sortie standard 1 (le terminal)  */
					if ((descripteurs[0].revents & POLLIN) != 0)
					{
						int taille = read(s, tampon, MAX_TAMPON);
						if (taille <= 0)
							break;
						write(1, tampon, taille);
						reponse = 1;
					}
					if (!strncmp(tampon, "> CONNECTE", 3))
					{
						connecte = 1;
						sscanf(tampon, "> CONNECTE %s", idClient);
					}
				}
			}
		}
	}
}

//Fonction pour communiquer en TCP
void communicationTCP()
{
	char *serveur = adresse_g;
	char *port = port_g;

	/* Connection au serveur */
	s = connexionServeurTCP(serveur, port);

	char tampon[MAX_TAMPON];

	descripteurs[0].fd = s;
	descripteurs[0].events = POLLIN;
	descripteurs[1].fd = 0;
	descripteurs[1].events = POLLIN;

	connecte = 0;

	connexionJeu();

	if (connecte == 1)
	{

		while (1)
		{
			int nb = poll(descripteurs, 2, -1);

			//printf("Connecté\n");

			char tmp[MAX_TAMPON];
			if (nb < 0)
			{
				perror("main.poll");
				exit(EXIT_FAILURE);
			}
			/* Si quelque chose est reçu sur la socket s, 
    		on l'écrit sur la sortie standard 1 (le terminal)  */
			if ((descripteurs[0].revents & POLLIN) != 0)
			{
				int taille = read(s, tampon, MAX_TAMPON);
				char messageCmd[524];

				if (sscanf(tampon, "CMD STO%[^\n]", messageCmd) == 1)
				{
					printf("Fin du jeu : fermeture de la fenêtre\n");
					fin_partie = 1;
				}

				if (taille <= 0)
					break;
				write(1, tampon, taille);
			}

			/* Si quelque chose est tapé sur le terminal (entrée standard 0), 
    		on écrit ça sur la socket pour que ça soit envoyé au serveur */
			if ((descripteurs[1].revents & POLLIN) != 0)
			{
				int taille = read(0, tampon, MAX_TAMPON);
				if (tampon[0] == '/')
				{

					strcpy(tmp, "CMD ");
					strcat(tmp, StrSub(tampon, 1, taille - 1));
					if (taille <= 0)
						break;
					write(s, tmp, taille + 3);
				}
				else
				{

					strcpy(tmp, "MSG ");
					strcat(tmp, tampon);

					if (taille <= 0)
						break;
					write(s, tmp, taille + 4);
				}
			}
		}
	}

	/* On termine la connexion */
	shutdown(s, SHUT_RDWR);
}

/* Fonction pour la reception UDP */
int gestionClientUDP(char *message, char *hote, char *service)
{
	int choix = 0;
	strcpy(adresse_g, StrSub(hote, 7, strlen(hote)));
	strcpy(partie.nomdepartie, StrSub((char *)message, 2, 11));
	partie.port = (unsigned char)message[0] * 256 + (unsigned char)message[1];
	printf("Partie : %s\nPort : %d\nAdresse : %s\n", partie.nomdepartie, partie.port, adresse_g);
	sprintf(port_g, "%d", partie.port);
	printf("\n Voulez-vous vous connecter à cette partie ?\n1 : Yes 	2 : No\n");
	scanf("%d", &choix);
	return choix;
}

int receptionUDP(int socket)
{
	/* Ecoute UDP */
	return receptionServeurUDP(socket, gestionClientUDP);
}

int gestionObjetUDP(char *message, char *hote, char *service)
{
	P(OBJ);
	obj = (objet2D *)message;
	V(OBJ);
	return 0;
}

void jeu()
{
	unsigned char resultat = creerFenetre(LARGEUR, HAUTEUR, TITRE);
	if (!resultat)
	{
		fprintf(stderr, "Problème graphique !\n");
		exit(-1);
	}

	int touche;

	unsigned char fenetre, quitter;

	char trame_touche[2] = {0, 0b00000000};
	void *handle;

	//Envoi des touches sur le numéro de port TCP + 1
	int port;
	sscanf(port_g, "%d", &port);
	port++;
	char portChar[8];
	sprintf(portChar, "%d", port);

	char *portTrame = portChar;
	char *portObjet = port_g;

	int socketTrame = creationSocketUDP(adresse_g, portTrame, &handle, 1); // pour envoi trame de touche
	int socket = initialisationSocketUDP(portObjet);					   // reception graphique
	while (fin_partie == 0)
	{
		int evenement = attendreEvenement(&touche, &fenetre, &quitter);
		if (!evenement)
		{
			effacerFenetre();
			int nboctets = receptionServeurUDP(socket, gestionObjetUDP);
			int no = nboctets / sizeof(objet2D);
			P(OBJ);
			dessine_2D(obj, no);
			V(OBJ);
			synchroniserFenetre();
		}
		if (touche)
		{
			if (touche == TOUCHE_DROITE)
			{
				trame_touche[1] = trame_touche[1] | 0b00000010;
			}
			else
			{
				trame_touche[1] = trame_touche[1] & 0b11111101;
			}

			if (touche == TOUCHE_GAUCHE)
			{
				trame_touche[1] = trame_touche[1] | 0b00000001;
			}
			else
			{
				trame_touche[1] = trame_touche[1] & 0b11111110;
			}

			if (touche == TOUCHE_HAUT)
			{
				trame_touche[1] = trame_touche[1] | 0b00000100;
			}
			else
			{
				trame_touche[1] = trame_touche[1] & 0b11111011;
			}

			if (touche == TOUCHE_BAS)
			{
				trame_touche[1] = trame_touche[1] | 0b00001000;
			}
			else
			{
				trame_touche[1] = trame_touche[1] & 0b11110111;
			}
			if (touche == TOUCHE_TIR)
			{
				trame_touche[1] = trame_touche[1] | 0b00010000;
			}
			else
			{
				trame_touche[1] = trame_touche[1] & 0b11101111;
			}

			messageUDP(socketTrame, trame_touche, sizeof(trame_touche), handle);
		}
	}
	fermerFenetre();
}

int main()
{
	//Pour initialiser le gestionnaire de signaux
	struct sigaction action;
	action.sa_handler = hand;
	sigaction(SIGINT, &action, NULL);
	sigaction(SIGQUIT, &action, NULL);

	int connexion = 0;
	fin_partie = 0;

	//On initialise le tableau de mutex
	init_mutex(mtx);

	/* Creation Socket pour reception UDP */
	int socket = initialisationSocketUDP("4242");
	while (connexion != 1)
	{
		connexion = receptionUDP(socket);
	}

	tid[0] = lancerThread(communicationTCP, NULL, 0);
	tid[1] = lancerThread(jeu, NULL, 0);
	while (fin_partie == 0)
		;
	sleep(2);

	clean();

	return 0;
}
