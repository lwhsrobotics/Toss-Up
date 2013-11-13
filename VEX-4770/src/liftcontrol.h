/*
 * liftcontrol.h
 */

#ifndef LIFTCONTROL_H_
#define LIFTCONTROL_H_

#include "pid.h"

Semaphore liftSemaphore;

PidState liftStateLeft;
PidState liftStateRight;
/*
int LIFT_DT = 15;

const int LIFT_MIN = 0;
const int LIFT_MAX = 0;

float Kp = 0.05;
float Ki = 0.01;
float Kd = 1;

const int LIFT_INTEGRAL_THRESHOLD = 150;
*/
#define LIFT_ACQUIRE int __lift_had_semaphore = liftAcquireSemaphore();
#define LIFT_RELEASE if(!__lift_had_semaphore) liftReleaseSemaphore();

int liftAcquireSemaphore(int timeout);

int liftReleaseSemaphore();

void liftInit();

void liftSetTarget(int target);

void liftManual(int y);

void liftIncrementTarget(int amount);

void lift();

#endif /* LIFTCONTROL_H_ */
