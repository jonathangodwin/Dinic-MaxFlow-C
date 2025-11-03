#include <stdio.h>
#include <stdlib.h>
#include "Sommet.h"

void afficher_sommet (Sommet *s) {
	if (s != NULL)
		printf ("Sommet %d\n", s->value);
	else
		printf ("NULL\n");
}

void afficher_successeurs(Sommet *s) {
	Successeur *succ;
	afficher_sommet (s);
	if (s->successeurs != NULL){ 
		
		succ = s->successeurs->tete;
		while (succ != NULL){
			printf("([%d], C = %d, F = %d) -> ", succ->s->value, succ->capacite, succ->flot);
			succ = succ->suivant;
		}			
	}

	printf("NULL\n");

}

void ajouter_successeur(Sommet * origine, Sommet * s, int capacite, int flot) {
	Successeur * succ = malloc(sizeof(Successeur));
	succ->s = s;
	succ->capacite = capacite;
	succ->flot = flot;
	succ->suivant = NULL;

	if (origine->successeurs == NULL){
		origine->successeurs = malloc(sizeof(liste_successeurs));
		origine->successeurs->tete = NULL;
	}
	 
	if (origine->successeurs->tete == NULL){ 
		origine->successeurs->tete = succ;
		origine->successeurs->queue = succ;
	} else {
		origine->successeurs->queue->suivant = succ;
		origine->successeurs->queue = succ;
	}
} 

Successeur* trouver_successeur(Sommet *src, Sommet *dst) {
    if (src == NULL || src->successeurs == NULL) return NULL;
    
    Successeur *succ = src->successeurs->tete;
    while (succ != NULL) {
        if (succ->s == dst) return succ;
        succ = succ->suivant;
    }
    return NULL;
}

void clear_sommet_liste_successeurs(Sommet *s) {
    if (s != NULL && s->successeurs != NULL) {
        Successeur *succ_courant = s->successeurs->tete;
        while (succ_courant != NULL) {
            Successeur *succ_suivant = succ_courant->suivant;
            free(succ_courant);
            succ_courant = succ_suivant;
        }
        free(s->successeurs);
        s->successeurs = NULL;
    }
} 
