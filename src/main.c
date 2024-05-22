#include "../include/graph.h"
//gcc -I/usr/local/include -L/usr/local/lib -lraylib -lm -lpthread -ldl -lrt -lX11 src/main.c -o tmp



int main(void) {
    Maze maze;
    InitializeMaze(&maze, 10, 10); // Par exemple, un labyrinthe de 10x10
    GenerateMazeRecursiveBacktracker(&maze);

    // Initialisation de Raylib
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Test Raylib - Niveau 1");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Rendre le labyrinthe
        RenderMaze(&maze);

        EndDrawing();
    }

    CloseWindow();
    FreeMaze(&maze); // Libérer la mémoire allouée pour le labyrinthe
    return 0;
}
