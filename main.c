#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Graphe.h"


int main(void) {
	
	char *fichiers[4] = {"R1.txt","R2.txt", "G-900-2700.txt","G-2500-7500.txt"};
	int n = 4;
	

	for (int i = 0; i < n; i++){
		Graphe * G = buildGraph (fichiers[i]);
		
		printf ("===================== Graphe '%s' ======================\n", fichiers[i]);
		// afficher_graphe (G);
		int source = G->sommets[0].value;
		int puits = G->sommets[1].value;
		printf ("Source : %d\n", source);
		printf ("Puits : %d\n", puits);
		int flot_max = dinic (G, source, puits);
		printf ("Le flot Maximum associé au graphe '%s'  : %d\n", fichiers[i], flot_max);
		clear_graphe (G);		
	}

	
		

	
	return EXIT_SUCCESS;

}
