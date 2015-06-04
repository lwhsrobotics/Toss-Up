/*
 * hang.c
 *
 *  Created on: Mar 1, 2014
 *      Author: Jacob
 */

#include "main.h"
#include "motors.h"
#include "drivecontrol.h"
#include "API.h"
#include "liftcontrol.h"

static bool hookDeployed = false;

const int HANG_DT = 50;
const int STALL_THRESHOLD = 50;
const int STALL_TIME = 500;
const int HANG_HEIGHT = 900;

void hangReset() {
	digitalWrite(hookPiston, 1);
	digitalWrite(winchPiston, 0);
}

void hangInit() {
	pinMode(winchPiston, OUTPUT);
	pinMode(hookPiston, OUTPUT);

	hangReset();
}



void deployHook() {
	digitalWrite(hookPiston, 0);
	hookDeployed = true;
}

void retractHook() {
	digitalWrite(hookPiston, 1);
	hookDeployed = false;
}

bool isHookDeployed() {
	return hookDeployed;
}

void pushArrayValue(int* array, int length, int value) {
	for (int i = 0; i < length - 1; i++) {
		array[i] = array[i + 1];
	}

	array[length - 1] = value;
}

// returns -1 if not enough data
int sumStallChanges(int* changes, int length) {
	int sum = 0;
	for (int i = 0; i < length; i++) {
		int value = changes[i];
		if (value < 0) return -1;
		sum += value;
	}
	return sum;
}

void fillArray(int* array, int length, int value) {
	for (int i = 0; i < length; i++) {
		array[i] = value;
	}
}

void hangMotors(int speed) {
	motorSet(liftLeft, -speed);
	motorSet(liftRight, speed);
	driveMotors(speed, speed);
}

const int STALL_DELAY = 2000;

void hang() {
	liftTaskDelete();

	// engage the winch
	digitalWrite(winchPiston, 1);

	// give it a chance to engage
	taskDelay(100);

	int leftTicks = analogRead(potLiftLeft);
	int rightTicks = analogRead(potLiftRight);

	int changesArrayLength = STALL_TIME/HANG_DT;
	int changes[changesArrayLength]; // -1 indicates no data
	fillArray(changes, changesArrayLength, -1);

	int stallTicks = 0;

	while (true) {
		if (joystickGetDigital(1, 8, JOY_DOWN)) {
			return;
		}

		int newLeftTicks = analogRead(potLiftLeft);
		int newRightTicks = analogRead(potLiftRight);

		if (newLeftTicks < HANG_HEIGHT && newRightTicks < HANG_HEIGHT) {
			// we're at the right height, stop
			break;
		}

		if ((stallTicks * HANG_DT) > STALL_DELAY) {
			// do stall detection
			int change = abs(leftTicks - newLeftTicks) + abs(rightTicks - newRightTicks);
			pushArrayValue(changes, changesArrayLength, change);

			int totalChanges = sumStallChanges(changes, changesArrayLength);

			// if changes has data (>= 0) and it's stalled, stop motors
			if (totalChanges >= 0 && totalChanges < STALL_THRESHOLD) {
				// stalled, wait 3 seconds
				hangMotors(0);
				delay(3000);
				// clear changes
				fillArray(changes, changesArrayLength, -1);

				continue;
			}
		}

		leftTicks = newLeftTicks;
		rightTicks = newRightTicks;

		hangMotors(127);

		stallTicks++;

		taskDelay(HANG_DT);
	}

	hangMotors(0);
}

void unhang() {
	digitalWrite(winchPiston, 0);
}
