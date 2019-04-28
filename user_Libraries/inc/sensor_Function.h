#ifndef SENSOR_FUNCTION_H
#define SENSOR_FUNCTION_H 

#include <stdbool.h>
#include <stm32f4xx.h>

extern int reflectionRate;

extern int32_t volMeter;
extern int32_t voltage;
extern int32_t LFSensor;
extern int32_t RFSensor;
extern int32_t DLSensor;
extern int32_t DRSensor;
extern int32_t Outz;
extern int32_t aSpeed;
extern int32_t angle;

extern int sensorError;

extern bool frontWall;
extern bool leftWall;
extern bool rightWall;

extern int targetLeft;
extern int targetRight;
extern int targetFrontLeft;
extern int targetFrontRight;
extern int targetFront2;

extern int thresholdFrontLeft;
extern int thresholdFrontRight;

void readSensor(void);
void readGyro(void);
void readVolMeter(void);
void lowBatCheck(void);

void getSensorError(void);
void detectWalls(void);

#endif
