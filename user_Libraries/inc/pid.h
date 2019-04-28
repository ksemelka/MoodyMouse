#ifndef ENCODER_H
#define ENCODER_H
#include <stm32f4xx.h>
#include "encoder.h"
#include "sensor_Function.h"
#include "stdbool.h"

extern double accX;
extern double accW;
extern double decX;
extern double decW;

extern double sensorFeedback;
extern double turnFeedback;
extern double sensor_scale;
extern double turn_scale;
extern bool useSensors;

extern double curSpeedX;
extern double curSpeedW;
extern double targetSpeedX;
extern double targetSpeedW;

extern double encoderFeedbackX;
extern double encoderFeedbackW;

extern int outMin;
extern int outMax;

extern double ITermX;
extern double ITermW;

extern double lastSpeedX;
extern double lastSpeedW;

extern double kpX;
extern double kpW;
extern double kiX;
extern double kiW;
extern double kdX;
extern double kdW;

extern int32_t leftEncoderChange;
extern int32_t rightEncoderChange;

extern int leftBaseSpeed;
extern int rightBaseSpeed;

extern int rotationalFeedback;
extern double errorX, errorW;
extern double dInputX, dInputW;
extern double OutputX, OutputW;

void PID(void);
void updateCurrentSpeed(void);
void resetPID(void);

#endif
