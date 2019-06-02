#include "stm32f4xx.h"
#include "movement.h"
#include "encoder.h"
#include "pid.h"
#include "sensor_Function.h"
#include "state.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "delay.h"
#include <stdbool.h>
#include "pwm.h"
#include "buzzer.h"
#include "floodfill_algorithm.h"
#include "led.h"

extern void resetPID(void);

// Times for each stage of curve turn (ms)
int T1 = 25;
int T2 = 65;
int T3 = 25;

int moveSpeed = 85; // 80
int turnSpeed = 37;
int returnSpeed = 10;
int stopSpeed = 30;
int maxSpeed = 300; //>300// 1200?

double distanceLeftX;
double distanceLeftW;

// 43.888 ticks per mm
int oneCellDistance = 30250;
int startCellDistance = 9400;
int leftTurnDistance = 20200;
int rightTurnDistance = 20300;

extern double targetSpeedX;
extern double targetSpeedW;
extern double curSpeedX;
extern double decX;
extern bool useSensors;

double needToDecelerate(int dist, double curSpd, double endSpd) { //speed are in encoder counts/ms, dist is in encoder counts 
	if (curSpd<0) curSpd = -curSpd;
	if (endSpd<0) endSpd = -endSpd;
	if (dist<0) dist = 1;
	if (dist == 0) dist = 1;  //prevent divide by 0

	return (fabs(curSpd*curSpd - endSpd*endSpd)/dist/4/2);
	//calculate deceleration rate needed with input distance, input current speed and input targetspeed to determind if the deceleration is needed
	//use equation 2*a*S = Vt^2 - V0^2  ==>  a = (Vt^2-V0^2)/2/S
	//because the speed is the sum of left and right wheels(which means it's doubled), that's why there is a "/4" in equation since the square of 2 is 4
}

extern void adjuster(void);
u32 curt;
extern bool pid;

void moveOneCell() {
	shortBeep(25, 2100);
	LED4_ON;
	bool stateUpdated = false;
	distanceLeftX = oneCellDistance + startCellDistance;
	do {
		if (needToDecelerate(distanceLeftX, curSpeedX, moveSpeed) < decX) {
			targetSpeedX = moveSpeed;
		}
		else {
			targetSpeedX = 0;
		}

		/*
		// DETECT FALLING EDGE OF POSTS
		if ((curState & LEFT) && !stateUpdated) {
			// Detect falling edge of left post and update 
			if (!leftWall) {
				distanceLeftX = fallingDistance;
			}
		}
		if ((curState & RIGHT) && !stateUpdated) {
			// Detect falling edge of right post and update distanceLeftX
			if (!rightWall) {
				distanceLeftX = fallingDistance;
			}
		}
		*/

		/*
		// DETECT RISING EDGE OF POSTS
		if (!(curState & LEFT) && !stateUpdated) {
			// Detect falling edge of left post and update 
			if (!leftWall) {
				distanceLeftX = risingDistance;
			}
		}
		if (!(curState & RIGHT) && !stateUpdated) {
			// Detect falling edge of right post and update distanceLeftX
			if (rightWall) {
				distanceLeftX = risingDistance;
			}
		}
		*/

		// If mouse is 1/3 the way through the cell, update state for next cell.
		if ((distanceLeftX < 2 * oneCellDistance / 3) && !stateUpdated) {
			shortBeep(25, 2400);
			LED4_OFF;
			LED3_ON;
			updateState();
			stateUpdated = true;
		}
	}
	while( (encoderCountX - oldEncoderCount < oneCellDistance + startCellDistance)
		&& LFSensor < targetFrontLeft && RFSensor < targetFrontRight
		//((encoderCountX - oldEncoderCount < oneCellDistance + startCellDistance) // encoder count hasn't passed oneCellDistance yet AND there is no wall in front
		//&& LFSensor < targetFrontLeft2 && RFSensor < targetFrontRight2) // There is no wall in front
		//|| (LFSensor < targetFrontLeft && LFSensor >= targetFrontLeft2) // There is a wall in front but mouse isn't at center of cell yet
		//|| (RFSensor < targetFrontRight && RFSensor >= targetFrontRight2) // There is a wall in front but mouse isn't at center of cell yet
	);

	if (!stateUpdated) {
		// If this branch is entered, something went wrong. ABORTTT
		LED1_ON;
		shortBeep(250, 1500);
		delay_ms(500);
		shortBeep(250, 1500);
		resetPID();
		setLeftPwm(0);
		setRightPwm(0);
		pid = false;
		//sensors = false;
		useSensors = false;
		delay_ms(500000);
	}

	startCellDistance = 0;
	oldEncoderCount = encoderCountX;
	// Fuck Jerry
}

