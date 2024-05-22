// gcc -o maze maze.c -lraylib -lm -lpthread -ldl -lrt -lX11

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include "raylib.h"

#define WIDTH  21
#define HEIGHT 21
#define CELL_SIZE 20
#define INF INT_MAX

enum { WALL, PATH };

typedef struct {
    int x, y;
} Position;

typedef struct {
    int **matrix;
    int size;
} Graph;

enum { WALL, PATH };

void initialize_maze(int maze[HEIGHT][WIDTH]);
void generate_maze(int maze[HEIGHT][WIDTH], int x, int y);
void shuffle_directions(Position directions[], int size);
void draw_maze(int maze[HEIGHT][WIDTH]);
Graph* maze_to_graph(int maze[HEIGHT][WIDTH]);
void destroy_graph(Graph* graph);
void bellman_ford(Graph* graph, int start, int end);
void dijkstra(Graph* graph, int start, int end);
void a_star(Graph* graph, int start, int end);
void create_imperfect_maze(int maze[HEIGHT][WIDTH]);
int min_distance(int dist[], int visited[], int size);
void print_path(int parent[], int j);

int main() {
    srand(time(NULL));

    int maze[HEIGHT][WIDTH];
    initialize_maze(maze);
    generate_maze(maze, 1, 1);
    create_imperfect_maze(maze);

    Graph *graph = maze_to_graph(maze);

    const int screenWidth = WIDTH * CELL_SIZE;
    const int screenHeight = HEIGHT * CELL_SIZE;

    InitWindow(screenWidth, screenHeight, "Graph Theory Learning Game");

    SetTargetFPS(60);

    // Example of calling the pathfinding algorithms
    int start = 0;  // Top-left corner of the grid
    int end = (HEIGHT - 2) * WIDTH + (WIDTH - 2);  // Bottom-right corner of the grid

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        draw_maze(maze);

        // Add GUI elements to choose algorithms and show the path
        if (IsKeyPressed(KEY_ONE)) {
            bellman_ford(graph, start, end);
        }
        if (IsKeyPressed(KEY_TWO)) {
            dijkstra(graph, start, end);
        }
        if (IsKeyPressed(KEY_THREE)) {
            a_star(graph, start, end);
        }

        DrawText("Press 1 for Bellman-Ford", 10, screenHeight - 50, 20, DARKGRAY);
        DrawText("Press 2 for Dijkstra", 10, screenHeight - 30, 20, DARKGRAY);
        DrawText("Press 3 for A*", 10, screenHeight - 10, 20, DARKGRAY);

        EndDrawing();
    }

    destroy_graph(graph);
    CloseWindow();
    return 0;
}

void initialize_maze(int maze[HEIGHT][WIDTH]) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            maze[y][x] = WALL;
        }
    }
}

void generate_maze(int maze[HEIGHT][WIDTH], int x, int y) {
    static const Position directions[] = {{0, -2}, {2, 0}, {0, 2}, {-2, 0}};
    maze[y][x] = PATH;

    Position shuffledDirections[4];
    for (int i = 0; i < 4; i++) {
        shuffledDirections[i] = directions[i];
    }

    shuffle_directions(shuffledDirections, 4);

    for (int i = 0; i < 4; i++) {
        int newX = x + shuffledDirections[i].x;
        int newY = y + shuffledDirections[i].y;
        if (newX > 0 && newX < WIDTH - 1 && newY > 0 && newY < HEIGHT - 1 && maze[newY][newX] == WALL) {
            maze[newY - shuffledDirections[i].y / 2][newX - shuffledDirections[i].x / 2] = PATH;
            generate_maze(maze, newX, newY);
        }
    }
}

void shuffle_directions(Position directions[], int size) {
    for (int i = 0; i < size; i++) {
        int j = rand() % size;
        Position temp = directions[i];
        directions[i] = directions[j];
        directions[j] = temp;
    }
}

void create_imperfect_maze(int maze[HEIGHT][WIDTH]) {
    int numWallsToRemove = (WIDTH * HEIGHT) / 10; // Example: Remove 10% of walls to create imperfections

    for (int i = 0; i < numWallsToRemove; i++) {
        int x = rand() % WIDTH;
        int y = rand() % HEIGHT;
        if (maze[y][x] == WALL) {
            maze[y][x] = PATH;
        }
    }
}

// void generate_maze(int maze[HEIGHT][WIDTH], int x, int y) {
//     static const Position directions[] = {{0, -2}, {2, 0}, {0, 2}, {-2, 0}};
//     maze[y][x] = PATH;

//     // Shuffle the directions to ensure randomness
//     for (int i = 0; i < 4; i++) {
//         int j = rand() % 4;
//         Position temp = directions[i];
//         directions[i] = directions[j];
//         directions[j] = temp;
//     }

//     for (int i = 0; i < 4; i++) {
//         int newX = x + directions[i].x;
//         int newY = y + directions[i].y;
//         if (newX > 0 && newX < WIDTH - 1 && newY > 0 && newY < HEIGHT - 1 && maze[newY][newX] == WALL) {
//             maze[newY - directions[i].y / 2][newX - directions[i].x / 2] = PATH;
//             generate_maze(maze, newX, newY);
//         }
//     }
// }

void draw_maze(int maze[HEIGHT][WIDTH]) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (maze[y][x] == WALL) {
                DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, BLACK);
            } else {
                DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, WHITE);
            }
        }
    }
}

