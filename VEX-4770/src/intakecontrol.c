/*
 * intakecontrol.c
 *
 *  Created on: Feb 6, 2014
 *      Author: Jacob
 */

#include "motors.h"
#include "main.h"
#include "API.h"

void intakeIn() {
	motorSet(intakeLeft, -127);
	motorSet(intakeRight, 127);
}

void intakeOut() {
	motorSet(intakeLeft, 60);
	motorSet(intakeRight, -60);
}

void intakeOff() {
	motorSet(intakeLeft, 0);
	motorSet(intakeRight, 0);
}
