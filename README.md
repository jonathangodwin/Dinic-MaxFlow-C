# Dinic-MaxFlow-C

# Calculateur de Flot Maximum (Algorithme de Dinic)

![Langage](https://img.shields.io/badge/Langage-C-blue.svg)

Ce projet est une implémentation en C de l'algorithme de Dinic, conçu pour résoudre le problème du flot maximum dans un graphe orienté (un réseau de flot).

Le programme lit une description de graphe au format standard **DIMACS**, construit le réseau correspondant, puis calcule le flot maximum pouvant être envoyé d'un nœud source à un nœud puits. Ce projet a été réalisé dans un cadre académique, en mettant l'accent sur une structure de données modulaire et une implémentation efficace de l'algorithme.

## 🚀 Fonctionnalités

* **Lecture de Graphes** : Analyse syntaxique (parsing) de fichiers au format DIMACS pour les problèmes de flot.
* **Algorithme de Dinic** : Implémentation complète de l'algorithme de Dinic, incluant :
    * La construction d'un **graphe de niveaux** (via BFS).
    * La recherche de **chemins augmentants** (via DFS).
* **Graphe Résiduel** : Construction et mise à jour dynamiques du graphe résiduel (`buildRG`, `updateFlowInRG`).
* **Structures de Données Modulaires** : Le code est organisé en modules clairs :
    * `Graphe` : Gère le réseau principal.
    * `Sommet` : Gère les nœuds et leurs listes de successeurs (arcs).
    * `Chemin` : Gère la structure de données pour les chemins augmentants.
* **Gestion de la Compilation** : Un `Makefile` est fourni pour une compilation simple et efficace.

## 📂 Structure du Projet
├── main.c # Point d'entrée, charge les graphes et lance l'algorithme 
├── Graphe.h / Graphe.c # Cœur de l'implémentation (Graphe, Dinic, BFS, etc.) 
├── Sommet.h / Sommet.c # Gestion des sommets et des listes de successeurs (arcs) 
├── Chemin.h / Chemin.c # Gestion de la structure de données "Chemin" 
├── Makefile # Fichier de compilation 
├── R1.txt # Exemple de fichier graphe (DIMACS) 
├── R2.txt # Exemple de fichier graphe (DIMACS) 
├── G-*.txt # Autres exemples de graphes 
└── README.md # Ce fichier

## ⚙️ Compilation

Un `Makefile` est inclus pour simplifier le processus de compilation.

1.  **Nettoyer les fichiers objets (optionnel)** :
    Pour supprimer les fichiers objets (`.o`) et l'exécutable précédents.
    ```bash
    make clean
    ```
   

2.  **Compiler le projet** :
    Cette commande compile tous les fichiers `.c` et crée un exécutable nommé `main`.
    ```bash
    make
    ```
   

## 🏃 Exécution

L'exécutable `main` est configuré pour tester automatiquement les fichiers d'exemple (`R1.txt`, `R2.txt`, etc.) présents dans le dossier.

Pour lancer le programme après compilation :
```bash
./main
```

## Exemple de Sortie

Le programme affichera le flot maximum calculé pour chaque fichier de graphe testé :

===================== Graphe 'R1.txt' ======================
Source : 1
Puits : 4
Le flot Maximum associé au graphe 'R1.txt'  : 4
===================== Graphe 'R2.txt' ======================
Source : 1
Puits : 8
Le flot Maximum associé au graphe 'R2.txt'  : 10
...

📝 Format des Données (DIMACS)

Le programme utilise des fichiers d'entrée au format DIMACS pour les problèmes de flot maximum. Le format attendu est le suivant (basé sur l'analyse de Graphe.c) :

    Lignes de Commentaire (c) : c Ligne de commentaire

    Ligne de Problème (p) : p max <nombre_de_noeuds> <nombre_d_arcs> (Note: max n'est pas utilisé par le parseur mais est standard, p est sauté)

    Ligne de Nœud (n) :

        n <id_noeud> s (pour la source)

        n <id_noeud> t (pour le puits)

    Ligne d'Arc (a) : a <noeud_source> <noeud_destination> <capacite>

Exemple (basé sur R1.txt)

c Problème de flot simple
p max 4 5
c Source
n 1 s
c Puits
n 4 t
c Arcs
a 1 2 4
a 1 3 2
a 2 3 3
a 2 4 1
a 3 4 3

💡 Détails de l'Implémentation

Graphe (Graphe.c)

C'est le module principal. Il contient la structure Graphe (un tableau de Sommet) et la logique de l'algorithme :

    buildGraph : Lit un fichier DIMACS et construit la structure de graphe en mémoire.

    dinic : L'orchestrateur principal de l'algorithme. Il appelle shortestPath (BFS) en boucle pour trouver des chemins augmentants jusqu'à ce que le flot maximum soit atteint.

    buildRG : Construit le graphe résiduel initial.

    shortestPath : Implémentation d'un BFS (Breadth-First Search) pour trouver le plus court chemin augmentant de la source au puits dans le graphe résiduel.

    minCapa : Calcule la capacité résiduelle minimale le long d'un chemin trouvé.

    updateFlowInRG / updateFlowInNet : Mettent à jour les flots dans le graphe résiduel et le graphe original.

Sommet (Sommet.c)

Définit les structures Sommet (nœud) et Successeur (arc). Un sommet contient une liste chaînée de ses successeurs (liste_successeurs). Chaque successeur stocke le sommet de destination, la capacité de l'arc et le flot actuel.

Chemin (Chemin.c)

Fournit une structure de données simple de type "liste chaînée" (Chemin) pour stocker un chemin (une séquence de nœuds et d'arcs) trouvé par le BFS.
