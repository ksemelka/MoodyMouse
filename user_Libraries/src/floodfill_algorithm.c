#include "floodfill_algorithm.h"
#include "floodfill.h"
#include "movement.h"
#include "state.h"
#include <stdio.h>
#include "pid.h"
#include "delay.h"
#include "buzzer.h"
#include "led.h"


int orientation;
int start_x;
int start_y;
int goal_x;
int goal_y;
//int current_x;
//int current_y;
int change_x;
int change_y;
extern bool pid;
extern bool useSensors;
extern bool sensors;

extern struct Stack optimal;
extern Stack* op;
extern struct graph f[SIZ][SIZ];

void init_floodfill(void)
{
	init_adjacency();
	assign_dist();
	orientation = 0;
}

/*void run_search(void)
{
	orientation = 0;
	current_x = 0;
	current_y = 15;
	goal_x = 7;
	goal_y = 7;
	Stack_Init(s);
	floodfill_algorithm();
}*/

void run_fast(void)
{
	orientation = 0;
	current_x = 0;
	current_y = 15;
	goal_x = 7;
	goal_y = 7;
	init_floodfill();
	Stack_Init(s);
	//optimal_path();
}

//void return_to_start(void)
//{
//	goal_x = 0;
//	goal_y = 15;
//	Stack_Init(s);
//	floodfill_algorithm();
//}

void init_fastest_path(void) {
    for(int i = 0; i < SIZ; i++) {
      for(int j = 0; j < SIZ; j++) {
        f[i][j].x = i;
        f[i][j].y = j;
        if(i == SIZ) {f[i][j].dist = -1;}
        if(j == SIZ) {f[i][j].dist = -1;}
        f[i][j].visited = false;
      }
    }
}

/*void init_cost(void) {
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
		cost -= 1;
	}
	cost = 7;
	for (int i = SIZE/2; i < SIZE/ ++i) {
		for (int j = 0; j < SIZE/2; ++j) {
			maze[i][j].cost = cost;
			cost -= 1;
		}
		cost += 1;
		for (int j = SIZE/2; j < SIZE; ++j) {
			maze[i][j].cost = cost;
			cost += 1;
		}
		cost += 1;
	}
}*/

