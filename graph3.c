#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <raylib.h>
#include <time.h>

#define MAX_NODES 100
#define MAX_EDGES 200
#define MAX_ENEMIES 10

typedef struct {
    int id;
    int x;
    int y;
} Node;

typedef enum {
    L,
    T,
    G, 
} EnemyType;

typedef struct {
    int start;
    int end;
    int number_enemy; 
    EnemyType enemy_type;
} Edge;

typedef struct {
    Node nodes[MAX_NODES];
    Edge edges[MAX_EDGES];
    int numNodes;
    int numEdges;
} Graph;

typedef struct {
    int currentNode;
    int health;
} Player;

typedef struct {
    Graph graph;
} Maze;

// Tableau de lettres associées aux types d'ennemis
const char *enemyLetters[] = { "L", "T", "G" };

// Function prototypes
void InitializeGraph(Graph *graph);
void AddNode(Graph *graph, int x, int y);
void AddEdge(Graph *graph, int start, int end, EnemyType enemyType, int number_enemy);
void InitializePlayer(Player *player, int startNode);
void MovePlayer(Player *player, int nextNode);
void InitializeMaze(Maze *maze);
void InitializeMazeLevel1(Maze *maze);
void RenderMaze(Maze *maze);
void PrintPath(int *predecessors, int startNode, int goalNode);
void GenerateMaze(Maze *maze, int complexity);

void InitializeGraph(Graph *graph) {
    graph->numNodes = 0;
    graph->numEdges = 0;
}

void AddNode(Graph *graph, int x, int y) {
    if (graph->numNodes < MAX_NODES) {
        graph->nodes[graph->numNodes].id = graph->numNodes;
        graph->nodes[graph->numNodes].x = x;
        graph->nodes[graph->numNodes].y = y;
        graph->numNodes++;
    }
}

void AddEdge(Graph *graph, int start, int end, EnemyType enemyType, int number_enemy) {
    if (graph->numEdges < MAX_EDGES) {
        graph->edges[graph->numEdges].start = start;
        graph->edges[graph->numEdges].end = end;
        graph->edges[graph->numEdges].enemy_type = enemyType;
        graph->edges[graph->numEdges].number_enemy = number_enemy;
        graph->numEdges++;
    }
}

void InitializePlayer(Player *player, int startNode) {
    player->currentNode = startNode;
    player->health = 100; // Ex: initial health value
}

void MovePlayer(Player *player, int nextNode) {
    player->currentNode = nextNode;
}

void InitializeMaze(Maze *maze) {
    InitializeGraph(&maze->graph);
}

void InitializeMazeLevel1(Maze *maze) {
    InitializeMaze(maze);

    // Exemple simple de génération d'un labyrinthe 5x5
    int width = 5;
    int height = 5;

    // Ajouter des noeuds
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            AddNode(&maze->graph, x, y);
        }
    }

    // Ajouter des arêtes avec poids (nombre d'ennemis)
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int currentNode = y * width + x;
            if (x < width - 1) { // Ajouter une arête vers la droite
                AddEdge(&maze->graph, currentNode, currentNode + 1, rand() % 3, rand() % 10 + 1);
            }
            if (y < height - 1) { // Ajouter une arête vers le bas
                AddEdge(&maze->graph, currentNode, currentNode + width, rand() % 3, rand() % 10 + 1);
            }
        }
    }
}

void RenderMaze(Maze *maze) {
    for (int i = 0; i < maze->graph.numEdges; i++) {
        Edge edge = maze->graph.edges[i];
        Node startNode = maze->graph.nodes[edge.start];
        Node endNode = maze->graph.nodes[edge.end];

        DrawLine(startNode.x * 100, startNode.y * 100, endNode.x * 100, endNode.y * 100, BLACK);

        char enemyLabel[10];
        snprintf(enemyLabel, sizeof(enemyLabel), "%s %d", enemyLetters[edge.enemy_type], edge.number_enemy);
        DrawText(enemyLabel, (startNode.x * 100 + endNode.x * 100) / 2, (startNode.y * 100 + endNode.y * 100) / 2, 20, RED);
    }
}

void PrintPath(int *predecessors, int startNode, int goalNode) {
    // Paramètres inutilisés
    (void)predecessors;
    (void)startNode;
    (void)goalNode;
}

// Fonctionnalité utilitaire non implémentée (supprimez ces avertissements ou utilisez-les plus tard)
void GenerateMaze(Maze *maze, int complexity) {
    // Paramètres inutilisés
    (void)maze;
    (void)complexity;
}

int main() {
    // Initialisation du générateur de nombres aléatoires
    srand(time(NULL));
    
    // Initialisation de la fenêtre Raylib
    InitWindow(800, 600, "Maze Game");

    // Initialisation des structures
    Maze maze;
    Player player;

    // Initialiser le niveau 1 du labyrinthe
    InitializeMazeLevel1(&maze);

    // Initialiser le joueur au premier nœud
    InitializePlayer(&player, 0);

    // Boucle de jeu principale
    while (!WindowShouldClose()) {
        // Déplacement du joueur (exemple de mouvement avec les touches fléchées)
        if (IsKeyPressed(KEY_RIGHT)) {
            MovePlayer(&player, player.currentNode + 1);
        }
        if (IsKeyPressed(KEY_LEFT)) {
            MovePlayer(&player, player.currentNode - 1);
        }
        if (IsKeyPressed(KEY_DOWN)) {
            MovePlayer(&player, player.currentNode + 5);
        }
        if (IsKeyPressed(KEY_UP)) {
            MovePlayer(&player, player.currentNode - 5);
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Rendu du labyrinthe
        RenderMaze(&maze);

        // Rendu du joueur
        Node currentNode = maze.graph.nodes[player.currentNode];
        DrawCircle(currentNode.x * 100, currentNode.y * 100, 10, BLUE);

        EndDrawing();
    }

    // Fermeture de la fenêtre Raylib
    CloseWindow();

    return 0;
}
