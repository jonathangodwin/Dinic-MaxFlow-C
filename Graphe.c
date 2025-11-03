#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "Sommet.h"
#include "Graphe.h"

#define MAX_LINE_SIZE 256

/*
* Construction d'un graphe g à partir d'un fichiers
* Sous le format DIMACS 
*/

Graphe * buildGraph(char *file_name) {
    Graphe * g = malloc(sizeof(Graphe));
	char ligne[MAX_LINE_SIZE]; // pour le passage de ligne
	int n_nodes; // nombre de sommets
	int m_arcs; // nombre d'arcs
	int source; // numero de la source
	int puits; // numero du puits
	   
	// Ouverture du fichier si possible sinon exit on error
	FILE * fp = fopen(file_name,"r");
	if (fp==NULL) {
		fprintf(stderr, "%s non accessible\n", file_name);
		exit(1);
	}
	   
	// Lecture du fichier
	// c Pb de ﬂot max : ligne de commentaire, on passe ...
	fgets(ligne,MAX_LINE_SIZE,fp);
	   
	//c pb lines (nodes, links) : ligne de commentaire, on passe ...
	fgets(ligne,MAX_LINE_SIZE,fp);
	
	//p NODES ARCS
	getc(fp);getc(fp); //on passe l'entete 'p '
	// nombre de sommets
	fscanf(fp,"%d",&n_nodes);
	//printf("%d nodes\n",n_nodes);

	g->sommets = malloc (sizeof (Sommet) * n_nodes);
	g->n = n_nodes;
	g->occupes = 0;
	

	// nombre d'arcs
	fscanf(fp,"%d",&m_arcs);
	//printf("%d arcs\n",m_arcs);
	// terminer le traitement de cette ligne :
	fgets(ligne,MAX_LINE_SIZE,fp);
	
	//c source : ligne de commentaire, on passe ...
	fgets(ligne,MAX_LINE_SIZE,fp);
	//n ID s : determine la source
	getc(fp);getc(fp); //on passe l'entete 'n '
	fscanf(fp,"%d", &source);
	//printf("source=%d\n",source);
	ajouter_sommet (g, source);
	// terminer le traitement de cette ligne :
	fgets(ligne,MAX_LINE_SIZE,fp);
   
	//c sink : ligne de commentaire, on passe ...
	fgets(ligne,MAX_LINE_SIZE,fp);
	//n ID t : determine le puits
	getc(fp);getc(fp); //n 
	fscanf(fp,"%d", &puits);
	//printf("puits=%d\n",puits);
	ajouter_sommet (g, puits);

	// terminer le traitement de cette ligne :
	fgets(ligne,MAX_LINE_SIZE,fp);
	
	//c arc (from, to, capa) : ligne de commentaire, on passe ...
	fgets(ligne,MAX_LINE_SIZE,fp);
	
	//lire les m_arcs lignes au format "a SRC DST CAP" de DIMACS pour obtenir les donnees d'arcs
	{
		int a; // pour iterer sur les arcs
		int src, dst, cap; // donnees correspondantes (SRC DST CAP)
		
		for(a=1;a<=m_arcs;a++) {
			getc(fp);getc(fp); //on passe l'entete 'a '
			fscanf(fp,"%d",&src);
			fscanf(fp,"%d",&dst);
			fscanf(fp,"%d",&cap);
			ajouter_arc (g, src, dst, cap, 0);
			//printf("arc %d de %d a %d, capa=%d\n", a, src, dst, cap);
		}
	}
	   
	fclose(fp);

    return g;

}
   
/*
* Affichage d'un graphe G
*/
void afficher_graphe(Graphe * G) {
	if (G != NULL && G->sommets != NULL){
		for(int i = 0; i < G->occupes; i++)
			afficher_successeurs(&G->sommets[i]);
	}

	else 
		printf ("NULL\n");
	
}


Sommet *ajouter_sommet(Graphe *g, int value) {
    if (g == NULL || g->occupes >= g->n) {
        printf("Graphe nul ou nombre de sommets maximal atteint\n");
        return NULL;
    }

    Sommet *s = trouver_sommet(g, value);
    if (s == NULL) {
        int index_sommet_courant = g->occupes;

        g->sommets[index_sommet_courant].value = value;
        g->sommets[index_sommet_courant].index = index_sommet_courant;

        g->sommets[index_sommet_courant].successeurs = malloc(sizeof(liste_successeurs));
        g->sommets[index_sommet_courant].successeurs->tete = NULL;

        g->occupes++;

        s = &g->sommets[index_sommet_courant];
    }

    return s;
}


