#include "./libgame.h"

//Fonction pour tester la validité du pseudo
int testPseudo(char pseudo[64])
{
    char *charAccept = "abcdefghijklmnopkrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_";
    if (strlen(pseudo) == strspn(pseudo, charAccept))
        return 1;
    return -1;
}

//Fonction pour analyser la nature du message
int analyseMessage(char message[1024], FILE *dialogue, joueurs *Jeu)
{
    char pseudo[64];
    char messageCmd[524];
    char messageMp[524];
    int status = sscanf(message, "CONNEXION %s", pseudo);

    //Si un client veut se connecter
    if (status == 1)
    {
        if (testPseudo(pseudo) == 1)
        {
            printf("Pseudo : %s\n", pseudo);
            char reponse[32] = "CONNECTE";
            char nbJoueurs[4] = "0";
            sprintf(nbJoueurs, "%d", Jeu->nbJoueurs);
            strcat(reponse, " ");
            strcat(reponse, nbJoueurs);
            strcpy(Jeu->pseudosJoueurs[Jeu->nbJoueurs], pseudo);
            Jeu->nbJoueurs++;
            printf("%s\n", reponse);
            fprintf(dialogue, "> %s", reponse);
            fflush(dialogue);
            afficheJoueurs(*Jeu);
        }
        else
        {
            fprintf(dialogue, "> %s", "ERREUR PSEUDO INVALIDE");
        }
    }

    //Si un client veut lancer une commande
    else if (sscanf(message, "CMD %[^\n]", messageCmd) == 1)
    {
        //Pour lancer la partie ( commande reservé à l'admin )
        if (strcmp(messageCmd, "START") == 0)
        {
            printf("Début de partie\n");
            char messageAll[1024] = "START";
            partageCommande(messageAll, *Jeu);
            return 1;
        }
        //Pour arrêter la partie ( commande reservé à l'admin )
        else if (strcmp(messageCmd, "STOP") == 0)
        {
            //Arret de la partie ( commande reservé à l'admin )
            printf("Fin de partie\n");
            char messageAll[1024] = "STOP";
            partageCommande(messageAll, *Jeu);
            return 0;
        }
        //Pour envoyer un message privé
        else if (sscanf(messageCmd, "MP %s %[^\n]", pseudo, messageMp) == 2)
        {
            char pseudoP[64];
            int dialogueP = 0;
            for (int i = 0; i < Jeu->nbJoueurs; i++)
            {
                if (strcmp(pseudo, Jeu->pseudosJoueurs[i]) == 0)
                    dialogueP = Jeu->dialogue[i];
            }
            if (dialogueP == 0)
                fprintf(dialogue, "> %s", "JOUEUR INTROUVABLE\n");
            else
            {
                char messageJoueur[1024];
                trouverIdJoueur(dialogue, *Jeu, pseudoP);
                sprintf(messageJoueur, "> ||MP|| %s : %s\n", pseudoP, messageMp);
                int taille = strlen(messageJoueur);
                write(dialogueP, messageJoueur, taille);
            }
        }
        //Si la commande est invalide
        else
        {
            fprintf(dialogue, "> %s", "COMMANDE INVALIDE\n");
        }
    }
    //Sinon on affiche le message
    else
    {
        char messageAll[1024];
        status = sscanf(message, "MSG %[^\n]", messageAll);
        if (status == 1)
        {
            char pseudo[64];
            trouverIdJoueur(dialogue, *Jeu, pseudo);
            partageMessage(messageAll, *Jeu, pseudo);
        }
    }

    return 0;
}

//Fonction pour trouver l'ID du joueur en fonction de son pseudo
void trouverIdJoueur(FILE *dialogue, joueurs Jeu, char pseudo[64])
{
    for (int j = 0; j < Jeu.nbJoueurs; j++)
    {
        if (Jeu.file[j] == dialogue)
        {
            strcpy(pseudo, Jeu.pseudosJoueurs[j]);
        }
    }
}

//Fonction pour afficher les joueurs en ligne
void afficheJoueurs(joueurs Jeu)
{
    for (int j = 0; j < Jeu.nbJoueurs; j++)
    {
        char reponse[1024] = "JOUEURS";
        char nbJoueurs[4] = "0";
        sprintf(nbJoueurs, "%d", Jeu.nbJoueurs);
        write(Jeu.dialogue[j], "\n", sizeof("\n"));
        strcat(reponse, " ");
        strcat(reponse, nbJoueurs);

        for (int i = 0; i < Jeu.nbJoueurs; i++)
        {
            strcat(reponse, "\n");
            strcat(reponse, Jeu.pseudosJoueurs[i]);
        }
        strcat(reponse, "\n");
        write(Jeu.dialogue[j], reponse, sizeof(reponse));
    }
}

//Fonction pour partager les messages à tout le monde (broadcast)
void partageMessage(char messageAll[1024], joueurs Jeu, char pseudo[64])
{
    for (int j = 0; j < Jeu.nbJoueurs; j++)
    {
        char sender[1024] = "MSGFROM";
        char reponse[1024] = "MSG";
        strcat(sender, " ");
        strcat(sender, pseudo);
        strcat(sender, "\n");
        write(Jeu.dialogue[j], sender, sizeof(sender));
        strcat(reponse, " ");
        strcat(reponse, messageAll);
        strcat(reponse, "\n");
        write(Jeu.dialogue[j], reponse, sizeof(reponse));
    }
}

//Fonction pour partager les commandes à tout le monde (broadcast)
void partageCommande(char messageAll[1024], joueurs Jeu)
{
    for (int j = 0; j < Jeu.nbJoueurs; j++)
    {
        char reponse[1024] = "CMD";
        strcat(reponse, " ");
        strcat(reponse, messageAll);
        strcat(reponse, "\n");
        write(Jeu.dialogue[j], reponse, sizeof(reponse));
    }
}

//Fonction pour initialiser la position des joueurs
void init_position_joueurs(joueurs *Jeu)
{
    // On commence par initialiser le générateur de nombre pseudo-aléatoires.
    srand(time(NULL));
    for (int i = 0; i < Jeu->nbJoueurs; i++)
    {

        point p = {rand() % LABY_X * MUR_TAILLE, 0, MUR_TAILLE};
        Jeu->position_joueur[i] = p;
    }
}