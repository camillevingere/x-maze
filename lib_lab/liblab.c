#include "./liblab.h"

/* Soustraction de deux points */

point soustraire_points(point p1, point p2)
{
  point p;
  p.x = p1.x - p2.x;
  p.y = p1.y - p2.y;
  p.z = p1.z - p2.z;
  return p;
}

/* Copie un labyrinthe */

mur *duplique_murs(mur *murs, int nb)
{
  mur *copie = malloc(nb * sizeof(mur));
  memcpy(copie, murs, nb * sizeof(mur));
  return copie;
}

/* Compare la distance entre deux murs */

int compare_murs(const void *arg1, const void *arg2)
{
  const mur *mur1 = arg1;
  const mur *mur2 = arg2;
  int cx1 = (mur1->p[0].x + mur1->p[3].x) / 2;
  int cz1 = (mur1->p[0].z + mur1->p[3].z) / 2;
  int cx2 = (mur2->p[0].x + mur2->p[3].x) / 2;
  int cz2 = (mur2->p[0].z + mur2->p[3].z) / 2;
  int d1 = abs(cx1) + abs(cz1);
  int d2 = abs(cx2) + abs(cz2);
  return d2 - d1;
}

/* Compare la distance entre deux spheres */

int compare_spheres(const void *arg1, const void *arg2)
{
  const sphere *s1 = arg1;
  const sphere *s2 = arg2;
  float cx1 = s1->centre.x;
  int cy1 = s1->centre.y;
  int cx2 = s2->centre.x;
  int cy2 = s2->centre.y;
  int d1 = abs(cx1) + abs(cy1);
  int d2 = abs(cx2) + abs(cy2);
  return d2 - d1;
  return sqrt(pow(cx1 - cx2, 2) + pow(cy1 - cy2, 2));
}

/* Tri des murs */

void tri_murs(mur *murs, int n)
{
  qsort(murs, n, sizeof(mur), compare_murs);
}

/* Tri des spheres */

void tri_spheres(sphere *sphere, int n)
{
  qsort(sphere, n, sizeof(sphere), compare_spheres);
}

/* Tri Fusion entre les murs et les disques */

/*return 1 si le mur est devant la sphere*/

int devant(mur m, sphere s)
{
  int mx1 = m.p[0].x;
  int mx2 = m.p[1].x;
  int mx3 = m.p[2].x;
  int mx4 = m.p[3].x;
  int my1 = m.p[0].y;
  int my2 = m.p[1].y;
  int my3 = m.p[2].y;
  int my4 = m.p[3].y;
  int sx = s.centre.x;
  int sy = s.centre.y;
  int d1 = sqrt(pow(mx1, 2) + pow(my1, 2));
  int d2 = sqrt(pow(mx2, 2) + pow(my2, 2));
  int d3 = sqrt(pow(mx3, 2) + pow(my3, 2));
  int d4 = sqrt(pow(mx4, 2) + pow(my4, 2));
  int d = sqrt(pow(sx, 2) + pow(sy, 2));
  if (d < d1 && d < d2 && d < d3 && d < d4)
    return 0;
  else
    return 1;
}

/*objet3D* triFusion(mur* murs, int nbmur, sphere* spheres, int nbsphere)
{
	objet3D obj3D[nbmur+nbsphere];
	int i=0;
	int j=0;
	int k=0;
	for(;i<nbmur && j<nbsphere;k++)
	{
		if(devant(murs[i],spheres[j]))
		{
			obj3D[k].type=TYPE_SPHERE;
			obj3D[k].def.s=spheres[j];
			j++;
		}	
		else 
		{
			obj3D[k].type=TYPE_MUR;
			obj3D[k].def.m=murs[i];
			i++;
		}
	}
	if(i<nbmur)
	{
		for(;i<nbmur;k++)
		{
			obj3D[k].type=TYPE_MUR;
			obj3D[k].def.m=murs[i];
			i++;
		}
	}
	else
	{
		for(;j<nbsphere;k++)
		{
			obj3D[k].type=TYPE_SPHERE;
			obj3D[k].def.s=spheres[j];
			j++;
		}
	}
	return obj3D;
}*/

