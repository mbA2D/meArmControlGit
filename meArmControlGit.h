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
    meArmControlGit(); //defaults to 6,9,10,11 - timers may need to be changed.
	void beginArm(int pinBase, int pinShoulder, int pinElbow, int pinGripper)
	void moveGripperServo(int _gVal);
	void moveBaseServo(int _bVal);
	void moveShoulderServo(int _sVal);
	void moveElbowServo(int _eVal);
	
	//another function to move the servos smoothly.
	
	//move the arm - this will calculate the angles based on the desired height and distance.
	void moveArm(int Height, int Distance, int Base);
	
  private:
	//we need 3 lengths in here -  (all length in mm)
	int ShoulderElbowLength = 80;//
	int ElbowWristLength = 80; //
	int BaseClawOffest = 40; //
  
    Servo _base, _shoulder, _elbow, _gripper;
	int _pinBase, _pinShoulder, _pinElbow, _pinGripper;
	float RadToDeg(float Rad); //convert radians to degrees
};

#endif