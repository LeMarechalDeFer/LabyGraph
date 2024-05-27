#include "../include/graph.h"
#include "raylib.h"
#include "gamestate.h"
 cc game.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
  
#define CELL_SIZE 40  



int main() {
    // Initialisation de Raylib

    int screenWidth = 1200;
    int screenHeight = 800;

    int rectX = 70;
    int rectY = 160; 
    int rectWidth = 500;                // -> Player start points
    int rectHeight = 500;               // -> Player start points

    int LVL = 0;
    int LVL_actuelle =0;
    int counthelper =0;
    // Calculate the scaling factors
    float scaleX = (float)rectWidth / (WIDTH - 1);
    float scaleY = (float)rectHeight / (HEIGHT - 1);

    InitWindow(screenWidth, screenHeight, "Menu du jeu éducatif");
    SetTraceLogLevel(LOG_ALL); // Activer les logs détaillés

    // Charger les images 
    Image player_Helper = LoadImage("src/player.png");
    Image ESIEA_logo = LoadImage("src/ESIEA-logo.png");
    Image dialogue_box = LoadImage("src/dialogue_box.png");
    Image touches_direction =LoadImage("src/touches_direction.png");
    
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
 
     // Initialiser le générateur de nombres aléatoires
    srand(time(NULL));

    // Créer et initialiser le labyrinthe
    
    // Création et initialisation du labyrinthe
    Maze maze;
    Player player;

    int mazeWidth = 50;  // Largeur du labyrinthe en nombre de cellules
    int mazeHeight = 50; // Hauteur du labyrinthe en nombre de cellules


    InitializeMaze(&maze, WIDTH, HEIGHT);
    GenerateMaze(&maze);

    InitializePlayer(&player, 0);
   
   // Enemy enemies[MAX_ENEMIES];
    //InitializeEnemies(enemies,&maze);

    SetTargetFPS(60); // Définir le FPS pour une animation fluide
    
    char *monologue[] = 
    {
        "Bienvenue jeune aventurier dans notre jeu éducatif !",
        "Ton rôle est de monter au dernier étage de cette tour.",
        "Afin de voir si tu es bien un pro des graphes.",
        "Es-tu prêt ?",
        "Parfait !! Dans ce cas, bienvenue au premier étage.\nDans chaque étage, tu auras des défis à réaliser\n afin de pouvoir continuer ton ascension. ",
    };
    char *consignes[] = 
    {
        "Vous devez trouver le moyen d'affronter le moins\n d'énenmies possible en suivant\n l'algo de Bellman.",
        "Vous devez trouver le moyen d'affronter le plus\n d'énenmies possible en suivant\n l'algo de Bellman.",
        "la suite des consigne...",
    };

    int maxLines = sizeof(monologue) / sizeof(monologue[0]);
    int maxLinesconsigne = sizeof(consignes) / sizeof(consignes[0]);

    int currentLine = 0;
    int currentLineLVL = 0;

    // Définir les états de jeu
    GameScreen currentScreen = TITLE;
    double lastKeyPressTime = 0;  // Temps de la dernière pression de touche

    // Boucle principale du jeu
    while (!WindowShouldClose()) {    // Détecter la fermeture de la fenêtre
        double currentTime = GetTime();
        // Condition to make the circle stay in the maze
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
        // Mise à jour des entrées
        if (currentScreen == TITLE && IsKeyPressed(KEY_ENTER) && currentTime - lastKeyPressTime > 0.5)
        {
            currentScreen = GAMEPLAY; // Commence le jeu    
            lastKeyPressTime = currentTime;  // Mettre à jour le temps de la dernière action
        }
        if (currentScreen == ENDING && IsKeyPressed(KEY_ENTER) && currentTime - lastKeyPressTime > 0.5)
        {
            
            currentScreen = WINROOM; // Commence le jeu    
            lastKeyPressTime = currentTime;  // Mettre à jour le temps de la dernière action
        }
        // Commence à dessiner
        BeginDrawing();

        ClearBackground(WHITE);

        switch (currentScreen) {
            case TITLE: 
                PlaySound(son_start);
                if (IsKeyPressed(KEY_K))
                {
                    UnloadSound(son_start);  // Libère la mémoire utilisée par le son 
                }
                DrawText("Appuyez sur 'ENTRER' pour commencer", 200, screenHeight/2 -100, 45, LIGHTGRAY);
                DrawTexture(player_texture, screenWidth/2-50, screenHeight/2 , WHITE);
                //DrawTexture(ESIEA_logo_texture, screenWidth - screenWidth/2, 300, WHITE);
                Rectangle sourceRec = { 0, 0, (float)ESIEA_logo.width, (float)ESIEA_logo.height };
                Rectangle destRec = { screenWidth - 200, 0, 200, 150 };
                Vector2 origin = { 0, 0 };

                DrawTexturePro(ESIEA_logo_texture, sourceRec, destRec, origin, 0, WHITE);
            break;
            case EXPLANATION:
                DrawText("Fonction Bellman-Ford(G = (S, A), poids, s) \n pour u dans S faire \n |d[u] = +infini \n |pred[u] = null \n d[s] = 0 -> Boucle principale \n pour k = 1 à taille(S) - 1 faire \n|pour chaque arc (u, v) du graphe faire \n|      |    si d[u] + poids(u, v) < d[v] alors \n|      |    |    d[v] := d[u] + poids(u, v) \n|      |    |    pred[v]:= u \n retourner d, pred",
                 190, 300, 20, DARKGRAY);
                break;

            case GAMEPLAY:

                if (IsKeyPressed(KEY_K))
                {
                    UnloadSound(son_start);  // Libère la mémoire utilisée par le son 
                }
                // Ici, vous devriez mettre le code pour afficher le premier niveau du jeu
                DrawText("Niveau 1 ~ BELLMAN", 300, 0, 60, LIGHTGRAY);
                    
                    // rectangle stat player

                    Rectangle rec_player = { 585, 160, 420, 200 };  // Utilise la structure Rectangle pour déclarer
                    DrawRectangle(rec_player.x, rec_player.y, rec_player.width, rec_player.height, LIGHTGRAY);
                    int borderplayer = 5; // Épaisseur de la bordure
                    DrawRectangleLinesEx(rec_player,borderplayer , BLACK);

                    DrawText("Statistique Player:",595,170,20,BLACK); //mettre la barre de vie 
                    int Player_health =12 ; //a changer !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                    // Afficher la vie du personnage
                    DrawText(TextFormat("Health: %d/100", Player_health), 595, 230, 30, RED);

                    // rectangle stat mob
                    Rectangle rec_mob = { 585, 400, 420, 220 };  
                    DrawRectangle(rec_mob.x, rec_mob.y, rec_mob.width, rec_mob.height, LIGHTGRAY);
                    int bordermob = 5; 
                    DrawRectangleLinesEx(rec_mob, bordermob, BLACK);


                    DrawText("Statistique Mobs:",595,410,20,BLACK);
                   
                    DrawText("'L' pour LUTIN = 10",595,440,20,GREEN);
                    DrawText("'G' pour GARDE = 20",595,470,20,BLUE);
                    DrawText("'T' pour TROLL = 30",595,510,20,RED);
                    DrawText("'M' pour MONSTRE = 30",595,530,20,RED);


                    // rectangle indication deplacement
                    Rectangle rec_indication = { 20, 650, 565, 140 }; 
                    DrawRectangle(rec_indication.x, rec_indication.y, rec_indication.width, rec_indication.height, LIGHTGRAY);
                    int borderdeplacement = 5; 
                    DrawRectangleLinesEx(rec_indication, borderdeplacement, BLACK);

                    Rectangle sourcetouches = { 0, 0, (float)touches_direction.width, (float)touches_direction.height };
                    Rectangle desttouches = { 300, 650, 175, 110 };
                    Vector2 origintouches = { 0, 0 };
                    
                    DrawText("Déplacement:",35,660,20,BLACK);
                    DrawTexturePro(touches_direction_texture, sourcetouches, desttouches, origintouches, 0, WHITE);
                    DrawText("Utiliser les touches directionnelle \npour vous déplacer",35,700,17,BLACK);
                    //DrawTexture(touches_direction_texture,23,600, WHITE);

                    // rectangle consigne 
                    Rectangle rec_consigne = { 585, 650, 450, 140 }; 
                    DrawRectangle(rec_consigne.x, rec_consigne.y, rec_consigne.width, rec_consigne.height, LIGHTGRAY);
                    int borderconsigne = 5; 
                    DrawRectangleLinesEx(rec_consigne, borderconsigne, RED); 

                    DrawText("Consignes du LvL:",600,660,20,BLACK);

                    if (LVL < LVL_actuelle) { // changer la condition pour quelle change le texte quand on change de lvl
                        if (currentLineLVL < maxLinesconsigne - 1) {
                            currentLine++;
                            LVL_actuelle++;
                        }   
                    }  
                    if (currentLineLVL < maxLinesconsigne) {
                        DrawText(consignes[currentLineLVL], 600, 700, 15, BLACK);
                            
                    }
                

                    // rectangle graph 
                    Rectangle rec_graph = { 70, 160, 470, 470 }; 
                    DrawRectangle(rec_graph.x, rec_graph.y, rec_graph.width, rec_graph.height, LIGHTGRAY);
                    int bordergraph = 5; 
                    DrawRectangleLinesEx(rec_graph, bordergraph, BLACK);

                    // player helper
                    DrawTexture(player_texture, 1000, 70, WHITE);

                    // Paramètres du rectangle arrondi (bulle)
                    Rectangle rect_bulle = { 930, 60, 100, 50 };
                    float roundness = 1.0;  // Niveau d'arrondi des coins, 0.0 à 1.0
                    int segments = 20;      // Nombre de segments pour dessiner les coins arrondis
                    int borderbulle = 2; 
                    // Dessiner un rectangle arrondi (bulle)
                    DrawRectangleRounded(rect_bulle, roundness, segments, GRAY);
                    DrawRectangleRoundedLinesEx(rect_bulle,roundness,segments,borderbulle,BLACK);

                    //texte dans bulle
                    DrawText("Aide 'H'?",935,76,20,BLACK);

                     // rectangle explication helper  

                    if (IsKeyPressed(KEY_H)) {
                    {
                        counthelper++;
                    }
                    }
                    if(counthelper %2==0)
                    {
                        Rectangle rec_explication_helper = { 520, 100, 590, 700 }; 
                        DrawRectangle(rec_explication_helper.x, rec_explication_helper.y, rec_explication_helper.width, rec_explication_helper.height, GRAY);
                        int border_explication_helper = 5; 
                        DrawRectangleLinesEx(rec_explication_helper, border_explication_helper, BLACK);
                        
                        Rectangle rec_explication_bellman = { 530, 110, 570, 290 }; 
                        DrawRectangle(rec_explication_bellman.x, rec_explication_bellman.y, rec_explication_bellman.width, rec_explication_bellman.height, LIGHTGRAY);
                        int border_explication_bellman = 5; 
                        DrawRectangleLinesEx(rec_explication_bellman, border_explication_bellman, BLACK);
                        
                        DrawText("Étape 1: Initialisation\nInitialiser les distances :Pour chaque sommet v du graphe, initialisez la distance d[v]\n à l'infini .\n La distance du sommet de départ s à lui-même est définie à 0 (d[s] = 0)\n.Initialisez le prédécesseur de chaque sommet pred[v] à null.\n\nÉtape 2: Relaxation\nRelaxation des arêtes :\nRépétez les étapes suivantes |V| - 1 fois, où |V| est le nombre de sommets dans\n le graphe.\nPour chaque arête (u, v) avec un poids w(u, v) :\nSi d[u] + w(u, v) < d[v], mettez à jour d[v] et pred[v].\nd[v] = d[u] + w(u, v)\npred[v] = u\n\nÉtape 3: Détection des Cycles Négatifs\nVérification des cycles négatifs :\nPour chaque arête (u, v) :\nSi d[u] + w(u, v) < d[v], alors le graphe contient un cycle de poids négatif.\n Signalez une erreur.",545,110,12,BLACK);

                        Rectangle rec_explication_bellman_code = { 530, 410, 300, 380 }; 
                        DrawRectangle(rec_explication_bellman_code.x, rec_explication_bellman_code.y, rec_explication_bellman_code.width, rec_explication_bellman_code.height, LIGHTGRAY);
                        int border_explication_bellman_code = 5; 
                        DrawRectangleLinesEx(rec_explication_bellman_code, border_explication_bellman_code, BLACK);

                        DrawText("fonction BellmanFord(G, w, s)\n// Initialisation\nfor each vertex v in G\n    d[v] = INT_MAX;\n   pred[v] = null\nd[s] = 0\n\n// Relaxation\nfor i = 1 to |V| - 1\n   for each edge (u, v) in G\n     if d[u] + w(u, v) < d[v] then\n         d[v] = d[u] + w(u, v)\n         pred[v] = u\n\n// Détection des cycles négatifs\nfor each edge (u, v) in G\n    if d[u] + w(u, v) < d[v] then\n         error 'G contains a negative\n         -weight cycle'\nreturn d, pred",540,420,15,BLACK);


                        /*Rectangle rec_explication_bellman_exemple = { 830, 410, 250, 380 }; 
                        DrawRectangle(rec_explication_bellman_exemple.x, rec_explication_bellman_exemple.y, rec_explication_bellman_exemple.width, rec_explication_bellman_exemple.height, LIGHTGRAY);
                        int border_explication_bellman_exemple = 5; 
                        DrawRectangleLinesEx(rec_explication_bellman_exemple, border_explication_bellman_exemple, BLACK);
                        */

                        DrawText("Cliquer sur 'H' pour\n fermer :)",840,470, 20,BLACK);


                    }


                 // Fonction pour dessiner le labyrinthe
                RenderMaze(&maze, rectX, rectY, rectWidth, rectHeight);
                    
                Node currentNode = maze.graph.nodes[player.currentNode];
                int playerX = rectX + currentNode.x * scaleX;
                int playerY = rectY + currentNode.y * scaleY;
                DrawCircle(playerX * CELL_SIZE + CELL_SIZE / 2, playerY * CELL_SIZE + CELL_SIZE / 2, 10, BLUE);
                // Entrée du Maze
                DrawText("E",65,150,30,RED); // coordonnées de l'entrée
                // Sortie du Maze
                DrawText("S",525,610,30,RED); // coordonnées de la sortie

             /*   if(player.currentNode == edge.end)
                {
                    currentScreen = ENDING;
                }*/
                // Rendu de la barre de santé
                DrawHealthBar(&player);
                
                //Monologue 

                bool displayText0 = true; // Variable contrôlant l'affichage du texte

                Rectangle sourceMonologue = { 0, 0, (float)dialogue_box_texture.width, (float)dialogue_box_texture.height };
                Rectangle dimMonologue = { 200, 50, 1000, 300 };
                Vector2 originMonologue = { 0, 0 };

                Color transparentColor = Fade(WHITE, 1.0f);  // Ajuste le second paramètre pour contrôler la transparence 1.0 =100%
                DrawTexturePro(dialogue_box_texture, sourceMonologue, dimMonologue, originMonologue, 0, transparentColor);

                if (IsKeyPressed(KEY_P)) {
                    if (currentLine < maxLines - 1) {
                        currentLine++;
                    }   
                }

                if (currentLine == maxLines - 1) {
                    displayText0 = !displayText0;
                    UnloadTexture(dialogue_box_texture); // Décharger la texture
                    dialogue_box_texture.id = 0; // Assure-toi de réinitialiser l'ID après le déchargement
                }
                
                if (displayText0) {
                    DrawTexturePro(dialogue_box_texture, sourceMonologue, dimMonologue, originMonologue, 0, transparentColor);
                    if (currentLine < maxLines) {
                        DrawText(monologue[currentLine], 250, 180, 30, RED);
                        DrawText("Appuier sur P pour continuer", 800, 300, 18, RED);  
                    }
                }
                break;
              case ENDING:
                DrawText("Vous avez echoué ", 175, 200, 100, LIGHTGRAY);
                DrawText("Appuier sur ENTRER pour continuer", 200, 300, 18, BLACK);  
                DrawTexture(player_texture, screenWidth/2-50, screenHeight/2 , WHITE);
                break;
            
            case WINROOM:
                 ClearBackground(BLUE);
                DrawText("Vous avez Réussi !", 175, 200, 100, LIGHTGRAY);
                DrawText("Appuier sur ENTRER pour continuer", 300, 300, 18, BLACK);  
                DrawTexture(player_texture, screenWidth/2-50, screenHeight/2 , WHITE);
                break;
            default:
                break;

            
        }

        EndDrawing();
    }

    // De-initialisation
    UnloadTexture(ESIEA_logo_texture); // Ne pas oublier de décharger la texture pour libérer la ressource
    UnloadSound(son_start);  // Libère la mémoire utilisée par le son fxWav
    CloseAudioDevice();  // Ferme le dispositif audio
    UnloadTexture(player_texture); // Ne pas oublier de décharger la texture pour libérer la ressource
    CloseWindow();        // Ferme la fenêtre et termine le programme

    return 0;
}
