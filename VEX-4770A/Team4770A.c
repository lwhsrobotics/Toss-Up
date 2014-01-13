#pragma platform(VEX)

//Competition Control and Duration Settings
#pragma competitionControl(Competition)
#pragma autonomousDuration(15)
#pragma userControlDuration(105)

#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!

/////////////////////////////////////////////////////////////////////////////////////////
//
//                          Pre-Autonomous Functions
//
// You may want to perform some actions before the competition starts. Do them in the
// following function.
//
/////////////////////////////////////////////////////////////////////////////////////////
void pre_auton() {
	// Set bStopTasksBetweenModes to false if you want to keep user created tasks running between
	// Autonomous and Tele-Op modes. You will need to manage all user created tasks if set to false.
	bStopTasksBetweenModes = true;

	// All activities that occur before the competition starts
	// Example: clearing encoders, setting servo positions, ...
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//                                 Autonomous Task
//
// This task is used to control your robot during the autonomous phase of a VEX Competition.
// You must modify the code to add your own robot specific commands here.
//
/////////////////////////////////////////////////////////////////////////////////////////
task autonomous() {

}
/////////////////////////////////////////////////////////////////////////////////////////
//
//                                 User Control Task
//
// This task is used to control your robot during the user control phase of a VEX Competition.
// You must modify the code to add your own robot specific commands here.
//
/////////////////////////////////////////////////////////////////////////////////////////
task usercontrol() {
	bMotorReflected[port8] = true;
	bMotorReflected[port9] = true;
	bMotorReflected[port3] = true;
	bMotorReflected[port5] = true;

	while (true) {
		// This is the main execution loop for the user control program. Each time through the loop
		// your program should update motor + servo values based on feedback from the joysticks.


 		// Drive
		motor[port2] = vexRT[Ch4] + vexRT[Ch1] - vexRT[Ch3];
		motor[port3] = vexRT[Ch4] - vexRT[Ch1] + vexRT[Ch3];
		motor[port9] = vexRT[Ch4] + vexRT[Ch1] + vexRT[Ch3];
		motor[port8] = vexRT[Ch4] - vexRT[Ch1] - vexRT[Ch3];

		// Arm Up and Down
		if (vexRT[Btn5U]) {
			motor[port4] = -127;
		} else if (vexRT[Btn5D]) {
			motor[port4] = 127;
		} else {
			motor[port4] = 0;
		}

		// Hang Robot
		if (vexRT[Btn6U]) {
			motor[port4] = 127;
			motor[port7] = 127;
		} else if (vexRT[Btn6D]) {
			motor[port4] = -127;
			motor[port7] = -127;
		} else {
			motor[port4] = 0;
			motor[port7] = 0;
		}

		// Ball Manipulator
		if (vexRT[Btn5U]) {
			motor[port5] = 127;
			motor[port6] = 127;
		} else if (vexRT[Btn5D]) {
			motor[port5] = -127;
			motor[port6] = -127;
		} else {
			motor[port5] = 0;
			motor[port6] = 0;
		}

		// Hang
		if (vexRT[Btn8U]) {
			motor[port7] = 127;
			motor[port10] = 127;
		} else if (vexRT[Btn8D]) {
			motor[port7] = -127;
			motor[port10] = -127;
		} else {
			motor[port7] = 0;
			motor[port10] = 0;
}
		wait1Msec(20);
	}
}
