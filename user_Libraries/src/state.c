#include "stm32f4xx.h"
#include "state.h"
#include "sensor_Function.h"
#include "movement.h"
#include "delay.h"
#include "pid.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "floodfill.h"
#include "led.h"

int nextCellState = LEFT + RIGHT;
int stateCount = 0;

void updateState() {
  nextCellState = 0;
  if (frontWall) {
    nextCellState += FRONT;
  }
  if (rightWall) {
    nextCellState += RIGHT;
  }
  if (leftWall) {
    nextCellState += LEFT;
  }
}


void navigate() {
	//LED3_ON;
	switch (nextCellState) {
	case 0:       // Randomly choose left, right, or straight
		if (rand() % 3 == 2) {
			targetSpeedX = 0;
			delay_ms(300);
			turnRightPID();
		}
		else if (rand() % 3 == 1) {
			targetSpeedX = 0;
			delay_ms(300);
			turnLeftPID();
		}
		moveOneCell();
		break;
	case FRONT:
		if (rand() % 2) {   // Turn left or right randomly
			turnLeftPID();
		}
		else {
			turnRightPID();
		}
		moveOneCell();
		break;
	case RIGHT:
		if (rand() % 2) {   // Turn left or go forward randomly
			targetSpeedX = 0;
			delay_ms(300);
			turnLeftPID();
		}
		moveOneCell();
		break;
	case LEFT:
		if (rand() % 2) {   // Turn right or go forward randomly
			targetSpeedX = 0;
			delay_ms(300);
			turnRightPID();
		}
			moveOneCell();
		break;
	case FRONT + RIGHT:
		turnLeftPID();
		moveOneCell();
		break;
	case FRONT + LEFT:
		turnRightPID();
		moveOneCell();
		break;
	case RIGHT + LEFT:
		moveOneCell();
		break;
	case FRONT + RIGHT + LEFT:
		turnAround();
		moveOneCell();
		break;
	default:
		printf("Error: Default\n");
		break;
	}
	//LED3_OFF;
}


void solveRightHand() {
  switch (nextCellState) {
    case 0:
    	turnRightPID();
      moveOneCell();
      break;
    case FRONT:
        turnRightPID();
        moveOneCell();
      break;
    case RIGHT:
        moveOneCell();
      break;
    case LEFT:
       turnRightPID();
       moveOneCell();
      break;
    case FRONT + RIGHT:
      turnLeft();
      moveOneCell();
      break;
    case FRONT + LEFT:
      turnRightPID();
      moveOneCell();
      break;
    case RIGHT + LEFT:
      moveOneCell();
      break;
    case FRONT + RIGHT + LEFT:
      turnAround();
      moveOneCell();
      break;
    default:
			printf("Error: Default\n");
			break;
  }
}



