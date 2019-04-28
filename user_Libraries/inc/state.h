#ifndef STATE_H
#define STATE_H

#define FRONT 1
#define RIGHT 2
#define LEFT 4

extern int nextCellState;
extern int orientation;

void updateState(void);
void navigate(void);
void solveRightHand(void);
void floodfill_algorithm(void);

/*
  *********************************************************
  0 == undefined
  1 == FRONT                (Randomly choose left or right)
  2 == RIGHT                (Forward until wall)
  3 == FRONT + RIGHT        (Turn left)
  4 == LEFT                 (Forward until wall)
  5 == LEFT + FRONT         (Turn right)
  6 == LEFT + RIGHT         (Go forward)
  7 == LEFT + RIGHT + FRONT (Dead end, turn around)
  *********************************************************
*/

#endif
