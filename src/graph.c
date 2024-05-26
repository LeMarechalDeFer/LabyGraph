
#include "../include/graph.h"
#define CELL_SIZE 40

const char *enemyLetters[] = { "L", "T", "G" };

void InitializeGraph(Graph *graph) {
    graph->numNodes = 0;
    graph->numEdges = 0;
}

void AddNode(Graph *graph, int x, int y) {
    if (graph->numNodes < MAX_NODES) {
        graph->nodes[graph->numNodes] = (Node){graph->numNodes, x, y};
        graph->numNodes++;
    }
}

void AddEdge(Graph *graph, int start, int end, int weight) {
    if (graph->numEdges < MAX_EDGES) {
        graph->edges[graph->numEdges] = (Edge){start, end, weight};
        graph->numEdges++;
    }
}

void InitializeMaze(Maze *maze) {
    InitializeGraph(&maze->graph);
    maze->numEnemies = 0;
}
void InitializeMazeLevel1(Maze *maze) 
{
    InitializeMaze(maze);
    // Ajouter des noeuds
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            AddNode(&maze->graph, x, y);
        }
    }

    // Ajouter des arêtes avec poids (nombre d'ennemis)
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            int currentNode = y * WIDTH + x;
            if (x < WIDTH - 1) { // Ajouter une arête vers la droite
                AddEdge(&maze->graph, currentNode, currentNode + 1, rand() % 3);
            }
            if (y < HEIGHT - 1) { // Ajouter une arête vers le bas
                AddEdge(&maze->graph, currentNode, currentNode + WIDTH, rand() % 3);
            }
        }
    }
    // Colorier les arêtes
    ColorEdges(&maze->graph);
}

void ColorEdges(Graph *graph) {
    int edgeColors[MAX_EDGES] = { -1 }; // Initialiser les couleurs des arêtes à -1 (non colorié)
    bool availableColors[MAX_ENEMIES];  // Couleurs disponibles

    for (int i = 0; i < graph->numEdges; i++) {
        memset(availableColors, true, sizeof(availableColors));

        // Vérifier les arêtes adjacentes pour éviter les conflits de couleur
        for (int j = 0; j < graph->numEdges; j++) {
            if (i != j && (graph->edges[i].start == graph->edges[j].start ||
                           graph->edges[i].start == graph->edges[j].end ||
                           graph->edges[i].end == graph->edges[j].start ||
                           graph->edges[i].end == graph->edges[j].end)) {
                if (edgeColors[j] != -1) {
                    availableColors[edgeColors[j]] = false;
                }
            }
        }

        // Assigner la première couleur disponible
        for (int color = 0; color < MAX_ENEMIES; color++) {
            if (availableColors[color]) {
                edgeColors[i] = color;
                break;
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


void GenerateMaze(Maze *maze, int complexity) {
    // Paramètres inutilisés
    (void)maze;
    (void)complexity;
}

bool MovePlayer(Player *player, Maze *maze, int nextNode) {
    // Trouver l'arête entre le nœud actuel et le prochain nœud
    for (int i = 0; i < maze->graph.numEdges; i++) {
        Edge edge = maze->graph.edges[i];
        if ((edge.start == player->currentNode && edge.end == nextNode) ||
            (edge.end == player->currentNode && edge.start == nextNode)) {
            player->health += edge.number_enemy; // Ajouter le nombre d'ennemis à la santé du joueur
            player->currentNode = nextNode;
            return true;
        }
    }
    return false; // Déplacement invalide
}
void InitializePlayer(Player *player, int startNode) {
    player->currentNode = startNode;
    player->health = 100; // Ex: initial health value
}

void DrawHealthBar(Player *player) {
    int barWidth = 200;
    int barHeight = 20;
    int x = 10;
    int y = 10;
    DrawRectangle(x, y, barWidth, barHeight, GRAY); // Fond de la barre de santé
    DrawRectangle(x, y, barWidth * (player->health / 100.0), barHeight, RED); // Barre de santé
    DrawRectangleLines(x, y, barWidth, barHeight, BLACK); // Bordure de la barre de santé
}


/*void InitializeEnemies(Enemy enemies[], Maze *maze) {
    int enemyCount = 0;

    for (int y = 0; y < maze->height && enemyCount < MAX_ENEMIES; y++) {
        for (int x = 0; x < maze->width && enemyCount < MAX_ENEMIES; x++) {
            // Check if the current cell is not a wall and has at least one open path
            if (maze->grid[y][x] != 0 && ((maze->grid[y][x] & S) || (maze->grid[y][x] & E) || 
                                          (maze->grid[y][x] & N) || (maze->grid[y][x] & W))) {
                enemies[enemyCount].x = x;
                enemies[enemyCount].y = y;
                enemies[enemyCount].radius = CELL_SIZE / 4; // Set the radius for enemies
                enemies[enemyCount].strength = GetRandomValue(1,50);
                enemyCount++;
            }
        }
    }
}

bool CheckCollisionPlayerEnemy(Rectangle player, Enemy enemy) {
    return CheckCollisionCircleRec((Vector2){player.x + player.width / 2, player.y + player.height / 2}, 
                                    player.width / 2, 
                                    (Rectangle){enemy.x * CELL_SIZE + CELL_SIZE / 2 + 70, 
                                                enemy.y * CELL_SIZE + CELL_SIZE / 2 + 160, 
                                                enemy.radius, 
                                                enemy.radius});
}
*/

