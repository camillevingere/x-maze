#ifndef LIB_LAB_H
#define LIB_LAB_H

/**** Programme de creation de labyrinthe ****/

/** Fichiers d'inclusion **/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#include "../Graphique/libgraph.h"

/** Constantes **/

#define TITRE "Labyrinthe XMAZE"
#define LARGEUR 640
#define HAUTEUR 480

#define LABY_X 8
#define LABY_Y 8
#define MUR_TAILLE 200
#define MUR_HAUTEUR 200

#define FOCALE (3 * MUR_TAILLE)

#define ATTENTE 10000

#define POINTS_MAX 32

#define TYPE_MUR 0
#define TYPE_SPHERE 1

#define M_PI 3.14159265358979323846

#define DISTANCE_MAX_COLLISION 10

/** Macros **/

#define max(a, b) (((a) < (b)) ? (a) : (b))
#define min(a, b) (((a) > (b)) ? (b) : (a))
#define sign(a) (((a) == 0) ? 0 : (((a) > 0) ? 1 : -1))

/** Structures **/

typedef struct
{
    int x, y, z;
} point;

typedef struct
{
    int x, y;
} point2D;

typedef struct
{
    point p[4];
} mur;

typedef struct s_disque
{
    point2D centre;
    int rayon;
} s_disque;

typedef struct sphere
{
    point centre;
    int rayon;
} sphere;

typedef struct
{
    int type;
    union
    {
        point2D p[4];
        s_disque s;
    } def;
} objet2D;

typedef struct
{
    int type; //0 pour un mur, 1 pour une sphère
    union
    {
        sphere s;
        mur m;
    } def;
} objet3D;

mur murs[(LABY_X + 1) * LABY_Y + (LABY_Y + 1) * LABY_X];

/** Variables globales **/

extern char *laby[2 * LABY_Y + 1];

/** Fonctions **/

/* Serveur */
point soustraire_points(point p1, point p2);
mur *duplique_murs(mur *murs, int nb);
int compare_murs(const void *arg1, const void *arg2);
int compare_spheres(const void *arg1, const void *arg2);
void tri_murs(mur *murs, int n);
void tri_spheres(sphere *sphere, int n);
int devant(mur m, sphere s);

objet3D *triFusion(mur *murs, int nbmur, sphere *spheres, int nbsphere);

int dessin_vers_murs(char *laby[], mur *murs);

void decale_murs(mur *murs, int nb, point position);
void decale_obj(objet3D *obj, int nb, point position);

void rotation_murs(mur *murs, int nb, int angle);
void rotation_obj(objet3D *obj, int nb, int angle);

void projete_murs(mur *murs, int nb, objet2D *objets, int *no);
void projete_obj(objet3D *obj, int nb, objet2D *objets, int *no);

void inter_poly_rect(point2D *orig, int no, point2D *result, int *nr);

void dessine_2D(objet2D *objet, int no);

int collisions(objet3D *obj, point position, int nb);

#endif
