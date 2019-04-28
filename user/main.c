#include "stm32f4xx.h"
#include "delay.h"
#include "led.h"
#include "button.h"
#include "encoder.h"
#include <stdio.h>
#include <stdlib.h>
#include "usart.h"
#include "SPI.h"
#include "matrixDisplay.h"
#include "pwm.h"
#include "buzzer.h"
#include "main.h"
#include "sensor_Function.h"
#include "adc.h"
#include "movement.h"
#include "state.h"
#include "pid.h"
#include "floodfill.h"

extern void PID(void);
extern void updateCurrentSpeed(void);
extern void resetPID(void);
extern int targetSpeedX;
extern int targetSpeedW;
extern bool useSensors;
int count;
bool rightHand = false;
bool pid = false;
bool sensors = false;
u32 time;

void systick(void) {
	//readGyro();
	if (sensors) {
		readSensor();
//		if (LFSensor > 3000) {
//			time = millis();
//			targetSpeedX = 0;
//			while (millis() - time < 500) {
//				shortBeep(20, 4000);
//			}
//		}
	}
	updateEncoderStatus();
	if (pid) {
		
		updateCurrentSpeed();
		PID();
	}
}

void button1_interrupt(void) {
}


void button2_interrupt(void) {
}

int leftAdjust;
int rightAdjust;
void adjuster(void)
{
	leftAdjust = 0.15 * (targetFrontLeft- LFSensor);
	rightAdjust = 0.15 * (targetFrontRight - RFSensor);
	if (leftAdjust > 300) 
		leftAdjust = 200;
	if (rightAdjust > 300)
		rightAdjust = 200;
	setLeftPwm(leftAdjust);//set left motor speed
	setRightPwm(rightAdjust);//set right motor speed
	delay_ms(1);
}

int main(void) {
	Systick_Configuration();
	LED_Configuration();
	button_Configuration();
	usart1_Configuration(9600);
	SPI_Configuration();
  TIM4_PWM_Init();
	Encoder_Configration();
	buzzer_Configuration();
	ADC_Config();
	
	delay_ms(500); // Start delay
	onTone();
	LED_Fancy_On();
	LED_Fancy_On();
	ALL_LED_ON;
	onTone();
	
	while (1) {
		readSensor();
    if (RFSensor > 2000) {
			rightHand = true;
      break;
		}
		else if (LFSensor > 2000) {
			rightHand = false;
			break;
		}
    delay_ms(50);
  }
	ALL_LED_OFF;
	startTone();
	delay_ms(1000);	// Start delay
	pid = true;
	sensors = true;
	readSensor();
  targetLeft = DLSensor;
  targetRight = DRSensor;
  useSensors = true;
	
	srand(((DLSensor + DRSensor + RFSensor + LFSensor * 128 - 3) % (171 + LFSensor)) * (5221 + DRSensor % 111));
	resetPID();
	targetSpeedX = 0;
	targetSpeedW = 0;
	
//	pid = false;
//	sensors = false;
//	useSensors = false;
	
	while(1) {
		shortBeep(100, 900);
		if (rightHand) {
			LED2_ON;
			navigate();
		}
		else {
			LED1_ON;
			floodfill_algorithm();
		}
		
		//readSensor();
//		
		//printf("LF %d RF %d DL %d DR %d\n", LFSensor, RFSensor, DLSensor, DRSensor);
		//printf("leftWall: %d rightWall: %d\n", leftWall, rightWall);
		//printf("distanceLeftW: %f\n", distanceLeftW);
		//printf("aSpeed %d angle %d \r\n", aSpeed, angle);
		//printf("lenc %d renc %d\n", getLeftEncCount(), getRightEncCount()); 
	}
}