/* Calcul des murs en fonction du dessin */

int dessin_vers_murs(char *laby[], mur *murs)
{
  int nb = 0;
  int i;
  for (i = 0; i < 9; i++)
  {
    int j;
    for (j = 0; j < 9; j++)
    {
      if (laby[2 * i][2 * j + 1] == '-')
      {
        murs[nb].p[0].x = j * MUR_TAILLE;
        murs[nb].p[0].z = i * MUR_TAILLE;
        murs[nb].p[0].y = 0;
        murs[nb].p[1].x = j * MUR_TAILLE;
        murs[nb].p[1].z = i * MUR_TAILLE;
        murs[nb].p[1].y = MUR_HAUTEUR;
        murs[nb].p[2].x = (j + 1) * MUR_TAILLE;
        murs[nb].p[2].z = i * MUR_TAILLE;
        murs[nb].p[2].y = MUR_HAUTEUR;
        murs[nb].p[3].x = (j + 1) * MUR_TAILLE;
        murs[nb].p[3].z = i * MUR_TAILLE;
        murs[nb++].p[3].y = 0;
      }
      if (i < 8 && laby[2 * i + 1][2 * j] == '|')
      {
        murs[nb].p[0].x = j * MUR_TAILLE;
        murs[nb].p[0].z = i * MUR_TAILLE;
        murs[nb].p[0].y = 0;
        murs[nb].p[1].x = j * MUR_TAILLE;
        murs[nb].p[1].z = i * MUR_TAILLE;
        murs[nb].p[1].y = MUR_HAUTEUR;
        murs[nb].p[2].x = j * MUR_TAILLE;
        murs[nb].p[2].z = (i + 1) * MUR_TAILLE;
        murs[nb].p[2].y = MUR_HAUTEUR;
        murs[nb].p[3].x = j * MUR_TAILLE;
        murs[nb].p[3].z = (i + 1) * MUR_TAILLE;
        murs[nb++].p[3].y = 0;
      }
    }
  }
  return nb;
}

/* Décalage d'un labyrinthe */

void decale_murs(mur *murs, int nb, point position)
{
  int i, j;
  for (i = 0; i < nb; i++)
    for (j = 0; j < 4; j++)
      murs[i].p[j] = soustraire_points(murs[i].p[j], position);
}

/* Déclage d'un objet */

/*void decale_obj(objet3D *obj, int nb, point position)
{
  int i, j;
  for (i = 0; i < nb; i++)
  {
    if (obj[i].type == TYPE_MUR)
    {
      for (j = 0; j < 4; j++)
        obj[i].def.m.p[j] = soustraire_points(obj[i].def.m.p[j], position);
    }
    else
    {
        obj[i].def.s.centre = soustraire_points(obj[i].def.s.centre, position);
    }
  }
}*/

/* Rotation d'un labyrinthe */

void rotation_murs(mur *murs, int nb, int angle)
{
  int i, j;
  float radians = -2 * M_PI * angle / 360;
  for (i = 0; i < nb; i++)
  {
    for (j = 0; j < 4; j++)
    {
      int x = murs[i].p[j].x * cos(radians) + murs[i].p[j].z * sin(radians);
      int y = murs[i].p[j].y;
      int z = -murs[i].p[j].x * sin(radians) + murs[i].p[j].z * cos(radians);
      murs[i].p[j].x = x;
      murs[i].p[j].y = y;
      murs[i].p[j].z = z;
    }
  }
}

/* Rotation d'un objet */

