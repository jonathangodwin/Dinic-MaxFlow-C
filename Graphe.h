#ifndef GRAPHE_H
#define GRAPHE_H
#include "Sommet.h"
#include "Chemin.h"

typedef struct Graphe {
    Sommet *sommets;
    int occupes; // nombre de sommets utilisés
    int n;       // taille max du tableau de sommets
} Graphe;


Graphe *buildGraph(char *file_name); 
// Construit un graphe à partir d’un fichier au format DIMACS.

void afficher_graphe(Graphe *G); 
// Affiche tous les sommets et leurs successeurs dans le graphe.

void clear_graphe(Graphe *g); 
// Libère la mémoire allouée pour le graphe et ses composants.

Sommet *ajouter_sommet(Graphe *g, int value); 
// Ajoute un sommet de valeur `value` au graphe s’il n’existe pas déjà.

Sommet *trouver_sommet(Graphe *g, int value); 
// Recherche un sommet par sa valeur. Retourne un pointeur vers le sommet ou NULL.

void ajouter_arc(Graphe *g, int src, int dst, int capacite, int flot); 
// Ajoute un arc entre deux sommets, en créant les sommets s’ils n’existent pas.

// Fonctions spécifiques à Dinic
void buildRG(Graphe *rG, Graphe *g); 
// Construit le graphe résiduel à partir du graphe original.

Chemin* shortestPath(Graphe *g, Sommet **file, Sommet **predecesseurs, Successeur **arcs_predecesseurs, int *distances, int *visite, Sommet* source, Sommet *puits); 
// Calcule le plus court chemin de la source au puits en nombre d’arcs.

int minCapa(Graphe *g, Chemin *chemin); 
// Retourne la capacité minimale sur un chemin donné.

void updateFlowInRG(Graphe *rG, Chemin *chemin, int k); 
// Met à jour les flots dans le graphe résiduel après l’augmentation de flot `k`.

void updateFlowInNet(Graphe *reseau, Graphe *rG); 
// Met à jour les flots dans le graphe original en fonction du graphe résiduel.

int dinic(Graphe *g, int source, int puits); 
// Implémente l’algorithme de Dinic pour calculer le flot maximum.

#endif