//void moveOneCell() {
//	shortBeep(25, 2100);
//	//targetSpeedW = 0;
//	//targetSpeedX = moveSpeed;
//	distanceLeftX = oneCellDistance + startCellDistance;
//	LED4_ON;
//	do {
//		/*you can call int needToDecelerate(int32_t dist, int16_t curSpd, int16_t endSpd)
//		here with current speed and distanceLeft to decide if you should start to decelerate or not.*/
//		if(needToDecelerate(distanceLeftX, curSpeedX, moveSpeed) < decX)
//			targetSpeedX = moveSpeed;
//		else
//			targetSpeedX = 0;
//		//x = needToDecelerate(distanceLeftX, curSpeedX, 0);
//		//printf("%f\n", x);
//		
//		if (distanceLeftX > 500) {
//			useSensors = true;
//		}
//		
//		if (distanceLeftX > 30000 / 5 && distanceLeftX < (30000 / 5) + 500) {
//			LED4_OFF;
//			LED3_ON;
//			updateState();
//		}
//		
//		if (distanceLeftX <= 0) {
//			break;
//		}
//		
//		//there is something else you can add here. Such as detecting falling edge of post to correct longitudinal position of mouse when running in a straight path
//	}
//	while( (( encoderCountX-oldEncoderCount < oneCellDistance + startCellDistance) && LFSensor < targetFront2 - 5 && RFSensor < targetFront2)	// If no wall in front, use encoders
//		|| (LFSensor < targetFrontLeft && LFSensor > targetFront2 - 5)
//	  || (RFSensor < targetFrontRight && RFSensor > targetFront2 - 5) // If has front wall, use sensors
//		 );
//	useSensors = true;
//	//LED3_OFF;
//	
//	//shortBeep(25, 1500);
//	LED3_OFF;
//	//targetSpeedX = 0;
//	
//	if (distanceLeftX > 200) {
//			updateState();
//	}
//	
//	startCellDistance = 0;
//	//LFvalues1 and RFvalues1 are the front wall sensor threshold when the center of mouse between the boundary of the cells.
//	//LFvalues2 and RFvalues2 are the front wall sensor threshold when the center of the mouse staying half cell farther than LFvalues1 and 2
//	//and LF/RFvalues2 are usually the threshold to determine if there is a front wall or not. You should probably move this 10mm closer to front wall when collecting
//	//these thresholds just in case the readings are too weak.
//	
//	if (frontWall) {
//		//nextCellState == FRONT || nextCellState == FRONT + LEFT || nextCellState == FRONT + RIGHT || nextCellState == FRONT + LEFT + RIGHT) {
//		/*while(LFSensor < targetFrontLeft && RFSensor < targetFrontRight) {
//			targetSpeedX = moveSpeed;
//		}*/
//		targetSpeedX = 0;
//		pid = false;
//		curt = millis();
//		while ((millis() - curt) < 500) {
//			adjuster();
//		}
//		targetSpeedX = 0;
//		pid = true;
//		resetPID();
//	}
//	oldEncoderCount = encoderCountX; //update here for next movement to minimized the counts loss between cells.
//}

extern double curSpeedW;
extern double decW;

