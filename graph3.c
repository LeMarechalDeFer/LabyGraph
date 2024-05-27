// #include <stdlib.h>
// #include <stdio.h>
// #include <stdbool.h>
// #include <string.h>
// #include <raylib.h>
// #include <time.h>
// #include <limits.h>

// #define MAX_NODES 100
// #define MAX_EDGES 200
// #define MAX_ENEMIES 10

// typedef struct {
//     int id;
//     int x;
//     int y;
// } Node;

// typedef enum {
//     L,
//     T,
//     G, 
//     M,
// } EnemyType;

// typedef struct {
//     int start;
//     int end;
//     int number_enemy; 
//     EnemyType enemy_type;
// } Edge;

// typedef struct {
//     Node nodes[MAX_NODES];
//     Edge edges[MAX_EDGES];
//     int numNodes;
//     int numEdges;
// } Graph;

// typedef struct {
//     int currentNode;
//     int health;
// } Player;

// typedef struct {
//     Graph graph;
//     int width;
//     int height;
//     bool walls[MAX_NODES][MAX_NODES];
// } Maze;


// //definition des level ac

// typedef enum GameScreen { 
//     LOGO = 0, TITLE,LEVEL1, GAMEPLAY, EXPLANATION ,ENDING,WINROOM,
// }GameScreen;

// // Tableau de lettres associées aux types d'ennemis
// const char *enemyLetters[] = { "L", "T", "G", "M" };

// // Function prototypes
// void InitializeGraph(Graph *graph);
// void AddNode(Graph *graph, int x, int y);
// void AddEdge(Graph *graph, int start, int end, EnemyType enemyType, int number_enemy);
// void InitializePlayer(Player *player, int startNode);
// bool MovePlayer(Player *player, Maze *maze, int nextNode);
// void InitializeMaze(Maze *maze, int width, int height);
// void GenerateMaze(Maze *maze);
// void RenderMaze(Maze *maze, int screenWidth, int screenHeight, int cellSize);
// void DrawHealthBar(Player *player);
// void PrintPath(int *predecessors, int startNode, int goalNode);
// bool IsEnemyTypeValid(Maze *maze, int nodeIndex, EnemyType proposedType);
// void BellmanFord(Graph *graph, int startNode, int *distances, int *predecessors);

// void InitializeGraph(Graph *graph) {
//     graph->numNodes = 0;
//     graph->numEdges = 0;
//     printf("Graph initialized.\n");
// }

// void AddNode(Graph *graph, int x, int y) {
//     if (graph->numNodes < MAX_NODES) {
//         graph->nodes[graph->numNodes].id = graph->numNodes;
//         graph->nodes[graph->numNodes].x = x;
//         graph->nodes[graph->numNodes].y = y;
//         graph->numNodes++;
//         printf("Node added: (%d, %d)\n", x, y);
//     }
// }

// void AddEdge(Graph *graph, int start, int end, EnemyType enemyType, int number_enemy) {
//     if (graph->numEdges < MAX_EDGES) {
//         graph->edges[graph->numEdges].start = start;
//         graph->edges[graph->numEdges].end = end;
//         graph->edges[graph->numEdges].enemy_type = enemyType;
//         graph->edges[graph->numEdges].number_enemy = number_enemy;
//         graph->numEdges++;
//         printf("Edge added: (%d -> %d) with %d enemies of type %d\n", start, end, number_enemy, enemyType);
//     }
// }

// void InitializePlayer(Player *player, int startNode) {
//     player->currentNode = startNode;
//     player->health = 0; // La santé commence à 0
//     printf("Player initialized at node %d with health %d.\n", startNode, player->health);
// }

// bool MovePlayer(Player *player, Maze *maze, int nextNode) {
//     // Trouver l'arête entre le nœud actuel et le prochain nœud
//     for (int i = 0; i < maze->graph.numEdges; i++) {
//         Edge edge = maze->graph.edges[i];
//         if ((edge.start == player->currentNode && edge.end == nextNode) ||
//             (edge.end == player->currentNode && edge.start == nextNode)) {
//             player->health += edge.number_enemy; // Ajouter le nombre d'ennemis à la santé du joueur
//             player->currentNode = nextNode;
//             printf("Player moved to node %d. Current health: %d\n", nextNode, player->health);
//             return true;
//         }
//     }
//     return false; // Déplacement invalide
// }

