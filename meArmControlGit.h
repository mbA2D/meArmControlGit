/*
  https://www.arduino.cc/en/Hacking/LibraryTutorial
  https://github.com/yorkhackspace/meArm
  meArmControlGit.h - my library for controlling the meArm.
*/
#ifndef meArmControlGit_h
#define meArmControlGit_h

#include "Arduino.h"
#include <Servo.h>
#include <math.h>

class meArmControlGit
{
  public:
    meArmControlGit();
	void beginArm(uint8_t pinBase, uint8_t pinShoulder, uint8_t pinElbow, uint8_t pinGripper);
	void moveGripperServo(double _gVal);
	void moveBaseServo(double _bVal);
	void moveShoulderServo(double _sVal);
	void moveElbowServo(double _eVal);
	void openClaw();
	void closeClaw();
	
	//another function to move the servos smoothly.
	
	//move the arm - this will calculate the angles based on the desired height and distance.
	void moveArm(int Height, int Distance, int Base); //Base 0-180
	int getHeight();
	int getDistance();
	int getBase();
	
  private:
	//we need 3 lengths in here -  (all length in mm)
	int ShoulderElbowLength = 80;//length between point of rotation at elbow and point of rotation at shoulder
	int ElbowWristLength = 80; //
	int BaseClawOffest = 40; //horizontal offset of where it block is gripped to the point of rotation at the wrist, and the point shoulder set back from where you want to measure from
  
	int _Height, _Distance, _Base;
  
    Servo _base, _shoulder, _elbow, _gripper;
	int _pinBase, _pinShoulder, _pinElbow, _pinGripper;
	float RadToDeg(float Rad); //convert radians to degrees
};

#endif