void rotation_obj(objet3D *obj, int nb, int angle)
{
  int i, j;
  float radians = -2 * M_PI * angle / 360;
  for (i = 0; i < nb; i++)
  {
    if (obj[i].type == TYPE_MUR)
    {
      for (j = 0; j < 4; j++)
      {
        int x = obj[i].def.m.p[j].x * cos(radians) + obj[i].def.m.p[j].z * sin(radians);
        int y = obj[i].def.m.p[j].y;
        int z = -obj[i].def.m.p[j].x * sin(radians) + obj[i].def.m.p[j].z * cos(radians);
        obj[i].def.m.p[j].x = x;
        obj[i].def.m.p[j].y = y;
        obj[i].def.m.p[j].z = z;
      }
    }
    else
    {
      int x = obj[i].def.s.centre.x * cos(radians) + obj[i].def.s.centre.z * sin(radians);
      int y = obj[i].def.s.centre.y;
      int z = -obj[i].def.s.centre.x * sin(radians) + obj[i].def.s.centre.z * cos(radians);
      obj[i].def.s.centre.x = x;
      obj[i].def.s.centre.y = y;
      obj[i].def.s.centre.z = z;
    }
  }
}

/* Projection en 2D */

/*void projete_obj(objet3D *obj, int nb, objet2D *objets, int *no)
{
  int i, j;
  *no = 0;
  for (i = 0; i < nb; i++)
  {
    if (obj[i].type == TYPE_MUR)
    {
      int x1 = obj[i].def.m.p[0].x;
      int z1 = obj[i].def.m.p[0].z;
      int x2 = obj[i].def.m.p[3].x;
      int z2 = obj[i].def.m.p[3].z;
      if (z1 <= 0 && z2 <= 0)
        continue;
      if (z1 <= 0)
      {
        obj[i].def.m.p[0].x = x2 + (x1 - x2) * (1 - z2) / (z1 - z2);
        obj[i].def.m.p[0].z = 1;
        obj[i].def.m.p[1].x = obj[i].def.m.p[0].x;
        obj[i].def.m.p[1].z = obj[i].def.m.p[0].z;
      }
      if (z2 <= 0)
      {
        obj[i].def.m.p[3].x = x1 + (x2 - x1) * (1 - z1) / (z2 - z1);
        obj[i].def.m.p[3].z = 1;
        obj[i].def.m.p[2].x = obj[i].def.m.p[3].x;
        obj[i].def.m.p[2].z = obj[i].def.m.p[3].z;
      }
      objets[*no].type = TYPE_MUR;
      for (j = 0; j < 4; j++)
      {
        int z = obj[i].def.m.p[j].z;
        int x = obj[i].def.m.p[j].x;
        int y = obj[i].def.m.p[j].y;
        int px, py;
        px = LARGEUR / 2 + x * FOCALE / z;
        py = HAUTEUR / 4 + (y - HAUTEUR / 4) * FOCALE / z;
        objets[*no].def.p[j].x = px;
        objets[*no].def.p[j].y = py;
#ifdef DEBUG
        printf("[%d,%d,%d](%d,%d) ", x, y, z, px, py);
#endif
      }
      (*no)++;
#ifdef DEBUG
      printf("\n");
#endif
    }
    else
    {
      int x = obj[i].def.s.centre.x;
      int z = obj[i].def.s.centre.z;
      int y = obj[i].def.s.centre.y;
      if (z <= 0)
        continue;
      objets[*no].type = TYPE_SPHERE;
      int px,py;
      px = LARGEUR / 2 + x * FOCALE / z;
      py = HAUTEUR / 4 + (y - HAUTEUR / 4) * FOCALE / z;
      objets[*no].def.s.centre.x = px;
      objets[*no].def.s.centre.y = py;
      objets[*no].def.s.rayon = obj[i].def.s.rayon * FOCALE / z;
      (*no)++;
    }
  }
}*/

