#include "floodfill2.h"
#include "movement.h"
#include "state.h"
#include <stdio.h>
#include "pid.h"
#include "delay.h"
#include "buzzer.h"
#include "led.h"

/*
Maze Orientation:
(0, SIZE)________(SIZE, SIZE)
				|				|
				|				|
				|_______|
(0, 0)					(SIZE, 0)
*/

int current_x = 0; // Initialized to starting cell in maze
int current_y = 0;

struct Vertex maze[SIZE][SIZE];

// Initialize walls to be just the borders of the maze.
void init_walls(void)
{
	// Initialize all walls to 0
	for (int i = 0; i < SIZE; ++i) {
		for (int j = 0; j < SIZE; ++j) {
			maze[i][j].walls = 0;
			maze[i][j].x = i;
			maze[i][j].y = j;
		}
	}
	
	// Add borders
	for (int i = 0; i < SIZE; ++i) {
		maze[0][i].walls = WEST_WALL;
		maze[i][SIZE - 1].walls = NORTH_WALL;
		maze[SIZE - 1][i].walls = EAST_WALL;
		maze[i][0].walls = SOUTH_WALL;
	}
	maze[0][0].walls += EAST_WALL;	// Starting cell wall
}

/*void init_costs(void) {
	int cost = 14;
	for (int i = 0; i < SIZE/2; ++i) {
		for (int j = 0; j < SIZE/2; ++j) {
			maze[i][j].cost = cost;
			cost -= 1;
		}
		for (int j = SIZE/2; j < SIZE; ++j) {
			maze[i][j].cost = cost;
			cost += 1;
		}
		cost -= 1;
	}
	for (int i = SIZE/2; i < SIZE; ++i) {
		for (int j = 0; j < SIZE/2; ++j) {
			maze[i][j].cost = cost;
			cost -= 1;
		}
		for (int j = SIZE/2; j < SIZE; ++j) {
			maze[i][j].cost = cost;
			cost += 1;
		}
		cost += 1;
	}
	for (int i = 0; i < SIZE; ++i) {
		for (int j = 0; j < SIZE; ++j) {
			printf("%d, ", maze[i][j].cost);
		}
		printf("\n");
	}
}*/

void init_costs(void) {
	int cost = 14;
	for (int i = 0; i < SIZE/2; ++i) {
		for (int j = 0; j < SIZE/2; ++j) {
			maze[i][j].cost = cost;
			cost -= 1;
		}
		cost += 1;
		for (int j = SIZE/2; j < SIZE; ++j) {
			maze[i][j].cost = cost;
			cost += 1;
		}
		cost -= 2;
	}
	cost = 7;
	for (int i = SIZE/2; i < SIZE; ++i) {
		for (int j = 0; j < SIZE/2; ++j) {
			maze[i][j].cost = cost;
			cost -= 1;
		}
		cost += 1;
		for (int j = SIZE/2; j < SIZE; ++j) {
			maze[i][j].cost = cost;
			cost += 1;
		}
	}
	for (int i = 0; i < SIZE; ++i) {
		for (int j = 0; j < SIZE; ++j) {
			printf("%d, ", maze[i][j].cost);
		}
		printf("\n");
	}
}