Graph* maze_to_graph(int maze[HEIGHT][WIDTH]) {
    int size = WIDTH * HEIGHT;
    Graph *graph = (Graph*) malloc(sizeof(Graph));
    graph->size = size;
    graph->matrix = (int**) malloc(size * sizeof(int*));

    for (int i = 0; i < size; i++) {
        graph->matrix[i] = (int*) malloc(size * sizeof(int));
        for (int j = 0; j < size; j++) {
            graph->matrix[i][j] = INF;
        }
    }

    for (int y = 1; y < HEIGHT - 1; y++) {
        for (int x = 1; x < WIDTH - 1; x++) {
            if (maze[y][x] == PATH) {
                int vertex = y * WIDTH + x;
                if (maze[y-1][x] == PATH) {
                    int neighbor = (y-1) * WIDTH + x;
                    graph->matrix[vertex][neighbor] = 1;
                    graph->matrix[neighbor][vertex] = 1;
                }
                if (maze[y+1][x] == PATH) {
                    int neighbor = (y+1) * WIDTH + x;
                    graph->matrix[vertex][neighbor] = 1;
                    graph->matrix[neighbor][vertex] = 1;
                }
                if (maze[y][x-1] == PATH) {
                    int neighbor = y * WIDTH + (x-1);
                    graph->matrix[vertex][neighbor] = 1;
                    graph->matrix[neighbor][vertex] = 1;
                }
                if (maze[y][x+1] == PATH) {
                    int neighbor = y * WIDTH + (x+1);
                    graph->matrix[vertex][neighbor] = 1;
                    graph->matrix[neighbor][vertex] = 1;
                }
            }
        }
    }

    return graph;
}

void destroy_graph(Graph* graph) {
    for (int i = 0; i < graph->size; i++) {
        free(graph->matrix[i]);
    }
    free(graph->matrix);
    free(graph);
}

void bellman_ford(Graph* graph, int start, int end) {
    int V = graph->size;
    int dist[V];
    int parent[V];

    for (int i = 0; i < V; i++) {
        dist[i] = INF;
        parent[i] = -1;
    }
    dist[start] = 0;

    for (int i = 1; i <= V-1; i++) {
        for (int u = 0; u < V; u++) {
            for (int v = 0; v < V; v++) {
                if (graph->matrix[u][v] != INF && dist[u] != INF && dist[u] + graph->matrix[u][v] < dist[v]) {
                    dist[v] = dist[u] + graph->matrix[u][v];
                    parent[v] = u;
                }
            }
        }
    }

    for (int u = 0; u < V; u++) {
        for (int v = 0; v < V; v++) {
            if (graph->matrix[u][v] != INF && dist[u] != INF && dist[u] + graph->matrix[u][v] < dist[v]) {
                printf("Graph contains negative weight cycle\n");
                return;
            }
        }
    }

    printf("Bellman-Ford shortest path from %d to %d is %d\n", start, end, dist[end]);
    print_path(parent, end);
    printf("\n");
}

int min_distance(int dist[], int visited[], int size) {
    int min = INF, min_index;

    for (int v = 0; v < size; v++) {
        if (!visited[v] && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}

void dijkstra(Graph* graph, int start, int end) {
    int V = graph->size;
    int dist[V];
    int visited[V];
    int parent[V];

    for (int i = 0; i < V; i++) {
        dist[i] = INF;
        visited[i] = 0;
        parent[i] = -1;
    }
    dist[start] = 0;

    for (int count = 0; count < V-1; count++) {
        int u = min_distance(dist, visited, V);
        visited[u] = 1;

        for (int v = 0; v < V; v++) {
            if (!visited[v] && graph->matrix[u][v] != INF && dist[u] != INF && dist[u] + graph->matrix[u][v] < dist[v]) {
                dist[v] = dist[u] + graph->matrix[u][v];
                parent[v] = u;
            }
        }
    }

    printf("Dijkstra's shortest path from %d to %d is %d\n", start, end, dist[end]);
    print_path(parent, end);
    printf("\n");
}

int heuristic(int node, int end, int width) {
    int node_x = node % width;
    int node_y = node / width;
    int end_x = end % width;
    int end_y = end / width;
    return abs(node_x - end_x) + abs(node_y - end_y);
}

void a_star(Graph* graph, int start, int end) {
    int V = graph->size;
    int openSet[V];
    int closedSet[V];
    int gScore[V];
    int fScore[V];
    int parent[V];

    for (int i = 0; i < V; i++) {
        openSet[i] = 0;
        closedSet[i] = 0;
        gScore[i] = INF;
        fScore[i] = INF;
        parent[i] = -1;
    }
    openSet[start] = 1;
    gScore[start] = 0;
    fScore[start] = heuristic(start, end, WIDTH);

    while (1) {
        int current = -1;
        int lowestF = INF;
        for (int i = 0; i < V; i++) {
            if (openSet[i] && fScore[i] < lowestF) {
                lowestF = fScore[i];
                current = i;
            }
        }

        if (current == -1) {
            printf("No path found\n");
            return;
        }

        if (current == end) {
            printf("A* shortest path from %d to %d is %d\n", start, end, gScore[end]);
            print_path(parent, end);
            printf("\n");
            return;
        }

        openSet[current] = 0;
        closedSet[current] = 1;

        for (int neighbor = 0; neighbor < V; neighbor++) {
            if (graph->matrix[current][neighbor] != INF && !closedSet[neighbor]) {
                int tentativeG = gScore[current] + graph->matrix[current][neighbor];
                if (!openSet[neighbor]) {
                    openSet[neighbor] = 1;
                } else if (tentativeG >= gScore[neighbor]) {
                    continue;
                }
                parent[neighbor] = current;
                gScore[neighbor] = tentativeG;
                fScore[neighbor] = gScore[neighbor] + heuristic(neighbor, end, WIDTH);
            }
        }
    }
}

void print_path(int parent[], int j) {
    if (parent[j] == -1) {
        printf("%d ", j);
        return;
    }
    print_path(parent, parent[j]);
    printf("%d ", j);
}
