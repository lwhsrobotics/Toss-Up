/** @file opcontrol.c
 * @brief File for operator control code
 *
 * This file should contain the user operatorControl() function and any functions related to it.
 */

#include "main.h"
#include "liftcontrol.h"
#include <math.h>
#include "motors.h"

int ramp(int x) {
	return x * abs(x) / 127;
}

PidState liftStateLeft;
PidState liftStateRight;

const int LIFT_SPEED = 17;

//const float LIFT_KP = 0.8;
//const float LIFT_KI = 0.05;
//const float LIFT_KD = 2.0;

void operatorControl() {
	// Encoder Tick Counters
	/*int driveBackLeftCount = 0;
	int driveBackRightCount = 0;
	int driveFrontLeftCount = 0;
	int driveFrontRightCount = 0;*/
	//int liftLeftCount = 0;
	//int liftRightCount = 0;

	int dt = 20;

	liftInit();

	//pidInitState(&liftStateLeft, Kp, Ki, Kd, 150);
	//pidInitState(&liftStateRight, Kp, Ki, Kd, 150);
	//pidSetTarget(&liftStateLeft, liftLeftCount);
	//pidSetTarget(&liftStateRight, liftRightCount);

	while (true) {
		//imeGet(liftBackLeft, &liftLeftCount);
		//imeGet(liftBackRight, &liftRightCount);

		// Drive
		int leftX  = ramp(joystickGetAnalog(1, 3));
		int leftY  = ramp(joystickGetAnalog(1, 4));
		int rightX = ramp(joystickGetAnalog(1, 1));

		motorSet(driveFrontRight, leftY + rightX - leftX);
		motorSet(driveBackRight,  leftY - rightX + leftX);
		motorSet(driveFrontLeft,  leftY + rightX + leftX);
		motorSet(driveBackLeft,   leftY - rightX - leftX);

		// Lift
		int liftUp = joystickGetDigital(1, 6, JOY_UP);
		int liftDown = joystickGetDigital(1, 6, JOY_DOWN);

		if (liftUp) {
			/*
			motorSet(liftBackLeft, -127);
			motorSet(liftFrontRight, -127);
			motorSet(liftBackRight, 127);
			motorSet(liftFrontLeft, 127);
			*/
			//pidIncrementTarget(&liftStateLeft, 15);
			//pidIncrementTarget(&liftStateRight, 15);
			liftManual(LIFT_SPEED);
		} else if (liftDown) {
			/*
			motorSet(liftBackLeft, -127);
			motorSet(liftFrontRight, -127);
			motorSet(liftBackRight, 127);
			motorSet(liftFrontLeft, 127);
			*/
			//pidIncrementTarget(&liftStateLeft, -15);
			//pidIncrementTarget(&liftStateRight, -15);
			liftManual(-LIFT_SPEED);
		} else {
			/*
			motorSet(liftBackLeft, 0);
			motorSet(liftFrontRight, 0);
			motorSet(liftBackRight, 0);
			motorSet(liftFrontLeft, 0);
			*/
		}

		//int liftLeft = pid(&liftStateLeft, liftLeftCount, dt);
		//int liftRight = pid(&liftStateRight, liftRightCount, dt);

		//motorSet(liftBackRight, liftRight);
		//motorSet(liftFrontRight, -liftRight);
		//motorSet(liftBackLeft, -liftLeft);
		//motorSet(liftBackRight, liftLeft);

		// Manipulator
		int manipulatorIn = joystickGetDigital(1, 5, JOY_UP);
		int manipulatorOut = joystickGetDigital(1, 5, JOY_DOWN);

		if (manipulatorIn) {
			motorSet(intakeLeft, 127);
			motorSet(intakeRight, -127);
		} else if (manipulatorOut) {
			motorSet(intakeLeft, -127);
			motorSet(intakeRight, 127);
		} else {
			motorSet(intakeLeft, 0);
			motorSet(intakeRight, 0);
		}

		delay(dt);
	}
}