void projete_murs(mur *murs, int nb, objet2D *objets, int *no)
{
  int i, j;
  *no = 0;
  for (i = 0; i < nb; i++)
  {
    int x1 = murs[i].p[0].x;
    int z1 = murs[i].p[0].z;
    int x2 = murs[i].p[3].x;
    int z2 = murs[i].p[3].z;
    if (z1 <= 0 && z2 <= 0)
      continue;
    if (z1 <= 0)
    {
      murs[i].p[0].x = x2 + (x1 - x2) * (1 - z2) / (z1 - z2);
      murs[i].p[0].z = 1;
      murs[i].p[1].x = murs[i].p[0].x;
      murs[i].p[1].z = murs[i].p[0].z;
    }
    if (z2 <= 0)
    {
      murs[i].p[3].x = x1 + (x2 - x1) * (1 - z1) / (z2 - z1);
      murs[i].p[3].z = 1;
      murs[i].p[2].x = murs[i].p[3].x;
      murs[i].p[2].z = murs[i].p[3].z;
    }
    objets[*no].type = TYPE_MUR;
    for (j = 0; j < 4; j++)
    {
      int z = murs[i].p[j].z;
      int x = murs[i].p[j].x;
      int y = murs[i].p[j].y;
      int px, py;
      px = LARGEUR / 2 + x * FOCALE / z;
      py = HAUTEUR / 4 + (y - HAUTEUR / 4) * FOCALE / z;
      objets[*no].def.p[j].x = px;
      objets[*no].def.p[j].y = py;
#ifdef DEBUG
      printf("[%d,%d,%d](%d,%d) ", x, y, z, px, py);
#endif
    }
    (*no)++;
#ifdef DEBUG
    printf("\n");
#endif
  }
}

/* Intersection d'un segment */

unsigned char inter_seg_v(point2D a, point2D b, int x, int *y)
{
  if (sign(a.x - x) == sign(b.x - x))
    return 0;
  *y = a.y + (b.y - a.y) * (x - a.x) / (b.x - a.x);
  return 1;
}

unsigned char inter_seg_h(point2D a, point2D b, int *x, int y)
{
  if (sign(a.y - y) == sign(b.y - y))
    return 0;
  *x = a.x + (b.x - a.x) * (y - a.y) / (b.y - a.y);
  return 1;
}

/* Intersection d'un polygone avec un rectangle */

void inter_poly_rect(point2D *orig, int no, point2D *result, int *nr)
{
  point2D avant[POINTS_MAX];
  point2D apres[POINTS_MAX];
  int i, j;
  for (i = 0; i < no; i++)
    avant[i] = orig[i];
#ifdef DEBUG
  printf("??");
  for (i = 0; i < no; i++)
    printf("(%d,%d) ", avant[i].x, avant[i].y);
  printf("\n");
#endif
  int nv = no;
  for (i = 0; i < 4; i++)
  {
    int np = 0;
    for (j = 0; j < nv; j++)
    {
      int p = (j + nv - 1) % nv;
      point2D a = avant[p];
      point2D b = avant[j];
      int x, y;
      unsigned char inta, intb;
      switch (i)
      {
      case 0:
        x = 0;
        inter_seg_v(a, b, x, &y);
        if (a.x >= 0)
          inta = 1;
        else
          inta = 0;
        if (b.x >= 0)
          intb = 1;
        else
          intb = 0;
        break;
      case 1:
        y = HAUTEUR;
        inter_seg_h(a, b, &x, y);
        if (a.y <= HAUTEUR)
          inta = 1;
        else
          inta = 0;
        if (b.y <= HAUTEUR)
          intb = 1;
        else
          intb = 0;
        break;
      case 2:
        x = LARGEUR;
        inter_seg_v(a, b, x, &y);
        if (a.x <= LARGEUR)
          inta = 1;
        else
          inta = 0;
        if (b.x <= LARGEUR)
          intb = 1;
        else
          intb = 0;
        break;
      case 3:
        y = 0;
        inter_seg_h(a, b, &x, y);
        if (a.y >= 0)
          inta = 1;
        else
          inta = 0;
        if (b.y >= 0)
          intb = 1;
        else
          intb = 0;
        break;
      }
      if (intb)
      {
        if (!inta)
        {
          apres[np].x = x;
          apres[np].y = y;
          np++;
        }
        apres[np++] = b;
      }
      else
      {
        if (inta)
        {
          apres[np].x = x;
          apres[np].y = y;
          np++;
        }
      }
    }
    for (j = 0; j < np; j++)
      avant[j] = apres[j];
    nv = np;
  }
  for (i = 0; i < nv; i++)
    result[i] = avant[i];
#ifdef DEBUG
  printf("!!");
  for (i = 0; i < nv; i++)
    printf("(%d,%d) ", result[i].x, result[i].y);
  printf("\n");
#endif
  *nr = nv;
}

