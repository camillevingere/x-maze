[![Product Name Screen Shot][logo-screenshot]](https://archives.plil.fr/jdelabre/XMaze_VINGERE_DELABRE)

# Projet Système et Réseau

## Un programme en C pour jouer au jeu XMAZE !

## Le projet

[![Product Name Screen Shot][product-screenshot]](https://archives.plil.fr/jdelabre/XMaze_VINGERE_DELABRE)

Polytech’Lille, SE4, PSR

L'objectif du projet est de mettre en place un serveur de calcul de jeu pour envoyer les objets à afficher à d'autres clients.

### Réalisé avec

Ce projet a été réalisé avec les technologies suivantes

- [C](https://fr.wikipedia.org/wiki/C)

## Pour commencer

Il est nécéssaire d'installer la bibliothèque SDL. La SDL est une bibliothèque particulièrement utilisée pour créer des jeux en 2D.

### Prérequis

Vous pouvez l'installer comme ceci.

- SDL

  ```sh
  sudo apt install libsdl2-2.0-0 libsdl2-gfx-1.0-0 libsdl2-image-2.0-0 libsdl2-mixer-2.0-0 libsdl2-net-2.0-0 libsdl2-ttf-2.0-0
  ```

### Démarrage

1. Cloner le dépôt

   ```sh
   git clone https://archives.plil.fr/jdelabre/XMaze_VINGERE_DELABRE.git
   ```

2. Lancer le serveur de jeu.

   ```sh
   cd Serveur
   ```

   ```sh
   ./serveur
   ```

3. Dans un autre terminal, lancer un client de jeu.

   ```sh
   cd Client
   ```

   ```sh
   ./client
   ```

4. Commencez à jouer !

## État actuel du projet

### Serveur

Le serveur est capable de :

- Diffuser à toutes les adresses IP du réseau le nom du serveur et le port TCP à utiliser pour la communication
- Gérer les joueurs (demander le pseudo, connexion, chat...)
- Gérer le jeu (recevoir les touches et mettre à jour la position des joueurs)
- Calculer l'environnement graphique pour le renvoyer au client

### Client

Le client est capable de :

- Recevoir les communications diffusées par les différents serveur
- Demander le pseudo, se connecter au serveur, gérer le chat...
- Créer la fenêtre graphique et envoyer les commandes
- Recevoir les objets à dessiner dans la fenêtre graphique envoyés par le serveur

### Librairies et techniques mises en oeuvre

Les différentes librairies et techniques mises en ouvre pour faire fonctionner le projet sont les suivantes :

- La librairie lib_thread pour lancer des threads et initialiser les mutex
- La librarie lib_game pour les structures de joueurs et les gérer
- La libraire lib_network pour les méthodes de communication UDP et TCP
- La librairie lib_lab pour gérer les objets 2D et les afficher en 3D
- La librairie Graphique pour l'affichage dans la fenêtre graphique
- La librairie lib_util pour toutes les fonctions qui servent de façon générale
- Un gestionnaire de signaux pour arrêter le programme proprement et éviter les fuites de mémoire et éventuellement les processus zombies

## Améliorations

Les améliorations que nous devrions/pourrions ajouter au projet sont les suivantes :

- Représenter les joueurs avec des sphères dans la fenêtre graphique
- Gérer les collisions
- Gérer les tirs
- Gérer les morts des joueurs
- Améliorer la sécurité côté serveur

## Contact

Julien Delabre - Julien.Delabre@polytech-lille.net
Camille Vingere - Camille.Vingere@polytech-lille.net

Lien du projet: [https://archives.plil.fr/jdelabre/XMaze_VINGERE_DELABRE](https://archives.plil.fr/jdelabre/XMaze_VINGERE_DELABRE)

[product-screenshot]: images/screenshot.png
[logo-screenshot]: images/logo.jpg
