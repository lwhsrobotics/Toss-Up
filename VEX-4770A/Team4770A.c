#pragma platform(VEX)

//Competition Control and Duration Settings
#pragma competitionControl(Competition)
#pragma autonomousDuration(15)
#pragma userControlDuration(105)

#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!
#include "pid.c"

PidState liftStateRight;
PidState liftStateLeft;

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

	pidInitState(liftStateRight, 0.5, 0.2, 0, 150);
	pidInitState(liftStateLeft, 0.5, 0.2, 0, 150);



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

int RightPot;
int LeftPot;
/////////////////////////////////////////////////////////////////////////////////////////
//
//                                 User Control Task
//
// This task is used to control your robot during the user control phase of a VEX Competition.
// You must modify the code to add your own robot specific commands here.
//


/////////////////////////////////////////////////////////////////////////////////////////
task usercontrol() {
pidSetTarget(liftStateRight, SensorValue[in2]);
pidSetTarget(liftStateLeft, SensorValue[in1]);

	while (true) {
		// This is the main execution loop for the user control program. Each time through the loop
		// your program should update motor + servo values based on feedback from the joysticks.

	RightPot = SensorValue[in2];
	LeftPot = SensorValue[in1];
bMotorReflected[port1] = true;
bMotorReflected[port10] = true;
bMotorReflected[port8] = true;
bMotorReflected[port7] = true;
bMotorReflected[port6] = true;

 		// Drive
		motor[port9] = vexRT[Ch4] + vexRT[Ch1] - vexRT[Ch3]; //back Left
		motor[port10] = vexRT[Ch4] - vexRT[Ch1] + vexRT[Ch3]; //back right
		motor[port2] = vexRT[Ch4] + vexRT[Ch1] + vexRT[Ch3]; //front Left
		motor[port1] = vexRT[Ch4] - vexRT[Ch1] - vexRT[Ch3]; //front right


		// Arm Up and Down
		if (vexRT[Btn5U]) {
			pidIncrementTarget(liftStateRight, 4);
			pidIncrementTarget(liftStateLeft, 4);

		} else if (vexRT[Btn5D]) {
		pidIncrementTarget(liftStateRight, -4);
		pidIncrementTarget(liftStateLeft, -4);

		}

		motor[port8] = pid(liftStateRight,SensorValue[in2],20);
		motor[port3] = pid(liftStateLeft,SensorValue[in1],20);
		// Hang Robot

		// Ball Manipulator
		if (vexRT[Btn8U]) {
			motor[port5] = 127;
			motor[port6] = 127;
		} else if (vexRT[Btn8D]) {
			motor[port5] = -127;
			motor[port6] = -127;
		} else {
			motor[port5] = 0;
			motor[port6] = 0;
		}




		wait1Msec(20);
	}
}