/* Dessin d'un labyrinthe */

void dessine_2D(objet2D *objet, int no)
{
  int i, j;
  short int x[POINTS_MAX];
  short int y[POINTS_MAX];
  for (i = 0; i < no; i++)
  {
    if (objet[i].type == TYPE_MUR)
    {
      point2D poly[POINTS_MAX];
      int np;
      inter_poly_rect(objet[i].def.p, 4, poly, &np);
      for (j = 0; j < np; j++)
      {
        x[j] = poly[j].x;
        y[j] = HAUTEUR - poly[j].y;
      }
      polygonePlein(x, y, np, COULEUR_ROUGE, COULEUR_ROSE);
    }
    else
    {
      disque(objet[i].def.s.centre.x, objet[i].def.s.centre.y, objet[i].def.s.rayon, COULEUR_BLEU, COULEUR_VERT);
    }
  }
}

int collisions(objet3D *obj, point position, int nb)
{
  int i;
  for (i = 0; i < nb; i++)
  {
    if (obj[i].type == TYPE_MUR)
    {
      int x1 = obj[i].def.m.p[0].x;
      int y1 = obj[i].def.m.p[0].z;
      int x2 = obj[i].def.m.p[1].x;
      int y2 = obj[i].def.m.p[1].z;
      int a, b;
      a = (y2 - y1) / (x2 - x1);
      b = y1 - a * x1;
      int d1 = sqrt(pow(position.x - x1, 2) + pow(position.z - y1, 2));
      int d2 = sqrt(pow(position.x - x2, 2) + pow(position.z - y2, 2));
      int d3 = abs(a * position.x - position.y + b) / sqrt(pow(a, 2) + 1);
      if (d1 < DISTANCE_MAX_COLLISION || d2 < DISTANCE_MAX_COLLISION || d3 < DISTANCE_MAX_COLLISION)
        return 1;
    }
    else
    {
      if (sqrt(pow(position.x - obj[i].def.s.centre.x, 2) + pow(position.z - obj[i].def.s.centre.z, 2)) < DISTANCE_MAX_COLLISION)
        return 1;
    }
  }
  return 0;
}

/* Fonction principale */

/*int main(void)
{
  unsigned char resultat = creerFenetre(LARGEUR, HAUTEUR, TITRE);
  if (!resultat)
  {
    fprintf(stderr, "Problème graphique !\n");
    exit(-1);
  }
  int nb = dessin_vers_murs(laby, murs);
  point p = {LABY_X / 2 * MUR_TAILLE, 0, MUR_TAILLE};
  int angle = 0;
  int touche;
  unsigned char fenetre, quitter;
  while (1)
  {
    int evenement = attendreEvenement(&touche, &fenetre, &quitter);
    if (!evenement)
    {
      usleep(ATTENTE);
      continue;
    }
    if (touche)
    {
      if (touche == TOUCHE_DROITE)
        angle += 5;
      if (touche == TOUCHE_GAUCHE)
        angle -= 5;
      if (angle < 0 || angle > 360)
        angle = angle % 360;
      if (touche == TOUCHE_HAUT)
      {
        p.x += MUR_TAILLE / 10 * sin(2 * M_PI * angle / 360);
        p.z += MUR_TAILLE / 10 * cos(2 * M_PI * angle / 360);
      }
      if (touche == TOUCHE_BAS)
      {
        p.x -= MUR_TAILLE / 10 * sin(2 * M_PI * angle / 360);
        p.z -= MUR_TAILLE / 10 * cos(2 * M_PI * angle / 360);
      }
    }
    if (quitter == 1)
      break;
    if (touche || fenetre)
    {
      mur *m2 = duplique_murs(murs, nb);
      decale_murs(m2, nb, p);
      rotation_murs(m2, nb, angle);
      tri_murs(m2, nb);
      objet2D *objets = malloc(nb * sizeof(objet2D));
      int no;
      projete_murs(m2, nb, objets, &no);
      free(m2);
      effacerFenetre();
      dessine_2D(objets, no);
      free(objets);
      synchroniserFenetre();
    }
  }
  fermerFenetre();
  return 0;
}*/
