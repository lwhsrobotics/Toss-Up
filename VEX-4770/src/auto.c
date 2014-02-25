 /** @file auto.c
 * @brief File for autonomous code
 *
 * This file should contain the user autonomous() function and any functions related to it.
 */

#include "main.h"
#include "motors.h"
#include "drivecontrol.h"
#include "lcd.h"
#include "liftcontrol.h"
#include "intakecontrol.h"

void autonomous() {
	lcdInit(uart1);
	lcdClear(uart1);
	lcdSetBacklight(uart1, true);

	// stop PID so we can control the lift manually
	liftTaskDelete();

	// unfold
	motorSet(liftLeft, 127);
	motorSet(liftRight, -127);
	delay(300);

	// start PID, automatically puts lift down
	liftTaskCreate();

	driveStraight(48, 127);
	liftSetTarget(2350);

	driveStraight(12, 127);
	liftWaitTarget(150);
	driveStraight(12, 127);
	intakeOut();
	delay(500);
	intakeOff();

	driveStraight(-12, 127);
	liftSetTarget(800);
	liftWaitTarget(500);
	driveStraight(-60, 127);

	delay(800);

	liftSetTarget(1600);
	driveStraight(30, 127);

	driveStraight(-30, 127);
	delay(1000);

	driveStraight(44, 127);

	// drive straight under barrier, get large ball
	/*driveStraight(48, 100);
	delay(1000);
	// turn towards the stash
	driveTurn(-45, 90);
	// raise lift to maximum scoring height
	liftSetTarget(2350);
	delay(1000);
	// drive straight towards stash
	driveStraight(20, 127);
	delay(1000);
	// release buckyball
	intakeOut();
	delay(750);//delay(400);
	// back up, release large ball
	driveStraight(-5, 100);
	delay(650);
	intakeOff();
	// lower lift and drive back underneath barrier
	liftSetTarget(800);
	driveStraight(-60, 100);
	// turn to face second large ball on barrier
	driveTurn(-30, 90);
	// raise lift, drive straight towards large ball, knock into scoring zone
	liftSetTarget(2350);
	driveStraight(60, 127);*/

	/*liftSetTarget(2000);
	// drive straight, knock down first large ball
	driveMotors(127, 127);
	delay(700);
	driveMotors(0, 0);
	intakeOut();
	delay(900);
	intakeOff();
	// back up
	driveMotors(-100, -100);
	delay(920);
	// wait for driver to reposition robot
	driveMotors(0, 0);
	delay(3000);
	// drive straight, knock down second large ball
	driveMotors(127, 127);
	delay(1750);
	// back up
	driveMotors(-70, -70);
	delay(425);
	driveMotors(0, 0);

	liftSetTarget(1250);
	intakeOut();
	driveMotors(127, 127);
	delay(600);
	driveMotors(-127, 127);
	delay(500);
	driveMotors(0, 0);
	*/
}
