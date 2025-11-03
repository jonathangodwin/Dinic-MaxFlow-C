#ifndef CHEMIN_H
#define CHEMIN_H

#include "Sommet.h"

/*
* Noeud d'un chemin donné
*/
typedef struct NoeudChemin {
    Sommet *sommet;
    Successeur *arc_utilise;
    struct NoeudChemin *suivant;
} NoeudChemin;

typedef struct {
    NoeudChemin *tete;
    NoeudChemin *queue;
    int longueur; // Longueur du chemin
} Chemin;
 /*
 * Création d'un chemin
 */
Chemin* creer_chemin();
/*
* Ajout d'un arc à un chemin donné
*/
void ajouter_au_chemin(Chemin *chemin, Sommet *sommet, Successeur *arc);

/*
* Libère l'espace alloué pour un chemin donné 
*/
void liberer_chemin(Chemin *chemin);

/*
* Affichage d'un chemin 
*/
void afficher_chemin(Chemin *chemin);

#endif