// void InitializeMaze(Maze *maze, int width, int height) {
//     InitializeGraph(&maze->graph);
//     maze->width = width;
//     maze->height = height;
//     for (int y = 0; y < height; y++) {
//         for (int x = 0; x < width; x++) {
//             maze->walls[y][x] = true;
//         }
//     }
//     printf("Maze initialized with width %d and height %d.\n", width, height);
// }

// bool IsEnemyTypeValid(Maze *maze, int nodeIndex, EnemyType proposedType) {
//     for (int i = 0; i < maze->graph.numEdges; i++) {
//         Edge edge = maze->graph.edges[i];
//         if ((edge.start == nodeIndex || edge.end == nodeIndex) && edge.enemy_type == proposedType) {
//             return false;
//         }
//     }
//     return true;
// }

// void GenerateMaze(Maze *maze) {
//     int width = maze->width;
//     int height = maze->height;
//     InitializeMaze(maze, width, height);

//     // Initialiser le générateur de nombres aléatoires
//     srand(time(NULL));

//     // Ajouter des noeuds
//     for (int y = 0; y < height; y++) {
//         for (int x = 0; x < width; x++) {
//             AddNode(&maze->graph, x, y);
//         }
//     }

//     // Liste des arêtes candidates
//     Edge candidateEdges[MAX_EDGES];
//     int numCandidates = 0;

//     // Commencer avec un seul nœud
//     maze->walls[0][0] = false;
//     int startX = 0;
//     int startY = 0;

//     // Ajouter les arêtes adjacentes du premier nœud
//     if (startX < width - 1) candidateEdges[numCandidates++] = (Edge){startX * width + startY, (startX + 1) * width + startY, rand() % 10 + 1, rand() % 4};
//     if (startY < height - 1) candidateEdges[numCandidates++] = (Edge){startX * width + startY, startX * width + (startY + 1), rand() % 10 + 1, rand() % 4};

//     while (numCandidates > 0) {
//         // Choisir une arête aléatoire
//         int edgeIndex = rand() % numCandidates;
//         Edge edge = candidateEdges[edgeIndex];

//         // Trouver les coordonnées des nœuds de l'arête
//         int startX = maze->graph.nodes[edge.start].x;
//         int startY = maze->graph.nodes[edge.start].y;
//         int endX = maze->graph.nodes[edge.end].x;
//         int endY = maze->graph.nodes[edge.end].y;

//         // Si l'un des deux nœuds n'a pas encore été visité
//         if (maze->walls[endY][endX]) {
//             // Marquer le nœud comme visité
//             maze->walls[endY][endX] = false;

//             // Choisir un type d'ennemi valide pour l'arête
//             EnemyType enemyType;
//             do {
//                 enemyType = rand() % 4; // Suppose 4 types d'ennemis (L, T, G, M)
//             } while (!IsEnemyTypeValid(maze, edge.start, enemyType) || !IsEnemyTypeValid(maze, edge.end, enemyType));

//             // Ajouter l'arête au graphe du labyrinthe avec le type d'ennemi valide
//             AddEdge(&maze->graph, edge.start, edge.end, enemyType, edge.number_enemy);

//             // Ajouter les nouvelles arêtes candidates
//             if (endX > 0 && maze->walls[endY][endX - 1]) candidateEdges[numCandidates++] = (Edge){endY * width + endX, endY * width + (endX - 1), rand() % 10 + 1, enemyType};
//             if (endX < width - 1 && maze->walls[endY][endX + 1]) candidateEdges[numCandidates++] = (Edge){endY * width + endX, endY * width + (endX + 1), rand() % 10 + 1, enemyType};
//             if (endY > 0 && maze->walls[endY - 1][endX]) candidateEdges[numCandidates++] = (Edge){endY * width + endX, (endY - 1) * width + endX, rand() % 10 + 1, enemyType};
//             if (endY < height - 1 && maze->walls[endY + 1][endX]) candidateEdges[numCandidates++] = (Edge){endY * width + endX, (endY + 1) * width + endX, rand() % 10 + 1, enemyType};
//         }

//         // Retirer l'arête traitée de la liste des candidats
//         candidateEdges[edgeIndex] = candidateEdges[--numCandidates];
//     }

//     printf("Maze generated.\n");
// }

