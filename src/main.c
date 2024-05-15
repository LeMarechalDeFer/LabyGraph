#include "../include/graph.h"
//gcc -I/usr/local/include -L/usr/local/lib -lraylib -lm -lpthread -ldl -lrt -lX11 src/main.c -o tmp

int main(void)
{
    // Initialisation
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Test Raylib");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    // Boucle de jeu principale
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Commencez le dessin
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText("Congrats! Raylib is working!", 190, 200, 20, LIGHTGRAY);

        EndDrawing();
    }

    // Déinitialisation
    CloseWindow();                  // Close window and OpenGL context

    return 0;
}
