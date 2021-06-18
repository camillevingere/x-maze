#include "libnet.h"
#include "libthread.h"
#include "libgame.h"
#include "liblab.h"
#include "libutil.h"
#include <signal.h>

game partie;			   //Structure pour enregistrer le nom du serveur et le port
joueurs Jeu;			   //Pour enregistrer tous les joueurs et leurs infos
char *serviceUDP = "4242"; //Port UDP
char *serviceTCP = "9000"; //Port TCP
pthread_t tid[4];		   //Ids des threads

mur *m2; //Pour sauvegarder les murs

int start = 0; //Pour commencer l'envoie d'objets quand la partie a commencé

char *laby[2 * LABY_Y + 1] = {
	" - - - - - - - - ",
	"|. . . . . . . .|",
	" -               ",
	"|. .|. . . . .|.|",
	"       - -       ",
	"|. .|.|. . .|. .|",
	"             - - ",
	"|. .|.|. .|. . .|",
	"   -           - ",
	"|. .|. . .|. . .|",
	"       - -       ",
	"|. .|.|. . . . .|",
	" -           -   ",
	"|. .|.|. . .|. .|",
	"       - -       ",
	"|. . . . . . . .|",
	" - - - - - - - - "};

/********** Fonction pour nettoyer le programme avant son arrêt **********/
void clean(void)
{
	printf("\nLe serveur est en cours d'arrêt\n");

	pthread_cancel(tid[1]);
	pthread_cancel(tid[2]);
	pthread_cancel(tid[3]);

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

/********** Fonction pour gerer le chat des clients en TCP **********/
void gestionChatTCP(void *arg)
{
	int s = *(int *)arg;
	/* Obtient une structure de fichier */
	FILE *dialogue = fdopen(s, "a+");

	if (dialogue == NULL)
	{
		perror("gestionClient.fdopen");
		exit(EXIT_FAILURE);
	}
	/* Echo */
	char ligne[MAX_LIGNE];
	while (fgets(ligne, MAX_LIGNE, dialogue) != NULL)
	{
		P(ID_JOUEUR);
		Jeu.dialogue[Jeu.nbJoueurs] = s;
		Jeu.file[Jeu.nbJoueurs] = dialogue;
		start = analyseMessage(ligne, dialogue, &Jeu);
		V(ID_JOUEUR);
	}

	/* Termine la connexion */
	fclose(dialogue);
}

/********** Fonction pour broadcaster le port TCP et le nom du serveur **********/
void broadcastUDP()
{
	void *handle;
	char *hote = "255.255.255.255";
	char message[MAX_UDP_MESSAGE] = {0x23, 0x28, 'E', 'u', 'r', 'o', 'p', 'e'};

	int socket = creationSocketUDP(hote, serviceUDP, &handle, 1);

	while (1)
	{
		messageUDP(socket, message, sizeof(message), handle);
		sleep(5);
	}
}

/********** Thread pour envoyer les objets à tous les joueurs **********/
void envoiObjetsUDP()
{
	void *handle;
	char *hote = "255.255.255.255";

	int socket = creationSocketUDP(hote, serviceTCP, &handle, 1);

	while (1)
	{
		for (int i = 0; i < Jeu.nbJoueurs; i++)
		{
			int nb = dessin_vers_murs(laby, murs);
			m2 = duplique_murs(murs, nb);
			decale_murs(m2, nb, Jeu.position_joueur[i]);
			rotation_murs(m2, nb, Jeu.angle_joueur[i]);
			tri_murs(m2, nb);
			objet2D *objets = malloc(nb * sizeof(objet2D));
			int *no = malloc(sizeof(int));
			projete_murs(m2, nb, objets, no);

			/* Envoi des objets */
			messageUDP(socket, (char *)objets, nb * sizeof(objet2D), handle);

			/* Libération de la mémoire */
			free(no);
			free(m2);
			free(objets);
		}
	}
}

/********** Fonction pour lancer le thread gestionTchatTCP **********/
int lancerGestionChatTCP(int s)
{
	lancerThread(gestionChatTCP, &s, sizeof(int));
	return 0;
}

/********** Fonction d'écoute TCP **********/
void ecouteChatTCP()
{

	/* Initialisation du serveur */
	int s = initialisationServeurTCP(serviceTCP, MAX_CONNEXIONS);

	/* Lancement de la boucle d'ecoute */
	boucleServeurTCP(s, lancerGestionChatTCP);
}

/* Fonction pour gérer les touches du envoyées par le joueur en UDP */
int gestionTouchesUDP(char *message, char *hote, char *serviceTCP)
{
	/*Gestion des touches */
	int id_client = message[0];

	char touche[8];
	decToBin((unsigned char)message[1], touche);

	P(POS_JOUEUR);
	int angle = Jeu.angle_joueur[id_client];
	point p = Jeu.position_joueur[id_client];
	V(POS_JOUEUR);

#ifdef DEBUG
	/* print result */
	for (int n = 0; n < 8; n++)
		printf("%d", touche[7 - n]);
	printf("\n");
#endif

	if (touche[1] == 1)
	{
		angle += 5;
	}
	if (touche[0] == 1)
		angle -= 5;
	if (angle < 0 || angle > 360)
		angle = angle % 360;
	if (touche[2] == 1)
	{
		p.x += MUR_TAILLE / 10 * sin(2 * M_PI * angle / 360);
		p.z += MUR_TAILLE / 10 * cos(2 * M_PI * angle / 360);
	}
	if (touche[3] == 1)
	{
		p.x -= MUR_TAILLE / 10 * sin(2 * M_PI * angle / 360);
		p.z -= MUR_TAILLE / 10 * cos(2 * M_PI * angle / 360);
	}
#ifdef DEBUG
	printf("Touche : %hhu, angle : %d\n", touche, Jeu.angle_joueur[id_client]);
#endif
	P(POS_JOUEUR);
	Jeu.position_joueur[id_client] = p;
	Jeu.angle_joueur[id_client] = angle;
	V(POS_JOUEUR);

	return 0;
}

/********** Fonction d'écoute pour recevoir les touches en UDP **********/
void receptionTouchesUDP()
{

	/* Initialisation du serveur */
	int socket = initialisationSocketUDP("9001");

	/* Lancement de la boucle d'ecoute */
	while (1)
	{
		receptionServeurUDP(socket, gestionTouchesUDP);
	}
}

/* Fonction principale */
int main(void)
{
	//Pour initialiser le gestionnaire de signaux
	struct sigaction action;
	action.sa_handler = hand;
	sigaction(SIGINT, &action, NULL);
	sigaction(SIGQUIT, &action, NULL);

	Jeu.nbJoueurs = 0;

	//On initialise le tableau de mutex
	init_mutex(mtx);

	tid[0] = lancerThread(broadcastUDP, NULL, 0);
	tid[1] = lancerThread(ecouteChatTCP, NULL, 0);

	//Attente tant que l'admin ne lance pas la partie
	while (start != 1)
		;

	//Pour arrêter le thread de broadcast une fois que la partie est lancée
	pthread_cancel(tid[0]);

	//Initialisation au hasard de la position des joeurs
	init_position_joueurs(&Jeu);

	tid[2] = lancerThread(receptionTouchesUDP, NULL, 0);
	tid[3] = lancerThread(envoiObjetsUDP, NULL, 0);

	//Threads en action tant que l'admin ne met pas fin à la partie
	while (1)
		;
	return 0;
}
