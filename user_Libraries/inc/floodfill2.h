#ifndef FLOODFILL_H
#define FLOODFILL_H

#define SIZE 16
#define NORTH_WALL 1
#define EAST_WALL 2
#define SOUTH_WALL 4
#define WEST_WALL 8
#define INF 1000

struct Vertex {
	unsigned char walls;
	int cost;
	int x, y;
};

void add_walls(const int x, const int y, const unsigned char walls);
void init_walls(void);
void init_costs(void);
void flood_fill(void);
void run_search(void);
void move_floodfill(struct Vertex next_cell);
void update_walls(void);


#endif //FLOODFILL_H
