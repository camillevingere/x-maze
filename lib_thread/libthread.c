#include "libthread.h"

void *lancementFonction(void *args_inter)
{
    struct_thread *str = args_inter;
    str->fonction(str->parametre);
    if (str->parametre != NULL)
        free(str->parametre);
    free(str);
    pthread_exit(NULL);
}

pthread_t lancerThread(void (*fonction)(void *), void *parametre, int taille_parametre)
{

    pthread_t thr;
    pthread_attr_t attr;                                         //pour mettre les threads en détaché
    pthread_attr_init(&attr);                                    //on initialise l'attribut avec les valeurs par defaut
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED); //le thread est detaché

    struct_thread *str = malloc(sizeof(struct_thread)); //la structure est placé dans le tas, car sinon à la fin de lancerFlux, les parametre placé en Pile ne sont plus accessibles par lancementFct
    str->fonction = fonction;

    if (taille_parametre > 0)
    {
        str->parametre = malloc(taille_parametre);
        memcpy(str->parametre, parametre, taille_parametre);
    }
    else
    {
        str->parametre = NULL;
    }

    pthread_create(&thr, &attr, lancementFonction, str);

    return thr;
}

void init_mutex(pthread_mutex_t m[])
{
    for (int i = 0; i < 10; i++)
    {
        pthread_mutex_init(&(m[i]), NULL);
    }
}

void P(int i)
{
    pthread_mutex_lock(&(mtx[i]));
}

void V(int i)
{
    pthread_mutex_unlock(&(mtx[i]));
}