Sommet *trouver_sommet (Graphe *g, int src) {
	Sommet * sommet_courant = (Sommet *) 0; 
	int i = 0;
	int trouve = 0;
	if (g != NULL && g->sommets != NULL) {
		while (i < g->occupes && !trouve){
			if (g->sommets[i].value == src) {
				sommet_courant = &g->sommets[i];
				trouve = 1;
			}

			i++;
		}		
	}

	return sommet_courant;
}



void ajouter_arc (Graphe *g, int src, int dst, int capacite, int flot) {
	Sommet *sommet_src = trouver_sommet (g, src);
	Sommet *sommet_dst = trouver_sommet (g, dst);
	if (sommet_src == NULL)
		sommet_src = ajouter_sommet (g, src);
		
	if (sommet_dst == NULL)
		sommet_dst = ajouter_sommet (g, dst);
	

	ajouter_successeur(sommet_src, sommet_dst, capacite, flot);
}


void buildRG(Graphe *rG, Graphe *g) {
    rG->n = g->n;
    rG->occupes = 0;
    rG->sommets = malloc(sizeof(Sommet) * rG->n);

    // Ajout des sommets au graphe résiduel
    for (int i = 0; i < g->occupes; i++) {
        ajouter_sommet(rG, g->sommets[i].value);
    }

    // Ajout  des arcs directs et inverses
    for (int i = 0; i < g->occupes; i++) {
        Sommet *u = &g->sommets[i];

        if (u->successeurs != NULL && u->successeurs->tete != NULL) {
            Successeur *succ = u->successeurs->tete;

            while (succ != NULL) {
                int v_val = succ->s->value;

                // Arc direct résiduel (u -> v)
                ajouter_arc(rG, u->value, v_val, succ->capacite, succ->flot);

                // Arc inverse (v -> u) avec capacité 0, mais flot identique
                ajouter_arc(rG, v_val, u->value, 0, succ->flot);

                succ = succ->suivant;
            }
        }
    }
}


