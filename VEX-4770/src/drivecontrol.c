/*
 * drivecontrol.c
 */

#include "motors.h"
#include "main.h"
#include <math.h>
#include <API.h>

/*
 * Wheel diameter: 4 inches
 * Drive width: 17 inches
 * Ticks per revolution: 360 * (15/6) quad encoder (drive axle)
 * Drive axle turns 15 times for every 6 times the encoder axle.
 *
 *
 * Ticks per inch = Ticks per revolution / (Wheel diameter * Pi)
 * Drive circle circumference = Drive width * Pi
 *
 * Expressions must be baked because C is stupid.
 */
const float TICKS_PER_INCH = 71.6197243913529;
const float DRIVE_CIRCLE_CIRCUMFERENCE = 53.40707505;
const float DRIVE_MAGIC_RATIO = 0.85;
const float TURN_MAGIC_RATIO = 0.75;

Encoder driveEncLeft;
Encoder driveEncRight;

void driveInit() {
	driveEncLeft = encoderInit(5, 6, false);
	driveEncRight = encoderInit(3, 4, true);
}

void driveMotors(int leftSpeed, int rightSpeed) {
	motorSet(driveFrontRight,  -rightSpeed);
	motorSet(driveMiddleRight, -rightSpeed);
	motorSet(driveBackRight,   -rightSpeed);
	motorSet(driveFrontLeft,   leftSpeed);
	motorSet(driveMiddleLeft,  leftSpeed);
	motorSet(driveBackLeft,    leftSpeed);
}

int inchToTicks(int inches) {
	return inches * TICKS_PER_INCH * DRIVE_MAGIC_RATIO;
}

int turnToTicks(int degrees) {
	return inchToTicks(DRIVE_CIRCLE_CIRCUMFERENCE * degrees / 360) * TURN_MAGIC_RATIO;
}

int getLeftTicks() {
	//int imeTicks;
	//imeGet(imeDriveLeft, &imeTicks);
	return encoderGet(driveEncLeft);
}

int getRightTicks() {
	//int imeTicks;
	//imeGet(imeDriveRight, &imeTicks); // right is reversed
	return encoderGet(driveEncRight);
}

void clearEncoders() {
	//imeReset(imeDriveLeft);
	//imeReset(imeDriveRight);

	encoderReset(driveEncLeft);
	encoderReset(driveEncRight);
}

int signum(int n) {
	if (n > 0) return 1;
	else if (n < 0) return -1;
	else return 0;
}

int clamp(int n, int a, int b) {
	if (n > b) return b;
	if (n < a) return a;
	return n;
}

int clampMotorSpeed(int speed) {
	return clamp(speed, -127, 127);
}

float fclamp(float n, float a, float b) {
	if (n > b) return b;
	if (n < a) return a;
	return n;
}

const int MIN_SPEED = 30;
const int SCALE_START_DISTANCE = 3;
const int SCALE_END_DISTANCE = 6;
const float CAMBER_COEF = 1.0f;

float scale(int scaleDistance, int distance) {
	return fclamp((float) distance / inchToTicks(scaleDistance), 0, 1);
}

int scaleAutonomousDriveSpeed(int speed, int distanceTraveled, int distanceLeft) {
	// do linear scaling between speed and MIN_SPEED after scale difference
	int speedDifference = clamp(speed - MIN_SPEED, 0, 127);
	//int scaleDistanceTicks = inchToTicks(SCALE_DISTANCE);
	//float scale = fclamp((float) distanceLeft / scaleDistanceTicks, 0, 1);
	float startScale = scale(SCALE_START_DISTANCE, distanceTraveled);
	float endScale = scale(SCALE_END_DISTANCE, distanceLeft);
	float scale = fmin(startScale, endScale);
	return speed - speedDifference * (1 - scale);
}

void driveHelper(int distance, int speed, int scaleLeft, int scaleRight, bool ramp) {
	int target = abs(distance);
	int direction = signum(distance);

	clearEncoders();

	int left = 0;
	int right = 0;

	while (true) {
		left = abs(getLeftTicks());
		right = abs(getRightTicks());

		int distanceTraveled = (left + right) / 2;
		int distanceLeft = target - distanceTraveled;
		if (distanceLeft < 0) break;

		// positive means left is further, negative means right is further
		float camber = (left - right) * CAMBER_COEF;

		int leftSpeed = scaleAutonomousDriveSpeed(clampMotorSpeed(speed - camber), distanceTraveled, distanceLeft);
		int rightSpeed = scaleAutonomousDriveSpeed(clampMotorSpeed(speed + camber), distanceTraveled, distanceLeft);
		driveMotors(direction * scaleLeft * leftSpeed, direction * scaleRight * rightSpeed);

		lcdPrint(uart1, 1, "Left  %d", leftSpeed);
		lcdPrint(uart1, 2, "Right %d", rightSpeed);

		//lcdPrint(uart1, 1, "Distance %d", distanceTraveled);
		//lcdPrint(uart1, 2, "Target %d", target);

		taskDelay(50);
	}

	driveMotors(0, 0);
	lcdClear(uart1);

	// wait for the robot to stop moving
	while (true) {
		taskDelay(100);

		int newLeft = abs(getLeftTicks());
		int newRight = abs(getRightTicks());

		if (newLeft - left == 0 && newRight - right == 0) break;

		left = newLeft;
		right = newRight;
	}
}

void driveStraight(int distance, int speed) {
	driveHelper(inchToTicks(distance), speed, 1, 1, true);
}

void driveStraightWithoutSlowing(int distance, int speed) {
	driveHelper(inchToTicks(distance), speed, 1, 1, false);
}

void driveTurn(int degrees, int speed) {
	driveHelper(turnToTicks(degrees), speed, 1, -1, true);
}
