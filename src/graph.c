
#include "../include/graph.h"
#define CELL_SIZE 40

void InitializeGraph(Graph *graph) {
    graph->numNodes = 0;
    graph->numEdges = 0;
}

void AddNode(Graph *graph, int x, int y) {
    if (graph->numNodes < MAX_NODES) {
        graph->nodes[graph->numNodes] = (Node){graph->numNodes, x, y};
        graph->numNodes++;
    }
}

void AddEdge(Graph *graph, int start, int end, int weight) {
    if (graph->numEdges < MAX_EDGES) {
        graph->edges[graph->numEdges] = (Edge){start, end, weight};
        graph->numEdges++;
    }
}

void InitializeMaze(Maze *maze, int width, int height) {
    InitializeGraph(&maze->graph);
    maze->width = width;
    maze->height = height;
    maze->grid = (int **)malloc(height * sizeof(int *));
    for (int i = 0; i < height; i++) {
        maze->grid[i] = (int *)malloc(width * sizeof(int));
        memset(maze->grid[i], 0, width * sizeof(int));
    }
}

void FreeMaze(Maze *maze) {
    for (int i = 0; i < maze->height; i++) {
        free(maze->grid[i]);
    }
    free(maze->grid);
}

void CarvePassagesFrom(int cx, int cy, Maze *maze) {
    int dx[4] = {1, -1, 0, 0};
    int dy[4] = {0, 0, 1, -1};
    int dirs[4] = {E, W, S, N};

    for (int i = 0; i < 4; i++) {
        int r = rand() % 4;
        int temp = dirs[i];
        dirs[i] = dirs[r];
        dirs[r] = temp;
    }

    for (int i = 0; i < 4; i++) {
        int nx = cx + dx[i];
        int ny = cy + dy[i];

        if (nx >= 0 && nx < maze->width && ny >= 0 && ny < maze->height && maze->grid[ny][nx] == 0) {
            maze->grid[cy][cx] |= dirs[i];
            maze->grid[ny][nx] |= (dirs[i] == N ? S : (dirs[i] == S ? N : (dirs[i] == E ? W : E)));
            CarvePassagesFrom(nx, ny, maze);
        }
    }
}

void GenerateMazeRecursiveBacktracker(Maze *maze) {
    srand(time(NULL)); // Initialization of random seed
    int startx = rand() % maze->width;
    int starty = rand() % maze->height;
    CarvePassagesFrom(startx, starty, maze);
}

void RenderMaze(Maze *maze) {
    //int mazeWidthTotal = maze->width * CELL_SIZE;
    //int mazeHeightTotal = maze->height * CELL_SIZE;

    int startX = 70;//(GetScreenWidth() - mazeWidthTotal) /4;
    int startY = 160;//(GetScreenHeight() - mazeHeightTotal) / 2;

    for (int y = 0; y < maze->height; y++) {
        for (int x = 0; x < maze->width; x++) {
            int x1 = startX + x * CELL_SIZE;
            int y1 = startY + y * CELL_SIZE;

            if (maze->grid[y][x] & S)
                DrawLine(x1, y1 + CELL_SIZE, x1 + CELL_SIZE, y1 + CELL_SIZE, BLACK);
            if (maze->grid[y][x] & E)
                DrawLine(x1 + CELL_SIZE, y1, x1 + CELL_SIZE, y1 + CELL_SIZE, BLACK);
        }
    }
}

