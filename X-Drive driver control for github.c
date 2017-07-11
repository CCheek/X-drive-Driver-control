#pragma config(Motor,  port2,           frontLeft,     tmotorVex393_MC29, openLoop, driveLeft)
#pragma config(Motor,  port3,           frontRight,    tmotorVex393_MC29, openLoop, reversed, driveRight)
#pragma config(Motor,  port4,           rearRight,     tmotorVex393_MC29, openLoop, reversed, driveRight)
#pragma config(Motor,  port5,           rearLeft,      tmotorVex393_MC29, openLoop, driveLeft)

#pragma platform(VEX2)
#pragma competitionControl(Competition)
#include "Vex_Competition_Includes.c"


void pre_auton()
{
}
task autonomous()
{
}

#define maxMotorSpeed 127
#define numberOfMotors  4

//**-----------------------SUPPORT FUNCTIONS-----------------------------------**//

float FindMaxFloat(float a, float b,  float c = (0), float d = (0), float e = (0),
float f = (0), float g = (0), float h = (0), float i = (0), float j = (0))
{
	float values[] = {a,b,c,d,e,f,g,h,i,j};
	float maxValue = 0;
	for(int z = 0; z < 9; z++) {
		if(abs(values[z]) > maxValue) maxValue = abs(values[z]);
	}
	return maxValue;
}

//**-----------------------MAIN FUNCTIONS------------------------------------**//

void HolonomicRadianOutput(float radians, float speed = 1, byte rotation = 0)
{
	if(speed > 0)
	{
		float frontLeftOutput 	= maxMotorSpeed * cos(PI/4 - radians),
		frontRightOutput 	=  -maxMotorSpeed * cos(PI/4 + radians),
		rearRightOutput 	=  -maxMotorSpeed * cos(PI/4 - radians),
		rearLeftOutput		= maxMotorSpeed * cos(PI/4 + radians);

		frontLeftOutput += rotation;
		frontRightOutput += rotation;
		rearRightOutput += rotation;
		rearLeftOutput += rotation;


		float maxOutput = FindMaxFloat(frontLeftOutput, frontRightOutput, rearRightOutput, rearLeftOutput);
		// The goal is to rescale all values to -127<=out<=127. See README.txt for algebraic explanation.
		speed *= (maxMotorSpeed / maxOutput);


		frontLeftOutput *= speed;
		frontRightOutput *= speed;
		rearLeftOutput *= speed;
		rearRightOutput *= speed;

		motor[frontLeft] = (byte)frontLeftOutput;
		motor[frontRight] = (byte)frontRightOutput;
		motor[rearLeft] = (byte)rearLeftOutput;
		motor[rearRight] = (byte)rearRightOutput;
	}
	else if (rotation > 20 || rotation < -20)
	{
		motor[frontLeft] = rotation;
		motor[frontRight] = rotation;
		motor[rearLeft] = rotation;
		motor[rearRight] = rotation;
	}
	else
	{
		motor[frontLeft] = 0;
		motor[frontRight] = 0;
		motor[rearLeft] = 0;
		motor[rearRight] = 0;
	}
}

#define JOY_Y_DEADBAND 20
#define JOY_X_DEADBAND 20

typedef struct
{
	float radians;
	float speed;
} PolarJoystick;

void getPolarJoy(float *radians, float *speed, TVexJoysticks joy_x = Ch2, TVexJoysticks joy_y = Ch1) {
	byte x_val = vexRT[joy_x];
	byte y_val = vexRT[joy_y];
	if((abs(x_val) < 20) && (abs(y_val) < 20)) {
		*radians = 0;
		*speed = 0;
	}
	else {
		*radians = atan2(y_val,x_val);
		float tmpSpeed = sqrt((y_val * y_val) + (x_val * x_val));
		*speed = tmpSpeed/127;
		if(*speed > 1) *speed = 1;
	}
}

task usercontrol()
{
	while(true) {

		bLCDBacklight = true;

		clearLCDLine(0);
		clearLCDLine(1);

		displayLCDCenteredString(0,"Enter text here");
		PolarJoystick joystick;
		getPolarJoy(&joystick.radians, &joystick.speed);
		HolonomicRadianOutput(joystick.radians, joystick.speed, vexRT[Ch4]);

	}
}
