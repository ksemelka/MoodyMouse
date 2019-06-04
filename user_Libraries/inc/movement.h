#ifndef MOVEMENT_H
#define MOVEMENT_H

extern int T1;
extern int T2;
extern int T3;

extern int moveSpeed;
extern int turnSpeed;
extern int returnSpeed;
extern int stopSpeed;
extern int maxSpeed;

extern double distanceLeftX;
extern double distanceLeftW;

extern int oneCellDistance;
extern int startCellDistance;
extern int oneTurnDistance;

double needToDecelerate(int, double, double);
void moveOneCell(void);
void moveCells(int cells);
void Forward(void);
void turnLeft(void);
void turnLeftPID(void);
void turnRight(void);
void turnRightPID(void);
void turnAround(void);
void turnAroundLeft(void);
void turnAroundRight(void);
void turnLeftCurve(void);
void turnRightCurve(void);

#endif
