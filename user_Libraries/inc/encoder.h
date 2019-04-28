#ifndef ENCODER_H
#define ENCODER_H 
#include <stm32f4xx.h>

extern int leftEncoder;
extern int rightEncoder;

extern double encoderCountX;
extern int encoderCountW;
extern int leftEncoderCount;
extern int rightEncoderCount;

extern double encoderChange;
extern int leftEncoderChange;
extern int rightEncoderChange;

extern double oldEncoderCount;
extern int leftEncoderOld;
extern int rightEncoderOld;

void Encoder_Configration(void);
int32_t getLeftEncCount(void);
int32_t getRightEncCount(void);
void resetRightEncCount(void);
void resetLeftEncCount(void);
void updateEncoderStatus(void);

#endif