// void BellmanFord(Graph *graph, int startNode, int *distances, int *predecessors) {
//     int V = graph->numNodes;
//     int E = graph->numEdges;

//     // Initialisation
//     for (int i = 0; i < V; i++) {
//         distances[i] = INT_MAX;
//         predecessors[i] = -1;
//     }
//     distances[startNode] = 0;

//     // Relaxation des arêtes
//     for (int i = 1; i <= V - 1; i++) {
//         for (int j = 0; j < E; j++) {
//             int u = graph->edges[j].start;
//             int v = graph->edges[j].end;
//             int weight = graph->edges[j].number_enemy;
//             if (distances[u] != INT_MAX && distances[u] + weight < distances[v]) {
//                 distances[v] = distances[u] + weight;
//                 predecessors[v] = u;
//             }
//             if (distances[v] != INT_MAX && distances[v] + weight < distances[u]) {
//                 distances[u] = distances[v] + weight;
//                 predecessors[u] = v;
//             }
//         }
//     }

//     // Vérification des cycles négatifs
//     for (int i = 0; i < E; i++) {
//         int u = graph->edges[i].start;
//         int v = graph->edges[i].end;
//         int weight = graph->edges[i].number_enemy;
//         if (distances[u] != INT_MAX && distances[u] + weight < distances[v]) {
//             printf("Graph contains negative weight cycle\n");
//             return;
//         }
//     }
// }

// void RenderMaze(Maze *maze, int screenWidth, int screenHeight, int cellSize) {
//     int halfCell = cellSize / 2;

//     // Dessiner les murs
//     for (int y = 0; y < maze->height; y++) {
//         for (int x = 0; x < maze->width; x++) {
//             if (maze->walls[y][x]) {
//                 DrawRectangle(x * cellSize, y * cellSize, cellSize, cellSize, BLACK);
//             }
//         }
//     }

//     // Dessiner les arêtes
//     for (int i = 0; i < maze->graph.numEdges; i++) {
//         Edge edge = maze->graph.edges[i];
//         Node startNode = maze->graph.nodes[edge.start];
//         Node endNode = maze->graph.nodes[edge.end];

//         DrawLine(startNode.x * cellSize + halfCell, startNode.y * cellSize + halfCell, endNode.x * cellSize + halfCell, endNode.y * cellSize + halfCell, WHITE);

//         char enemyLabel[10];
//         snprintf(enemyLabel, sizeof(enemyLabel), "%s %d", enemyLetters[edge.enemy_type], edge.number_enemy);
//         DrawText(enemyLabel, (startNode.x * cellSize + endNode.x * cellSize) / 2 + halfCell, (startNode.y * cellSize + endNode.y * cellSize) / 2 + halfCell, 20, RED);
//     }

//     // Dessiner les nœuds
//     for (int i = 0; i < maze->graph.numNodes; i++) {
//         Node node = maze->graph.nodes[i];
//         if (node.id == 99) {
//             DrawCircle(node.x * cellSize + halfCell, node.y * cellSize + halfCell, 10, GREEN); // Nœud de sortie en bleu
//         } else {
//             DrawCircle(node.x * cellSize + halfCell, node.y * cellSize + halfCell, 5, BLUE); // Autres nœuds en gris foncé
//         }
//     }
// }

// void DrawHealthBar(Player *player) {
//     int barWidth = 200;
//     int barHeight = 20;
//     int x = 10;
//     int y = 10;
//     DrawRectangle(x, y, barWidth, barHeight, GRAY); // Fond de la barre de santé
//     DrawRectangle(x, y, barWidth * (player->health / 100.0), barHeight, RED); // Barre de santé
//     DrawRectangleLines(x, y, barWidth, barHeight, BLACK); // Bordure de la barre de santé
// }

// void PrintPath(int *predecessors, int startNode, int goalNode) {
//     if (goalNode == -1) {
//         printf("No path from %d to %d\n", startNode, goalNode);
//         return;
//     }

//     int path[MAX_NODES];
//     int count = 0;
//     for (int v = goalNode; v != startNode; v = predecessors[v]) {
//         path[count++] = v;
//     }
//     path[count] = startNode;

//     printf("Path from %d to %d: ", startNode, goalNode);
//     for (int i = count; i >= 0; i--) {
//         printf("%d ", path[i]);
//     }
//     printf("\n");
// }

