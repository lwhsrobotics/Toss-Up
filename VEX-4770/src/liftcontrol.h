/*
 * liftcontrol.h
 */

#ifndef LIFTCONTROL_H_
#define LIFTCONTROL_H_

#include "pid.h"

Semaphore liftSemaphore;

PidState liftStateLeft;
PidState liftStateRight;

void liftWaitTarget();

void liftTaskCreate();

void liftTaskDelete();

int liftAcquireSemaphore(int timeout);

int liftReleaseSemaphore();

void liftInit();

void liftSetTarget(int target);

void liftManual(int y);

void liftIncrementTarget(int amount);

void lift();

#endif /* LIFTCONTROL_H_ */
