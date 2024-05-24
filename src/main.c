#include "../include/graph.h"
#include "raylib.h"
//#include "gamestate.h"
//cc game.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
  
int main() {
    // Initialisation de Raylib
    const int screenWidth = 1200;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Menu du jeu éducatif");
        SetTraceLogLevel(LOG_ALL); // Activer les logs détaillés


    // Charger l'image du personnage
    Image player = LoadImage("src/player.png");
    Image ESIEA_logo = LoadImage("src/ESIEA-logo.png");
    Image dialogue_box = LoadImage("src/dialogue_box.png");
    
    Texture2D player_texture = LoadTextureFromImage(player);
    Texture2D ESIEA_logo_texture = LoadTextureFromImage(ESIEA_logo);
    Texture2D dialogue_box_texture = LoadTextureFromImage(dialogue_box);

    UnloadImage(player);
    UnloadImage(ESIEA_logo);
    UnloadImage(dialogue_box);


    //generation du labyrinthe §§§§§§§§§§§§§§§§§§§§§
     // Initialiser le générateur de nombres aléatoires
    srand(time(NULL));

    // Créer et initialiser le labyrinthe
    
    // Création et initialisation du labyrinthe
    Maze maze;
    InitializeMaze(&maze, 10, 10);  // 10x10 est un exemple, ajustez selon vos besoins
    GenerateMazeRecursiveBacktracker(&maze);

    SetTargetFPS(60); // Définir le FPS pour une animation fluide
    
    char *monologue[] = {
        "Bienvenue dans notre jeu!",
        "Appuyez sur 'P' pour continuer le monologue.",
        "Chaque pression vous mènera à la prochaine partie de l'histoire.",
        "C'est la fin de notre démonstration. Merci!"
    };
    int maxLines = sizeof(monologue) / sizeof(monologue[0]);
    int currentLine = 0;
    // Définir les états de jeu
    //typedef enum GameScreen { LOGO = 0, TITLE,LEVEL1, GAMEPLAY, EXPLANATION ,ENDING } GameScreen;
    GameScreen currentScreen = TITLE;
    double lastKeyPressTime = 0;  // Temps de la dernière pression de touche

    // Boucle principale du jeu
    while (!WindowShouldClose()) {    // Détecter la fermeture de la fenêtre
        double currentTime = GetTime();
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
        
       /* if ((currentScreen = TITLE) && IsKeyPressed(KEY_RIGHT))
        {
            currentScreen = LEVEL1;
        }*/
        // Commence à dessiner
        BeginDrawing();

        ClearBackground(RAYWHITE);

        switch (currentScreen) {
            case TITLE: 
                DrawText("Appuyez sur 'ENTRER' pour commencer", 200, screenHeight/2 -100, 45, LIGHTGRAY);
                DrawTexture(player_texture, screenWidth/2-50, screenHeight/2 , WHITE);
                //DrawTexture(ESIEA_logo_texture, screenWidth - screenWidth/2, 300, WHITE);
                Rectangle sourceRec = { 0, 0, (float)ESIEA_logo.width, (float)ESIEA_logo.height };
                Rectangle destRec = { screenWidth - 200, 0, 200, 150 };
                Vector2 origin = { 0, 0 };

                DrawTexturePro(ESIEA_logo_texture, sourceRec, destRec, origin, 0, WHITE);
            break;
            case GAMEPLAY:
                // Ici, vous devriez mettre le code pour afficher le premier niveau du jeu
                DrawText("Niveau 1 ~ BELLMAN", screenWidth/2-200, 0, 60, LIGHTGRAY);

                 // Fonction pour dessiner le labyrinthe
                RenderMaze(&maze);

                //Monologue 
                if (IsKeyPressed(KEY_P)) {
                    if (currentLine < maxLines - 1) {
                        currentLine++;
                    }
                }  
                Rectangle sourceMonologue = { 0, 0, (float)dialogue_box_texture.width, (float)dialogue_box_texture.height };
                Rectangle dimMonologue = { 100, 130, 1000, 300 };
                Vector2 originMonologue = { 0, 0 };

                DrawTexturePro(dialogue_box_texture, sourceMonologue, dimMonologue, originMonologue, 0, WHITE);
                //DrawTexture(dialogue_box_texture, 100, 180 , WHITE);
                DrawText("appuier sur P pour continuer", 300, 230, 10, RED);  //changer la couleur en black 
                if (currentLine < maxLines) {
                    DrawText(monologue[currentLine], 190, 200, 30, RED);

                }

                break;
                // Ajoutez la logique du jeu ici
            case EXPLANATION:
              // Display une deuxième fenêtre
                DrawText("Fonction Bellman-Ford(G = (S, A), poids, s) \n pour u dans S faire \n |d[u] = +infini \n |pred[u] = null \n d[s] = 0 -> Boucle principale \n pour k = 1 à taille(S) - 1 faire \n|pour chaque arc (u, v) du graphe faire \n|      |    si d[u] + poids(u, v) < d[v] alors \n|      |    |    d[v] := d[u] + poids(u, v) \n|      |    |    pred[v]:= u \n retourner d, pred", 190, 300, 20, DARKGRAY);
                break;
           /* case LEVEL1:
                DrawText("Appuyez sur 'ENTRER' pour commencer", 190, 200, 20, LIGHTGRAY);
                break;*/
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
    
    UnloadTexture(player_texture); // Ne pas oublier de décharger la texture pour libérer la ressource
    CloseWindow();        // Ferme la fenêtre et termine le programme

    return 0;
}
