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

int moveSpeed = 80;
int turnSpeed = 15;
int returnSpeed = 10;
int stopSpeed = 30;
int maxSpeed = 600; //>600

double distanceLeftX;
double distanceLeftW;

// 43.888 ticks per mm
int oneCellDistance = 15800;
int startCellDistance = 3100;
int leftTurnDistance = 10100;
int rightTurnDistance = 10025;

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
	useSensors = true;
	targetSpeedW = 0;
	targetSpeedX = moveSpeed;
	distanceLeftX = oneCellDistance + startCellDistance;
	LED4_ON;
	do {
		/*you can call int needToDecelerate(int32_t dist, int16_t curSpd, int16_t endSpd)
		here with current speed and distanceLeft to decide if you should start to decelerate or not.*/
		if(needToDecelerate(distanceLeftX, curSpeedX, moveSpeed) < decX)
			targetSpeedX = moveSpeed;
		else
			targetSpeedX = 0;
		//x = needToDecelerate(distanceLeftX, curSpeedX, 0);
		//printf("%f\n", x);
		
		if (distanceLeftX > 5000 && distanceLeftX < 5700) {
//			shortBeep(25, 2100);
			LED4_OFF;
			LED3_ON;
			updateState();
		}
		
		//there is something else you can add here. Such as detecting falling edge of post to correct longitudinal position of mouse when running in a straight path
	}
	while( (( encoderCountX-oldEncoderCount < oneCellDistance + startCellDistance) && LFSensor < targetFront2 && RFSensor < targetFront2)	// If no wall in front, use encoders
		|| (LFSensor < targetFrontLeft && LFSensor > targetFront2)
	  || (RFSensor < targetFrontRight && RFSensor > targetFront2) // If has front wall, use sensors
		 );
	//LED3_OFF;
	
	shortBeep(25, 1500);
	LED3_OFF;
	
	startCellDistance = 0;
	//LFvalues1 and RFvalues1 are the front wall sensor threshold when the center of mouse between the boundary of the cells.
	//LFvalues2 and RFvalues2 are the front wall sensor threshold when the center of the mouse staying half cell farther than LFvalues1 and 2
	//and LF/RFvalues2 are usually the threshold to determine if there is a front wall or not. You should probably move this 10mm closer to front wall when collecting
	//these thresholds just in case the readings are too weak.
	
	if (nextCellState == FRONT || nextCellState == FRONT + LEFT || nextCellState == FRONT + RIGHT || nextCellState == FRONT + LEFT + RIGHT) {
		while(LFSensor < targetFrontLeft || RFSensor < targetFrontRight) {
			targetSpeedX = moveSpeed;
		}
		targetSpeedX = 0;
		pid = false;
		curt = millis();
		while ((millis() - curt) < 300) {
			adjuster();
		}
		targetSpeedX = 0;
		pid = true;
		resetPID();
	}
	oldEncoderCount = encoderCountX; //update here for next movement to minimized the counts loss between cells.
}

void turnLeft() {
	targetSpeedX = 0;
	useSensors = false;
	distanceLeftW = leftTurnDistance;
	targetSpeedW = 15;
	while (distanceLeftW > 0) {
		delay_ms(1);
	}
	targetSpeedW = 0;
	delay_ms(200);
	resetPID();
}

void turnRight() {
	targetSpeedX = 0;
	useSensors = false;
	distanceLeftW = -rightTurnDistance;
	targetSpeedW = -turnSpeed;
	while (distanceLeftW < 0) {
		delay_ms(1);
		//printf("%f\n", distanceLeftW);
	}
	targetSpeedW = 0;
	delay_ms(200);
	resetPID();
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
}

void turnAround() {
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
	}
}

void turnAroundLeft() {
	turnLeftPID();
	
	// Adjust position in cell using left wall
	pid = false;
	curt = millis();
	while ((millis() - curt) < 600) {
		adjuster();
	}
	pid = true;
	resetPID();
	
	turnLeftPID();
}

void turnAroundRight() {
	turnRightPID();
	
	// Adjust position in cell using right wall
	pid = false;
	curt = millis();
	while ((millis() - curt) < 600) {
		adjuster();
	}
	pid = true;
	resetPID();
	
	turnRightPID();
}

void turnLeftCurve() {
	
}

void turnRightCurve() {
	
}
