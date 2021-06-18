#ifndef LIB_GAME_H
#define LIB_GAME_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <poll.h>
#include <time.h>

#include "liblab.h"

#define MAX_JOUEURS 256

typedef struct game
{
    int port;
    char nomdepartie[9];
} game;

typedef struct joueurs
{
    int nbJoueurs;
    char pseudosJoueurs[MAX_JOUEURS][9];
    FILE *file[MAX_JOUEURS];
    int dialogue[MAX_JOUEURS];
    point position_joueur[MAX_JOUEURS];
    int angle_joueur[MAX_JOUEURS];
} joueurs;

int testPseudo(char pseudo[64]);
int analyseMessage(char message[1024], FILE *dialogue, joueurs *Jeu);
void afficheJoueurs(joueurs Jeu);
void trouverIdJoueur(FILE *dialogue, joueurs Jeu, char pseudo[64]);
void partageMessage(char messageAll[1024], joueurs Jeu, char pseudo[64]);
void partageCommande(char messageAll[1024], joueurs Jeu);
void init_position_joueurs(joueurs *Jeu);

#endif
