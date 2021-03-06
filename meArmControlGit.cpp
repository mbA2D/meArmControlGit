/*
  https://www.arduino.cc/en/Hacking/LibraryTutorial
  https://github.com/yorkhackspace/meArm
 
 */

#include "Arduino.h"
#include "meArmControlGit.h"
#include "Servo.h"
	#define pi 3.14159265
	
	#define CLAW_OFFSET_X 80 //mm
	#define CLAW_OFFSET_Y (5) //mm bigger to account for 'play' in the base servo
	#define SHOULDER_X (20) //mm  point of rotation offset from the point measured from
	#define SHOULDER_Y 53 //mm

	//other values
	//#define SHOULDER_MIN_VALUE 2400 //when shoulder servo is as far forward as it can be
	//#define SHOULDER_MAX_VALUE 600 //when shoulder servo is as far backwards as it can be
	//#define ELBOW_MIN_VALUE 1800 //when elbow servo arm is at -30 degrees to X axis
	//#define ELBOW_MAX_VALUE 600 //when elbow servo arm is at 90 degress to X axis

	#define SHOULDER_MIN_VALUE 2400 //when shoulder servo is as far forward as it can be
	#define SHOULDER_MAX_VALUE 900 //when shoulder servo is as far backwards as it can be
	#define ELBOW_MIN_VALUE 1800 //when elbow servo arm is at -30 degrees to X axis
	#define ELBOW_MAX_VALUE 700 //when elbow servo arm is at 90 degress to X axis
	
	#define OPEN_CLAW_US 1600 //when the claw is open enough to grab a block
	#define RELEASE_CLAW_US 1750 //open just enough to release the block
	#define CLOSE_CLAW_US 2300 //claw is closed to hold the block
	//have another one to open the claw just enough to release?
	
	//Elbow degrees positions
	#define ELBOW_MIN_POSITION (-30) //relative to X
	#define ELBOW_MAX_POSITION 90 //90 degrees relative to X (straight up)
	
	//Shoulder degrees positions
	#define SHOULDER_MIN_POSITION 0 //relative to X - all the way forwards
	#define SHOULDER_MAX_POSITION 170 //all the way backwards
	
	#define MAX_TRAVEL_DIST 240 //maximum travel that the arm will need.
	#define MAX_TRAVEL_DELAY 600 //time required to travel that distance.
	#define MAX_BASE_TRAVEL 180 //biggest angle the base servo will move
	#define MAX_BASE_DELAY 400 //time required for the base to move that much

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
	if(_gVal < 200) {_gripper.write(_gVal); return;} //upper and lower bounds for angles
	_gripper.writeMicroseconds(constrain(_gVal, 400, 2800)); return;//upper and lower bounds for servo Us
}

void meArmControlGit::moveBaseServo(double _bVal){
	if(_bVal < 200) {_base.write(_bVal); return;} //upper and lower bounds for angles
	_base.writeMicroseconds(constrain(_bVal, 400, 2800)); return;//upper and lower bounds for servo Us
}

void meArmControlGit::moveShoulderServo(double _sVal){
	if(_sVal < 200) {_shoulder.write(_sVal); return;} //upper and lower bounds for angles
	_shoulder.writeMicroseconds(constrain(_sVal, 400, 2800)); return; //upper and lower bounds for servo Us
}

void meArmControlGit::moveElbowServo(double _eVal){
	if(_eVal < 200) {_elbow.write(_eVal); return;} //upper and lower bounds for angles
	_elbow.writeMicroseconds(constrain(_eVal, 400, 2800)); return; //upper and lower bounds for servo Us
}