// Add walls to a cell, and all of its adjacent cells
void add_walls(const int x, const int y, const unsigned char walls)
{
	// Make sure x and y are within bounds of the maze
	if (x < 0 || x >= SIZE || y < 0 || y >= SIZE) {
		return;
	} 
	shortBeep(2000, 3000);
	if (walls & NORTH_WALL) {
		// Add the wall to this cell
		if (!(maze[x][y].walls & NORTH_WALL)) { // Check if this cell already has this wall
			maze[x][y].walls += NORTH_WALL;	// Only add if this cell doesn't already have this wall
		}

		// Add the wall to adjacent cell
		if (y < SIZE - 1) { // Check bounds to make sure this isn't a border wall
			if (!(maze[x][y + 1].walls & SOUTH_WALL)) { // Check if adjacent cell already has a south wall
				maze[x][y + 1].walls += SOUTH_WALL;	// Only add if adjacent cell doesn't already have this wall
			}
		}
	}
	if (walls & EAST_WALL) {
		// Add the wall to this cell
		if (!(maze[x][y].walls & EAST_WALL)) { // Check if this cell already has this wall
			maze[x][y].walls += EAST_WALL;	// Only add if this cell doesn't already have this wall
		}

		if (x < SIZE - 1) { // Check bounds to make sure this isn't a border wall
			if (!(maze[x + 1][y].walls & WEST_WALL)) { // Check if adjacent cell already has a west wall
				maze[x + 1][y].walls += WEST_WALL;	// Only add if adjacent cell doesn't already have this wall
			}
		}
	}
	if (walls & SOUTH_WALL) {
		// Add the wall to this cell
		if (!(maze[x][y].walls & SOUTH_WALL)) { // Check if this cell already has this wall
			maze[x][y].walls += SOUTH_WALL;	// Only add if this cell doesn't already have this wall
		}

		if (y > 0) { // Check bounds to make sure this isn't a border wall
			if (!(maze[x][y - 1].walls & NORTH_WALL)) { // Check if adjacent cell already has a north wall
				maze[x][y - 1].walls += NORTH_WALL;	// Only add if adjacent cell doesn't already have this wall
			}
		}
	}
	if (walls & WEST_WALL) {
		// Add the wall to this cell
		if (!(maze[x][y].walls & WEST_WALL)) { // Check if this cell already has this wall
			maze[x][y].walls += WEST_WALL;	// Only add if this cell doesn't already have this wall
		}

		if (x > 0) { // Check bounds to make sure this isn't a border wall
			if (!(maze[x - 1][y].walls & EAST_WALL)) { // Check if adjacent cell already has a east wall
				maze[x - 1][y].walls += EAST_WALL;	// Only add if adjacent cell doesn't already have this wall
			}
		}
	}
}

// x: coordinate of current cell
// y: coordinate of current cell
// Returns the neighbor with the lowest cost
struct Vertex least_cost_neighbor(const int x, const int y)
{
	int min_cost = INF;
	struct Vertex best_neighbor;
	if (!(maze[x][y].walls & NORTH_WALL) && y < SIZE - 1) {	// If no north wall AND cell is not on north border
		if (min_cost > maze[x][y + 1].cost) {	// Update min_cost if neigbor's cost is less than current cell's cost
			min_cost = maze[x][y + 1].cost;
			best_neighbor = maze[x][y + 1];
		}
	}
	if (!(maze[x][y].walls & EAST_WALL) && x < SIZE - 1) {
		if (min_cost > maze[x + 1][y].cost) {
			min_cost = maze[x + 1][y].cost;
			best_neighbor = maze[x + 1][y];
		}
	}
	if (!(maze[x][y].walls & SOUTH_WALL) && y > 0) {
		if (min_cost > maze[x][y - 1].cost) {
			min_cost = maze[x][y - 1].cost;
			best_neighbor = maze[x][y - 1];
		}
	}
	if (!(maze[x][y].walls & WEST_WALL) && x > 0) {
		if (min_cost > maze[x - 1][y].cost) {
			min_cost = maze[x - 1][y].cost;
			best_neighbor = maze[x - 1][y];
		}
	}

	// Check to see if mouse thinks it's trapped
	/*if (best_neighbor.cost == INF) {
		// ABORTTTTT
	}*/
	return best_neighbor;
}
#include <stdbool.h>
void flood_fill(void)
{
	// Fill all cells with most optimistic distance in cells to the goal:
	// While the current cell's value is incremented
	bool value_incremented;
	do {
		value_incremented = false;
		//shortBeep(20, 3000);

		// For every cell in the maze
		for (int i = 0; i < SIZE; ++i) {
			for (int j = 0; j < SIZE; ++j) {
				// Is the cell's value greater than the minimum value of its accessible neighbors?			
				struct Vertex best_neighbor = least_cost_neighbor(i, j);
				
				//printf("%d, ", best_neighbor.cost);

				// If no, change the cell's value to one greater than the minimum value of its accessible neighbors
				if (maze[i][j].cost > best_neighbor.cost) {
					maze[i][j].cost = best_neighbor.cost + 1;
					value_incremented = true;
				}
			}
			//printf("\n");
		}
	} while(value_incremented);
}