// int main() {


//     // Dimensions de la fenêtre
//     int screenWidth = 1920;
//     int screenHeight = 1080;


//     int LVL = 0;
//     int LVL_actuelle =0;
//     int counthelper =0;

//     // Charger les images 
//     Image player_Helper = LoadImage("./player.png");
//     Image ESIEA_logo = LoadImage("./ESIEA-logo.png");
//     Image dialogue_box = LoadImage("./dialogue_box.png");
//     Image touches_direction =LoadImage("./touches_direction.png");
    
//     // Vérifiez si les images sont chargées correctement
//     if (player_Helper.data == NULL) {
//         printf("Erreur: Impossible de charger player.png\n");
//         return 1;
//     }
//     if (ESIEA_logo.data == NULL) {
//         printf("Erreur: Impossible de charger ESIEA-logo.png\n");
//         return 1;
//     }
//     if (dialogue_box.data == NULL) {
//         printf("Erreur: Impossible de charger dialogue_box.png\n");
//         return 1;
//     }
//     if (touches_direction.data == NULL) {
//         printf("Erreur: Impossible de charger touches_direction.png\n");
//         return 1;
//     }
    
//     printf("AAAAAAAAAAAAAAAAAAA\n");
//     Texture2D player_texture = LoadTextureFromImage(player_Helper);
//     printf("BBBBBBBBB");
//     Texture2D ESIEA_logo_texture = LoadTextureFromImage(ESIEA_logo);
//     printf("1");
//     Texture2D dialogue_box_texture = LoadTextureFromImage(dialogue_box);
//     printf("1");
//     Texture2D touches_direction_texture = LoadTextureFromImage(touches_direction);
//     printf("1");
//     UnloadImage(player_Helper);
//     UnloadImage(ESIEA_logo);
//     UnloadImage(dialogue_box);
//     UnloadImage(touches_direction);

//     // Charger les sons
//     InitAudioDevice();
//     Sound son_start = LoadSound("./voracious.mp3");  

//     // Vérifiez si le son est chargé correctement
//     if (son_start.frameCount == 0) {
//         printf("Erreur: Impossible de charger voracious.mp3\n");
//         return 1;
//     }

//     // Initialisation du générateur de nombres aléatoires
//     srand(time(NULL));

//     // Initialisation de la fenêtre Raylib
//     InitWindow(screenWidth, screenHeight, "Maze Game");
//     SetTargetFPS(60); // Définir le FPS pour une animation fluide
    
//     char *monologue[] = 
//     {
//         "Bienvenue jeune aventurier dans notre jeu éducatif !",
//         "Ton rôle est de monter au dernier étage de cette tour.",
//         "Afin de voir si tu es bien un pro des graphes.",
//         "Es-tu prêt ?",
//         "Parfait !! Dans ce cas, bienvenue au premier étage.\nDans chaque étage, tu auras des défis à réaliser\n afin de pouvoir continuer ton ascension. ",
//     };
//     char *consignes[] = 
//     {
//         "Vous devez trouver le moyen d'affronter le moins\n d'énenmies possible en suivant\n l'algo de Bellman.",
//         "Vous devez trouver le moyen d'affronter le plus\n d'énenmies possible en suivant\n l'algo de Bellman.",
//         "la suite des consigne...",
//     };

//     int maxLines = sizeof(monologue) / sizeof(monologue[0]);
//     int maxLinesconsigne = sizeof(consignes) / sizeof(consignes[0]);

//     int currentLine = 0;
//     int currentLineLVL = 0;
//     // Initialisation des structures
//     Maze maze;
//     Player player;

//     // Initialiser le labyrinthe
//     int mazeWidth = 10;  // Largeur du labyrinthe en nombre de cellules
//     int mazeHeight = 10; // Hauteur du labyrinthe en nombre de cellules
//     InitializeMaze(&maze, mazeWidth, mazeHeight);
//     GenerateMaze(&maze);

//     // Calculer dynamiquement la taille des cellules en fonction des dimensions de la fenêtre
//     int cellSize = (screenWidth < screenHeight ? screenWidth : screenHeight) / (mazeWidth > mazeHeight ? mazeWidth : mazeHeight);

//     // Initialiser le joueur au premier nœud
//     InitializePlayer(&player, 0);

