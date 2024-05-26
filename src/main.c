#include "../include/graph.h"
#include "raylib.h"
// #include "gamestate.h"
//  cc game.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
  
#define CELL_SIZE 40  



int main() {
    // Initialisation de Raylib

    int screenWidth = 1200;
    int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Menu du jeu éducatif");
        SetTraceLogLevel(LOG_ALL); // Activer les logs détaillés

    // Charger les images 
    Image player_Helper = LoadImage("src/player.png");
    Image ESIEA_logo = LoadImage("src/ESIEA-logo.png");
    Image dialogue_box = LoadImage("src/dialogue_box.png");

    
    Texture2D player_texture = LoadTextureFromImage(player_Helper);
    Texture2D ESIEA_logo_texture = LoadTextureFromImage(ESIEA_logo);
    Texture2D dialogue_box_texture = LoadTextureFromImage(dialogue_box);

    
    UnloadImage(player_Helper);
    UnloadImage(ESIEA_logo);
    UnloadImage(dialogue_box);


    // Charger les sons
    InitAudioDevice();
    Sound son_start = LoadSound("src/voracious.mp3");  

 
     // Initialiser le générateur de nombres aléatoires
    srand(time(NULL));

    // Créer et initialiser le labyrinthe
    
    // Création et initialisation du labyrinthe
    Maze maze;
    Player player;

    InitializeMazeLevel1(&maze);

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

    int maxLines = sizeof(monologue) / sizeof(monologue[0]);
    int currentLine = 0;
    // Définir les états de jeu
    GameScreen currentScreen = TITLE;
    double lastKeyPressTime = 0;  // Temps de la dernière pression de touche

    // Boucle principale du jeu
    while (!WindowShouldClose()) {    // Détecter la fermeture de la fenêtre
        double currentTime = GetTime();

        // Condition to make the circle stay in the maze

        // Déplacement du joueur (exemple de mouvement avec les touches fléchées)
        if (IsKeyPressed(KEY_RIGHT) && player.currentNode %  WIDTH < WIDTH - 1) {
            MovePlayer(&player, player.currentNode + 1);
        }
        if (IsKeyPressed(KEY_LEFT) && player.currentNode % WIDTH > 0) {
            MovePlayer(&player, player.currentNode - 1);
        }
        if (IsKeyPressed(KEY_DOWN) && player.currentNode / WIDTH < HEIGHT - 1) {
            MovePlayer(&player, player.currentNode + WIDTH);
        }
        if (IsKeyPressed(KEY_UP) && player.currentNode / WIDTH > 0) {
            MovePlayer(&player, player.currentNode - WIDTH);
        }
        // Mise à jour des entrées
        if (currentScreen == TITLE && IsKeyPressed(KEY_ENTER) && currentTime - lastKeyPressTime > 0.5)
        {
            currentScreen = GAMEPLAY; // Commence le jeu    
            lastKeyPressTime = currentTime;  // Mettre à jour le temps de la dernière action
        }

        if (currentScreen == GAMEPLAY && IsKeyPressed(KEY_ENTER)&& currentTime - lastKeyPressTime > 0.5)
        {
            currentScreen = EXPLANATION;
            lastKeyPressTime = currentTime;
        }
        // Commence à dessiner
        BeginDrawing();

        ClearBackground(RAYWHITE);

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
                    // rectangle state player
                    Rectangle rec_player = { 585, 160, 420, 200 };  // Utilise la structure Rectangle pour déclarer
                    DrawRectangle(rec_player.x, rec_player.y, rec_player.width, rec_player.height, LIGHTGRAY);
                    int borderplayer = 5; // Épaisseur de la bordure
                    DrawRectangleLinesEx(rec_player,borderplayer , BLACK);

                    // rectangle state mob
                    Rectangle rec_mob = { 585, 400, 420, 220 };  
                    DrawRectangle(rec_mob.x, rec_mob.y, rec_mob.width, rec_mob.height, LIGHTGRAY);
                    int bordermob = 5; 

                    DrawRectangleLinesEx(rec_mob, bordermob, BLACK);

                    // rectangle indication deplacement
                    Rectangle rec_indication = { 20, 650, 565, 140 }; 
                    DrawRectangle(rec_indication.x, rec_indication.y, rec_indication.width, rec_indication.height, LIGHTGRAY);
                    int borderdeplacement = 5; 
                    DrawRectangleLinesEx(rec_indication, borderdeplacement, BLACK);

                    // rectangle consigne 
                    Rectangle rec_consigne = { 585, 650, 450, 140 }; 
                    DrawRectangle(rec_consigne.x, rec_consigne.y, rec_consigne.width, rec_consigne.height, LIGHTGRAY);
                    int borderconsigne = 5; 
                    DrawRectangleLinesEx(rec_consigne, borderconsigne, RED);    

                    // rectangle graph 
                    Rectangle rec_graph = { 70, 160, 470, 470 }; 
                    DrawRectangle(rec_graph.x, rec_graph.y, rec_graph.width, rec_graph.height, LIGHTGRAY);
                    int bordergraph = 5; 
                    DrawRectangleLinesEx(rec_graph, bordergraph, BLACK);

                    // player helper
                    DrawTexture(player_texture, 1000, 70, WHITE);
              
               /* for (int i = 0; i < MAX_ENEMIES; i++) 
                    {    
                        Vector2 enemyPos = { enemies[i].x * CELL_SIZE + CELL_SIZE / 2 + 70, enemies[i].y * CELL_SIZE + CELL_SIZE / 2 + 160 };
                        DrawText(TextFormat("%d", enemies[i].strength),enemyPos.x,enemyPos.y, enemies[i].radius, BLACK);
                    }

                for (int i = 0; i < MAX_ENEMIES; i++) 
                {
                    if (CheckCollisionPlayerEnemy(character, enemies[i])) 
                    {
                        playerHealth -= enemies[i].strength; // Decrease player's health based on enemy's power
                        if (playerHealth <= 0) 
                        {
                            currentScreen = ENDING; // Game over condition, handle as needed
                        }
                    }
                    if (CheckCollisionPlayerEnemy(character, enemies[i]))
                    {
                        enemies[i].strength = 0;
                    }
                }*/


                Node currentNode = maze.graph.nodes[player.currentNode];
                DrawCircle(currentNode.x * 100, currentNode.y * 100, 10, BLUE);

                    // Draw the player's health bar
                   /* Vector2 HealthPos = { characterPos.x - 5, characterPos.y - 10 };
                    DrawText(TextFormat("%d",playerHealth), HealthPos.x, HealthPos.y, 5, DARKGRAY); // Health bar of the character
                    */
                    // Paramètres du rectangle arrondi (bulle)
                    Rectangle rect_bulle = { 930, 60, 100, 50 };
                    float roundness = 1.0;  // Niveau d'arrondi des coins, 0.0 à 1.0
                    int segments = 20;      // Nombre de segments pour dessiner les coins arrondis
                    int borderbulle = 2; 
                    // Dessiner un rectangle arrondi (bulle)
                    DrawRectangleRounded(rect_bulle, roundness, segments, GRAY);
                    DrawRectangleRoundedLinesEx(rect_bulle,roundness,segments,borderbulle,BLACK);

                    //texte dans bulle
                    DrawText("  Aide ?",935,76,20,BLACK);

                 // Fonction pour dessiner le labyrinthe
                    RenderMaze(&maze);
                
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
                DrawText("Fin du jeu", 350, 200, 20, LIGHTGRAY);
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
