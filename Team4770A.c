task main()
{
bMotorReflected[port8] = true;
bMotorReflected[port9] = true;
bMotorReflected[port10] = true;
bMotorReflected[port4] = true;
bMotorReflected[port7] = true;
while(1 == 1)
{

motor[port2] = vexRT[Ch4] + vexRT[Ch1] - vexRT[Ch3];
motor[port3] = vexRT[Ch4] - vexRT[Ch1] + vexRT[Ch3];
motor[port9] = vexRT[Ch4] + vexRT[Ch1] + vexRT[Ch3];
motor[port8] = vexRT[Ch4] - vexRT[Ch1] - vexRT[Ch3];


//Arm up and Down\/
if(vexRT[btn5U] == 1)
{
	motor[port1] = -127;
	motor[port10] = -127;
}
else if(vexRT[btn5D] == 1)
{
	motor[port1] = 127;
	motor[port10] = 127;
}
else
{
	motor[port1] = 0;
	motor[port10] = 0;
}
//Extention Code \/
if(vexRT[btn6U] == 1)
{
	motor[port4] = 127;
	motor[port7] = 127;
}
else if(vexRT[btn6D] == 1)
{
	motor[port4] = -127;
	motor[port7] = -127;
}
else
{
	motor[port4] = 0;
	motor[port7] = 0;
}

//Ball Collection Code \/
if(vexRT[btn8U] == 1)
{
	motor[port5] = 127;
	motor[port6] = 127;
}
else if(vexRT[btn8U] == 1)
{
	motor[port5] = -127;
	motor[port6] = -127;
}
else
{
	motor[port5] = 0;
	motor[port6] = 0;
}

}

}
