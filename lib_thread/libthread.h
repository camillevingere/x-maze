#ifndef LIB_THREAD_H
#define LIB_THREAD_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#define ID_JOUEUR 1
#define POS_JOUEUR 2
#define OBJ 3

#define NB_MUTEX 10

//gestion des mutex
pthread_mutex_t mtx[NB_MUTEX];

typedef struct
{
    void (*fonction)(void *);
    void *parametre;
} struct_thread;

void *lancementFonction(void *);
pthread_t lancerThread(void (*fonction)(void *), void *parametre, int taille_parametre);
void P(int);
void V(int);
void init_mutex(pthread_mutex_t m[]);

#endif
