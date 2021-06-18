#ifndef LIB_NETWORK_H
#define LIB_NETWORK_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdint.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <netinet/tcp.h>
#include <time.h>
#include <poll.h>

#define MAX_UDP_MESSAGE 10000
#define MAX_CONNEXIONS 255
#define MAX_LIGNE 1024
#define MAX_DESCRIPTEUR 1024
#define MAX_TAMPON 1024

/*****UDP*****/
int creationSocketUDP(char *, char *, void **, int);
void messageUDP(int, char *, int, void *);
void destructionSocket(int, void *);
int initialisationSocketUDP(char *);
int receptionServeurUDP(int s, int (*traitement)(char *, char *, char *));

/*****TCP*****/
int connexionServeurTCP(char *hote, char *service);
int boucleServeurTCP(int ecoute, int (*traitement)(int));
int initialisationServeurTCP(char *service, int connexions);

#endif