//     // Exécuter Bellman-Ford pour trouver le chemin optimal
//     int distances[MAX_NODES];
//     int predecessors[MAX_NODES];
//     BellmanFord(&maze.graph, 0, distances, predecessors);

//     // Afficher le chemin optimal
//     PrintPath(predecessors, 0, mazeWidth * mazeHeight - 1);

//     GameScreen currentScreen = TITLE;
//     double lastKeyPressTime = 0;  // Temps de la dernière pression de touche

//     // Boucle de jeu principale
//     while (!WindowShouldClose()) {
//         double currentTime = GetTime();

//         // Déplacement du joueur (exemple de mouvement avec les touches fléchées)
//         if (IsKeyPressed(KEY_RIGHT)) {
//             MovePlayer(&player, &maze, player.currentNode + 1);
//         }
//         if (IsKeyPressed(KEY_LEFT)) {
//             MovePlayer(&player, &maze, player.currentNode - 1);
//         }
//         if (IsKeyPressed(KEY_DOWN)) {
//             MovePlayer(&player, &maze, player.currentNode + maze.width);
//         }
//         if (IsKeyPressed(KEY_UP)) {
//             MovePlayer(&player, &maze, player.currentNode - maze.width);
//         }

//         // Vérifier si le joueur a atteint le nœud 99 (dernier nœud)
//         if (player.currentNode == 99) {
//             printf("Congratulations! You have reached the end of the maze.\n");
//             break; // Terminer la boucle de jeu
//         }

//         // Mise à jour des rooms
//         if (currentScreen == TITLE && IsKeyPressed(KEY_ENTER) && currentTime - lastKeyPressTime > 0.5)
//         {
//             currentScreen = GAMEPLAY; // Commence le jeu    
//             lastKeyPressTime = currentTime;  // Mettre à jour le temps de la dernière action
//         }
//         if (currentScreen == ENDING && IsKeyPressed(KEY_ENTER) && currentTime - lastKeyPressTime > 0.5)
//         {
            
//             currentScreen = WINROOM; // Commence le jeu    
//             lastKeyPressTime = currentTime;  // Mettre à jour le temps de la dernière action
//         }

//         BeginDrawing();
//         ClearBackground(BLACK);
        
        
//     switch (currentScreen) {
//         case TITLE: 
//             PlaySound(son_start);
//             if (IsKeyPressed(KEY_K))
//             {
//                 UnloadSound(son_start);  // Libère la mémoire utilisée par le son 
//             }
//             DrawText("Appuyez sur 'ENTRER' pour commencer", 200, screenHeight/2 -100, 45, LIGHTGRAY);
//             DrawTexture(player_texture, screenWidth/2-50, screenHeight/2 , WHITE);
//             //DrawTexture(ESIEA_logo_texture, screenWidth - screenWidth/2, 300, WHITE);
//             Rectangle sourceRec = { 0, 0, (float)ESIEA_logo.width, (float)ESIEA_logo.height };
//             Rectangle destRec = { screenWidth - 200, 0, 200, 150 };
//             Vector2 origin = { 0, 0 };

//             DrawTexturePro(ESIEA_logo_texture, sourceRec, destRec, origin, 0, WHITE);
//         break;
//         case GAMEPLAY:
//         if (IsKeyPressed(KEY_K))
//                 {
//                     UnloadSound(son_start);  // Libère la mémoire utilisée par le son 
//                 }
//                 // Ici, vous devriez mettre le code pour afficher le premier niveau du jeu
//                 DrawText("Niveau 1 ~ BELLMAN", 300, 0, 60, LIGHTGRAY);
                    
//                     // rectangle stat player

//                     Rectangle rec_player = { 585, 160, 420, 200 };  // Utilise la structure Rectangle pour déclarer
//                     DrawRectangle(rec_player.x, rec_player.y, rec_player.width, rec_player.height, LIGHTGRAY);
//                     int borderplayer = 5; // Épaisseur de la bordure
//                     DrawRectangleLinesEx(rec_player,borderplayer , BLACK);

//                     DrawText("Statistique Player:",595,170,20,BLACK); //mettre la barre de vie 
//                     int Player_health =12 ; //a changer !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//                     // Afficher la vie du personnage
//                     DrawText(TextFormat("Health: %d/100", Player_health), 595, 230, 30, RED);

