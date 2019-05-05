#ifndef FLOODFILL_ALGORITHM_H
#define FLOODFILL_ALGORITHM_H

extern int orientation;
extern int start_x;
extern int start_y;
extern int goal_x;
extern int goal_y;
extern int current_x;
extern int current_y;
extern int change_x;
extern int change_y;

void floodfill_algorithm(void);

#endif