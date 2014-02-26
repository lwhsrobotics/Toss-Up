/*
 * liftcontrol.c
 */

#include "main.h"
#include <math.h>
#include "liftcontrol.h"
#include "pid.h"
#include "motors.h"

Semaphore liftSemaphore;

PidState liftStateLeft, liftStateRight;
int liftInitialTargetLeft, liftInitialTargetRight;

int LIFT_DT = 15;

const int LIFT_MIN = 800;//885;
const int LIFT_MAX = 2350;//2175;
const int LIFT_MID = 1600;

float Kp = 0.2;
float Ki = 0.1;
float Kd = 3.0;

const int LIFT_INTEGRAL_THRESHOLD = 150;
const int LIFT_DOWN_FORCE = 20;
const int LIFT_UP_FORCE = 30;

#define LIFT_ACQUIRE bool __lift_had_semaphore = liftAcquireSemaphore(1000);
#define LIFT_RELEASE if(!__lift_had_semaphore) liftReleaseSemaphore();

int liftAcquireSemaphore(int timeout) {
	return semaphoreTake(liftSemaphore, timeout);
}

int liftReleaseSemaphore() {
	return semaphoreGive(liftSemaphore);
}

TaskHandle liftTask = NULL;

void liftTaskCreate() {
	liftTaskDelete();
	liftTask = taskCreate(lift, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);
	if (liftSemaphore == NULL) liftSemaphore = semaphoreCreate();
}

void liftTaskDelete() {
	if (liftTask != NULL) {
		taskDelete(liftTask);
		liftTask = NULL;
	}
}

void liftInit() {
	liftInitialTargetLeft = analogRead(potLiftLeft);
	liftInitialTargetRight = analogRead(potLiftRight);

	pidInitState(&liftStateLeft, Kp, Ki, Kd, 150);
	pidInitState(&liftStateRight, Kp, Ki, Kd, 150);
	liftReset();
}

void liftReset() {
	pidSetTarget(&liftStateLeft, liftInitialTargetLeft);
	pidSetTarget(&liftStateRight, liftInitialTargetRight);
}

void liftSetTarget(int target) {
	LIFT_ACQUIRE

	if (target <= LIFT_MAX && target >= LIFT_MIN) {
		pidSetTarget(&liftStateRight, target);
		pidSetTarget(&liftStateLeft, target);
	}

	LIFT_RELEASE
}

void liftSetMax() {
	liftSetTarget(LIFT_MAX);
}

void liftSetMid() {
	liftSetTarget(LIFT_MID);
}

void liftSetMin() {
	liftSetTarget(LIFT_MIN);
}

void liftManual(int y) {
	int oldLeft = liftStateLeft.target;
	int oldRight = liftStateRight.target;

	int offset = y;

	if (offset < 0) {
		int minValue = fmin(oldLeft, oldRight);
		if (minValue + offset < LIFT_MIN) {
			offset = LIFT_MIN - minValue;
		}
	} else if (offset > 0) {
		int maxValue = fmax(oldLeft, oldRight);
		if (maxValue + offset > LIFT_MAX) {
			offset = LIFT_MAX - maxValue;
		}
	}

	pidIncrementTarget(&liftStateLeft, offset);
	pidIncrementTarget(&liftStateRight, offset);
}

void liftIncrementTarget(int amount) {
	LIFT_ACQUIRE

	if (amount + pidGetTarget(&liftStateLeft) < LIFT_MAX && amount + pidGetTarget(&liftStateRight) > LIFT_MIN) {
		pidIncrementTarget(&liftStateLeft, amount);
		pidIncrementTarget(&liftStateRight, amount);
	}

	LIFT_RELEASE
}

int liftMapOffset(int offset) {
	return offset / 10;
}

bool liftAtTarget(int threshold) {
	LIFT_ACQUIRE

	int dLeft = abs(analogRead(potLiftLeft) - pidGetTarget(&liftStateLeft));
	int dRight = abs(analogRead(potLiftRight) - pidGetTarget(&liftStateRight));
	bool atTarget = dLeft < threshold && dRight < threshold;

	LIFT_RELEASE

	return atTarget;
}

void liftWaitTarget(int threshold) {
	while (!liftAtTarget(threshold)) delay(10);
}

void lift() {
	while (true) {
		//LIFT_ACQUIRE

		int liftLeftCount = analogRead(potLiftLeft);
		int liftRightCount = analogRead(potLiftRight);

		//printf("Left Potentiometer: %d    Right Potentiometer: %d\r\n", liftLeftCount, liftRightCount);

		int initialOffset = liftStateRight.target - liftStateLeft.target;
		int realOffset = liftRightCount - liftLeftCount;
		int offset = realOffset - initialOffset;
		int offsetSpeed = liftMapOffset(offset);

		int leftSpeed = pid(&liftStateLeft, liftLeftCount, LIFT_DT) + offsetSpeed;
		int rightSpeed = pid(&liftStateRight, liftRightCount, LIFT_DT) - offsetSpeed;

		// if the lift is at bottom apply a gentle downforce to keep it low
		if (liftStateRight.target <= LIFT_MIN || liftStateLeft.target <= LIFT_MIN) {
			leftSpeed = fmin(leftSpeed, -LIFT_DOWN_FORCE);
			rightSpeed = fmin(rightSpeed, -LIFT_DOWN_FORCE);
		}

		// if lift is wherever apply something
		if (liftStateRight.target >= LIFT_MAX || liftStateLeft.target >= LIFT_MAX) {
			leftSpeed = fmax(leftSpeed, LIFT_UP_FORCE);
			rightSpeed = fmax(rightSpeed, LIFT_UP_FORCE);
		}

		motorSet(liftLeft, leftSpeed);
		motorSet(liftRight, -rightSpeed);

		//printf("Left Speed: %d    Right Speed: %d\r\n", leftSpeed, rightSpeed);

		//printf("Left Target: %d    Right Target: %d\r\n", liftStateLeft.target, liftStateRight.target);

		//LIFT_RELEASE

		delay(LIFT_DT);
	}
}



