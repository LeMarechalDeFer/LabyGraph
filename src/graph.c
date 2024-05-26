
#include "../include/graph.h"
#define CELL_SIZE 40



// Tableau de lettres associées aux types d'ennemis
const char *enemyLetters[] = { "L", "T", "G" };

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
//470
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

void RenderMaze(Maze *maze, int rectX, int rectY, int rectWidth, int rectHeight) {
    
     // Calculate scaling factors
    float scaleX = (float)rectWidth / WIDTH;
    float scaleY = (float)rectHeight / HEIGHT;
    float scale = fmin(scaleX, scaleY); // Use the smaller scaling factor to maintain aspect ratio
    int halfCell = scale / 2;

    int offsetX = rectX + (rectWidth - (scale * maze->width)) / 2;
    int offsetY = rectY + (rectHeight - (scale * maze->height)) / 2;

    // Dessiner les murs
    for (int y = 0; y < maze->height; y++) {
        for (int x = 0; x < maze->width; x++) {
            if (maze->walls[y][x]) {
               DrawRectangle(offsetX + x * scale, offsetY + y * scale, scale, scale, WHITE);
            }
        }
    }

    // Dessiner les arêtes
    for (int i = 0; i < maze->graph.numEdges; i++) {
        Edge edge = maze->graph.edges[i];
        Node startNode = maze->graph.nodes[edge.start];
        Node endNode = maze->graph.nodes[edge.end];

        DrawLine(offsetX + startNode.x * scale + halfCell, offsetY + startNode.y * scale + halfCell,
                 offsetX + endNode.x * scale + halfCell, offsetY + endNode.y * scale + halfCell, BLACK);

        char enemyLabel[10];
        snprintf(enemyLabel, sizeof(enemyLabel), "%s %d", enemyLetters[edge.enemy_type], edge.number_enemy);
        DrawText(enemyLabel, (offsetX + startNode.x * scale + offsetX + endNode.x * scale) / 2,
                 (offsetY + startNode.y * scale + offsetY + endNode.y * scale) / 2, 20, RED);
    }

    // Dessiner les nœuds
    for (int i = 0; i < maze->graph.numNodes; i++) {
        Node node = maze->graph.nodes[i];
        DrawCircle(offsetX + node.x * scale + halfCell, offsetY + node.y * scale + halfCell, 5, BLUE);
    }
}

void PrintPath(int *predecessors, int startNode, int goalNode) {
    // Paramètres inutilisés
    (void)predecessors;
    (void)startNode;
    (void)goalNode;
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
    if (startX < width - 1) candidateEdges[numCandidates++] = (Edge){startX * width + startY, (startX + 1) * width + startY, rand() % 10 + 1, rand() % 3};
    if (startY < height - 1) candidateEdges[numCandidates++] = (Edge){startX * width + startY, startX * width + (startY + 1), rand() % 10 + 1, rand() % 3};

    while (numCandidates > 0) {
        // Choisir une arête aléatoire
        int edgeIndex = rand() % numCandidates;
        Edge edge = candidateEdges[edgeIndex];

        // Trouver les coordonnées des nœuds de l'arête
        int startX = maze->graph.nodes[edge.start].x;
        int startY = maze->graph.nodes[edge.start].y;
        int endX = maze->graph.nodes[edge.end].x;
        int endY = maze->graph.nodes[edge.end].y;

        // Si l'un des deux nœuds n'a pas encore été visité
        if (maze->walls[endY][endX]) {
            // Marquer le nœud comme visité
            maze->walls[endY][endX] = false;

            // Ajouter l'arête au graphe du labyrinthe
            AddEdge(&maze->graph, edge.start, edge.end, edge.enemy_type, edge.number_enemy);

            // Ajouter les nouvelles arêtes candidates
            if (endX > 0 && maze->walls[endY][endX - 1]) candidateEdges[numCandidates++] = (Edge){endY * width + endX, endY * width + (endX - 1), rand() % 10 + 1, rand() % 3};
            if (endX < width - 1 && maze->walls[endY][endX + 1]) candidateEdges[numCandidates++] = (Edge){endY * width + endX, endY * width + (endX + 1), rand() % 10 + 1, rand() % 3};
            if (endY > 0 && maze->walls[endY - 1][endX]) candidateEdges[numCandidates++] = (Edge){endY * width + endX, (endY - 1) * width + endX, rand() % 10 + 1, rand() % 3};
            if (endY < height - 1 && maze->walls[endY + 1][endX]) candidateEdges[numCandidates++] = (Edge){endY * width + endX, (endY + 1) * width + endX, rand() % 10 + 1, rand() % 3};
        }

        // Retirer l'arête traitée de la liste des candidats
        candidateEdges[edgeIndex] = candidateEdges[--numCandidates];
    }

    printf("Maze generated.\n");
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
void InitializePlayer(Player *player, int startNode) {
    player->currentNode = startNode;
    player->health = 0; // La santé commence à 0
    printf("Player initialized at node %d with health %d.\n", startNode, player->health);
}


void DrawHealthBar(Player *player) {
    int barWidth = 200;
    int barHeight = 20;
    int x = 600;
    int y = 200;
    DrawRectangle(x, y, barWidth, barHeight, GRAY); // Fond de la barre de santé
    DrawRectangle(x, y, barWidth * (player->health / 100.0), barHeight, RED); // Barre de santé
    DrawRectangleLines(x, y, barWidth, barHeight, BLACK); // Bordure de la barre de santé
    DrawText(TextFormat("%d",player->health),x,y + 50,30,RED);
}
