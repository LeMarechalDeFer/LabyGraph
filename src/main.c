#include "../include/graph.h"

int main() {
    // Initialisation de Raylib

    // Dimensions de la fenêtre
    int screenWidth = 1920;
    int screenHeight = 1080;

    // Initialisation de la fenêtre Raylib
    InitWindow(screenWidth, screenHeight, "Menu du jeu éducatif");
    SetTraceLogLevel(LOG_ALL); // Activer les logs détaillés
    SetTargetFPS(60); // Définir le FPS pour une animation fluide

    int LVL = 0;
    int LVL_actuelle = 0;
    int counthelper = 1;
    int level = 1;  // Niveau actuel

    // Charger les images
    Image player_Helper = LoadImage("src/player.png");
    Image ESIEA_logo = LoadImage("src/ESIEA-logo.png");
    Image dialogue_box = LoadImage("src/dialogue_box.png");
    Image touches_direction = LoadImage("src/touches_direction.png");

    // Vérifiez si les images sont chargées correctement
    if (player_Helper.data == NULL) {
        printf("Erreur: Impossible de charger player.png\n");
        return 1;
    }
    if (ESIEA_logo.data == NULL) {
        printf("Erreur: Impossible de charger ESIEA-logo.png\n");
        return 1;
    }
    if (dialogue_box.data == NULL) {
        printf("Erreur: Impossible de charger dialogue_box.png\n");
        return 1;
    }
    if (touches_direction.data == NULL) {
        printf("Erreur: Impossible de charger touches_direction.png\n");
        return 1;
    }

    Texture2D player_texture = LoadTextureFromImage(player_Helper);
    Texture2D ESIEA_logo_texture = LoadTextureFromImage(ESIEA_logo);
    Texture2D dialogue_box_texture = LoadTextureFromImage(dialogue_box);
    Texture2D touches_direction_texture = LoadTextureFromImage(touches_direction);

    UnloadImage(player_Helper);
    UnloadImage(ESIEA_logo);
    UnloadImage(dialogue_box);
    UnloadImage(touches_direction);

    // Charger les sons
    InitAudioDevice();
    Sound son_start = LoadSound("src/voracious.mp3");

    // Vérifiez si le son est chargé correctement
    if (son_start.frameCount == 0) {
        printf("Erreur: Impossible de charger voracious.mp3\n");
        return 1;
    }

    // Initialisation du générateur de nombres aléatoires
    srand(time(NULL));

    char *monologue[] = {
        "Bienvenue jeune aventurier dans notre jeu éducatif !",
        "Ton rôle est de monter au dernier étage de cette tour.",
        "Afin de voir si tu es bien un pro des graphes.",
        "Es-tu prêt ?",
        "Parfait !! Dans ce cas, bienvenue au premier étage.\nDans chaque étage, tu auras des défis à réaliser\n afin de pouvoir continuer ton ascension. ",
    };
    char *consignes[] = {
        "Vous devez trouver le moyen d'affronter le moins\n d'ennemis possible en suivant\n l'algo de Bellman.",
        "Vous devez trouver le moyen d'affronter le plus\n d'ennemis possible en suivant\n l'algo de Bellman.",
        
    };

    int maxLines = sizeof(monologue) / sizeof(monologue[0]);
    int maxLinesconsigne = sizeof(consignes) / sizeof(consignes[0]);

    int currentLine = 0;
    int currentLineLVL = 0;
    // Initialisation des structures
    Maze maze;
    Player player;

    // Initialiser le labyrinthe
    int mazeWidth = 10;  // Largeur du labyrinthe en nombre de cellules
    int mazeHeight = 10; // Hauteur du labyrinthe en nombre de cellules
    InitializeMaze(&maze, mazeWidth, mazeHeight);
    GenerateMaze(&maze);

    // Calculer dynamiquement la taille des cellules en fonction des dimensions de la fenêtre
    int cellSize = (screenWidth < screenHeight ? screenWidth : screenHeight) / (mazeWidth > mazeHeight ? mazeWidth : mazeHeight);

    // Initialiser le joueur au premier nœud
    InitializePlayer(&player, 0);

    // Exécuter Bellman-Ford pour trouver le chemin optimal
    int distances[MAX_NODES];
    int predecessors[MAX_NODES];
    int operations;

    operations = BellmanFord(&maze.graph, 0, distances, predecessors);
    PrintPath(predecessors, 0, mazeWidth * mazeHeight - 1);
    printf("Bellman-Ford: %d opérations\n", operations);

    // Exécuter Dijkstra pour trouver le chemin optimal
    operations = Dijkstra(&maze.graph, 0, distances, predecessors);
    PrintPath(predecessors, 0, mazeWidth * mazeHeight - 1);
    printf("Dijkstra: %d opérations\n", operations);

    // Exécuter A* pour trouver le chemin optimal
    operations = AStar(&maze.graph, 0, mazeWidth * mazeHeight - 1, distances, predecessors);
    PrintPath(predecessors, 0, mazeWidth * mazeHeight - 1);
    printf("A*: %d opérations\n", operations);

    GameScreen currentScreen = TITLE;
    double lastKeyPressTime = 0;  // Temps de la dernière pression de touche

    // Boucle de jeu principale
    while (!WindowShouldClose()) {
        double currentTime = GetTime();

        // Déplacement du joueur (exemple de mouvement avec les touches fléchées)
        if (IsKeyPressed(KEY_RIGHT)) {
            MovePlayer(&player, &maze, player.currentNode + 1);
        }
        if (IsKeyPressed(KEY_LEFT)) {
            MovePlayer(&player, &maze, player.currentNode - 1);
        }
        if (IsKeyPressed(KEY_DOWN)) {
            MovePlayer(&player, &maze, player.currentNode + maze.width);
        }
        if (IsKeyPressed(KEY_UP)) {
            MovePlayer(&player, &maze, player.currentNode - maze.width);
        }

        // Vérifier si le joueur a atteint le nœud 99 (dernier nœud)
        if (player.currentNode == 99) {
            currentScreen = WINROOM;
            lastKeyPressTime = currentTime;

            if (IsKeyPressed(KEY_ENTER)) {
                if (level < 3) {
                    level++;
                } else {
                    level = 1; // Revenir au niveau 1 après le niveau 3
                }
                // Réinitialiser le jeu
                ResetGame(&maze, &player, screenWidth, screenHeight);
                currentScreen = GAMEPLAY;
                continue;
            }
        }

        // Mise à jour des rooms
        if (currentScreen == TITLE && IsKeyPressed(KEY_ENTER) && currentTime - lastKeyPressTime > 0.5) {
            currentScreen = GAMEPLAY;
            lastKeyPressTime = currentTime;
        }
        if (currentScreen == ENDING && IsKeyPressed(KEY_ENTER) && currentTime - lastKeyPressTime > 0.5) {
            currentScreen = WINROOM;
            lastKeyPressTime = currentTime;
        }

        BeginDrawing();
        ClearBackground(LIGHTGRAY);

        switch (currentScreen) {
            case TITLE: 
                PlaySound(son_start);
                if (IsKeyPressed(KEY_K)) {
                    UnloadSound(son_start);
                }
                DrawText("Appuyez sur 'ENTRER' pour commencer", 450, screenHeight/2 - 100, 70, BLACK);
                DrawTexture(player_texture, screenWidth / 2 - 50, screenHeight / 2, WHITE);
                Rectangle sourceRec = { 0, 0, (float)ESIEA_logo.width, (float)ESIEA_logo.height };
                Rectangle destRec = { screenWidth - 200, 0, 200, 150 };
                Vector2 origin = { 0, 0 };

                DrawTexturePro(ESIEA_logo_texture, sourceRec, destRec, origin, 0, WHITE);
                break;
            case GAMEPLAY:
                if (IsKeyPressed(KEY_K)) {
                    UnloadSound(son_start);
                }
                DrawText(TextFormat("Niveau %d", level), 300, 0, 60, LIGHTGRAY);
                Rectangle rec_graph = { 0, 0, 1080, 1080 };
                DrawRectangle(rec_graph.x, rec_graph.y, rec_graph.width, rec_graph.height, LIGHTGRAY);
                int bordergraph = 5;
                DrawRectangleLinesEx(rec_graph, bordergraph, BLACK);

                RenderMaze(&maze, cellSize);

                DrawHealthBar(&player);

                Node currentNode = maze.graph.nodes[player.currentNode];
                DrawCircle(currentNode.x * cellSize + cellSize / 2, currentNode.y * cellSize + cellSize / 2, 10, YELLOW);

                Rectangle rec_player = { 1150, 83, 420, 200 };
                DrawRectangle(rec_player.x, rec_player.y, rec_player.width, rec_player.height, LIGHTGRAY);
                int borderplayer = 5;
                DrawRectangleLinesEx(rec_player, borderplayer, BLACK);
                DrawText("Statistique Player:", 1170, 90, 20, BLACK);
                DrawText(TextFormat("Health: %d/200", player.health), 1160, 230, 30, RED);

                Rectangle rec_mob = { 1150, 300, 420, 220 };
                DrawRectangle(rec_mob.x, rec_mob.y, rec_mob.width, rec_mob.height, LIGHTGRAY);
                int bordermob = 5;
                DrawRectangleLinesEx(rec_mob, bordermob, BLACK);

                DrawText("Statistique Mobs:", 1170, 310, 20, BLACK);
                DrawText("'L' pour LUTIN ", 1170, 340, 20, GREEN);
                DrawText("'G' pour GARDE ", 1170, 370, 20, BLUE);
                DrawText("'T' pour TROLL ", 1170, 410, 20, RED);
                DrawText("'M' pour MONSTRE ", 1170, 430, 20, RED);

                Rectangle rec_indication = { 1090, 650, 465, 140 };
                DrawRectangle(rec_indication.x, rec_indication.y, rec_indication.width, rec_indication.height, LIGHTGRAY);
                int borderdeplacement = 5;
                DrawRectangleLinesEx(rec_indication, borderdeplacement, BLACK);

                Rectangle sourcetouches = { 0, 0, (float)touches_direction.width, (float)touches_direction.height };
                Rectangle desttouches = { 1350, 650, 175, 110 };
                Vector2 origintouches = { 0, 0 };

                DrawText("Déplacement:", 1100, 660, 20, BLACK);
                DrawTexturePro(touches_direction_texture, sourcetouches, desttouches, origintouches, 0, WHITE);
                DrawText("Utiliser les touches directionnelle \npour vous déplacer", 1110, 700, 17, BLACK);

                Rectangle rec_consigne = { 1090 + 465, 650, 450, 140 };
                DrawRectangle(rec_consigne.x, rec_consigne.y, rec_consigne.width, rec_consigne.height, LIGHTGRAY);
                int borderconsigne = 5;
                DrawRectangleLinesEx(rec_consigne, borderconsigne, RED);

                DrawText("Consignes du LvL:", 1090 + 465 + 10, 660, 20, BLACK);

                if (LVL < LVL_actuelle) {
                    if (currentLineLVL < maxLinesconsigne - 1) {
                        currentLine++;
                        LVL_actuelle++;
                    }
                }
                if (currentLineLVL < maxLinesconsigne) {
                    DrawText(consignes[currentLineLVL], 1090 + 465 + 10, 700, 15, BLACK);
                }

                DrawTexture(player_texture, 1600, 90, WHITE);

                Rectangle rect_bulle = { 1550, 70, 100, 50 };
                float roundness = 1.0;
                int segments = 20;
                int borderbulle = 2;
                DrawRectangleRounded(rect_bulle, roundness, segments, GRAY);
                DrawRectangleRoundedLinesEx(rect_bulle, roundness, segments, borderbulle, BLACK);
                DrawText("Aide 'H'?", 1560, 76, 20, BLACK);

                if (IsKeyPressed(KEY_H)) {
                    counthelper++;
                }
                if (counthelper % 2 == 0) {
                    Rectangle rec_explication_helper = { 900, 80, 690, 800 };
                    DrawRectangle(rec_explication_helper.x, rec_explication_helper.y, rec_explication_helper.width, rec_explication_helper.height, GRAY);
                    int border_explication_helper = 5;
                    DrawRectangleLinesEx(rec_explication_helper, border_explication_helper, BLACK);

                    Rectangle rec_explication_bellman = { 920, 90, 670, 390 };
                    DrawRectangle(rec_explication_bellman.x, rec_explication_bellman.y, rec_explication_bellman.width, rec_explication_bellman.height, LIGHTGRAY);
                    int border_explication_bellman = 5;
                    DrawRectangleLinesEx(rec_explication_bellman, border_explication_bellman, BLACK);

                    DrawText("Étape 1: Initialisation\nInitialiser les distances :Pour chaque sommet v du graphe, initialisez la distance d[v]\n à l'infini .\n La distance du sommet de départ s à lui-même est définie à 0 (d[s] = 0)\n.Initialisez le prédécesseur de chaque sommet pred[v] à null.\n\nÉtape 2: Relaxation\nRelaxation des arêtes :\nRépétez les étapes suivantes |V| - 1 fois, où |V| est le nombre de sommets dans\n le graphe.\nPour chaque arête (u, v) avec un poids w(u, v) :\nSi d[u] + w(u, v) < d[v], mettez à jour d[v] et pred[v].\nd[v] = d[u] + w(u, v)\npred[v] = u\n\nÉtape 3: Détection des Cycles Négatifs\nVérification des cycles négatifs :\nPour chaque arête (u, v) :\nSi d[u] + w(u, v) < d[v], alors le graphe contient un cycle de poids négatif.\n Signalez une erreur.", 930, 110, 15, BLACK);

                    Rectangle rec_explication_bellman_code = { 920, 470, 400, 480 };
                    DrawRectangle(rec_explication_bellman_code.x, rec_explication_bellman_code.y, rec_explication_bellman_code.width, rec_explication_bellman_code.height, LIGHTGRAY);
                    int border_explication_bellman_code = 5;
                    DrawRectangleLinesEx(rec_explication_bellman_code, border_explication_bellman_code, BLACK);

                    DrawText("fonction BellmanFord(G, w, s)\n// Initialisation\nfor each vertex v in G\n    d[v] = INT_MAX;\n   pred[v] = null\nd[s] = 0\n\n// Relaxation\nfor i = 1 to |V| - 1\n   for each edge (u, v) in G\n     if d[u] + w(u, v) < d[v] then\n         d[v] = d[u] + w(u, v)\n         pred[v] = u\n\n// Détection des cycles négatifs\nfor each edge (u, v) in G\n    if d[u] + w(u, v) < d[v] then\n         error 'G contains a negative\n         -weight cycle'\nreturn d, pred", 930, 490, 17, BLACK);

                    DrawText("Cliquer sur 'H' pour\n fermer :)", 1350, 650, 20, BLACK);
                }

                bool displayText0 = true;

                Rectangle sourceMonologue = { 0, 0, (float)dialogue_box_texture.width, (float)dialogue_box_texture.height };
                Rectangle dimMonologue = { 800, 50, 1000, 300 };
                Vector2 originMonologue = { 0, 0 };

                Color transparentColor = Fade(WHITE, 1.0f);
                DrawTexturePro(dialogue_box_texture, sourceMonologue, dimMonologue, originMonologue, 0, transparentColor);

                if (IsKeyPressed(KEY_P)) {
                    if (currentLine < maxLines - 1) {
                        currentLine++;
                    }
                }

                if (currentLine == maxLines - 1) {
                    displayText0 = !displayText0;
                    UnloadTexture(dialogue_box_texture);
                    dialogue_box_texture.id = 0;
                }

                if (displayText0) {
                    DrawTexturePro(dialogue_box_texture, sourceMonologue, dimMonologue, originMonologue, 0, transparentColor);
                    if (currentLine < maxLines) {
                        DrawText(monologue[currentLine], 850, 180, 30, RED);
                        DrawText("Appuier sur P pour continuer", 1200, 300, 18, RED);
                    }
                }
                break;
            case ENDING:
                DrawText("Vous avez échoué ", 175, 200, 100, LIGHTGRAY);
                DrawText("Appuier sur ENTRER pour continuer", 200, 300, 18, BLACK);
                DrawTexture(player_texture, screenWidth / 2 - 50, screenHeight / 2, WHITE);
                break;

            case WINROOM:
                ClearBackground(BLUE);
                DrawText("Vous avez Réussi !", 175, 200, 100, LIGHTGRAY);
                DrawText("Appuier sur ENTRER pour continuer", 300, 300, 18, BLACK);
                DrawTexture(player_texture, screenWidth / 2 - 50, screenHeight / 2, WHITE);
                break;
            default:
                break;
        }

        EndDrawing();
    }

    // De-initialisation
    UnloadTexture(ESIEA_logo_texture);
    UnloadSound(son_start);
    CloseAudioDevice();
    UnloadTexture(player_texture);
    CloseWindow();

    return 0;
}

