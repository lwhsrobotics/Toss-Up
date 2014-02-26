/*
 * drivecontrol.h
 *
 *  Created on: Jan 25, 2014
 *      Author: Jacob
 */

#ifndef DRIVECONTROL_H_
#define DRIVECONTROL_H_

void driveMotors(int leftSpeed, int rightSpeed);
void driveStraight(int distance, int speed);
void driveStraightWithoutSlowing(int distance, int speed);
void driveTurn(int degrees, int speed);
void driveInit();

#endif /* DRIVECONTROL_H_ */
