
#include "../include/graph.h"


// Définition et initialisation du tableau de lettres associées aux types d'ennemis
const char *enemyLetters[] = { "L", "T", "G", "M" };


void InitializeGraph(Graph *graph) {
    graph->numNodes = 0;
    graph->numEdges = 0;
    printf("Graph initialized.\n");
}

void AddNode(Graph *graph, int x, int y) {
    if (graph->numNodes < MAX_NODES) {
        graph->nodes[graph->numNodes].id = graph->numNodes;
        graph->nodes[graph->numNodes].x = x;
        graph->nodes[graph->numNodes].y = y;
        graph->numNodes++;
        printf("Node added: (%d, %d)\n", x, y);
    }
}

void AddEdge(Graph *graph, int start, int end, EnemyType enemyType, int number_enemy) {
    if (graph->numEdges < MAX_EDGES) {
        graph->edges[graph->numEdges].start = start;
        graph->edges[graph->numEdges].end = end;
        graph->edges[graph->numEdges].enemy_type = enemyType;
        graph->edges[graph->numEdges].number_enemy = number_enemy;
        graph->numEdges++;
        printf("Edge added: (%d -> %d) with %d enemies of type %d\n", start, end, number_enemy, enemyType);
    }
}

void InitializePlayer(Player *player, int startNode) {
    player->currentNode = startNode;
    player->health = 0; // La santé commence à 0
    printf("Player initialized at node %d with health %d.\n", startNode, player->health);
}

bool MovePlayer(Player *player, Maze *maze, int nextNode) {
    // Trouver l'arête entre le nœud actuel et le prochain nœud
    for (int i = 0; i < maze->graph.numEdges; i++) {
        Edge edge = maze->graph.edges[i];
        if ((edge.start == player->currentNode && edge.end == nextNode) ||
            (edge.end == player->currentNode && edge.start == nextNode)) {
            player->health += edge.number_enemy; // Ajouter le nombre d'ennemis à la santé du joueur
            player->currentNode = nextNode;
            printf("Player moved to node %d. Current health: %d\n", nextNode, player->health);
            return true;
        }
    }
    return false; // Déplacement invalide
}

void InitializeMaze(Maze *maze, int width, int height) {
    InitializeGraph(&maze->graph);
    maze->width = width;
    maze->height = height;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            maze->walls[y][x] = true;
        }
    }
    printf("Maze initialized with width %d and height %d.\n", width, height);
}

bool IsEnemyTypeValid(Maze *maze, int nodeIndex, EnemyType proposedType) {
    for (int i = 0; i < maze->graph.numEdges; i++) {
        Edge edge = maze->graph.edges[i];
        if ((edge.start == nodeIndex || edge.end == nodeIndex) && edge.enemy_type == proposedType) {
            return false;
        }
    }
    return true;
}

void GenerateMaze(Maze *maze) {
    int width = maze->width;
    int height = maze->height;
    InitializeMaze(maze, width, height);

    // Initialiser le générateur de nombres aléatoires
    srand(time(NULL));

    // Ajouter des noeuds
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            AddNode(&maze->graph, x, y);
        }
    }

    // Liste des arêtes candidates
    Edge candidateEdges[MAX_EDGES];
    int numCandidates = 0;

    // Commencer avec un seul nœud
    maze->walls[0][0] = false;
    int startX = 0;
    int startY = 0;

    // Ajouter les arêtes adjacentes du premier nœud
    if (startX < width - 1) candidateEdges[numCandidates++] = (Edge){startX * width + startY, (startX + 1) * width + startY, rand() % 10 + 1, rand() % 4};
    if (startY < height - 1) candidateEdges[numCandidates++] = (Edge){startX * width + startY, startX * width + (startY + 1), rand() % 10 + 1, rand() % 4};

    while (numCandidates > 0) {
        // Choisir une arête aléatoire
        int edgeIndex = rand() % numCandidates;
        Edge edge = candidateEdges[edgeIndex];

        // Trouver les coordonnées des nœuds de l'arête (desacitvées pour le -Wall)
        // int startX = maze->graph.nodes[edge.start].x;
        // int startY = maze->graph.nodes[edge.start].y;
        int endX = maze->graph.nodes[edge.end].x;
        int endY = maze->graph.nodes[edge.end].y;

        // Si l'un des deux nœuds n'a pas encore été visité
        if (maze->walls[endY][endX]) {
            // Marquer le nœud comme visité
            maze->walls[endY][endX] = false;

            // Choisir un type d'ennemi valide pour l'arête
            EnemyType enemyType;
            do {
                enemyType = rand() % 4; // Suppose 4 types d'ennemis (L, T, G, M)
            } while (!IsEnemyTypeValid(maze, edge.start, enemyType) || !IsEnemyTypeValid(maze, edge.end, enemyType));

            // Ajouter l'arête au graphe du labyrinthe avec le type d'ennemi valide
            AddEdge(&maze->graph, edge.start, edge.end, enemyType, edge.number_enemy);

            // Ajouter les nouvelles arêtes candidates
            if (endX > 0 && maze->walls[endY][endX - 1]) candidateEdges[numCandidates++] = (Edge){endY * width + endX, endY * width + (endX - 1), rand() % 10 + 1, enemyType};
            if (endX < width - 1 && maze->walls[endY][endX + 1]) candidateEdges[numCandidates++] = (Edge){endY * width + endX, endY * width + (endX + 1), rand() % 10 + 1, enemyType};
            if (endY > 0 && maze->walls[endY - 1][endX]) candidateEdges[numCandidates++] = (Edge){endY * width + endX, (endY - 1) * width + endX, rand() % 10 + 1, enemyType};
            if (endY < height - 1 && maze->walls[endY + 1][endX]) candidateEdges[numCandidates++] = (Edge){endY * width + endX, (endY + 1) * width + endX, rand() % 10 + 1, enemyType};
        }

        // Retirer l'arête traitée de la liste des candidats
        candidateEdges[edgeIndex] = candidateEdges[--numCandidates];
    }

    printf("Maze generated.\n");
}

