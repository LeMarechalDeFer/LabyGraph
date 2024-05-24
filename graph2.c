/*#include "../include/graph.h"

// void InitializeGraph(Graph *graph) {
//     graph->numNodes = 0;
//     graph->numEdges = 0;
// }

// void AddNode(Graph *graph, int x, int y) {
//     if (graph->numNodes < MAX_NODES) {
//         graph->nodes[graph->numNodes].id = graph->numNodes;
//         graph->nodes[graph->numNodes].x = x;
//         graph->nodes[graph->numNodes].y = y;
//         graph->numNodes++;
//     }
// }

// void AddEdge(Graph *graph, int start, int end, int weight) {
//     if (graph->numEdges < MAX_EDGES) {
//         graph->edges[graph->numEdges].start = start;
//         graph->edges[graph->numEdges].end = end;
//         graph->edges[graph->numEdges].weight = weight;
//         graph->numEdges++;
//     }
// }

// void InitializePlayer(Player *player, int startNode) {
//     player->currentNode = startNode;
//     player->health = 100; // Ex: initial health value
// }

// void MovePlayer(Player *player, int nextNode) {
//     player->currentNode = nextNode;
// }

// void InitializeMaze(Maze *maze) {
//     InitializeGraph(&maze->graph);
//     maze->numEnemies = 0;
// }

// void GenerateMaze(Maze *maze, int complexity) {
//     // Implementation of maze generation algorithm
// }

// // void RenderMaze(Maze *maze) {
// //     // Implementation of rendering maze using Raylib
// // }

// int GetNodeIndexById(Graph *graph, int id) {
//     for (int i = 0; i < graph->numNodes; i++) {
//         if (graph->nodes[i].id == id) {
//             return i;
//         }
//     }
//     return -1; // Node not found
// }


// void InitializeMazeLevel1(Maze *maze) {
//     InitializeMaze(maze);

//     // Exemple simple de génération d'un labyrinthe 5x5
//     int width = 5;
//     int height = 5;

//     // Ajouter des noeuds
//     for (int y = 0; y < height; y++) {
//         for (int x = 0; x < width; x++) {
//             AddNode(&maze->graph, x, y);
//         }
//     }

//     // Ajouter des arêtes avec poids (nombre d'ennemis)
//     for (int y = 0; y < height; y++) {
//         for (int x = 0; x < width; x++) {
//             int currentNode = y * width + x;
//             if (x < width - 1) { // Ajouter une arête vers la droite
//                 AddEdge(&maze->graph, currentNode, currentNode + 1, rand() % 10);
//             }
//             if (y < height - 1) { // Ajouter une arête vers le bas
//                 AddEdge(&maze->graph, currentNode, currentNode + width, rand() % 10);
//             }
//         }
//     }

//     // Colorier les arêtes
//     ColorEdges(&maze->graph);
// }


// void ColorEdges(Graph *graph) {
//     int edgeColors[MAX_EDGES] = { -1 }; // Initialiser les couleurs des arêtes à -1 (non colorié)
//     bool availableColors[MAX_ENEMIES];  // Couleurs disponibles

//     for (int i = 0; i < graph->numEdges; i++) {
//         memset(availableColors, true, sizeof(availableColors));

//         // Vérifier les arêtes adjacentes pour éviter les conflits de couleur
//         for (int j = 0; j < graph->numEdges; j++) {
//             if (i != j && (graph->edges[i].start == graph->edges[j].start ||
//                            graph->edges[i].start == graph->edges[j].end ||
//                            graph->edges[i].end == graph->edges[j].start ||
//                            graph->edges[i].end == graph->edges[j].end)) {
//                 if (edgeColors[j] != -1) {
//                     availableColors[edgeColors[j]] = false;
//                 }
//             }
//         }

//         // Assigner la première couleur disponible
//         for (int color = 0; color < MAX_ENEMIES; color++) {
//             if (availableColors[color]) {
//                 edgeColors[i] = color;
//                 break;
//             }
//         }

//         // Assigner le type d'ennemi à l'arête
//         graph->edges[i].weight = edgeColors[i];
//     }
// }


// void RenderMaze(Maze *maze) {
//     for (int i = 0; i < maze->graph.numEdges; i++) {
//         Edge edge = maze->graph.edges[i];
//         Node startNode = maze->graph.nodes[edge.start];
//         Node endNode = maze->graph.nodes[edge.end];

//         DrawLine(startNode.x * 100, startNode.y * 100, endNode.x * 100, endNode.y * 100, BLACK);

//         const char *enemyTypes[] = { "T", "G", "L" }; // T for Troll, G for Gobelin, L for Lutin
//         const char *enemyType = enemyTypes[edge.weight % 3];
//         DrawText(enemyType, (startNode.x * 100 + endNode.x * 100) / 2, (startNode.y * 100 + endNode.y * 100) / 2, 20, RED);
//     }
// }




// void PrintPath(int *predecessors, int startNode, int goalNode) {
//     // Implementation of path printing
// }


// Directions
static const int DX[9] = {0, 1, 0, -1, 0};
static const int DY[9] = {0, 0, 1, 0, -1};
static const int OPPOSITE[9] = {0, S, N, 0, W, 0, 0, 0, E};

// Initialisation du graphe
void InitializeGraph(Graph *graph) {
    graph->numNodes = 0;
    graph->numEdges = 0;
}

// Ajouter un noeud
void AddNode(Graph *graph, int x, int y) {
    if (graph->numNodes < MAX_NODES) {
        graph->nodes[graph->numNodes].id = graph->numNodes;
        graph->nodes[graph->numNodes].x = x;
        graph->nodes[graph->numNodes].y = y;
        graph->numNodes++;
    }
}

// Ajouter une arête
void AddEdge(Graph *graph, int start, int end, int weight) {
    if (graph->numEdges < MAX_EDGES) {
        graph->edges[graph->numEdges].start = start;
        graph->edges[graph->numEdges].end = end;
        graph->edges[graph->numEdges].weight = weight;
        graph->numEdges++;
    }
}

// Initialisation du joueur
void InitializePlayer(Player *player, int startNode) {
    player->currentNode = startNode;
    player->health = 100; // Exemple : valeur initiale de la santé
}

// Déplacer le joueur
void MovePlayer(Player *player, int nextNode) {
    player->currentNode = nextNode;
}

// Initialisation du labyrinthe
void InitializeMaze(Maze *maze, int width, int height) {
    InitializeGraph(&maze->graph);
    maze->numEnemies = 0;
    maze->width = width;
    maze->height = height;
    maze->grid = (int **)malloc(height * sizeof(int *));
    for (int i = 0; i < height; i++) {
        maze->grid[i] = (int *)malloc(width * sizeof(int));
        memset(maze->grid[i], 0, width * sizeof(int));
    }
}

// Libérer la mémoire du labyrinthe
void FreeMaze(Maze *maze) {
    for (int i = 0; i < maze->height; i++) {
        free(maze->grid[i]);
    }
    free(maze->grid);
}

// Mélanger les éléments d'un tableau
void ShuffleArray(int *array, int size) {
    for (int i = 0; i < size; i++) {
        int r = rand() % size;
        int temp = array[i];
        array[i] = array[r];
        array[r] = temp;
    }
}

// Générer le labyrinthe avec l'algorithme de retour sur trace
void GenerateMazeRecursiveBacktracker(Maze *maze) {
    srand(time(NULL));
    int cx = rand() % maze->width;
    int cy = rand() % maze->height;
    CarvePassagesFrom(cx, cy, maze);
}

void CarvePassagesFrom(int cx, int cy, Maze *maze) {
    int directions[4] = {N, S, E, W};
    ShuffleArray(directions, 4);

    for (int i = 0; i < 4; i++) {
        int dir = directions[i];
        int nx = cx + DX[dir];
        int ny = cy + DY[dir];

        if (ny >= 0 && ny < maze->height && nx >= 0 && nx < maze->width && maze->grid[ny][nx] == 0) {
            maze->grid[cy][cx] |= dir;
            maze->grid[ny][nx] |= OPPOSITE[dir];
            CarvePassagesFrom(nx, ny, maze);
        }
    }
}

// Rendu du labyrinthe
void RenderMaze(Maze *maze) {
    for (int y = 0; y < maze->height; y++) {
        for (int x = 0; x < maze->width; x++) {
            if (maze->grid[y][x] & S) DrawLine(x * 100, y * 100, x * 100 + 100, y * 100, BLACK);
            if (maze->grid[y][x] & E) DrawLine(x * 100 + 100, y * 100, x * 100 + 100, y * 100 + 100, BLACK);
        }
    }
}

// Initialiser le labyrinthe pour le niveau 1
void InitializeMazeLevel1(Maze *maze) {
    InitializeMaze(maze, 10, 10); // Par exemple, un labyrinthe de 10x10
    GenerateMazeRecursiveBacktracker(maze);
}

// Fonctionnalité utilitaire non implémentée (supprimez ces avertissements ou utilisez-les plus tard)
void GenerateMaze(Maze *maze, int complexity) {
    // Paramètres inutilisés
    (void)maze;
    (void)complexity;
}

void PrintPath(int *predecessors, int startNode, int goalNode) {
    // Paramètres inutilisés
    (void)predecessors;
    (void)startNode;
    (void)goalNode;
}
*/