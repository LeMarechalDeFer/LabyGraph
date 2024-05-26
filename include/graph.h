#ifndef __GRAPH__H__
#define __GRAPH__H__

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <raylib.h>
#include <time.h>


// Définition des constantes
#define MAX_NODES 100
#define MAX_EDGES 200
#define MAX_ENEMIES 20
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MAX_HEALTH 100 // -> à redéfinir après l'implémentation de l'algo

#define TROLL 0
#define LUTIN 1
#define GARDE 2



//definition des level ac

typedef enum GameScreen 
    { 
        LOGO = 0, TITLE,LEVEL1, GAMEPLAY, EXPLANATION ,ENDING
    }GameScreen;
    

// Directions
#define N 1
#define S 2
#define E 4
#define W 8

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
    int type;      // Type d'ennemi (troll, gobelin, etc.)
    int x;
    int y;           // Enemy position
    int strength;   // Force ou nombre d'ennemis dans un couloir
    int radius;
} Enemy;



// Structure pour représenter le joueur
typedef struct Player {
    int currentNode;    // Noeud où se trouve actuellement le joueur
    int health;         // Santé du joueur
} Player;

typedef struct character {
    int x;
    int y;
} character;

// Structure pour représenter le labyrinthe
typedef struct Maze {
    Graph graph;              // Graphe représentant le labyrinthe
    Enemy enemies[MAX_ENEMIES]; // Liste des ennemis dans le labyrinthe
    int numEnemies;           // Nombre total d'ennemis dans le labyrinthe
    int width, height;        // Dimensions du labyrinthe
    int **grid;               // Grille de bits représentant les passages du labyrinthe
    Player player;            // Le joueur
} Maze;



// Fonctions pour gérer le graphe
void InitializeGraph(Graph *graph);
void AddNode(Graph *graph, int x, int y);
void AddEdge(Graph *graph, int start, int end, int weight);



// Fonctions pour gérer le joueur
void InitializePlayer(Player *player, int startNode);
void MovePlayer(Player *player, int nextNode);


// Fonctions pour gérer le labyrinthe
void InitializeMaze(Maze *maze, int width, int height); // void InitializeMaze(Maze *maze);
void GenerateMazeRecursiveBacktracker(Maze *maze); // void GenerateMaze(Maze *maze, int complexity);
void RenderMaze(Maze *maze);
void CarvePassagesFrom(int cx, int cy, Maze *maze);
void FreeMaze(Maze *maze); // Ajout de cette déclaration


// Fonctions utilitaires
int GetNodeIndexById(Graph *graph, int id);
void PrintPath(int *predecessors, int startNode, int goalNode);
void InitializeEnemies(Enemy enemies[], Maze *maze);

// Algorithmes de plus court chemin
void BellmanFord(Graph *graph, int startNode, int *distances, int *predecessors);
void Dijkstra(Graph *graph, int startNode, int *distances, int *predecessors);
void AStar(Graph *graph, int startNode, int goalNode, int *distances, int *predecessors);


// Fonction pour colorier les arêtes du graphe (assignation des types d'ennemis)
void ColorEdges(Graph *graph);

#endif // __GRAPH__H__