Chemin* shortestPath(Graphe *g, Sommet **file, Sommet **predecesseurs, Successeur **arcs_predecesseurs, int *distances, int *visite, Sommet* source, Sommet *puits) {
    int n = g->n;

    // Vérification des paramètres d'entrée
    if (g == NULL) return NULL;
    if (source == NULL || puits == NULL) return NULL;

    int idx_source = source->index;

    // Initialisation des tableaux : distances, prédécesseurs, arcs et visite
    for (int i = 0; i < n; i++) {
        distances[i] = -1;          // -1 signifie sommet non encore atteint
        predecesseurs[i] = NULL;   // Aucun prédécesseur défini au départ
        arcs_predecesseurs[i] = NULL; // Aucun arc prédécesseur au départ
        visite[i] = 0;             // Sommet non visité
    }

    // Initialisation de la file pour le BFS
    int debut = 0, fin = 0;
    file[fin++] = source;          // On place la source dans la file

    distances[idx_source] = 0;     // Distance de la source à elle-même = 0
    visite[idx_source] = 1;        // Marque la source comme visitée

    int trouve = 0;                // Booléen indiquant si le puits est trouvé

    // Boucle principale du BFS pour explorer le graphe en largeur
    while (debut < fin && !trouve) {
        Sommet *courant = file[debut++];   // On récupère le sommet courant à traiter

        // Si on atteint le puits, on arrête la recherche
        if (courant == puits) {
            trouve = 1;
            break;
        }

        // On récupère la liste des successeurs du sommet courant
        Successeur *succ = courant->successeurs ? courant->successeurs->tete : NULL;

        int idx_courant = courant->index;

        // Exploration des successeurs accessibles
        while (succ != NULL) {
            int capacite_residuelle = succ->capacite - succ->flot; // Capacité résiduelle de l'arc

            int idx_voisin = succ->s->index;

            // Si l'arc a une capacité résiduelle > 0 et que le voisin n'est pas encore visité
            if (capacite_residuelle > 0 && idx_voisin != -1 && !visite[idx_voisin]) {
                visite[idx_voisin] = 1;                      // Marque le voisin comme visité
                distances[idx_voisin] = distances[idx_courant] + 1;  // Met à jour la distance
                predecesseurs[idx_voisin] = courant;        // Définit le prédécesseur pour reconstruire le chemin
                arcs_predecesseurs[idx_voisin] = succ;      // Mémorise l'arc utilisé
                file[fin++] = succ->s;                       // Ajoute le voisin à la file pour exploration future
            }

            succ = succ->suivant;  // Passe au successeur suivant
        }
    }

    Chemin *chemin = NULL;

    // Si le puits a été atteint, on reconstruit le chemin trouvé
    if (trouve) {
        chemin = creer_chemin();          // Création du chemin final
        Chemin *chemin_inverse = creer_chemin();  // Création d'un chemin temporaire à l'envers
        Sommet *actuel = puits;

        int idx_actuel = actuel->index;

        // Reconstruction du chemin à l'envers depuis le puits vers la source
        while (actuel != source) {
            ajouter_au_chemin(chemin_inverse, actuel, arcs_predecesseurs[idx_actuel]);
            actuel = predecesseurs[idx_actuel];

            // Mise à jour de l'index du sommet actuel
            for (int i = 0; i < g->occupes; i++) {
                if (&g->sommets[i] == actuel) {
                    idx_actuel = i;
                    break;
                }
            }
        }

        ajouter_au_chemin(chemin_inverse, source, NULL);  // Ajoute la source en début du chemin inverse

        // Inversion du chemin pour obtenir l'ordre source -> puits
        NoeudChemin *courant_inv = chemin_inverse->tete;
        Sommet **sommets_chemin = malloc(sizeof(Sommet*) * chemin_inverse->longueur);
        Successeur **arcs_chemin = malloc(sizeof(Successeur*) * chemin_inverse->longueur);

        int i = chemin_inverse->longueur - 1;
        while (courant_inv != NULL) {
            sommets_chemin[i] = courant_inv->sommet;
            arcs_chemin[i] = courant_inv->arc_utilise;
            courant_inv = courant_inv->suivant;
            i--;
        }

        // Ajout des sommets et arcs dans le chemin final dans le bon ordre
        for (int j = 0; j < chemin_inverse->longueur; j++) {
            ajouter_au_chemin(chemin, sommets_chemin[j], arcs_chemin[j]);
        }

        // Libération de la mémoire temporaire
        free(sommets_chemin);
        free(arcs_chemin);
        liberer_chemin(chemin_inverse);
    }

    // Retourne le chemin trouvé ou NULL si aucun chemin n'existe
    return chemin;
}


// Cette fonction retourne la capacité résiduelle minimale sur un chemin donné dans un graphe
int minCapa(Graphe *g, Chemin *chemin) {
    // Cas où le chemin est invalide ou trop court pour avoir une capacité
    if (chemin == NULL || chemin->tete == NULL || chemin->longueur < 2) {
        return 0;
    }
    
    int min_capacite = INT_MAX;           // Initialisation à une valeur très grande
    NoeudChemin *courant = chemin->tete->suivant;  // On commence au deuxième noeud (le premier n'a pas d'arc)

    // Parcours du chemin pour trouver la plus petite capacité résiduelle
    while (courant != NULL) {
        if (courant->arc_utilise != NULL) {
            // Calcul de la capacité résiduelle = capacité - flot actuel
            int capacite_residuelle = courant->arc_utilise->capacite - courant->arc_utilise->flot;
            
            // On met à jour min_capacite si on trouve une capacité plus petite
            if (capacite_residuelle < min_capacite) {
                min_capacite = capacite_residuelle;
            }
        }
        courant = courant->suivant;
    }
    
    // Si aucune capacité valide n'a été trouvée, on retourne 0, sinon la min_capacite trouvée
    return (min_capacite == INT_MAX) ? 0 : min_capacite;
}


