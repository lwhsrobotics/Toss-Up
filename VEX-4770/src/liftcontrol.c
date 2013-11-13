/*
 * liftcontrol.c
 */

#include "main.h"
#include <math.h>
#include "pid.h"
#include "motors.h"

Semaphore liftSemaphore;

PidState liftStateLeft;
PidState liftStateRight;

int LIFT_DT = 15;

const int LIFT_MIN = 0;
const int LIFT_MAX = 0;

float Kp = 0.05;
float Ki = 0.01;
float Kd = 1;

const int LIFT_INTEGRAL_THRESHOLD = 150;

#define LIFT_ACQUIRE int __lift_had_semaphore = liftAcquireSemaphore(36000);
#define LIFT_RELEASE if(!__lift_had_semaphore) liftReleaseSemaphore();

int liftAcquireSemaphore(int timeout) {
	return semaphoreTake(liftSemaphore, timeout);
}

int liftReleaseSemaphore() {
	return semaphoreGive(liftSemaphore);
}

void liftInit() {
	int liftLeftCount = 0, liftRightCount = 0;

	imeGet(driveBackLeft, &liftLeftCount);
	imeGet(driveBackRight, &liftRightCount);

	pidInitState(&liftStateLeft, Kp, Ki, Kd, 150);
	pidInitState(&liftStateRight, Kp, Ki, Kd, 150);
	pidSetTarget(&liftStateLeft, liftLeftCount);
	pidSetTarget(&liftStateRight, liftRightCount);
}

void liftSetTarget(int target) {
	LIFT_ACQUIRE

	if (target <= LIFT_MAX && target >= LIFT_MIN) {
		pidSetTarget(&liftStateRight, target);
		pidSetTarget(&liftStateLeft, target);
	}

	LIFT_RELEASE
}

void liftManual(int y) {
	int oldLeft = liftStateLeft.target;
	int oldRight = liftStateRight.target;

	int offset = y;

	if (offset < 0) {
		int minValue = fmin(oldLeft, oldRight);
		if (minValue + offset < LIFT_MIN)
			offset = LIFT_MIN - minValue;
	} else if (offset > 0) {
		int maxValue = fmax(oldLeft, oldRight);
		if (maxValue + offset > LIFT_MAX)
			offset = LIFT_MAX - maxValue;
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

void lift() {
	liftSemaphore = semaphoreCreate();

	while (true) {
		int liftLeftCount = 0, liftRightCount = 0;

		imeGet(driveBackLeft, &liftLeftCount);
		imeGet(driveBackRight, &liftRightCount);

		int initialOffset = liftStateRight.target - liftStateLeft.target;
		int realOffset = liftRightCount - liftLeftCount;
		int offset = realOffset - initialOffset;
		int offsetSpeed = liftMapOffset(offset);

		int leftSpeed = pid(&liftStateLeft, liftLeftCount, LIFT_DT) - offsetSpeed;
		int rightSpeed = pid(&liftStateRight, liftRightCount, LIFT_DT) + offsetSpeed;

		motorSet(driveBackLeft, -leftSpeed);
		motorSet(driveFrontLeft, leftSpeed);
		motorSet(driveBackRight, rightSpeed);
		motorSet(driveFrontRight, -rightSpeed);

		delay(LIFT_DT);
	}
}



