#include <stdio.h>
#include <stdlib.h>
#include "Chemin.h"

// ========== FONCTIONS POUR LA GESTION DES CHEMINS ==========

Chemin* creer_chemin() {
    Chemin *c = malloc(sizeof(Chemin));
    c->tete = NULL;
    c->queue = NULL;
    c->longueur = 0;
    return c;
}

void ajouter_au_chemin(Chemin *chemin, Sommet *sommet, Successeur *arc) {
    NoeudChemin *nouveau = malloc(sizeof(NoeudChemin));
    nouveau->sommet = sommet;
    nouveau->arc_utilise = arc;
    nouveau->suivant = NULL;
    
    if (chemin->queue == NULL) {
        chemin->tete = nouveau;
        chemin->queue = nouveau;
    } else {
        chemin->queue->suivant = nouveau;
        chemin->queue = nouveau;
    }
    chemin->longueur++;
}

void liberer_chemin(Chemin *chemin) {
    if (chemin == NULL) return;
    
    NoeudChemin *courant = chemin->tete;
    while (courant != NULL) {
        NoeudChemin *suivant = courant->suivant;
        free(courant);
        courant = suivant;
    }
    free(chemin);
}

void afficher_chemin(Chemin *chemin) {
    if (chemin == NULL || chemin->tete == NULL) {
        printf("Chemin vide\n");
        return;
    }
    
    NoeudChemin *courant = chemin->tete;
    printf("Chemin: ");
    while (courant != NULL) {
        printf("%d", courant->sommet->value);
        if (courant->suivant != NULL) printf(" -> ");
        courant = courant->suivant;
    }
    printf(" (longueur: %d)\n", chemin->longueur);
}

