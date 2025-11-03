#ifndef SOMMET_H
#define SOMMET_H

typedef struct Sommet Sommet;

/*
* Structure successeur qui matérialise un arc 
* Pour lequel l'extremite terminale est le sommet 's'
* 'suivant' est le successeur dans la liste de successeurs d'un sommet donné
*/
typedef struct Successeur {
    Sommet *s;
    int capacite;
    int flot;
    struct Successeur *suivant;
} Successeur;

/*
* Structur Permettant de stocker la liste des successeurs d'un sommet donné.
*/
typedef struct liste_successeurs {
    Successeur *tete;
    Successeur *queue;
} liste_successeurs;

struct Sommet {
    int value; // Valeur du sommet
    int index; // Index du sommet dans le Graphe 
    liste_successeurs *successeurs;
};

/*
* Affichage d'un sommet
*/
void afficher_sommet(Sommet *s);

/*
* A ffichage de la liste des successeurs d'un sommet
*/
void afficher_successeurs(Sommet *s);

/*
* Ajoute le sommet 's' à liste_successeurs du sommet 'origine'
* Avec un flot de valeur 'flot' et une capacité 'capacite'
*/
void ajouter_successeur(Sommet *origine, Sommet *s, int capacite, int flot);

/*
* Recherche le sommet 'dst' dans liste_successeurs de 'src'
* Renvoie l'adresse du successeur contenant le sommet 'dst'
* Sinon renvoie NULL
*/
Successeur* trouver_successeur(Sommet *src, Sommet *dst);

/*
* Permet de libérer l'espace alloué pour liste_successeurs du sommet 's' 
*/
void clear_sommet_liste_successeurs(Sommet *s);

#endif
