#ifndef __GRAPH__H__
#define __GRAPH__H__

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <raylib.h>
#include <time.h>
#include <limits.h>
#include <math.h>

#define MAX_NODES 100
#define MAX_EDGES 200
#define MAX_ENEMIES 10

typedef struct {
    int id;
    int x;
    int y;
} Node;

typedef enum {
    L,
    T,
    G, 
    M,
} EnemyType;

typedef struct {
    int start;
    int end;
    int number_enemy; 
    EnemyType enemy_type;
} Edge;

typedef struct {
    Node nodes[MAX_NODES];
    Edge edges[MAX_EDGES];
    int numNodes;
    int numEdges;
} Graph;

typedef struct {
    int currentNode;
    int health;
} Player;

typedef struct {
    Graph graph;
    int width;
    int height;
    bool walls[MAX_NODES][MAX_NODES];
} Maze;


//definition des level ac

typedef enum GameScreen { 
    LOGO = 0, TITLE,LEVEL1, GAMEPLAY, EXPLANATION ,ENDING,WINROOM,
}GameScreen;


// Déclaration du tableau de lettres associées aux types d'ennemis
extern const char *enemyLetters[];


// Function prototypes
void InitializeGraph(Graph *graph);
void AddNode(Graph *graph, int x, int y);
void AddEdge(Graph *graph, int start, int end, EnemyType enemyType, int number_enemy);
void InitializePlayer(Player *player, int startNode);
bool MovePlayer(Player *player, Maze *maze, int nextNode);
void InitializeMaze(Maze *maze, int width, int height);
void GenerateMaze(Maze *maze);
void RenderMaze(Maze *maze, int cellSize);
void DrawHealthBar(Player *player);
void PrintPath(int *predecessors, int startNode, int goalNode);
bool IsEnemyTypeValid(Maze *maze, int nodeIndex, EnemyType proposedType);
int BellmanFord(Graph *graph, int startNode, int *distances, int *predecessors);
int Dijkstra(Graph *graph, int startNode, int *distances, int *predecessors);
int AStar(Graph *graph, int startNode, int goalNode, int *distances, int *predecessors);
int Heuristic(Node a, Node b) ;
int FindLowestFScore(bool *openSet, int *fScore, int V);
void ResetGame(Maze *maze, Player *player, int screenWidth, int screenHeight);

#endif // __GRAPH__H__