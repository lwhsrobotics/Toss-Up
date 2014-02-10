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
bMotorReflected[port9] = true;
bMotorReflected[port10] = true;
bMotorReflected[port7] = true;
bMotorReflected[port6] = true;
bMotorReflected[port3] = true;

motor[port1] = 127;
motor[port2] = 127;
motor[port9] = 127;
motor[port10] = 127;
wait1Msec(700);
motor[port3] = 127;
motor[port4] = 127;
motor[port7] = 127;
motor[port8] = 127;
wait1Msec(300);

motor[port1] = -127;
motor[port2] = -127;
motor[port9] = -127;
motor[port10] = -127;
wait1Msec(700);
motor[port1] = 0;
motor[port2] = 0;
motor[port9] = 0;
motor[port10] = 0;
motor[port3] = 0;
motor[port4] = 0;
motor[port7] = 0;
motor[port8] = 0;
wait1Msec(4000);
//turn
motor[port1] = 127;
motor[port2] = 127;
motor[port9] = -127;
motor[port10] = -127;
wait1Msec(300);

motor[port1] = 127;
motor[port2] = 127;
motor[port9] = 127;
motor[port10] = 127;
wait1Msec(900);
motor[port3] = 127;
motor[port4] = 127;
motor[port7] = 127;
motor[port8] = 127;
wait1Msec(600);

motor[port1] = -127;
motor[port2] = -127;
motor[port9] = -127;
motor[port10] = -127;
wait1Msec(700);
motor[port1] = 0;
motor[port2] = 0;
motor[port9] = 0;
motor[port10] = 0;
motor[port3] = 0;
motor[port4] = 0;
motor[port7] = 0;
motor[port8] = 0;
wait1Msec(40);
//mission Drop
motor[port1] = 127;
motor[port2] = 127;
motor[port9] = 127;
motor[port10] = 127;
wait1Msec(200);
motor[port1] = 0;
motor[port2] = 0;
motor[port9] = 0;
motor[port10] = 0;
motor[port5] = 127;
motor[port6] = 127;
wait1Msec(300);
motor[port1] = 0;
motor[port2] = 0;
motor[port3] = 0;
motor[port4] = 0;
motor[port5] = 0;
motor[port6] = 0;
motor[port7] = 0;
motor[port8] = 0;
motor[port9] = 0;
motor[port10] = 0;



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

	while (true) {
		// This is the main execution loop for the user control program. Each time through the loop
		// your program should update motor + servo values based on feedback from the joysticks.


bMotorReflected[port9] = true;
bMotorReflected[port10] = true;
bMotorReflected[port7] = true;
bMotorReflected[port6] = true;
bMotorReflected[port3] = true;


 		// Drive
		motor[port1]  = (vexRT[Ch3] + vexRT[Ch4])/2;  // (y + x)/2
    motor[port9] = (vexRT[Ch3] - vexRT[Ch4])/2;  // (y - x)/2
    motor[port2]  = (vexRT[Ch3] + vexRT[Ch4])/2;  // (y + x)/2
    motor[port10] = (vexRT[Ch3] - vexRT[Ch4])/2;  // (y - x)/2


		// Hang Robot

		// Ball Manipulator
		if (vexRT[Btn6U]) {
			motor[port5] = 127;
			motor[port6] = 127;
		} else if (vexRT[Btn6D]) {
			motor[port5] = -127;
			motor[port6] = -127;
		} else {
			motor[port5] = 0;
			motor[port6] = 0;
		}

		//up down
		if (vexRT[Btn5U]) {
			motor[port3] = 127;
			motor[port4] = 127;
			motor[port7] = 127;
			motor[port8] = 127;
		} else if (vexRT[Btn5D]) {
			motor[port3] = -127;
			motor[port4] = -127;
			motor[port7] = -127;
			motor[port8] = -127;
		} else {
			motor[port3] = 0;
			motor[port4] = 0;
			motor[port7] = 0;
			motor[port8] = 0;
		}




		wait1Msec(20);
	}
}
