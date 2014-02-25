/** @file opcontrol.c
 * @brief File for operator control code
 *
 * This file should contain the user operatorControl() function and any functions related to it.
 */

#include "main.h"
#include "liftcontrol.h"
#include <math.h>
#include "motors.h"
#include "lcd.h"

int ramp(int x) {
	return x * abs(x) / 127;
}

const int LIFT_SPEED = 60;

void operatorControl() {
	lcdInit(uart1);
	lcdClear(uart1);
	lcdSetBacklight(uart1, true);

	liftTaskCreate();

	pinMode(1, OUTPUT);
	int imeLeft;
	int imeRight;
	while (true) {
		imeGet(0, &imeLeft);
		imeGet(1, &imeRight);
		lcdPrint(uart1, 1, "IME Left %d", imeLeft);
		lcdPrint(uart1, 2, "IME Right %d", imeRight);

		if (joystickGetDigital(1, 7, JOY_DOWN)) {
			digitalWrite(1, 1);
			digitalWrite(1, 0);
		}

		// Drive
		int leftY  = ramp(joystickGetAnalog(1, 3));
		int rightY = ramp(joystickGetAnalog(1, 2));
		motorSet(driveFrontRight,  -rightY);
		motorSet(driveMiddleRight, -rightY);
		motorSet(driveBackRight,   -rightY);
		motorSet(driveFrontLeft,   leftY);
		motorSet(driveMiddleLeft,  leftY);
		motorSet(driveBackLeft,    leftY);

		// Lift
		int liftUp   = joystickGetDigital(1, 6, JOY_UP);
		int liftDown = joystickGetDigital(1, 6, JOY_DOWN);

		if (liftUp && liftDown) {
			//motorSet(liftLeft, 40);
			//motorSet(liftRight, -40);
		} else if (liftUp) {
			liftManual(LIFT_SPEED);
			//motorSet(liftLeft, 127);
			//motorSet(liftRight, -127);
		} else if (liftDown) {
			liftManual(-LIFT_SPEED);
			//motorSet(liftLeft, -127);
			//motorSet(liftRight, 127);
		} else {
			//motorSet(liftLeft, 0);
			//motorSet(liftRight, 0);
		}

		// Manipulator
		int manipulatorIn = joystickGetDigital(1, 5, JOY_DOWN);
		int manipulatorOut = joystickGetDigital(1, 5, JOY_UP);
		if (manipulatorIn) {
			motorSet(intakeLeft, -127);
			motorSet(intakeRight, 127);
		} else if (manipulatorOut) {
			motorSet(intakeLeft, 60);
			motorSet(intakeRight, -60);
		} else {
			motorSet(intakeLeft, 0);
			motorSet(intakeRight, 0);
		}


		delay(50);
	}
}
