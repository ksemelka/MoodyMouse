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
#include "floodfill_algorithm.h"

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
	leftAdjust = 0.25 * (targetFrontLeft- LFSensor);
	rightAdjust = 0.25 * (targetFrontRight - RFSensor);
	if (leftAdjust > 150) 
		leftAdjust = 150;
	if (rightAdjust > 150)
		rightAdjust = 150;
	setLeftPwm(leftAdjust);//set left motor speed
	setRightPwm(rightAdjust);//set right motor speed
	delay_ms(1);
}
int selector = 0;
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
	
	delay_ms(250); // Start delay
	onTone();
	LED_Fancy_On();
	LED_Fancy_On();
	ALL_LED_ON;
	onTone();

	selector = 0;
	MAIN_MENU:
	pid = false;
	sensors = false;
	useSensors = false;
	if (read_Vol_Meter < 2700) {
		while(1) {
			displayMatrixScroll("BATT");
			delay_ms(500);
			displayMatrixScroll("LOW ");
			delay_ms(500);
		}
	}
	int prevEncVal = 0;
	while (1) {
		// displayMatrixScroll("FUCK");
		if (rightEncoderCount - prevEncVal > 200 || rightEncoderCount - prevEncVal < -200) {
			shortBeep(100, 900);
			prevEncVal = rightEncoderCount;
		}
		readSensor();
    	if (RFSensor > 2000) {
			rightHand = true;
			shortBeep(40, 400);
			delay_ms(40);
			shortBeep(20, 600);
      		break;
		}
		else if (LFSensor > 2000) {
			shortBeep(40, 400);
			delay_ms(40);
			shortBeep(20, 600);
			rightHand = false;
			break;
		}

    	delay_ms(50);

		if (rightEncoderCount % 20000 > 15000) {
			selector = 1; //Floodfill
			displayMatrixScroll("FLOD");
		}
		else if (rightEncoderCount % 20000 >= 10000) {
			selector = 2; //Fastest path
			displayMatrixScroll("FAST");
		}
		else if (rightEncoderCount % 20000 >= 5000) {
			selector = 3; //Fastest path
			displayMatrixScroll("RAND");
		}
		else {
			displayMatrixScroll("SLCT");
		}

		// displayMatrixScroll("KYLE");
		// delay_ms(500);
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
	clearScreen();
	
	/*pid = false;
	sensors = false;
	useSensors = false;*/
	
	while(1) {
		if (selector == 3) {
			//displayMatrixScroll("RAND");
			LED2_ON;
			navigate();
		}
		else if (selector == 1) {
			// displayMatrixScroll("FLOD");
			LED1_ON;
			init_floodfill();
			run_search();
			//chirp();
			// displayMatrixScroll("RTRN");
			//return_to_start();
			goto MAIN_MENU;
		}
		else if (selector == 2) {
			// displayMatrixScroll("FAST");
			LED1_ON;
			run_search();
			goto MAIN_MENU;
		}
		
		//readSensor();
		//delay_ms(200);
//		
		//printf("LF %d RF %d DL %d DR %d\n", LFSensor, RFSensor, DLSensor, DRSensor);
		//printf("leftWall: %d rightWall: %d\n", leftWall, rightWall);
		//printf("distanceLeftW: %f\n", distanceLeftW);
		//printf("aSpeed %d angle %d \r\n", aSpeed, angle);
		//printf("lenc %d renc %d\n", getLeftEncCount(), getRightEncCount()); 
		//delay_ms(1000);
	}
}