// Met à jour les flots dans le graphe résiduel en augmentant le flot sur les arcs du chemin
void updateFlowInRG(Graphe *rG, Chemin *chemin, int k) {
    if (chemin == NULL || chemin->tete == NULL || k <= 0) return;

    NoeudChemin *courant = chemin->tete;

    // Parcours du chemin noeud par noeud (jusqu'à l'avant dernier)
    while (courant != NULL && courant->suivant != NULL) {
        NoeudChemin *suivant = courant->suivant;

        if (suivant->arc_utilise != NULL) {
            // On augmente le flot sur l'arc direct de k unités
            suivant->arc_utilise->flot += k;

            // On diminue le flot sur l'arc inverse (arc retour dans le graphe résiduel)
            Successeur *arc_inverse = trouver_successeur(suivant->sommet, courant->sommet);
            if (arc_inverse != NULL) {
                arc_inverse->flot -= k;
            }
        }

        courant = courant->suivant;
    }
}



// Met à jour les flots dans le graphe original à partir des flots du graphe résiduel
void updateFlowInNet(Graphe *reseau, Graphe *rG) {
    if (reseau == NULL || rG == NULL) return;

    // Parcours de tous les sommets du réseau original
    for (int i = 0; i < reseau->occupes; i++) {
        Sommet *u = &reseau->sommets[i];
        int u_index = u->index;
        Sommet *u_rg = &reseau->sommets[u_index];

        if (u->successeurs != NULL && u_rg != NULL && u_rg->successeurs != NULL) {
            Successeur *succ_reseau = u->successeurs->tete;

            // Parcours des successeurs de u dans le réseau original
            while (succ_reseau != NULL) {
                int v_index = succ_reseau->s->index;

                Sommet *v_rg = &rG->sommets[v_index];
                Successeur *succ_rg = trouver_successeur(u_rg, v_rg);

                if (succ_rg != NULL) {
                    // Mise à jour du flot dans le réseau original :
                    // flot = capacité - flot résiduel dans le graphe résiduel
                    succ_reseau->flot = succ_reseau->capacite - succ_rg->flot;
                }

                succ_reseau = succ_reseau->suivant;
            } 
        }
    }
}


// Implémentation de l'algorithme de flot maximum de Dinic 
int dinic(Graphe *g, int s, int p) {
    Graphe *rG = malloc(sizeof(Graphe));
    buildRG(rG, g);  // Construction du graphe résiduel à partir du graphe original

    Sommet *source = trouver_sommet(rG, s);
    Sommet *puits = trouver_sommet(rG, p);
    int n = g->n;

    // Allocation des structures pour la recherche de chemin 
    Sommet **file = malloc(sizeof(Sommet*) * n);
    int *distances = malloc(sizeof(int) * n);
    Sommet **predecesseurs = malloc(sizeof(Sommet*) * n);
    Successeur **arcs_predecesseurs = malloc(sizeof(Successeur*) * n);
    int *visite = malloc(sizeof(int) * n);
        
    if (g == NULL) return 0;
    
    
    int flot_max = 0;
    
    while (1) {
        // Recherche d'un chemin augmentant dans le graphe résiduel
        Chemin *chemin = shortestPath(rG, file, predecesseurs, arcs_predecesseurs, distances, visite, source, puits);
        
        if (chemin == NULL || chemin->longueur < 2) {
            // Aucun chemin augmentant trouvé, on arrête
            if (chemin != NULL) liberer_chemin(chemin);
            break;
        }
        
        // Trouver la capacité minimale sur ce chemin
        int min_cap = minCapa(rG, chemin);
        
        if (min_cap > 0) {
            // Mise à jour du flot dans le graphe résiduel
            updateFlowInRG(rG, chemin, min_cap);
            
            // Mise à jour du flot dans le graphe original à partir du graphe résiduel
            updateFlowInNet(g, rG);
            
            // Accumuler le flot maximum total
            flot_max += min_cap;
        }
        
        liberer_chemin(chemin);
        
        if (min_cap == 0) break;  // Plus d'augmentation possible
    }

    // Libération de la mémoire utilisée
    free(file);
    free(distances);
    free(predecesseurs);
    free(arcs_predecesseurs);
    free(visite);
    clear_graphe(rG);
    
    return flot_max;
}



void clear_graphe (Graphe *g) {
	for (int i = 0; i < g->occupes; i++)
		clear_sommet_liste_successeurs (&g->sommets[i]);
	free (g->sommets);
	free(g);
}
