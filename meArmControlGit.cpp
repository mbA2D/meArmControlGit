/*
  https://www.arduino.cc/en/Hacking/LibraryTutorial
  https://github.com/yorkhackspace/meArm
 */

#include "Arduino.h"
#include "meArmControlGit.h"
#include "Servo.h"

	#define pi 3.14159	
		
	#define CLAW_OFFSET_X 67 //mm
	#define CLAW_OFFSET_Y (15) //mm bigger to account for 'play' in the base servo
	#define SHOULDER_X (17) //mm  point of rotation offset from the point measured from
	#define SHOULDER_Y 53 //mm

	#define SHOULDER_MIN_VALUE 2400 //when shoulder servo is as far forward as it can be
	#define SHOULDER_MAX_VALUE 600 //when shoulder servo is as far backwards as it can be
	#define ELBOW_MIN_VALUE 1800 //when elbow servo arm is at -30 degrees to X axis
	#define ELBOW_MAX_VALUE 600 //when elbow servo arm is at 90 degress to X axis
	
	//Elbow degrees positions
	#define ELBOW_MIN_POSITION (-30) //relative to X
	#define ELBOW_MAX_POSITION 90 //90 degrees relative to X (straight up)
	
	//Shoulder degrees positions
	#define SHOULDER_MIN_POSITION 0 //relative to X - all the way forwards
	#define SHOULDER_MAX_POSITION 170 //all the way backwards

meArmControlGit::meArmControlGit(){
	//starting values so that the function will runand not use uninitialized variables.
	_Height = 10;
	_Distance = 10;
	_Base = 10;
}

void meArmControlGit::beginArm(uint8_t pinBase, uint8_t pinShoulder, uint8_t pinElbow, uint8_t pinGripper){
	_pinBase = pinBase;
	_pinShoulder = pinShoulder;
	_pinElbow = pinElbow;
	_pinGripper = pinGripper;
	
	_base.attach(_pinBase);
	_shoulder.attach(_pinShoulder);
	_elbow.attach(_pinElbow);
	_gripper.attach(_pinGripper);
}

void meArmControlGit::moveGripperServo(double _gVal){
	if(_gVal < 200) {_gripper.write(_gVal); Serial.print("Gripper: "); Serial.print(_gVal); return;} //upper and lower bounds for angles
	_gripper.writeMicroseconds(constrain(_gVal, 400, 2800)); return;//upper and lower bounds for servo Us
}

void meArmControlGit::moveBaseServo(double _bVal){
	if(_bVal < 200) {_base.write(_bVal); Serial.print("Gripper: "); Serial.print(_bVal); return;} //upper and lower bounds for angles
	_base.writeMicroseconds(constrain(_bVal, 400, 2800)); return;//upper and lower bounds for servo Us
}

void meArmControlGit::moveShoulderServo(double _sVal){
	if(_sVal < 200) {_shoulder.write(_sVal); Serial.print("Shoulder: "); Serial.println(_sVal);return;} //upper and lower bounds for angles
	_shoulder.writeMicroseconds(constrain(_sVal, 400, 2800)); return; //upper and lower bounds for servo Us
}

void meArmControlGit::moveElbowServo(double _eVal){
	if(_eVal < 200) {_elbow.write(_eVal); Serial.print("Elbow: "); Serial.println(_eVal);return;} //upper and lower bounds for angles
	_elbow.writeMicroseconds(constrain(_eVal, 400, 2800)); return; //upper and lower bounds for servo Us
}

//we want to be able to control the arm based on 3 values - height, distance, and base angle.
void meArmControlGit::moveArm(int Height, int Distance, int Base){//mm, mm, degrees
	//check the current height and distance to see if it changes.
	
	if((Height != _Height) || (Distance != _Distance)){ //if height or distance have changed.
	
		//add an isReachable() function
		
		//apply offsets and constrain values

		//Height += CLAW_OFFSET_Y;
		Height -= SHOULDER_Y;
		Distance -= CLAW_OFFSET_X;
		Distance += SHOULDER_X;
		
		Height = constrain(Height, -60, 160);
		Distance = constrain(Distance, 30, 160);
		
		Serial.print("Triangle Height: ");
		Serial.println(Height);
		Serial.print("Triangle Distance: ");
		Serial.println(Distance);
		Serial.println("");
		
		//these are the three distances in the triangle.
		float ShoulderToWristDist = sqrt((sq(Distance))+(sq(Height))); //hypotenuse of distance, height triangle.
		float ShoulderToElbowDist = (float)ShoulderElbowLength;
		float ElbowToWristDist = (float)ElbowWristLength;
		//these are the three angles
		float ShoulderAngle = RadToDeg(acos((sq(ShoulderToWristDist) - sq(ShoulderToElbowDist) - sq(ElbowToWristDist)) / (-2 * (ShoulderToWristDist) * (ShoulderToElbowDist)))); //cosine law
		float WristAngle = ShoulderAngle;
		float ElbowAngle = 180 - (2*WristAngle); //Elbow angle is not needed.
		
		Serial.print("Shoulder Angle: ");
		Serial.println(ShoulderAngle);
		Serial.print("WristAngle: ");
		Serial.println(WristAngle);
		Serial.println("");
		
		//adjust angle relative to X-axis
		float adjust = RadToDeg(atan(Height / Distance));
		ShoulderAngle += adjust;
		ElbowAngle -= adjust;
		
		//then write to servos based on calibration data, mapping the values.
		
		//Target Us Values
		int ShoulderServoUs = map(ShoulderAngle, SHOULDER_MIN_POSITION, SHOULDER_MAX_POSITION, SHOULDER_MIN_VALUE, SHOULDER_MAX_VALUE);
		int ElbowServoUs = map(ElbowAngle, ELBOW_MIN_POSITION, ELBOW_MAX_POSITION, ELBOW_MIN_VALUE, ELBOW_MAX_VALUE);
		
		Serial.print("ShoulderServoUs: ");
		Serial.println(ShoulderServoUs);
		Serial.print("ElbowServoUs: ");
		Serial.println(ElbowServoUs);
		Serial.println("");
		
		//update the height and distance
		_Height = Height;
		_Distance = Distance;
		
		//Now move the servos
		moveElbowServo(ElbowServoUs);
		moveShoulderServo(ShoulderServoUs);
	
	}
	
	if(Base != _Base){
		//move the servo and update the position
		moveBaseServo(Base);
		_Base = Base;
	}
}

float meArmControlGit::RadToDeg(float Rad){
    return (Rad * (180/pi));
}

int meArmControlGit::getHeight(){
	return _Height;
}

int meArmControlGit::getDistance(){
	return _Distance;
}

int meArmControlGit::getBase(){
	return _Base;
}