void BellmanFord(Graph *graph, int startNode, int *distances, int *predecessors) {
    int V = graph->numNodes;
    int E = graph->numEdges;

    // Initialisation
    for (int i = 0; i < V; i++) {
        distances[i] = INT_MAX;
        predecessors[i] = -1;
    }
    distances[startNode] = 0;

    // Relaxation des arêtes
    for (int i = 1; i <= V - 1; i++) {
        for (int j = 0; j < E; j++) {
            int u = graph->edges[j].start;
            int v = graph->edges[j].end;
            int weight = graph->edges[j].number_enemy;
            if (distances[u] != INT_MAX && distances[u] + weight < distances[v]) {
                distances[v] = distances[u] + weight;
                predecessors[v] = u;
            }
            if (distances[v] != INT_MAX && distances[v] + weight < distances[u]) {
                distances[u] = distances[v] + weight;
                predecessors[u] = v;
            }
        }
    }

    // Vérification des cycles négatifs
    for (int i = 0; i < E; i++) {
        int u = graph->edges[i].start;
        int v = graph->edges[i].end;
        int weight = graph->edges[i].number_enemy;
        if (distances[u] != INT_MAX && distances[u] + weight < distances[v]) {
            printf("Graph contains negative weight cycle\n");
            return;
        }
    }
}

void RenderMaze(Maze *maze, int cellSize) { //void RenderMaze(Maze *maze, int screenWidth, int screenHeight, int cellSize)
    int halfCell = cellSize / 2;

    // Dessiner les murs
    for (int y = 0; y < maze->height; y++) {
        for (int x = 0; x < maze->width; x++) {
            if (maze->walls[y][x]) {
                DrawRectangle(x * cellSize, y * cellSize, cellSize, cellSize, BLACK);
            }
        }
    }

    // Dessiner les arêtes
    for (int i = 0; i < maze->graph.numEdges; i++) {
        Edge edge = maze->graph.edges[i];
        Node startNode = maze->graph.nodes[edge.start];
        Node endNode = maze->graph.nodes[edge.end];

        DrawLine(startNode.x * cellSize + halfCell, startNode.y * cellSize + halfCell, endNode.x * cellSize + halfCell, endNode.y * cellSize + halfCell, BLACK);

        char enemyLabel[10];
        snprintf(enemyLabel, sizeof(enemyLabel), "%s %d", enemyLetters[edge.enemy_type], edge.number_enemy);
        DrawText(enemyLabel, (startNode.x * cellSize + endNode.x * cellSize) / 2 + halfCell, (startNode.y * cellSize + endNode.y * cellSize) / 2 + halfCell, 20, RED);
    }

    // Dessiner les nœuds
    for (int i = 0; i < maze->graph.numNodes; i++) {
        Node node = maze->graph.nodes[i];
        if (node.id == 99) {
            DrawCircle(node.x * cellSize + halfCell, node.y * cellSize + halfCell, 10, GREEN); // Nœud de sortie en bleu
        } else {
            DrawCircle(node.x * cellSize + halfCell, node.y * cellSize + halfCell, 5, BLUE); // Autres nœuds en gris foncé
        }
    }
}

void DrawHealthBar(Player *player) {
    int barWidth = 1020;
    int barHeight = 20;
    int x = 10;
    int y = 10;
    DrawRectangle(x, y, barWidth, barHeight, GRAY); // Fond de la barre de santé
    DrawRectangle(x, y, barWidth * (player->health / 1020.0), barHeight, RED); // Barre de santé
    DrawRectangleLines(x, y, barWidth, barHeight, BLACK); // Bordure de la barre de santé
}

void PrintPath(int *predecessors, int startNode, int goalNode) {
    if (goalNode == -1) {
        printf("No path from %d to %d\n", startNode, goalNode);
        return;
    }

    int path[MAX_NODES];
    int count = 0;
    for (int v = goalNode; v != startNode; v = predecessors[v]) {
        path[count++] = v;
    }
    path[count] = startNode;

    printf("Path from %d to %d: ", startNode, goalNode);
    for (int i = count; i >= 0; i--) {
        printf("%d ", path[i]);
    }
    printf("\n");
}
