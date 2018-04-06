#include "meArmControlGit.h"
#include <Servo.h>

#define basePin 11
#define shoulderPin 10
#define elbowPin 9
#define gripperPin 6

meArmControlGit arm;

void setup() {
  // put your setup code here, to run once:
  //attach all servos
  arm.begin(basePin, shoulderPin, elbowPin, gripperPin);
  delay(500);
  Serial.begin(115200);
  
  //move all servos to 90 deg.
  arm.moveGripperServo(90);
  arm.moveBaseServo(90);
  arm.moveShoulderServo(1500);
  arm.moveElbowServo(1500);
  
  delay(2000);
  
  moveArm(70, 100, 45);
}

void loop() {
  // put your main code here, to run repeatedly:

}