//we want to be able to control the arm based on 3 values - height, distance, and base angle.
void meArmControlGit::moveArm(int Height, int Distance, int Base){//mm, mm, degrees
	//check the current height and distance to see if it changes.
	
	int currentDist = sqrt(sq(_Height) + sq(_Distance));
	int targetDist = sqrt(sq(Height) + sq(Distance));
	int travelDist = abs(targetDist - currentDist);
	Serial.println("TravelDist:" + travelDist);
	
	if((Height != _Height) || (Distance != _Distance)){ //if height or distance have changed.
	
		//add an isReachable() function
		
		//apply offsets and constrain values
		
		/*
		Serial.println("Trying to go to:");
		Serial.print("Height: ");
		Serial.println(Height);
		Serial.print("Triangle Distance: ");
		Serial.println(Distance);
		Serial.println("");
		*/
		
		Height += CLAW_OFFSET_Y;
		Height -= SHOULDER_Y;
		Distance -= CLAW_OFFSET_X;
		//Serial.print("After claw offset: ");
		//Serial.println(Distance);
		Distance += SHOULDER_X;
		//Serial.print("After shoulder offset: ");
		//Serial.println(Distance);
		
		Height = constrain(Height, -20, 160);
		Distance = constrain(Distance, 1, 160);
		
		/*
		Serial.println("Offsets applied");
		Serial.print("Height: ");
		Serial.println(Height);
		Serial.print("Triangle Distance: ");
		Serial.println(Distance);
		Serial.println("");
		*/
		
		//these are the three distances in the triangle.
		float ShoulderToWristDist = sqrt((sq(Distance))+(sq(Height))); //hypotenuse of distance, height triangle.
		float ShoulderToElbowDist = (float)ShoulderElbowLength;
		float ElbowToWristDist = (float)ElbowWristLength;
		
		/*
		Serial.println("Triangle Edges");
		Serial.print("StoW dist: ");
		Serial.println(ShoulderToWristDist);
		Serial.print("StoE dist: ");
		Serial.println(ShoulderToElbowDist);
		Serial.print("EtoW dist: ");
		Serial.println(ElbowToWristDist);
		Serial.println("");
		*/
		
		//these are the three angles
		float ShoulderAngle = RadToDeg(acos(ShoulderToWristDist / (2 * ShoulderToElbowDist)));
		//float ShoulderAngle = RadToDeg(acos((sq(ShoulderToWristDist) - sq(ShoulderToElbowDist) - sq(ElbowToWristDist)) / (-2 * (ShoulderToWristDist) * (ShoulderToElbowDist)))); //cosine law
		//ShoulderAngle = 180 - ShoulderAngle;
		float WristAngle = ShoulderAngle;
		float ElbowAngle = 180 - (2*WristAngle); //Elbow angle is not needed.
		
		/*
		Serial.println("Angles in triangle");
		Serial.print("Shoulder Angle: ");
		Serial.println(ShoulderAngle);
		Serial.print("WristAngle: ");
		Serial.println(WristAngle);
		Serial.print("ElbowAngle: ");
		Serial.println(ElbowAngle);
		Serial.println("");
		*/
		
		//adjust angle relative to X-axis
		float adjust = RadToDeg(atan((float)Height / (float)Distance));
		//Serial.print("Adjustment: ");
		//adjust = 90
		//Serial.println(RadToDeg(atan(Height / Distance)));
		//Serial.println(RadToDeg(atan((float)Height / (float)Distance)));
		//Serial.println(adjust);
		ShoulderAngle += adjust;
		//ShoulderAngle = 180 - ShoulderAngle;
		//ElbowAngle -= adjust;
		//Serial.println(ElbowAngle - adjust);
		ElbowAngle = 180-ShoulderAngle-ElbowAngle;
		//Serial.println(ElbowAngle);
		
		/*
		Serial.println("Adjusted angles for servos");
		Serial.print("Shoulder Angle: ");
		Serial.println(ShoulderAngle);
		Serial.print("Elbow Angle: ");
		Serial.println(ElbowAngle);
		Serial.println();
		*/
		
		//then write to servos based on calibration data, mapping the values.
		
		//Target Us Values
		int ShoulderServoUs = map(ShoulderAngle, SHOULDER_MIN_POSITION, SHOULDER_MAX_POSITION, SHOULDER_MIN_VALUE, SHOULDER_MAX_VALUE);
		int ElbowServoUs = map(ElbowAngle, ELBOW_MIN_POSITION, ELBOW_MAX_POSITION, ELBOW_MIN_VALUE, ELBOW_MAX_VALUE);
		
		/*
		Serial.println("Servo Microseconds values");
		Serial.print("ShoulderServoUs: ");
		Serial.println(ShoulderServoUs);
		Serial.print("ElbowServoUs: ");
		Serial.println(ElbowServoUs);
		Serial.println("");
		*/
		
		//update the height and distance
		_Height = Height;
		_Distance = Distance;
		
		//Now move the servos
		moveElbowServo(ElbowServoUs);
		moveShoulderServo(ShoulderServoUs);
	
	}
	
	int baseDist = 0;
	if(Base != _Base){
		//move the servo and update the position
		moveBaseServo(Base);
		baseDist = abs(_Base - Base);
		_Base = Base;
	}
	
	//delay based on travelDist
	//delay if required for base movement
	//larger of the delays required for base and travel movement.
	
	int delayTravelTime = map(travelDist, 0, MAX_TRAVEL_DIST,0,MAX_TRAVEL_DELAY);
	int delayBaseTime = map(baseDist, 0, MAX_BASE_TRAVEL, 0 , MAX_BASE_DELAY);
	Serial.println("LIBRARY DELAY TIME  " + max(delayTravelTime, delayBaseTime));
	delay(max(delayTravelTime, delayBaseTime));
	
}

void meArmControlGit::openClaw(){
	moveGripperServo(OPEN_CLAW_US);
	Serial.println("Claw is open");
}

void meArmControlGit::releaseClaw(){
	moveGripperServo(RELEASE_CLAW_US);
	Serial.println("Claw is released");
}

void meArmControlGit::closeClaw(){
	moveGripperServo(CLOSE_CLAW_US);
	Serial.println("Claw is closed");
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