//                     // rectangle stat mob
//                     Rectangle rec_mob = { 585, 400, 420, 220 };  
//                     DrawRectangle(rec_mob.x, rec_mob.y, rec_mob.width, rec_mob.height, LIGHTGRAY);
//                     int bordermob = 5; 
//                     DrawRectangleLinesEx(rec_mob, bordermob, BLACK);


//                     DrawText("Statistique Mobs:",595,410,20,BLACK);
                   
//                     DrawText("'L' pour LUTIN = 10",595,440,20,GREEN);
//                     DrawText("'G' pour GARDE = 20",595,470,20,BLUE);
//                     DrawText("'T' pour TROLL = 30",595,510,20,RED);
//                     DrawText("'M' pour MONSTRE = 30",595,530,20,RED);


//                     // rectangle indication deplacement
//                     Rectangle rec_indication = { 20, 650, 565, 140 }; 
//                     DrawRectangle(rec_indication.x, rec_indication.y, rec_indication.width, rec_indication.height, LIGHTGRAY);
//                     int borderdeplacement = 5; 
//                     DrawRectangleLinesEx(rec_indication, borderdeplacement, BLACK);

//                     /*Rectangle sourcetouches = { 0, 0, (float)touches_direction.width, (float)touches_direction.height };
//                     Rectangle desttouches = { 300, 650, 175, 110 };
//                     Vector2 origintouches = { 0, 0 };
                    
//                     DrawText("Déplacement:",35,660,20,BLACK);
//                     DrawTexturePro(touches_direction_texture, sourcetouches, desttouches, origintouches, 0, WHITE);
//                     DrawText("Utiliser les touches directionnelle \npour vous déplacer",35,700,17,BLACK);
//                     //DrawTexture(touches_direction_texture,23,600, WHITE);*/

//                     // rectangle consigne 
//                     Rectangle rec_consigne = { 585, 650, 450, 140 }; 
//                     DrawRectangle(rec_consigne.x, rec_consigne.y, rec_consigne.width, rec_consigne.height, LIGHTGRAY);
//                     int borderconsigne = 5; 
//                     DrawRectangleLinesEx(rec_consigne, borderconsigne, RED); 

//                     DrawText("Consignes du LvL:",600,660,20,BLACK);

//                     if (LVL < LVL_actuelle) { // changer la condition pour quelle change le texte quand on change de lvl
//                         if (currentLineLVL < maxLinesconsigne - 1) {
//                             currentLine++;
//                             LVL_actuelle++;
//                         }   
//                     }  
//                     if (currentLineLVL < maxLinesconsigne) {
//                         DrawText(consignes[currentLineLVL], 600, 700, 15, BLACK);
                            
//                     }
                

//                     // rectangle graph 
//                     Rectangle rec_graph = { 0, 0, 1080, 1080 }; 
//                     DrawRectangle(rec_graph.x, rec_graph.y, rec_graph.width, rec_graph.height, LIGHTGRAY);
//                     int bordergraph = 5; 
//                     DrawRectangleLinesEx(rec_graph, bordergraph, BLACK);

//                     // player helper
//                     DrawTexture(player_texture, 1000, 70, WHITE);

//                     // Paramètres du rectangle arrondi (bulle)
//                     Rectangle rect_bulle = { 930, 60, 100, 50 };
//                     float roundness = 1.0;  // Niveau d'arrondi des coins, 0.0 à 1.0
//                     int segments = 20;      // Nombre de segments pour dessiner les coins arrondis
//                     int borderbulle = 2; 
//                     // Dessiner un rectangle arrondi (bulle)
//                     DrawRectangleRounded(rect_bulle, roundness, segments, GRAY);
//                     DrawRectangleRoundedLinesEx(rect_bulle,roundness,segments,borderbulle,BLACK);

//                     //texte dans bulle
//                     DrawText("Aide 'H'?",935,76,20,BLACK);

//                      // rectangle explication helper  

//                     if (IsKeyPressed(KEY_H)) {
//                     {
//                         counthelper++;
//                     }
//                     }
//                     if(counthelper %2==0)
//                     {
//                         Rectangle rec_explication_helper = { 520, 100, 590, 700 }; 
//                         DrawRectangle(rec_explication_helper.x, rec_explication_helper.y, rec_explication_helper.width, rec_explication_helper.height, GRAY);
//                         int border_explication_helper = 5; 
//                         DrawRectangleLinesEx(rec_explication_helper, border_explication_helper, BLACK);
                        
