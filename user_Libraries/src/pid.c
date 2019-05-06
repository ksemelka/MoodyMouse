#include "pid.h"
#include "pwm.h"
#include "encoder.h"
#include "sensor_Function.h"
#include "stdbool.h"


double accX = .5;
double accW = 3;
double decX = 2.5;//120
double decW = 3;

double sensorFeedback = 0;
double turnFeedback = 0;
double sensor_scale = 75;
double turn_scale = 1;
bool useSensors = false;

double curSpeedX = 0;
double curSpeedW = 0;
double targetSpeedX = 0;
double targetSpeedW = 0;

double encoderFeedbackX = 0;
double encoderFeedbackW = 0;

int outMin = -999;	// Anti-windup
int outMax = 999;

double ITermX;
double ITermW;

double lastSpeedX = 0;
double lastSpeedW = 0;

double kpX = 12;
double kpW = 10;
double kiX = 0.5;
double kiW = 1;
double kdX = 1;
double kdW = 1;

int leftBaseSpeed;
int rightBaseSpeed;
int rotationalFeedback;
double errorX, errorW;
double dInputX, dInputW;
double OutputX, OutputW;

void PID(void) {
	rightEncoderChange *= 1;
	encoderFeedbackX = rightEncoderChange  + leftEncoderChange;
	encoderFeedbackW = rightEncoderChange - leftEncoderChange;   // Positive if mouse rotates CW

	if (useSensors) {
		sensorFeedback = sensorError/sensor_scale;
	}
	else {
		sensorFeedback = 0;
	}
	turnFeedback = turnFeedback / turn_scale;
	rotationalFeedback = encoderFeedbackW - sensorFeedback - turnFeedback;
	
	errorX = curSpeedX - encoderFeedbackX;
	
	ITermX += (kiX * errorX);
	if (ITermX > outMax) ITermX = outMax;
	else if (ITermX < outMin) ITermX = outMin;
	dInputX = (curSpeedX - lastSpeedX);
	
	
	errorW = curSpeedW - rotationalFeedback;
	
	ITermW += (kiW * errorW);
	if (ITermW > outMax) ITermW = outMax;
	else if (ITermW < outMin) ITermW = outMin;
	dInputW = (curSpeedW - lastSpeedW);
	
	
	lastSpeedX = curSpeedX;
	lastSpeedW = curSpeedW;

	OutputX = kpX * errorX + ITermX - kdX * dInputX;
	if(OutputX > outMax) OutputX = outMax;
   else if(OutputX < outMin) OutputX = outMin;

   OutputW = kpW * errorW + ITermW - kdW * dInputW;
	if(OutputW > outMax) OutputW = outMax;
   else if(OutputW < outMin) OutputW = outMin;

	leftBaseSpeed = OutputX - OutputW;
	rightBaseSpeed = OutputX + OutputW;

	setLeftPwm(leftBaseSpeed);
	setRightPwm(rightBaseSpeed);
}

void updateCurrentSpeed(void) {
	if(curSpeedX < targetSpeedX) {
		curSpeedX += accX;
    	if(curSpeedX > targetSpeedX)
      	curSpeedX = targetSpeedX;
	} 
	else if(curSpeedX > targetSpeedX) {
		curSpeedX -= decX;
		if(curSpeedX < targetSpeedX)
			curSpeedX = targetSpeedX;
	}
	if(curSpeedW < targetSpeedW) {
		curSpeedW += accW;
		if(curSpeedW > targetSpeedW)
			curSpeedW = targetSpeedW;
	}
	else if(curSpeedW > targetSpeedW) {
		curSpeedW -= decW;
		if(curSpeedW < targetSpeedW)
			curSpeedW = targetSpeedW;
	}
}
//extern double encoderCountX;
void resetPID(void) {
	ITermX = 0;
	ITermW = 0;
	lastSpeedX = 0;
	lastSpeedW = 0;
	curSpeedX = 0;
	curSpeedW = 0;
	//encoderCountX = 0;
	//encoderCountW = 0;
}
