#ifndef FLOODFILL_H
#define FLOODFILL_H
#include <stdbool.h>

#define STACK_MAX 100
#define SIZ 16

struct vertex {
  int x,y;
};

struct Stack {
    struct vertex     data[STACK_MAX];
    int     size;
};
typedef struct Stack Stack;

struct graph {
	int dist;
	struct vertex v[4];
  struct vertex prev;
  int x,y;
  bool visited;
};

extern struct graph m[SIZ][SIZ];
extern struct Stack stack;
extern Stack* s;
extern struct vertex c;

// Used for optimal path
extern struct Stack optimal;
extern Stack* op;

// void print_maze();

void Stack_Init(Stack *);

struct vertex Stack_Top(Stack *S);

void Stack_Push(Stack *S, struct vertex d);

void Stack_Pop(Stack *S);

void init_adjacency(void);

void assign_dist(void);

struct vertex floodfill(int x,int y);

void updateDist(void);

void maze_wallinput(int x, int y, int i);

// struct vertex maze_update(int x,int y,int z);

#endif