void run_search(void)
{
	while (current_x != 7 && current_y != 7) {
		struct Vertex next_cell = least_cost_neighbor(current_x, current_y);

		// Move to next cell
		move_floodfill(next_cell);
		current_x = next_cell.x;
		current_y = next_cell.y;
	}
	targetSpeedX = 0;
	targetSpeedW = 0;
	LED_Fancy_On();
	shortBeep(2000, 3000);
	LED_Fancy_On();
	ALL_LED_ON;
	delay_ms(200);
	ALL_LED_OFF;
	delay_ms(200);
	ALL_LED_ON;
	delay_ms(200);
	ALL_LED_OFF;
	delay_ms(200);
	ALL_LED_ON;
	delay_ms(40);
	resetPID();
	delay_ms(3000);
}

void move_floodfill(struct Vertex next_cell)
{
	// Fix orientation
	const int change_x = next_cell.x - current_x;
	const int change_y = next_cell.y - current_y;
	
	if (change_x == 1) { // Move east
		if (orientation != 1) {	// Face east
			if (targetSpeedX > 0) {
				targetSpeedX = 0;
				delay_ms(300);
			}
			
			if (orientation == 0) {
				turnRight();
			}
			else if (orientation == 2) {
				turnLeft();
			}
			else if (orientation == 3) {
				turnAround();
			}
		}
	}
	else if (change_x == -1) { // Move west
		if (orientation != 3) {	// Face west
			if (targetSpeedX > 0) {
				targetSpeedX = 0;
				delay_ms(300);
			}
			
			if (orientation == 0) {
				turnLeft();
			}
			else if (orientation == 2) {
				turnRight();
			}
			else if (orientation == 1) {
				turnAround();
			}
		}
	}
	else if (change_y == 1) { // Move north
		if (orientation != 0) {	// Face north
			if (targetSpeedX > 0) {
				targetSpeedX = 0;
				delay_ms(300);
			}
			
			if (orientation == 1) {
				turnLeft();
			}
			else if (orientation == 3) {
				turnRight();
			}
			else if (orientation == 2) {
				turnAround();
			}
		}
	}
	else if (change_y == -1) { // Move south
		if (orientation != 2) {	// Face south
			if (targetSpeedX > 0) {
				targetSpeedX = 0;
				delay_ms(300);
			}
			
			if (orientation == 3) {
				turnLeft();
			}
			else if (orientation == 1) {
				turnRight();
			}
			else if (orientation == 0) {
				turnAround();
			}
		}
	}
	moveOneCell();
}

void update_walls()
{
	if (orientation == 0) {	// North-facing
		if (frontWall) {
			add_walls(current_x, current_y + 1, NORTH_WALL);
		}
		if (leftWall) {
			add_walls(current_x, current_y + 1, WEST_WALL);
		}
		if (rightWall) {
			add_walls(current_x, current_y + 1, EAST_WALL);
		}
	}
	else if (orientation == 1) {	// East-facing
		if (frontWall) {
			add_walls(current_x + 1, current_y, EAST_WALL);
		}
		if (leftWall) {
			add_walls(current_x + 1, current_y, NORTH_WALL);
		}
		if (rightWall) {
			add_walls(current_x + 1, current_y, SOUTH_WALL);
		}
	}
	else if (orientation == 2) {	// South-facing
		if (frontWall) {
			add_walls(current_x, current_y - 1, SOUTH_WALL);
		}
		if (leftWall) {
			add_walls(current_x, current_y - 1, EAST_WALL);
		}
		if (rightWall) {
			add_walls(current_x, current_y - 1, WEST_WALL);
		}
	}
	else if (orientation == 3) {	// West-facing
		if (frontWall) {
			add_walls(current_x - 1, current_y, WEST_WALL);
		}
		if (leftWall) {
			add_walls(current_x - 1, current_y, SOUTH_WALL);
		}
		if (rightWall) {
			add_walls(current_x - 1, current_y, NORTH_WALL);
		}
	}
}