//                         Rectangle rec_explication_bellman = { 530, 110, 570, 290 }; 
//                         DrawRectangle(rec_explication_bellman.x, rec_explication_bellman.y, rec_explication_bellman.width, rec_explication_bellman.height, LIGHTGRAY);
//                         int border_explication_bellman = 5; 
//                         DrawRectangleLinesEx(rec_explication_bellman, border_explication_bellman, BLACK);
                        
//                         DrawText("Étape 1: Initialisation\nInitialiser les distances :Pour chaque sommet v du graphe, initialisez la distance d[v]\n à l'infini .\n La distance du sommet de départ s à lui-même est définie à 0 (d[s] = 0)\n.Initialisez le prédécesseur de chaque sommet pred[v] à null.\n\nÉtape 2: Relaxation\nRelaxation des arêtes :\nRépétez les étapes suivantes |V| - 1 fois, où |V| est le nombre de sommets dans\n le graphe.\nPour chaque arête (u, v) avec un poids w(u, v) :\nSi d[u] + w(u, v) < d[v], mettez à jour d[v] et pred[v].\nd[v] = d[u] + w(u, v)\npred[v] = u\n\nÉtape 3: Détection des Cycles Négatifs\nVérification des cycles négatifs :\nPour chaque arête (u, v) :\nSi d[u] + w(u, v) < d[v], alors le graphe contient un cycle de poids négatif.\n Signalez une erreur.",545,110,12,BLACK);

//                         Rectangle rec_explication_bellman_code = { 530, 410, 300, 380 }; 
//                         DrawRectangle(rec_explication_bellman_code.x, rec_explication_bellman_code.y, rec_explication_bellman_code.width, rec_explication_bellman_code.height, LIGHTGRAY);
//                         int border_explication_bellman_code = 5; 
//                         DrawRectangleLinesEx(rec_explication_bellman_code, border_explication_bellman_code, BLACK);

//                         DrawText("fonction BellmanFord(G, w, s)\n// Initialisation\nfor each vertex v in G\n    d[v] = INT_MAX;\n   pred[v] = null\nd[s] = 0\n\n// Relaxation\nfor i = 1 to |V| - 1\n   for each edge (u, v) in G\n     if d[u] + w(u, v) < d[v] then\n         d[v] = d[u] + w(u, v)\n         pred[v] = u\n\n// Détection des cycles négatifs\nfor each edge (u, v) in G\n    if d[u] + w(u, v) < d[v] then\n         error 'G contains a negative\n         -weight cycle'\nreturn d, pred",540,420,15,BLACK);


//                         /*Rectangle rec_explication_bellman_exemple = { 830, 410, 250, 380 }; 
//                         DrawRectangle(rec_explication_bellman_exemple.x, rec_explication_bellman_exemple.y, rec_explication_bellman_exemple.width, rec_explication_bellman_exemple.height, LIGHTGRAY);
//                         int border_explication_bellman_exemple = 5; 
//                         DrawRectangleLinesEx(rec_explication_bellman_exemple, border_explication_bellman_exemple, BLACK);
//                         */

//                         DrawText("Cliquer sur 'H' pour\n fermer :)",840,470, 20,BLACK);


//                     }
//         /////////////////////////////////////////
//         // Rendu du labyrinthe
//         RenderMaze(&maze, screenWidth, screenHeight, cellSize);

//         // Rendu de la barre de santé
//         DrawHealthBar(&player);

//         // Rendu du joueur
//         Node currentNode = maze.graph.nodes[player.currentNode];
//         DrawCircle(currentNode.x * cellSize + cellSize / 2, currentNode.y * cellSize + cellSize / 2, 10, YELLOW);

//         break;
//         default :
//             break;
//     }

        

//         EndDrawing();
//     }

//     // De-initialisation
//     UnloadTexture(ESIEA_logo_texture); // Ne pas oublier de décharger la texture pour libérer la ressource
//     UnloadSound(son_start);  // Libère la mémoire utilisée par le son fxWav
//     CloseAudioDevice();  // Ferme le dispositif audio
//     UnloadTexture(player_texture); // Ne pas oublier de décharger la texture pour libérer la ressource
//     CloseWindow();        // Ferme la fenêtre et termine le programme

//     return 0;
// }
