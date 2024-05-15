#ifndef __GRAPH__H__
#define __GRAPH__H__

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <raylib.h>


// Définition des constantes
#define MAX_NODES 100
#define MAX_EDGES 200
#define MAX_ENEMIES 50
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

// Structure pour représenter un sommet (noeud) dans le graphe
typedef struct Node {
    int id;         // Identifiant unique du noeud
    int x, y;       // Coordonnées du noeud (utiles pour la visualisation)
} Node;

// Structure pour représenter une arête (edge) dans le graphe
typedef struct Edge {
    int start;      // Identifiant du noeud de départ
    int end;        // Identifiant du noeud de fin
    int weight;     // Poids de l'arête (nombre d'ennemis dans le couloir)
} Edge;

// Structure pour représenter un graphe
typedef struct Graph {
    Node nodes[MAX_NODES];    // Liste des noeuds
    Edge edges[MAX_EDGES];    // Liste des arêtes
    int numNodes;             // Nombre de noeuds dans le graphe
    int numEdges;             // Nombre d'arêtes dans le graphe
} Graph;

// Structure pour représenter un ennemi
typedef struct Enemy {
    int type;       // Type d'ennemi (troll, gobelin, etc.)
    int strength;   // Force ou nombre d'ennemis dans un couloir
} Enemy;

// Structure pour représenter le joueur
typedef struct Player {
    int currentNode;   // Noeud actuel où se trouve le joueur
    int health;        // Santé du joueur
} Player;

// Structure pour représenter le labyrinthe
typedef struct Maze {
    Graph graph;                   // Le graphe représentant le labyrinthe
    Player player;                 // Le joueur
    Enemy enemies[MAX_ENEMIES];    // Liste des ennemis
    int numEnemies;                // Nombre d'ennemis
} Maze;

// Fonctions pour gérer le graphe
void InitializeGraph(Graph *graph);
void AddNode(Graph *graph, int x, int y);
void AddEdge(Graph *graph, int start, int end, int weight);

// Fonctions pour gérer le joueur
void InitializePlayer(Player *player, int startNode);
void MovePlayer(Player *player, int nextNode);

// Fonctions pour gérer le labyrinthe
void InitializeMaze(Maze *maze);
void GenerateMaze(Maze *maze, int complexity);
void RenderMaze(Maze *maze);

// Algorithmes de plus court chemin
void BellmanFord(Graph *graph, int startNode, int *distances, int *predecessors);
void Dijkstra(Graph *graph, int startNode, int *distances, int *predecessors);
void AStar(Graph *graph, int startNode, int goalNode, int *distances, int *predecessors);

// Fonctions utilitaires
int GetNodeIndexById(Graph *graph, int id);
void PrintPath(int *predecessors, int startNode, int goalNode);



#endif // __GRAPH__H__