void floodfill_algorithm(void)
{
	/*graph maze(16);
	maze.populate();
  	maze.initialize();
  	//Vertex currCell = &(this->vertices.at(0));

  	while () {
		if ((orientation % 4) < 0) {
			orientation = (orientation % 4) + 4;
		}
		else {
			orientation = orientation % 4;
		}
		switch(nextCellState) {
			case FRONT:
				add_edge(currCell, (orientation + 0) % 4);
				break;
			case RIGHT:
				add_edge(currCell, (orientation + 1) % 4);
				break;
			case LEFT:
				add_edge(currCell, (orientation + 3) % 4);
				break;
			case FRONT + RIGHT:
				// Front
				add_edge(currCell, (orientation + 0) % 4);
				// Right
				add_edge(currCell, (orientation + 1) % 4);
				break;
			case FRONT + LEFT:
				// Front
				add_edge(currCell, (orientation + 0) % 4);
				// Left
				add_edge(currCell, (orientation + 3)% 4);
				break;
			case RIGHT + LEFT:
				// Right
				add_edge(currCell, (orientation + 1) % 4);
				// Lef
				add_edge(currCell, (orientation + 3) % 4);
				break;
			case FRONT + RIGHT + LEFT:
				// Front
				add_edge(currCell, (orientation + 0) % 4);
				// Right
				add_edge(currCell, (orientation + 1) % 4);
				// Left
				add_edge(currCell, (orientation + 3) % 4);
				break;
			default:
				Serial.println("Error: Default");
				break;
		}

		currCell = floodfill(currCell);
		change = currCell.position - prev.position;

		if (change == -1) {
			while ((orientation % 4) != 3) {
				if ((orientation % 4) < 0) {
					orientation = (orientation % 4) + 4;
				}
				else {
					orientation = orientation % 4;
				}
					turnLeft();
			}
		}
		else if (change == 1) {
			while ((orientation % 4) != 1) {
				if ((orientation % 4) < 0) {
					orientation = (orientation % 4) + 4;
				}
				else {
					orientation = orientation % 4;
				}
				turnLeft();
			}
		}
		else if (change == -16) {
			while ((orientation % 4)!= 2) {
				if ((orientation % 4) < 0) {
					orientation = (orientation % 4) + 4;
				}
				else {
					orientation = orientation % 4;
				}
				turnLeft();
			}
		}
		else if (change == 16) {
			while ((orientation % 4) != 0) {
				if ((orientation % 4) < 0) {
					orientation = (orientation % 4) + 4;
				}
				else {
					orientation = orientation % 4;
				}
				turnLeft();
			}
		}
		moveOneCell();
		*/

	// i = 0 1 2 3
	// Up, Right, Down, Left

	// Maze orientation
	//--------------
	//(0,0)		(0,15)
	//
	//(15,0)	(15,15)
	//--------------

	// Maze orientation
	//--------------
	//(0,0)		(15,0)
	//
	//(0,15)	(15,15)
	//--------------

//	init_adjacency();
//	assign_dist();
//	Stack_Init(s);

	// Init optimal path stack
	//Stack_Init(op);

	//    Format: (x, y)
	// pair<int, int> start (0, 0);
	// pair(int, int) goal (7, 7);
	// pair(int, int) current(0, 0);

	//current_x = start_x;
	//current_y = start_y;

//	struct vertex start;
//	start.x = start_x;
//	start.y = start_y;
//	Stack_Push(op, start);
	// Hardcode first cell
	// Left
	/*maze_wallinput(current_x, current_y, (orientation + 3) % 4);
	// Right
	maze_wallinput(current_x, current_y, (orientation + 1) % 4);
	c = floodfill(current_x, current_y);
	current_x = c.x;
	current_y = c.y;
	moveOneCell();

	maze_wallinput(current_x, current_y, (orientation + 3) % 4);
	// Right
	maze_wallinput(current_x, current_y, (orientation + 1) % 4);

	//moveOneCell();

	c = floodfill(current_x, current_y);
	current_x = c.x;
	current_y = c.y;

	moveOneCell();
*/

	// struct vertex start;
	// start.x = start_x;
	// start.y = start_y;
	// Stack_Push(op, start);

	while (!(current_x == goal_x && current_y == goal_y))
	{
		//printf("%d, %d\n", current_x, current_y);
		if ((orientation % 4) < 0)
		{
			orientation = (orientation % 4) + 4;
		}
		else
		{
			orientation = orientation % 4;
		}
		switch (nextCellState)
		{
		case 0:
			break;
		case FRONT:
			maze_wallinput(current_x, current_y, (orientation + 0) % 4);
			break;
		case RIGHT:
			maze_wallinput(current_x, current_y, (orientation + 1) % 4);
			break;
		case LEFT:
			maze_wallinput(current_x, current_y, (orientation + 3) % 4);
			break;
		case FRONT + RIGHT:
			// Front
			maze_wallinput(current_x, current_y, (orientation + 0) % 4);
			// Right
			maze_wallinput(current_x, current_y, (orientation + 1) % 4);
			break;
		case FRONT + LEFT:
			// Front
			maze_wallinput(current_x, current_y, (orientation + 0) % 4);
			// Left
			maze_wallinput(current_x, current_y, (orientation + 3) % 4);
			break;
		case RIGHT + LEFT:
			// Right
			maze_wallinput(current_x, current_y, (orientation + 1) % 4);
			// Left
			maze_wallinput(current_x, current_y, (orientation + 3) % 4);
			break;
		case FRONT + RIGHT + LEFT:
			// Front
			maze_wallinput(current_x, current_y, (orientation + 0) % 4);
			// Right
			maze_wallinput(current_x, current_y, (orientation + 1) % 4);
			// Left
			maze_wallinput(current_x, current_y, (orientation + 3) % 4);
			break;
		default:
			break;
		}

		// Get the next cell(vertex) which we need to traverse to
		extern int selector;
		if (selector == 2) {
			c = speed_floodfill(current_x, current_y);
		}
		else {
			c = floodfill(current_x, current_y);
		}

		/*if (current_x == goal_x)//7 || current_x == 8)
		{
			if (current_y == goal_y)//7 || current_y == 8)
			{
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
				// resetPID();
				// pid = false;
				// useSensors = false;
				// sensors = false;
				delay_ms(1000);
				return;
			}
		}*/

		// // Storing optimal path
		// // Remove if already traversed before
		// if (Stack_Top(op).x == current_x && Stack_Top(op).y == current_y) {
		// 	Stack_Pop(op);
		// }
		
		// if (Stack_Top(op).x == c.x && Stack_Top(op).y == c.y) {
		// 	//Stack_Pop(op);
		// }
		// // Otherwise, add to optimal path
		// else {
		// 	struct vertex curr;
		// 	curr.x = current_x;
		// 	curr.y = current_y;
		// 	Stack_Push(op, curr);
		// 	// Stack_Push(op, c);
		// }

		if (f[current_x][current_y].visited == false) {
			f[current_x][current_y].visited = true;
		}
		
		if (f[c.x][c.y].visited == true) {
			f[current_x][current_y].visited = false;
		}
		
		change_x = current_x - c.x;
		change_y = current_y - c.y;

		//printf("..%d, %d\n", c.x, c.y);
		//		printf("o: %d", orientation);

		// Check orientation to make mouse point in correct direction
		if (change_x == -1) //move East
		{
			while ((orientation % 4) != 1)
			{
				// printf("1: %d\n", orientation);
				if (targetSpeedX > 0) {
					targetSpeedX = 0;
					delay_ms(300);
				}
				// turnRightPID();

				if (orientation == 0)
				{
					turnRight();
				}
				else
				{
					turnLeft();
				}

				if ((orientation % 4) < 0)
				{
					orientation = (orientation % 4) + 4;
				}
				else
				{
					orientation = orientation % 4;
				}
			}
		}
		else if (change_x == 1) //move West
		{
			while ((orientation % 4) != 3)
			{
				if (targetSpeedX > 0) {
					targetSpeedX = 0;
					delay_ms(300);
				}
				// turnLeftPID();
				if (orientation == 0)
				{
					turnLeft();
				}
				else
				{
					turnRight();
				}

				// printf("2: %d\n", orientation);
				if ((orientation % 4) < 0)
				{
					orientation = (orientation % 4) + 4;
				}
				else
				{
					orientation = orientation % 4;
				}
			}
		}
		else if (change_y == -1) //move South
		{
			while ((orientation % 4) != 2)
			{
				if (targetSpeedX > 0) {
					targetSpeedX = 0;
					delay_ms(300);
				}

				// turnLeftPID();
				if (orientation == 3)
				{
					turnLeft();
				}
				else
				{
					turnRight();
				}
				// printf("3: %d\n", orientation);
				if ((orientation % 4) < 0)
				{
					orientation = (orientation % 4) + 4;
				}
				else
				{
					orientation = orientation % 4;
				}
			}
		}
		else if (change_y == 1) //move North
		{
			while ((orientation % 4) != 0)
			{
				if (targetSpeedX > 0) {
					targetSpeedX = 0;
					delay_ms(300);
				}
				// turnLeftPID();
				if (orientation == 1)
				{
					turnLeft();
				}
				else
				{
					turnRight();
				}
				// printf("4: %d\n", orientation);
				if ((orientation % 4) < 0)
				{
					orientation = (orientation % 4) + 4;
				}
				else
				{
					orientation = orientation % 4;
				}
			}
		}
		moveOneCell();

		current_x = c.x;
		current_y = c.y;
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
	// pid = false;
	// useSensors = false;
	// sensors = false;
	delay_ms(3000);
				
	return;
}

// void optimal_path(void) {
// 	while (!(current_x == goal_x && current_y == goal_y))
// 	{
// 		//printf("%d, %d\n", current_x, current_y);
// 		if ((orientation % 4) < 0)
// 		{
// 			orientation = (orientation % 4) + 4;
// 		}
// 		else
// 		{
// 			orientation = orientation % 4;
// 		}
// 		switch (nextCellState)
// 		{
// 		case 0:
// 			break;
// 		case FRONT:
// 			maze_wallinput(current_x, current_y, (orientation + 0) % 4);
// 			break;
// 		case RIGHT:
// 			maze_wallinput(current_x, current_y, (orientation + 1) % 4);
// 			break;
// 		case LEFT:
// 			maze_wallinput(current_x, current_y, (orientation + 3) % 4);
// 			break;
// 		case FRONT + RIGHT:
// 			// Front
// 			maze_wallinput(current_x, current_y, (orientation + 0) % 4);
// 			// Right
// 			maze_wallinput(current_x, current_y, (orientation + 1) % 4);
// 			break;
// 		case FRONT + LEFT:
// 			// Front
// 			maze_wallinput(current_x, current_y, (orientation + 0) % 4);
// 			// Left
// 			maze_wallinput(current_x, current_y, (orientation + 3) % 4);
// 			break;
// 		case RIGHT + LEFT:
// 			// Right
// 			maze_wallinput(current_x, current_y, (orientation + 1) % 4);
// 			// Left
// 			maze_wallinput(current_x, current_y, (orientation + 3) % 4);
// 			break;
// 		case FRONT + RIGHT + LEFT:
// 			// Front
// 			maze_wallinput(current_x, current_y, (orientation + 0) % 4);
// 			// Right
// 			maze_wallinput(current_x, current_y, (orientation + 1) % 4);
// 			// Left
// 			maze_wallinput(current_x, current_y, (orientation + 3) % 4);
// 			break;
// 		default:
// 			break;
// 		}

// 		// Get the next cell(vertex) which we need to traverse to
// 		c = floodfill(current_x, current_y);

// 		// if (f[current_x][current_y].visited == false) {
// 		// 	f[current_x][current_y].visited = true;
// 		// }
		
// 		// if (f[c.x][c.y].visited == true) {
// 		// 	f[current_x][current_y].visited = false;
// 		// }

// 		if (f[c.x][c.y].visited == false) {
// 			continue;
// 		}
		
// 		change_x = current_x - c.x;
// 		change_y = current_y - c.y;

// 		//printf("..%d, %d\n", c.x, c.y);
// 		//		printf("o: %d", orientation);

// 		// Check orientation to make mouse point in correct direction
// 		if (change_x == -1) //move East
// 		{
// 			while ((orientation % 4) != 1)
// 			{
// 				// printf("1: %d\n", orientation);
// 				if (targetSpeedX > 0) {
// 					targetSpeedX = 0;
// 					delay_ms(300);
// 				}
// 				// turnRightPID();
// 				if (orientation == 0)
// 				{
// 					turnRight();
// 				}
// 				else
// 				{
// 					turnLeft();
// 				}
// 				if ((orientation % 4) < 0)
// 				{
// 					orientation = (orientation % 4) + 4;
// 				}
// 				else
// 				{
// 					orientation = orientation % 4;
// 				}
// 			}
// 		}
// 		else if (change_x == 1) //move West
// 		{
// 			while ((orientation % 4) != 3)
// 			{
// 				if (targetSpeedX > 0) {
// 					targetSpeedX = 0;
// 					delay_ms(300);
// 				}
// 				// turnLeftPID();
// 				if (orientation == 0)
// 				{
// 					turnLeft();
// 				}
// 				else
// 				{
// 					turnRight();
// 				}

// 				// printf("2: %d\n", orientation);
// 				if ((orientation % 4) < 0)
// 				{
// 					orientation = (orientation % 4) + 4;
// 				}
// 				else
// 				{
// 					orientation = orientation % 4;
// 				}
// 			}
// 		}
// 		else if (change_y == -1) //move South
// 		{
// 			while ((orientation % 4) != 2)
// 			{
// 				if (targetSpeedX > 0) {
// 					targetSpeedX = 0;
// 					delay_ms(300);
// 				}

// 				// turnLeftPID();
// 				if (orientation == 3)
// 				{
// 					turnLeft();
// 				}
// 				else
// 				{
// 					turnRight();
// 				}
// 				// printf("3: %d\n", orientation);
// 				if ((orientation % 4) < 0)
// 				{
// 					orientation = (orientation % 4) + 4;
// 				}
// 				else
// 				{
// 					orientation = orientation % 4;
// 				}
// 			}
// 		}
// 		else if (change_y == 1) //move North
// 		{
// 			while ((orientation % 4) != 0)
// 			{
// 				if (targetSpeedX > 0) {
// 					targetSpeedX = 0;
// 					delay_ms(300);
// 				}
// 				// turnLeftPID();
// 				if (orientation == 1)
// 				{
// 					turnLeft();
// 				}
// 				else
// 				{
// 					turnRight();
// 				}
// 				// printf("4: %d\n", orientation);
// 				if ((orientation % 4) < 0)
// 				{
// 					orientation = (orientation % 4) + 4;
// 				}
// 				else
// 				{
// 					orientation = orientation % 4;
// 				}
// 			}
// 		}
// 		moveOneCell();

// 		current_x = c.x;
// 		current_y = c.y;
// 	}
// 	targetSpeedX = 0;
// 	targetSpeedW = 0;
// 	LED_Fancy_On();
// 	shortBeep(2000, 3000);
// 	LED_Fancy_On();
// 	ALL_LED_ON;
// 	delay_ms(200);
// 	ALL_LED_OFF;
// 	delay_ms(200);
// 	ALL_LED_ON;
// 	delay_ms(200);
// 	ALL_LED_OFF;
// 	delay_ms(200);
// 	ALL_LED_ON;
// 	delay_ms(40);
// 	resetPID();
// 	// pid = false;
// 	// useSensors = false;
// 	// sensors = false;
// 	delay_ms(3000);
				
// 	return;
// }