void turnLeft() {
	orientation--;
	if (orientation < 0) {
		orientation = 3;
	}
	targetSpeedX = 0;
	useSensors = false;
	distanceLeftW = leftTurnDistance;
	while (distanceLeftW > 0) {
		if(needToDecelerate(distanceLeftW, curSpeedW, turnSpeed) < decW)
			targetSpeedW = turnSpeed;
		else
			targetSpeedW = 0;
	}
	targetSpeedW = 0;
	delay_ms(300);
	resetPID();
	useSensors = true;
}

void turnRight() {
	orientation++;
	if (orientation > 3) {
		orientation = 0;
	}
	
	targetSpeedX = 0;
	useSensors = false;
	distanceLeftW = -rightTurnDistance;
	targetSpeedW = -turnSpeed;
	while (distanceLeftW < 0) {
		if(needToDecelerate(-distanceLeftW, -curSpeedW, turnSpeed) < decW)
			targetSpeedW = -turnSpeed;
		else
			targetSpeedW = 0;
	}
	targetSpeedW = 0;
	delay_ms(300);
	resetPID();
	useSensors = true;
}

void turnLeftPID() {
	orientation--;
	if (orientation < 0) {
		orientation = 3;
	}
	
	targetSpeedX = 0;
	useSensors = false;
	curt = millis();
	
	targetSpeedW = 50;
	elapseMillis(150, curt);
	targetSpeedW = 35;
	elapseMillis(210, curt);
	targetSpeedW = 25;
	elapseMillis(252, curt);
	targetSpeedW = 10;
	elapseMillis(285, curt);
	
	targetSpeedW = 0;
	elapseMillis(350, curt);
	resetPID();
	
	if (frontWall) {
		targetSpeedX = 0;
		pid = false;
		curt = millis();
		while ((millis() - curt) < 800) {
			adjuster();
		}
		targetSpeedX = 0;
		pid = true;
		resetPID();
	}
}

void turnRightPID() {
	orientation++;
	if (orientation > 3) {
		orientation = 0;
	}
	
	targetSpeedX = 0;
	useSensors = false;
	curt = millis();
	
	targetSpeedW = -50;
	elapseMillis(157, curt);
	targetSpeedW = -35;
	elapseMillis(200, curt);
	targetSpeedW = -25;
	elapseMillis(253, curt);
	targetSpeedW = -10;
	elapseMillis(278, curt);
	
	targetSpeedW = 0;
	elapseMillis(350, curt);
	resetPID();
	
	if (frontWall) {
		targetSpeedX = 0;
		pid = false;
		curt = millis();
		while ((millis() - curt) < 800) {
			adjuster();
		}
		targetSpeedX = 0;
		pid = true;
		resetPID();
	}
}

void turnAround() {
	if (frontWall) {
		if (rightWall) {
			turnAroundRight();
		}
		else {
			turnAroundLeft();
		}
	}
	/*
	switch(nextCellState) {
	case FRONT:
		turnLeftPID();
		turnLeftPID();
		break;
	case RIGHT:
		turnAroundRight();
		break;
	case LEFT:
		turnAroundLeft();
		break;
	case FRONT + RIGHT:
		turnAroundRight();
		break;
	case FRONT + LEFT:
		turnAroundLeft();
		break;
	case RIGHT + LEFT:
		turnAroundLeft();
		break;
	case FRONT + RIGHT + LEFT:
		turnAroundLeft();
		break;
	}*/
}

void turnAroundLeft() {
	turnLeft();
	
	// Adjust position in cell using left wall
	pid = false;
	curt = millis();
	while ((millis() - curt) < 500) {
		adjuster();
	}
	pid = true;
	resetPID();
	
	turnLeft();
}

void turnAroundRight() {
	turnRight();
	
	// Adjust position in cell using right wall
	pid = false;
	curt = millis();
	while ((millis() - curt) < 500) {
		adjuster();
	}
	pid = true;
	resetPID();
	
	turnRight();
}

void turnLeftCurve() {
	
}

void turnRightCurve() {
	
}
